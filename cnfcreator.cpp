#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <initializer_list>
#include <fstream>

// Orientation for a tetromino
enum TetrominoOrientation { UP, DOWN, LEFT, RIGHT };

// Dimension descriptor.
class Dimension {
public:
  Dimension(int w = 4, int h = 4) {
    width = w;
    height = h;
  }
  int Width() { return width; }
  int Height() { return height; }
  int Size() { return width * height; }

private:
  int width;
  int height;
};

/* 
   Representation of a Tetromino
   A tetromino is uniquely described by the cells that it cover
*/

class Tetromino {
public:
  Tetromino(TetrominoOrientation orntn, int startingLoc,
   Dimension dimension) : gridDimension(dimension),
  start(startingLoc),
  orientation(orntn) {
    int width = dimension.Width();

    // Cover the appropriate cells based on the orientation
    // and starting location of the tetromino.
    switch(orntn) {
      case UP:
      coveredCells[0] = startingLoc;
      coveredCells[1] = startingLoc + 1;
      coveredCells[2] = startingLoc + 2;
      coveredCells[3] = startingLoc - width + 1;
      break;
      case LEFT:
      coveredCells[0] = startingLoc;
      coveredCells[1] = startingLoc + width;
      coveredCells[2] = startingLoc + 2*width;
      coveredCells[3] = startingLoc + width - 1;
      break;
      case RIGHT:
      coveredCells[0] = startingLoc;
      coveredCells[1] = startingLoc + width;
      coveredCells[2] = startingLoc + 2*width;
      coveredCells[3] = startingLoc + width + 1;
      break;
      case DOWN:
      coveredCells[0] = startingLoc;
      coveredCells[1] = startingLoc + 1;
      coveredCells[2] = startingLoc + 2;
      coveredCells[3] = startingLoc + width + 1;
      break;
    }
  }

  // Check that a tetromino does not cover a 'bad' cell.
  // 'bad'cell - out of bounds/avoided cell.
  bool IsValid(std::vector<int>* avoidedCells = nullptr) {
    if (avoidedCells != nullptr) {
      for (int cell : coveredCells) {
        if (std::find(avoidedCells->begin(), avoidedCells->end(), cell) != avoidedCells->end()) {
          return false;
        }
      }
    }

    // Check that the tetromino lies inside its grid.
    if (start < 1 || start > gridDimension.Size()) {
      return false;
    }

    if (orientation == RIGHT) {
      return start % gridDimension.Width() != 0
      && gridDimension.Size() - start >= 2*gridDimension.Width(); 
    } else if (orientation == LEFT) {
      return (start - 1) % gridDimension.Width() !=0
      && gridDimension.Size() - start >= 2*gridDimension.Width();
    } else if (orientation == UP) {
      return (start > gridDimension.Width())
      && (((start + 1) % gridDimension.Width()) != 0)
      && (start % gridDimension.Width() != 0);
    } else {
      return (gridDimension.Size() - start > gridDimension.Width())
      && (((start + 1) % gridDimension.Width()) != 0)
      && (start % gridDimension.Width() != 0);
    }
  }

  TetrominoOrientation Orientation() { return orientation; }

  int CNFIndex() { return cnfIndex; }

  void SetIndex(int ind) { cnfIndex = ind; }

  void Print() {
    std::cout << "Orientation " << orientation << ", covered cells are:\n";
    for (int cell: coveredCells) { std::cout << cell << "-"; }
      std::cout << std::endl;
    std::cout << "-------------------------\n";
  }

  int coveredCells[4];
private:
  TetrominoOrientation orientation;
  int cnfIndex;
  Dimension gridDimension;
  int start = 0;
};

/*
  Representation of a grid.
*/
class Grid {
public:
  Grid(Dimension d, std::vector<int>* avoided = nullptr)
  : dimension(d), index(1) {
    if(avoided != nullptr) {
      for(int cell: *avoided) { avoidedCells.push_back(cell); }
    }
}

Grid(int w, int h, std::vector<int>* avoided = nullptr)
: dimension(Dimension(w,h)), index(1) {
  if (avoided != nullptr) { 
    for(int cell: *avoided) { avoidedCells.push_back(cell); }
  }
}

