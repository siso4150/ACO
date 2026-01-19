#pragma once

#include <vector>

#include "config.h"
#include "meshmap.h"

using namespace std;

struct Coord{
    int h,w,d;
};

class ant{
    private:

        const Config& cfg;
        meshmap& mp;
        
        Coord start;
        Coord goal;
        Coord cur;

        int dist;
        double risk;
        bool canMove;
        
        vector<double> pVec;//各セルへの確率を入れる配列
        vector<vector<int>> visit;
        vector<Coord> route;//作成した経路   

        vector<int> dh = {-1,-1,0,1,1,1,0,-1};
        vector<int> dw = {0,1,1,1,0,-1,-1,-1};

    public:
        ant(Config&,meshmap&);
        void initAnt();
        void search();
        void calcProb();
        int dirSelection();
        void resetAnt();

        int get_dist(){return dist;};
        double get_risk(){return risk;};
        vector<Coord>& get_route(){return route;};


};