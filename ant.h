#include <vector>

using namespace std;

struct Coord{
    int h,w;
};

class ant{
    private:
        
        Coord start;
        Coord goal;
        Coord currnt;

        int dist;
        double risk;
        bool canMove;
        
        vector<bool> probArray;
        vector<Coord> route;//作成した経路   

        



    public:
        ant();
        void initAnt();

};