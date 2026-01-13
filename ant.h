#include <vector>

#include "config.h"

using namespace std;

struct Coord{
    int h,w,d;
};

class ant{
    private:

        const Config& cfg;
        
        Coord start;
        Coord goal;
        Coord cur;

        int dist;
        double risk;
        bool canMove;
        
        vector<bool> pVec;//各セルへの確率を入れる配列
        vector<Coord> route;//作成した経路   

    public:
        ant(Config& config);
        void initAnt();
        void search();
        void calcProb();
        int dirSelection();

};