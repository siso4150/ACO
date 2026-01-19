#include "colony.h"

colony::colony(Config& config,meshmap& mp):
        cfg(config),mp(mp),minDist(1e9),minRisk(1e9),Q(1){
    
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
            mp(c.h,c.w).riskPheromone[c.d] += riskAdd;
        }
    }
}

bool colony::isInsideRoad(int h, int w){
    return h >= 0 && h < cfg.maxH && w >= 0 && w < cfg.maxW && mp(h,w).isRoad;
}


void colony::run(){
    for(int iter = 0; iter < cfg.antGen; iter++){

        for(auto& ant : antColony)ant.search();

        updateSolution();
        updatePheromone();

        for(auto& ant : antColony)ant.resetAnt();
    }
}