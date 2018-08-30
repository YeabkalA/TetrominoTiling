#include <iostream>

// Orientation for a tetromino
enum Orienation { UP, DOWN, LEFT, RIGHT };

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
  Tetromino(Orienation orntn, int startingLoc, 
            int width /* width of the grid where the tetromino lives */,
            int ind /* index of the grid cell where the tetromino 'starts' */) {
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
    cnsInd = ind;
  }
  Orientation Orientation() { return orientation; }
  int[] CoveredCells() { return coveredCells; }
  int CNSInd() { return cnsInd; }
private:
  Orienation orienation;
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
    if (cellMap.find(ind) == cellMap.end()) {
      std::vector<Tetromino*> map;
      map.push_back(tetromino);
      cellMap.insert(std::pair<int, std::vector<Tetromino*>>(index, map));
    } else {
      cellMap.find(ind)->second.push_back(tetromino);
    }
  }
private:
  std::map<int, std::vector<Tetromino*>> cellMap;
  std::vector<Tetromino*> tetrominos;
  int width;
  int height;
};