  // Map each valid cell to all the tetrominos that cover it.
void ConstructMap() {
  for (auto t: tetrominos) {
    for (int cell: t.coveredCells) {
      if (cell < 1 || cell > dimension.Size()) {
        continue;
      }
      if(cellMap.find(cell) == cellMap.end()) {
        cellMap.insert(std::pair<int, std::vector<Tetromino> >(cell, std::vector<Tetromino>()));
      }
      cellMap[cell].push_back(t);
    }
  }
}

  // Populate list of all possibly used tetrominos.
void FillTetrominosList() {
  for (int i = 1; i <= dimension.Size(); i++) {
    if (std::find(avoidedCells.begin(), avoidedCells.end(), i) != avoidedCells.end()) {
      continue;
    }
    Tetromino t[] = {Tetromino(UP, i, dimension),
     Tetromino(DOWN, i, dimension),
     Tetromino(LEFT, i, dimension),
     Tetromino(RIGHT, i, dimension)};
     for(Tetromino tetr: t) {
       if (tetr.IsValid(&avoidedCells)) {
         tetr.SetIndex(index++);
         tetrominos.push_back(tetr);
       }
     }
   }
 }

 void AddAvoidedCell(int ind) { avoidedCells.push_back(ind); }

 void GenerateCoveredClauses(int* size) {
  for (auto it=cellMap.begin(); it!=cellMap.end(); ++it) {
    coveredClauses.push_back("c 'COVERED'" + std::to_string(it->first));
    auto tetrs = it->second;
    std::string out;
    for (auto t: tetrs) {
      out = out + std::to_string(t.CNFIndex()) + " ";
    }
    out = out + " 0";
    coveredClauses.push_back(out);
    *size = *size + 1;
  }
}

void GenerateOnceClauses(int* size) {
  int count = 0;
  for (auto it=cellMap.begin(); it!=cellMap.end(); ++it) {
    auto tetrs = it->second;
    for(int i = 0; i < tetrs.size() -1; i++) {
      for(int j = i + 1; j < tetrs.size(); j++) {
        auto t1 = tetrs[i];
        auto t2 = tetrs[j];
        onceClauses.push_back(std::to_string(-1 * t1.CNFIndex()) + " "
          + std::to_string(-1 * t2.CNFIndex()) + " 0");
        *size = *size + 1;
      }
    }
  }
}

void GenereateCNFFile(int size, const std::string& fileName) {
  std::ofstream cnf_file;
  cnf_file.open(fileName);

  std::string first_line = "p cnf " + std::to_string(tetrominos.size())
  + " " + std::to_string(size);

  cnf_file << first_line << std::endl;
  for (const std::string& clause: coveredClauses) {
    cnf_file << clause << std::endl;
  }
  for (const std::string& clause: onceClauses) {
    cnf_file << clause << std::endl;
  }
  cnf_file.close();
}

void Run(const std::string& file_name = "problem.cnf") {
  FillTetrominosList();
  ConstructMap();
  int size = 0;
  GenerateCoveredClauses(&size);
  GenerateOnceClauses(&size);
  GenereateCNFFile(size, file_name);
}

void Print() {
  for(auto i = cellMap.begin(); i != cellMap.end(); i++) {
    std::cout << i->first << "-->\n";
    auto list = cellMap[i->first];
    for (Tetromino t: list) {
     t.Print();
   }
   std::cout << "\n**************\n";
 }
}

private:
  // Map of each cell to all tetrominos covering it.
  std::map<int, std::vector<Tetromino> > cellMap;
  // Dimension of the grid.
  Dimension dimension;
  // List of all possible tetrominos for the grid.
  std::vector<Tetromino> tetrominos;
  // Covered Clauses.
  std::vector<std::string> coveredClauses;
  // Once Clauses.
  std::vector<std::string> onceClauses;
  // Index (cnf index) of a tetromino to be added.
  // Constantly updated as new tetrominos are added
  // since the cnf index of a tetromino is unique.
  int index;
  // List of all avoided cells in map construction.
  // Allows usage of different shapes besides rectangles.
  std::vector<int> avoidedCells;
};

int main() {
  Dimension d(4,4);
  std::vector<int> avoided;
  Grid grid(d, &avoided);
  
  grid.Run();
}
