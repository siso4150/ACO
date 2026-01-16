#include <vector>

using namespace std;

struct cell{
    bool isRoad = false;//道路かどうか
    bool isCenter = false;//道路中心線かどうか

    vector<double> pheromone;//フェロモン値
    vector<double> heuristic;//ヒューリスティック値

    double toCenter;
    double toGoal;

    double riskVal;
};