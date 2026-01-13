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

    initRoad();//道路かどうかを入力
    initRisk();//リスク値を入力
    
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