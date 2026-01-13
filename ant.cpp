#include <vector>

#include "ant.h"

using namespace std;

vector<int> dh = {-1,-1,0,1,1,1,0,-1};
vector<int> dw = {0,1,1,1,0,-1,-1,-1};


ant::ant(Config& config):cfg(config){//コンストラクタ
    initAnt();
}

void ant::initAnt(){
    start.h = cfg.startH;
    start.w = cfg.startW;
}

void ant::search(){
    route.push_back({start.h,start.w,-1});
    
    while(1){
        cur.h = route.back().h;
        cur.w = route.back().w;

        if(cur.h == goal.h && cur.w == goal.w){//ゴールかどうか判定
            route.back().d = -1;
            break;
        }

        dist++;//距離を加算
        //リスク計算
        
        calcProb();//各セルへの移動確率を計算
        int dir = dirSelection();//移動方向の選択
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

        //meshmapの実装をした後
        //移動確率の実装をする
    }


}