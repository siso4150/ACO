#include <vector>
#include <math.h>
#include <random>
#include <iostream>
#include <chrono>

#include "ant.h"

using namespace std;

ant::ant(Config& config,meshmap& mp):cfg(config),mp(mp){//コンストラクタ
    initAnt();
    
    if(mp(cfg.startH,cfg.startW).isRoad == false){
        cerr << "エラー：スタート位置が道路ではありません" << endl;
        abort();
    }else if(mp(cfg.goalH,cfg.goalW).isRoad == false){
        cerr << "エラー：ゴール位置が道路ではありません" << endl;
        abort();
    }
}

void ant::initAnt(){
    start.h = cfg.startH;
    start.w = cfg.startW;
    goal.h = cfg.goalH;
    goal.w = cfg.goalW;

    dist = 0;
    risk = 0;
    canMove = true;
    route.clear();
    route.push_back({start.h,start.w,-1});//スタート位置を格納
    visit.assign(cfg.maxH,vector<int>(cfg.maxW,0));//訪問回数の初期化
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

        if(dist % 10000 == 0){
            //cout << "探索" << dist << "ステップ目" << endl;
            if(dist == 500000){
                
                cout << "探索をリスタート" << endl;
                resetAnt();
                continue;
            }
        }

        calcProb();//各セルへの移動確率を計算
        int dir = dirSelection();//移動方向の選択
        if(dir == -1){
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
        if(visit[movedH][movedW] > 25) pVec[i] = 0;
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

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    mt19937_64 gen(seed);
    uniform_real_distribution<double> dist(0,1);
    double r = dist(gen);

    for(int i = 0; i < acm.size();i++){
        if(r < acm[i]){
            return i;
        }
    }
}