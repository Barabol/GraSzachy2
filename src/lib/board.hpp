#ifndef __BINARY_BOARD__
#define __BINARY_BOARD__
#include "binaryMatrix.hpp"
enum FLAGS : int { MOVE, ATTACK, GUARD };
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
  bool notMoved;
  piece(char kolor, char typ);
  ~piece();
  char symbol();

private:
};

class board {
public:
  Matrix flags[2];
  piece *layout[8][8];
  bool playing;
  board();
  ~board();
  void operator=(board b);
  void clear();
  void print();
  void flag(int flagType, char color);

private:
  void _f_guard(char color);
  void _f_move(char color);
  void _f_attack(char color);
};
#endif
