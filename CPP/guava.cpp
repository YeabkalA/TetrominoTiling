#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <fstream>
#include <memory>
#include <stdio.h>
#include <stdlib.h>

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

std::vector<std::vector<int>> combos;
int checker ;
typedef struct Lims
{
  int start, end, id;
  std::vector<std::vector<int>>* combos = nullptr;
} Lims;

void* Run(void *lims)
{
  Lims *lim = (Lims *)lims;
  int start = lim->start;
  int end = lim->end;
  std::string file_name = std::to_string(lim->id) + ".cnf";
  std::cout << "( " << start << "," << end << ") " << file_name << std::endl;
  std::cout << "Size of combos before forloop " << combos.size() << std::endl;
  for (int i = start; i < end; i++)
  {
    if(i >= combos.size()) break;
    std::cout << "i=" << i << std::endl;
    std::vector<int> avoided = combos[i];
    std::cout << "below" << std::endl;
    Grid grid(13, 9, &avoided);
    grid.Run(file_name);
    bool satisfiable = isSatisfiable(file_name);
    if (satisfiable) {
      std::cout << "YES IT IS SATISFIABLE" << file_name << std::endl; 
      grid.PrintGrid();
      pthread_exit(NULL);
    }
  }
  pthread_exit(NULL);
}

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

  std::vector<int> InsideMonominosIndices()
  {
    return insideMonominosIndices;
  }

  std::vector<int> PeripheralMonominosIndices()
  {
    return peripheralMonominosIndices;
  }

  void FillCombos()
  {
    checker = 3543;
    int runNo = 0;
    for (int i = 0; i < 15; i++)
    {
      for (int j = i+1; j < 15; j++)
      {
        for (int k = j+1; k < 15; k++)
        {
          for (int l = k+1; l < 15; l++)
          {
            for (int m = l+1; m < 15; m++)
            {
              if (runNo % 1000 == 0)
                std::cout << runNo << std::endl;
              runNo++;
              std::vector<int> avoided;
              avoided.push_back(peripheralMonominosIndices[i]);
              avoided.push_back(peripheralMonominosIndices[j]);
              avoided.push_back(peripheralMonominosIndices[k]);
              avoided.push_back(peripheralMonominosIndices[l]);
              avoided.push_back(insideMonominosIndices[m]);
              combos.push_back(avoided);
              // Grid grid(width, height, &avoided);
              // grid.Run();
              // bool satisfiable = isSatisfiable();
              // if(satisfiable) std::cout << "YES" << std::endl;
            }
          }
        }
      }
    }
    std::cout << "---" << runNo << std::endl;
    // for(auto avoided: combos) {
    //   Grid grid(13,9, &avoided);
    //   grid.Run();
    //   bool satisfiable = isSatisfiable();
    //   if(satisfiable) {
    //     grid.PrintGrid();
    //     return;
    //   }
    // }

    pthread_t threads[4];
    int step = combos.size() / 4 + 1;
    int start = 0;
    int threadInd = 0;
    std::cout << "Size of combos = " << combos.size() << std::endl;
    while (start < combos.size())
    {
      //std::cout << "IN  " << start << std::endl;
      Lims lim;
      //*lim->combos  = combos
      std::cout << "tr" << std::endl;
      lim.start = start;
      int end = start + step;
      if (end > combos.size())
      {
        lim.end = combos.size();
      } else {
        lim.end = end;
      }
      start += step;
      lim.id = threadInd;
      std::cout << "Chacha " << combos[start].size() << std::endl;
      pthread_create(&threads[threadInd], NULL, Run, (void *)&lim);
      threadInd = threadInd + 1;
    }

    for(int i=0; i<threadInd; i++) {
      pthread_join(threads[i], NULL);
    }
  }

private:
  std::vector<int> insideMonominosIndices, peripheralMonominosIndices;
  int width;
  int height;
};

int main()
{
  GuavaExperiment exp(13, 9);
  exp.FillCombos();
  
  // for(int i=0; i<10; i++) {
  //   Grid grid(4, i == 5 ? 5 : 4);

  //   grid.Run();
  //   if(isSatisfiable()) {
  //     std::cout << "yay" << std::endl;
  //   } else {
  //     std::cout << "no working" << std::endl;
  //   }
  // }
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
