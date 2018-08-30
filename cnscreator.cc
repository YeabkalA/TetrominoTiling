#include <iostream>

// Orientation for a tetromino
enum Orienation { UP, DOWN, LEFT, RIGHT };

struct Coordinate {
public:
  int X() { return x; }
  int Y() { return y; }
private:
  int x;
  int y;
};

struct Tetromino {
public:
  Tetromino(Orienation orntn, int startingLoc) {
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
  }
  Orientation Orientation() { return orientation; }
  int[] CoveredCells() { return coveredCells; }
private:
  Orienation orienation;
  int coveredCells[4];
};

void PrintCNS(int width, int height, int avoided[]);
