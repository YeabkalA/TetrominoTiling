#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <fstream>
#include <memory>
#include<stdio.h>
#include<stdlib.h>

#include <string>
#include <initializer_list>
#include <fstream>
#include <cstdio>
#include <stdexcept>

#include "dimension.h"
#include "tetromino.h"
#include "grid.h"
#include "lang.cc"

#define IN_MONOMINOS 1
#define PERIPHERAL_MONOMINOS 4
#define TOTAL_MONOMINOS 5
class GuavaExperiment
{
public:
  GuavaExperiment(int w, int h) : width(w), height(h)
  {
    for (int i = 1; i <= width * height; i++)
    {
      if (i % width == 0 || i > (height - 1) * width)
        peripheralMonominosIndices.push_back(i);
      else
        insideMonominosIndices.push_back(i);
    }
  }

  std::vector<int> InsideMonominosIndices() {
    return insideMonominosIndices;
  }

  std::vector<int> PeripheralMonominosIndices() {
    return peripheralMonominosIndices;
  }

  void FillCombos()
  {
    int runNo = 0;
    for (int i = 0; i < peripheralMonominosIndices.size(); i++)
    {
      for (int j = 0; j < peripheralMonominosIndices.size(); j++)
      {
        for (int k = 0; k < peripheralMonominosIndices.size(); k++)
        {
          for (int l = 0; l < peripheralMonominosIndices.size(); l++)
          {
            for (int m = 0; m < insideMonominosIndices.size(); m++)
            {
              if(runNo % 1000 == 0) std::cout << runNo << std::endl;
              runNo++;
              std::vector<int> avoided;
              avoided.push_back(peripheralMonominosIndices[i]);
              avoided.push_back(peripheralMonominosIndices[j]);
              avoided.push_back(peripheralMonominosIndices[k]);
              avoided.push_back(peripheralMonominosIndices[l]);
              avoided.push_back(insideMonominosIndices[m]);
              combos.push_back(avoided);
              /*Grid grid(width, height, &avoided);
              grid.Run();
              bool satisfiable = isSatisfiable();
              if(satisfiable) std::cout << "YES" << std::endl;*/
            }
          }
        }
      }
    }
    std::cout << "---" << runNo << std::endl;
  }

  void Run() {
    
  }

private:
  std::vector<int> insideMonominosIndices, peripheralMonominosIndices;
  int width;
  int height;
  std::vector<std::vector<int>> combos;
};

int main() {
  GuavaExperiment exp(13, 9);
  exp.Run();
  // Grid grid(16,8);
  // //for(int n: exp.InsideMonominosIndices()) grid.AddAvoidedCell(n);
  // //for(int n: exp.PeripheralMonominosIndices()) grid.AddAvoidedCell(n);
  // grid.PrintGrid();
  // grid.Run();
  // bool satisfiable = isSatisfiable();
  // if (satisfiable)
  //   std::cout << "YES" << std::endl;

  //exp.Run();
}
