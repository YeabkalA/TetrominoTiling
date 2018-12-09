/*
   Representation of a Tetromino
   A tetromino is uniquely described by the cells that it covers
*/
#include <unordered_set>
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
  bool IsValid(std::unordered_set<int>* avoidedCells = nullptr) {
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
