#include <iostream>
#include <fstream>
#include <istream>
#include <sstream>
#include <string>

#include "meshmap.h"

using namespace std;

meshmap::meshmap(Config& config):cfg(config){
    
    //mapのサイズ初期化
    map.resize(cfg.maxH);
    for(int i = 0; i < cfg.maxH; i++){
        map[i].resize(cfg.maxW);
    }

    for(int i = 0; i < cfg.maxH; i++){
        for(int j = 0; j < cfg.maxW; j++){
            map[i][j].distPheromone.resize(8,1.0);//各セルの距離フェロモン値を初期化
            map[i][j].riskPheromone.resize(8,1.0);//各セルのリスクフェロモンを初期化
            map[i][j].heuristic.resize(8,0.0);//各セルのヒューリスティック値を初期化
        }
    }

    maxtoCenter = 0;
    maxtoGoal = 0;
    minToCenter = 1e9;
    minToGoal = 1e9;


    initRoad();//道路かどうかを入力
    initRisk();//リスク値を入力
    initCenter();//道路中心線を入力
    initHeuristic();//ヒューリスティック値の初期化
    
}

void meshmap::initRoad(){
    string str_buf;
    string str_conma_buf;
    ifstream ifs_csv(cfg.mapCsv);

    if(!ifs_csv.is_open()){
        cerr << "ファイルがありません" << endl;
        return;
    }

    int h = 0;
    while(getline(ifs_csv,str_buf)){
        istringstream i_stream(str_buf);
        int w = 0;
        while(getline(i_stream, str_conma_buf,',')){
            if(str_conma_buf == "1"){
                map[h][w].isRoad = true;
            }
            w++;
        }
        h++;
    }
}

void meshmap::initRisk(){
    string str_buf;
    string str_conma_buf;
    ifstream ifs_csv(cfg.somCsv);

    if(!ifs_csv.is_open()){
        cerr << "ファイルがありません" << endl;
        return;
    }

    int cnt = 0;
    while(getline(ifs_csv,str_buf)){
        
        if(cnt == 0){//１行目はヘッダなので飛ばす
            cnt++;
            continue;
        }

        int h = str_buf[0];
        int w = str_buf[1];
        double val = 0;
        for(int i = 2; i < str_buf.size();i++) val += str_buf[i];
        map[h][w].riskVal = val;
        
    }
}

void meshmap::initCenter(){

    string str_buf;
    string str_conma_buf;
    ifstream ifs_csv(cfg.centerCsv);

    if(!ifs_csv.is_open()){
        cerr << "ファイルがありません" << endl;
        return;
    }

    int h = 0;
    while(getline(ifs_csv,str_buf)){
        istringstream i_stream(str_buf);
        int w = 0;
        while(getline(i_stream, str_conma_buf,',')){
            if(str_conma_buf == "1"){
                map[h][w].isCenter = true;
            }
            w++;
        }
        h++;
    }
}

void meshmap::initHeuristic(){
    //ヒューリスティック値の計算
    for(int h = 0; h < cfg.maxH; h++){
        for(int w = 0; w < cfg.maxW; w++){
            if(!map[h][w].isRoad) continue;//道路でなければスキップ

            //初項（ゴールまでのチェビシェフ距離)の計算
            map[h][w].toGoal = abs(h - cfg.goalH) + abs(w - cfg.goalW);
            maxtoGoal = max(maxtoGoal, map[h][w].toGoal);
            minToGoal = min(minToGoal, map[h][w].toGoal);

            //第二項（道路中心線までの距離）の計算
            bool find = false;
            int dist = 50;
            for(int d = 1; d <= dist; d++){

                for(int i = 0; i < 8; i++){
                    int movedH = h + dh[i] * d;
                    int movedW = w + dw[i] * d;

                    if(isInsideRoad(movedH,movedW) && map[movedH][movedW].isCenter){
                        int difH = abs(movedH - h);
                        int difW = abs(movedW - w);
                        map[h][w].toCenter = min(difH,difW);
                        minToCenter = min(minToCenter,map[h][w].toCenter);
                        maxtoCenter = max(maxtoCenter,map[h][w].toCenter);
                        break;
                    }
                }
            }
        }
    }

    //それぞれ0-1正規化を行う
    for(int h = 0; h < cfg.maxH; h++){
        for(int w = 0; w < cfg.maxW; w++){
            if(!map[h][w].isRoad) continue;//道路でなければスキップ
            map[h][w].toGoal = normalization(map[h][w].toGoal,maxtoGoal,minToGoal);
            map[h][w].toCenter = normalization(map[h][w].toCenter,maxtoCenter,minToCenter);
        }
    }

    for(int h = 0; h < cfg.maxH; h++){
        for(int w = 0; w < cfg.maxW; w++){
            if(!map[h][w].isRoad) continue;//道路でなければスキップ
            
            for(int i = 0; i < 8; i++){
                int movedH = h + dh[i];
                int movedW = w + dw[i];
                
                if(isInsideRoad(movedH,movedW)){
                    double heu1 = cfg.hrsWeight * ((double)1 / (map[movedH][movedW].toGoal + 0.01));
                    double heu2 = ((double)1 - cfg.hrsWeight) * ((double)1 / (map[movedH][movedW].toCenter + 0.01));
                    map[h][w].heuristic[i] = heu1 + heu2;
                }
            }
        }
    }
}

bool meshmap::isInsideRoad(int h, int w){
    return  h >= 0 && h < cfg.maxH && w >= 0 && w < cfg.maxW && map[h][w].isRoad;
}

double meshmap::normalization(double x, double min, double max){
    double res = (x - min) / (max - min);
    return res;
}
