#include <vector>

#include "cell.h"
#include "config.h"

using namespace std;

class meshmap{
    private:
        const Config& cfg;
        vector<vector<cell>> map;

    public:
        meshmap(Config&);
        void initRoad();
        void initRisk();



};