#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <initializer_list>
#include <fstream>

#include "dimension.h"
#include "tetromino.h"
#include "grid.h"

int main() {
  Dimension d(4,4);
  std::vector<int> avoided;
  Grid grid(d, &avoided);
  Tetromino t(LEFT, 4, Dimension(10,10));
  t.SetIndex(3);
  std::cout << t.IsValid();
  grid.Run();
}
