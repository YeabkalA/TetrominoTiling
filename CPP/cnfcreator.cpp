#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <initializer_list>
#include <fstream>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

#include "dimension.h"
#include "tetromino.h"
#include "grid.h"
#include "lang.cc"

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
            result += buffer.data();
    }
    return result;
}

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
