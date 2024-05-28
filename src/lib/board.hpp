#ifndef __BINARY_BOARD__
#define __BINARY_BOARD__
#include "binaryMatrix.hpp"
enum TYPES : int {
  PAWN = 0,
  KNIGHT = 1,
  BISHOP = 2,
  ROOK = 3,
  QUEEN = 4,
  KING = 5,
  WHITE = 0,
  BLACK = 1
};
class piece {
public:
  char typ;
  char color;
  piece(char kolor, char typ);
  ~piece();
  char symbol();

private:
};
class board {
public:
  Matrix flag[2];
  piece *layout[8][8];
  board();
  ~board();
  void operator=(board b);
  void clear();
  void print();

private:
};
#endif
