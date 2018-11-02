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