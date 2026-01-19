#include <vector>
#include <math.h>
#include <random>

#include "ant.h"

using namespace std;

ant::ant(Config& config,meshmap& mp):cfg(config),mp(mp){//コンストラクタ
    initAnt();
}

void ant::initAnt(){
    start.h = cfg.startH;
    start.w = cfg.startW;

    dist = 0;
    risk = 0;
    canMove = true;
    route.clear();
    route.push_back({start.h,start.w,-1});//スタート位置を格納
    
    for(int i = 0; i < cfg.maxH;i++){
        visit[i].resize(cfg.maxW,0);
    }
}

void ant::resetAnt(){
    initAnt();
}

void ant::search(){
    
    while(true){
        cur.h = route.back().h;
        cur.w = route.back().w;

        visit[cur.h][cur.w]++;//各セルの訪問回数を記録
        dist++;//距離を加算
        risk += mp(cur.h,cur.w).riskVal;//リスク値を加算

        if(cur.h == goal.h && cur.w == goal.w){//ゴールかどうか判定
            route.back().d = -1;
            break;
        }

        calcProb();//各セルへの移動確率を計算
        int dir = dirSelection();//移動方向の選択
        if(dir == -1){
            canMove = false;
            resetAnt();
            continue;
        }
        
        route.back().d = dir;
        route.push_back({cur.h + dh[dir],cur.w + dw[dir],-1});//移動したセルを格納

    }

}

void ant::calcProb(){
    pVec.clear();
    pVec.resize(8,0);

    for(int i = 0; i < 8; i++){
        int movedH = cur.h + dh[i];
        int movedW = cur.w + dw[i];

        if(!(mp.isInsideRoad(movedH,movedW)))continue;

        double distP = mp(cur.h,cur.w).distPheromone[i] * cfg.phrWeight;
        double riskP = mp(cur.h,cur.w).riskPheromone[i] * ((double)1 - cfg.phrWeight);

        pVec[i] = pow(distP+riskP,cfg.alpha) * pow(mp(cur.h,cur.w).heuristic[i],cfg.beta);
        pVec[i] = pVec[i] / (visit[movedH][movedW] + 1);
    }
}

int ant::dirSelection(){
    double sum = 0;
    for(auto val : pVec) sum += val;
    if(sum == 0) return -1;//すべての移動セルが移動不可のため戻して最初からやり直し

    for(int i = 0; i < pVec.size();i++) pVec[i] /= sum;
    vector<double> acm(8,0);
    acm[0] = pVec[0];
    for(int i = 1; i < pVec.size();i++) acm[i] = acm[i-1] + pVec[i];

    mt19937_64 mt64(0);  //もとにする整数型の乱数は別途必要
    uniform_real_distribution<double> uni(0, 1);
    double r = uni(mt64);

    for(int i = 0; i < acm.size();i++){
        if(r < acm[i]){
            return i;
        }
    }
}