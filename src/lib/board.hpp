#ifndef __BINARY_BOARD__
#define __BINARY_BOARD__
enum TYPES : int {
  PAWN = 0,
  KNIGHT = 1,
  BISHOP = 2,
  ROOK = 3,
  QUEEN = 4,
  KING = 5
};

class piece {
public:
  piece();
  ~piece();

private:
};
class board {
public:
  board();
  ~board();
  void operator=(board b);
  void clear();
  void print();

private:
};
#endif
