#include <iostream>
#include <fstream>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;


struct Config{
  
  double alpha;
  double beta;
  double rho;

};

int main() {
    //ハイパーパラメータのロード
    Config config;

    //ファイルの読み込み
    ifstream f("hyperparameter.json");
    //jsonのパース
    json data = json::parse(f);

    //各データを代入
    config.alpha = data["alpha"];
    config.beta  = data["beta"];
    config.rho   = data["rho"];
    
    return 0;
}
