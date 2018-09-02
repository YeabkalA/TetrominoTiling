#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

// Orientation for a tetromino
enum TetrominoOrientation { UP, DOWN, LEFT, RIGHT };

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

class Tetromino {
public:
  Tetromino(TetrominoOrientation orntn, int startingLoc, Dimension dimension) {
    gridDimension = dimension;
    int width = dimension.Width();
    orientation = orntn;
    start = startingLoc;

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
    cnsInd = startingLoc;
  }
  
  TetrominoOrientation Orientation() { return orientation; }

  int* CoveredCells() { return coveredCells; }

  int CNSInd() { return cnsInd; }

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
      return start % gridDimension.Width() != 0 && gridDimension.Size() - start >= 2*gridDimension.Width(); 
    } else if (orientation == LEFT) {
      return (start - 1) % gridDimension.Width() !=0 && gridDimension.Size() - start >= 2*gridDimension.Width();
    } else if (orientation == UP) {
      return (start > gridDimension.Width()) && (((start + 1) % gridDimension.Width()) != 0) && (start % gridDimension.Width() != 0);
    } else {
      return gridDimension.Size() - start < gridDimension.Width() && gridDimension.Width() != 0 && start % gridDimension.Width() != 0;
    }
  }

  void SetIndex(int ind) { cnsInd = ind; }

  void Print() {
    std::cout << "Orientation " << orientation << ", covered cells are:\n";
    for (int cell: coveredCells) { std::cout << cell << "-"; }
    std::cout << std::endl;
    std::cout << "-------------------------\n";
  }
private:
  TetrominoOrientation orientation;
  int coveredCells[4];
  int cnsInd;
  Dimension gridDimension;
  int start = 0;
};

class Grid {
public:
  Grid(Dimension d) {
    dimension = d;
  }

  Grid(int w, int h) {
    dimension = Dimension(w,h);
  }

  // Associate a cell and a tetromino.
  void AddMapping(int index, Tetromino tetromino) {
    tetromino.Print();
    if (!tetromino.IsValid()) {
      return;
    }
    if (cellMap.find(index) == cellMap.end()) {
      cellMap.insert(std::pair<int, std::vector<Tetromino> >(index, std::vector<Tetromino>()));
      tetromino.SetIndex(index++);
      cellMap[index].push_back(tetromino);\
    } else {
      tetromino.SetIndex(index++);
      cellMap[index].push_back(tetromino);
    }
  }

  void FillMap() {
    std::cout << "Inside fill map " << std::endl;
    for (int i = 1; i <= dimension.Size(); i++) {
      if (std::find(avoidedCells.begin(), avoidedCells.end(), i) != avoidedCells.end()) {
        continue;
      }
      Tetromino t[] = {Tetromino(UP, i, dimension), Tetromino(DOWN, i, dimension), Tetromino(LEFT, i, dimension), Tetromino(RIGHT, i, dimension)};
      for(Tetromino tetr: t) {
	AddMapping(i, tetr);
      }
    }
  }

  void AddAvoidedCell(int ind) {
    avoidedCells.push_back(ind);
  }

  void Print() {
    for(auto i = cellMap.begin(); i != cellMap.end(); i++) {
      std::cout << i->first << "-->\n";
      for (Tetromino t: i->second) {
	t.Print();
      }
      std::cout << "\n**************\n";
    }
  }
private:
  std::map<int, std::vector<Tetromino> > cellMap;
  Dimension dimension;
  int index;
  std::vector<int> avoidedCells;
};

int main() {
  Dimension d(4,4);
  Grid grid(d);
  grid.AddAvoidedCell(1);
  (&grid)->FillMap();
  std::cout << "Hello World\n";
  grid.Print();
}
