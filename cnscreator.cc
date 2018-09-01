#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

// Orientation for a tetromino
enum TetrominoOrientation { UP, DOWN, LEFT, RIGHT };

class Tetromino {
public:
  Tetromino(TetrominoOrientation orntn, int startingLoc, 
            int width /* width of the grid where the tetromino lives */) {
    orientation = orntn;
    switch(orntn) {
    case UP:
      coveredCells[0] = startingLoc;
      coveredCells[1] = startingLoc + 1;
      coveredCells[2] = startingLoc + 3;
      coveredCells[3] = startingLoc - width + 1;
      break;
    case LEFT:
      coveredCells[0] = startingLoc;
      coveredCells[1] = startingLoc + width;
      coveredCells[2] = startingLoc + 2*width;
      coveredCells[3] = startingLoc - 1;
      break;
    case RIGHT:
      coveredCells[0] = startingLoc;
      coveredCells[1] = startingLoc + width;
      coveredCells[2] = startingLoc + 2*width;
      coveredCells[3] = startingLoc + 1;
      break;
    case DOWN:
      coveredCells[0] = startingLoc;
      coveredCells[1] = startingLoc + 1;
      coveredCells[2] = startingLoc + 3;
      coveredCells[3] = startingLoc + width + 1;
      break;
    }
    cnsInd = startingLoc;
  }
  
  TetrominoOrientation Orientation() { return orientation; }
  
  int* CoveredCells() { return coveredCells; }
  
  int CNSInd() { return cnsInd; }

  bool CoversBadCell() {
    for(int cell: coveredCells) {
      if (cell < 0) {
	return true;
      }
    }
    return false;
  }

  void SetIndex(int ind) { cnsInd = ind; }

  void Print() {
    std::cout << "Orientation " << orientation << ", covered cells are:\n";
    for (int cell: coveredCells) { std::cout << cell << "-"; }
    std::cout << std::endl;
  }
private:
  TetrominoOrientation orientation;
  int coveredCells[4];
  int cnsInd;
};

class Grid {
public:
  Grid(int w, int h, std::vector<int> avoidedList) {
    width = w;
    height = h;
    avoidedCells = avoidedList;
  }

  // Associate a cell and a tetromino.
  void AddMapping(int index, Tetromino* tetromino) {
    std::cout << "Trying to add to map for ind = " <<index << std::endl;
    if (std::find(avoidedCells.begin(), avoidedCells.end(), index) != avoidedCells.end() ||
	tetromino->CoversBadCell()) {
      return;
    }
    if (cellMap.find(index) == cellMap.end()) {
      std::vector<Tetromino*> map;
      tetromino->SetIndex(index++);
      map.push_back(tetromino);
      cellMap.insert(std::pair<int, std::vector<Tetromino*> >(index, map));
    } else {
      tetromino->SetIndex(index++);
      cellMap.find(index)->second.push_back(tetromino);
    }
  }

  void FillMap() {
    std::cout << "Inside fill map " << std::endl;
    int noCells = width * height;
    for (int i = 1; i <= noCells; i++) {
      Tetromino t[] = {Tetromino(UP, i, width), Tetromino(DOWN, i, width), Tetromino(LEFT, i, width), Tetromino(RIGHT, i, width)};
      for(Tetromino tetr: t) {
	AddMapping(i, &tetr);
      }
    }
  }

  void Print() {
    for(auto i = cellMap.begin(); i != cellMap.end(); i++) {
      std::cout << i->first << "-->\n";
      for (Tetromino* t: i->second) {
	t->Print();
      }
      std::cout << "\n**************\n";
    }
  }
private:
  std::map<int, std::vector<Tetromino*> > cellMap;
  std::vector<Tetromino*> tetrominos;
  int width;
  int height;
  int index;
  std::vector<int> avoidedCells;
};

int main() {
  std::vector<int> avoided;
  avoided.push_back(1);
  Grid grid(10, 10, avoided);
  grid.FillMap();
  std::cout << "Hello World\n";
  //  grid.Print();
}
