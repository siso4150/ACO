#include <string>

using namespace std;

struct Config{
  
  double alpha;
  double beta;
  double rho;

  int antNum;
  int antGen;

  int startH;
  int startW;
  int goalH;
  int goalW;

  int maxH;
  int maxW;

  int upperDir;

  string mapCsv;//道路のcsv
  string somCsv;//SOMのcsv

};