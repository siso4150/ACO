#include <iostream>
#include <fstream>

#include "config.h"
#include "json.hpp"
#include "ant.h"
#include "colony.h"
#include "meshmap.h"

using namespace std;
using json = nlohmann::json;


int main(int argc, char* argv[]) {//コマンドラインからjsonの指定

    //ハイパーパラメータのロード
    Config config;
    string config_file = "hyperparameter.json";

    if(argc >= 2){
        config_file = argv[1];
    }
    //ファイルの読み込み
    ifstream f(config_file);
    if (!f.is_open()) {
        cerr << "Error: Failed to open " << config_file << endl;
        return 1;
    }
    //jsonのパース
    json data = json::parse(f);

    //各データを代入
    config.alpha = data["alpha"].get<double>();
    config.beta  = data["beta"].get<double>();
    config.rho   = data["rho"].get<double>();
    config.antNum = data["antNum"].get<int>();
    config.antGen = data["antGen"].get<int>();

    config.startH = data["startH"].get<int>();
    config.startW = data["startW"].get<int>();

    config.goalH = data["goalH"].get<int>();
    config.goalW = data["goalW"].get<int>();

    config.maxH = data["maxH"].get<int>();
    config.maxW = data["maxW"].get<int>();

    config.upperDir = data["upperDir"].get<int>();
    config.mapCsv = data["mapCsv"].get<string>();
    config.somCsv = data["somCsv"].get<string>();
    config.centerCsv = data["centerCsv"].get<string>();

    config.outputCsv = data["outputCsv"].get<string>();

    config.phrWeight = data["phrWeight"].get<double>();
    config.hrsWeight = data["hrsWeight"].get<double>();
    config.lowerPhrmn = data["lowerPhrmn"].get<double>();

    cout << "jsonデータ読取、config設定完了" << endl;

    
    meshmap meshmap(config);
    cout << "meshmapクラスの初期化完了" << endl;
    colony colony(config,meshmap);
    cout << "Colony,Antクラスの初期化完了" << endl;
    
    cout << "ACO初期化終了,実行可能" << endl; 
    colony.run();

    cout << "結果を出力" << endl;
    colony.resultToCsv(argv[1]);
    
    return 0;
}
