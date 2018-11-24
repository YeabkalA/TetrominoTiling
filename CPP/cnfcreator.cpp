#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <fstream>
#include "dimension.h"
#include "tetromino.h"
#include "grid.h"
#include "lang.cc"
#include "tools.cc"

int main() {
  std::cout << "TETROMINO SOLVER " << std::endl;
  std::cout << "Enter width of the grid" << std::endl;
  int w = 0;
  std::cin >> w;
  std::cout << "Enter height of the grid" << std::endl;
  int h = 0;
  std::cin >> h;
  Dimension d(w,h);

  Query q(h, w);
  q.AcceptQueries();


  std::vector<int> avoided(q.Container().begin(), q.Container().end());
  Grid grid(d, &avoided);
  grid.Run();
  std::string result = exec("./minisat problem.cnf");
  if (result.find("UNSATISFIABLE") !=  std::string::npos) {
    std::cout << "UNSATISFIABLE" << std::endl;
  } else {
    std::cout << "SATISFIABLE" << std::endl;
  }
}
