#include <iostream>

#include "colony.h"


colony::colony(Config& config,meshmap& mp):
        cfg(config),mp(mp),minDist(1e9),minRisk(1e9),minCost(1e9),Q(1){
    
    initAntColony();
}

void colony::initAntColony(){
    for(int i = 0; i < cfg.antNum; i++){
        antColony.push_back(ant(cfg,mp));
    }
}

void colony::updateSolution(){
    for(auto& ant : antColony){
        double cost = ant.get_dist() + ant.get_risk();
        
        if(minCost > cost){
            minDist = ant.get_dist();
            minRisk = ant.get_risk();
            minCost = cost;

            bestRoute.clear();
            for(auto coord : ant.get_route()){
                bestRoute.push_back(coord);
            }
        }
    }
    cout << "現在までの最適解 距離:" << minDist << " リスク:" << minRisk << " コスト:" << minCost << endl;
}

void colony::updatePheromone(){
    //フェロモンの蒸発
    for(int h = 0; h < cfg.maxH; h++){
        for(int w = 0; w < cfg.maxW; w++){
            if(mp(h,w).isRoad == false)continue;
            for(int i = 0; i < 8; i++){
                int movedH = h + dh[i];
                int movedW = w + dw[i];
                if(!(isInsideRoad(movedH,movedW)))continue;
                double eva = mp(h,w).distPheromone[i] * ((double)1 - cfg.rho);
                mp(h,w).distPheromone[i] = max(eva,cfg.lowerPhrmn);
                double reva = mp(h,w).riskPheromone[i] * ((double)1 - cfg.rho);
                mp(h,w).distPheromone[i] = max(reva,cfg.lowerPhrmn);
            }
        }
    }

    //Q値の更新
    int k = 0;
    int min = minCost;
    while(min > 0){
        min /= 10;
        k++;
    }
    Q = pow(10,k);


    //フェロモンの加算
    for(auto ant : antColony){
        double distAdd = Q / ant.get_dist();
        double riskAdd = Q / ant.get_risk();

        for(auto c : ant.get_route()){
            if(c.d == -1)break;//ゴールについたので、処理終わり
            
            mp(c.h,c.w).distPheromone[c.d] += distAdd;
            if(mp(c.h,c.w).distPheromone[c.d] >= 100) mp(c.h,c.w).distPheromone[c.d] = 100;
            
            
            mp(c.h,c.w).riskPheromone[c.d] += riskAdd;
            if(mp(c.h,c.w).riskPheromone[c.d] >= 100) mp(c.h,c.w).riskPheromone[c.d] = 100;
        }
    }
}

bool colony::isInsideRoad(int h, int w){
    return h >= 0 && h < cfg.maxH && w >= 0 && w < cfg.maxW && mp(h,w).isRoad;
}


void colony::run(){
    for(int iter = 0; iter < cfg.antGen; iter++){
        cout << iter+1 << "世代目:探索開始" << endl;

        int cnt = 0;
        for(auto& ant : antColony){
            //cout << cnt+1 << "体目 :";
            ant.search();
            //cout << "距離: " << ant.get_dist() << " リスク: " << ant.get_risk() << endl;
            cnt++;
        }

        
        //cout << "世代の更新処理" << endl;
        updateSolution();
        updatePheromone();
        for(auto& ant : antColony)ant.resetAnt();
    }
}

void colony::resultToCsv(string cfg_file){
    cfg_file.erase(cfg_file.size()-4,4);
    cfg_file.erase(0,12);
    
    cfg_file = "output_csv/" + cfg_file + "csv";
    ofstream file(cfg_file);
    
    if (!file.is_open()) {
        std::cerr << "Error: ファイルを開けませんでした: " << cfg_file << endl;;
        return;
    }

    file << "h,w" << "\n";
    
    for(auto c : bestRoute){
        file << c.h << "," << c.w << "\n";
    }

    file.close();
    cout << cfg_file << "に結果を出力" << endl;
}