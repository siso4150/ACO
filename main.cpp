#include <iostream>
#include <fstream>

#include "config.h"
#include "json.hpp"
#include "ant.h"
#include "colony.h"
#include "meshmap.h"

using namespace std;
using json = nlohmann::json;


int main() {
    //ハイパーパラメータのロード
    Config config;

    //ファイルの読み込み
    ifstream f("hyperparameter.json");
    //jsonのパース
    json data = json::parse(f);

    //各データを代入
    config.alpha = data["alpha"].get<double>();
    config.beta  = data["beta"].get<double>();
    config.rho   = data["rho"].get<double>();

    config.startH = data["startH"].get<int>();
    config.startW = data["startW"].get<int>();

    config.maxH = data["maxH"].get<int>();
    config.maxW = data["maxW"].get<int>();

    config.upperDir = data["upperDir"].get<int>();

    meshmap meshmap(config);
    colony colony(config,meshmap);
    
    colony.run();
    
    return 0;
}
