#include <iostream>
#include <map>
#include <vector>

// Orientation for a tetromino
enum TetrominoOrientation { UP, DOWN, LEFT, RIGHT };

class Coordinate {
public:
  int X() { return x; }
  int Y() { return y; }
private:
  int x;
  int y;
};

class Tetromino {
public:
  Tetromino(TetrominoOrientation orntn, int startingLoc, 
            int width /* width of the grid where the tetromino lives */) {
    orientation = orntn;
    switch(orntn) {
    case UP:
      coveredCells = {startingLoc, startingLoc + 1, startingLoc + 2, startingLoc - width + 1};
      break;
    case LEFT:
      coveredCells = {startingLoc, startingLoc + width, startingLoc + 2*width, startingLoc - 1};
      break;
    case RIGHT:
      coveredCells = {startingLoc, startingLoc + width, startingLoc + 2*width, startingLoc + 1};
      break;
    case DOWN:
      coveredCells = {startingLoc, startingLoc + 1, startingLoc + 2, startingLoc + 1 + width};
      break;
    }
    cnsInd = startingLoc;
  }
  TetrominoOrientation Orientation() { return orientation; }
  int* CoveredCells() { return coveredCells; }
  int CNSInd() { return cnsInd; }
private:
  TetrominoOrientation orientation;
  int coveredCells[4];
  int cnsInd;
};

class Grid {
public:
  Grid(int w, int h) {
    width = w;
    height = h;
  }

  // Associate a cell and a tetromino.
  void AddMapping(int index, Tetromino* tetromino) {
    if (cellMap.find(index) == cellMap.end()) {
      std::vector<Tetromino*> map;
      map.push_back(tetromino);
      cellMap.insert(std::pair<int, std::vector<Tetromino*> >(index, map));
    } else {
      cellMap.find(index)->second.push_back(tetromino);
    }
  }

  void FillMap() {
    int noCells = width * height;
    for (int i = 1; i <= noCells; i++) {
      Tetromino t[] = {Tetromino(UP, i, width), Tetromino(DOWN, i, width), Tetromino(LEFT, i, width), Tetromino(RIGHT, i, width)};
      for(Tetromino tetr: t) {
	AddMapping(i, &tetr);
      }
    }
  }
private:
  std::map<int, std::vector<Tetromino*> > cellMap;
  std::vector<Tetromino*> tetrominos;
  int width;
  int height;
};
