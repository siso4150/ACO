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

    public:
        meshmap(Config&);
        void initRoad();
        void initRisk();
        void initCenter();
        void initHeuristic();

        bool isInsideRoad(int,int);
        double normalization(double,double,double);

};