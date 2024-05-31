#ifndef __BINARY_BOARD__
#define __BINARY_BOARD__
#include "binaryMatrix.hpp"
enum FLAGS : int { MOVE, ATTACK, GUARD, PLACED };
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
  Matrix flags[2][4];
  piece *layout[8][8];
  bool playing;
  board();
  ~board();
  void operator=(board b);
  void clear();
  void print();
  void flag(unsigned char x, unsigned char y);
  void flagAll(unsigned char color);
  void flagAll();

private:
  void flag_(unsigned char x, unsigned char y);
  void _f_guard(unsigned char x, unsigned char y);
  void _f_move(unsigned char x, unsigned char y);
  void _f_placed(unsigned char x, unsigned char y);
  void _f_attack(unsigned char x, unsigned char y);
  bool chk_layout(unsigned char x, unsigned char y, bool negated);
};
#endif
