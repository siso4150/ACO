#pragma once

#include <vector>

#include "cell.h"
#include "config.h"

using namespace std;

class meshmap{
    private:
        const Config& cfg;
        vector<vector<cell>> map;

        double maxtoCenter;
        double maxtoGoal;
        double minToCenter;
        double minToGoal;

        vector<int> dh = {-1,-1,0,1,1,1,0,-1};
        vector<int> dw = {0,1,1,1,0,-1,-1,-1};

    public:
        meshmap(Config&);
        void initRoad();
        void initRisk();
        void initCenter();
        void initHeuristic();

        bool isInsideRoad(int,int);
        double normalization(double,double,double);

        cell& operator()(int h, int w){//演算子オーバーロード (h,w)でセルの呼び出し
            return map[h][w];
        }

};