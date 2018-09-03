#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <initializer_list>

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
      return (gridDimension.Size() - start < gridDimension.Width())
	&& gridDimension.Width() != 0
	&& start % gridDimension.Width() != 0;
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
  Grid(Dimension d, std::vector<int>* avoided)
    : dimension(d), index(0) {
    if(avoided != nullptr) {
      for(int cell: *avoided) { avoidedCells.push_back(cell); }
    }
  }

  Grid(int w, int h, std::vector<int>* avoided)
    : dimension(Dimension(w,h)), index(0) {
    if (avoided != nullptr) { 
      for(int cell: *avoided) { avoidedCells.push_back(cell); }
    }
  }

  // Map each valid cell to all the tetrominos that cover it.
  void ConstructMap() {
    for (auto t: tetrominos) {
      for (int cell: t.coveredCells) {
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

  void GenerateCoveredClauses() {
    
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
  avoided.push_back(1);
  Grid grid(d, &avoided);
  grid.FillTetrominosList();
  grid.ConstructMap();
  grid.Print();
}
