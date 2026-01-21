#pragma once

#include <vector>
#include <math.h>
#include <fstream>

#include "config.h"
#include "cell.h"
#include "ant.h"
#include "meshmap.h"

using namespace std;

class colony{
    private:
        Config& cfg;
        meshmap& mp;
        
        int minDist;
        double minRisk;
        double minCost;
        vector<Coord> bestRoute;

        vector<ant> antColony;

        double Q;

        vector<int> dh = {-1,-1,0,1,1,1,0,-1};
        vector<int> dw = {0,1,1,1,0,-1,-1,-1};
        
    public:
        colony(Config&,meshmap&);
        void initAntColony();

        void run();
        void updateSolution();
        void updatePheromone();

        bool isInsideRoad(int,int);

        void resultToCsv(string cfg_file);
        
    
};