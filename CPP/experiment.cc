#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <fstream>
#include <memory>

#include <string>
#include <initializer_list>
#include <fstream>
#include <cstdio>
#include <stdexcept>

#include "dimension.h"
#include "tetromino.h"
#include "grid.h"
#include "lang.cc"

const int ribbon_width = 13;

class Experiment_13 {
public:
  Experiment_13(int w) : base_square_width(w), total_width(w + ribbon_width) {
    int lower_start = (ribbon_width + base_square_width) * base_square_width + 1;
    std::cout << "Lower start = " << lower_start << std::endl;
    for(int i=lower_start; i<=total_width*total_width; i++) {
      ribbon.push_back(i);
    }
    // Fill the right side
    for(int i=base_square_width + 1;
      i<=(base_square_width * (base_square_width + ribbon_width)); i++) {
      ribbon.push_back(i);
      if(i % (base_square_width+ribbon_width) == 0) {
        i += base_square_width;
      }
    }

    // Fill base sqaure
    for(int i=1; i<=(total_width * base_square_width) - ribbon_width; i++) {
      base_square.push_back(i);
      if(i%base_square_width == 0) {
        i += ribbon_width;
      }
    }
  }

  void RunExperiment(){
    Grid grid = Grid(total_width, total_width, &base_square);
    int prev = ribbon[0];
    for(int i=1; i<ribbon.size(); i++) {
      grid.AddAvoidedCell(prev);
      std::cout << "...Running experiment with " << prev << std::endl;
      grid.Run();
      if(isSatisfiable()) {
        std::cout << "Found 1: " << prev << std::endl;
      } else {
        std::cout << "Expriment failing at " << prev << std::endl;
      }
      grid.RemoveAvoidedCell(prev);
      prev = ribbon[i];
    }
  }

  void PrintRibbon() {
    std::cout << "Ribbons' size = " << ribbon.size() << std::endl;
    printVector(ribbon);
  }
  void PrintBaseSquare() {
    std::cout << "Base size = " << base_square.size() << std::endl;
    printVector(base_square);
  }

private:
  std::vector<int> ribbon;
  std::vector<int> base_square;
  int base_square_width;
  int total_width;
};

int main() {
  Experiment_13 exp(4);
  exp.RunExperiment();
}
