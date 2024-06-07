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
enum MOVEMENT_FLAGS : int {
  NONE,
  CAPTURE,
  PROMOTION,
  SZACH,
  SZACH_MAT,
  CASTLE_SHORT,
  CASTLE_LONG
};
class piece {
public:
  char typ;
  char color;
  bool notMoved;
  piece(const char kolor, const char typ);
  ~piece();
  char symbol();
  char value();

private:
};

class board {
public:
  Matrix flags[2][4];
  piece *layout[8][8];
  bool playing;
  char points[2];
  char kings[2][2];
  char szach[2];
  board();
  ~board();
  void operator=(const board b);
  void clear();
  void print();
  bool flag(const unsigned char x, const unsigned char y);
  void flagAll(const unsigned char color);
  void flagAll();
  void switchPlayer();
  char move(const char px, const char py, const char x, const char y);
  void kinglogic(const char x, const char y);
  void setPfunction(char (*f)(char));
  void clearAllFlags();
  Matrix kingFlagging(const char color);

private:
  struct {
    char x, y;
    piece *passantable;
  } enPassant;
  char (*promotionFunction)(char);
  Matrix __k_f(const char x, const char y);
  Matrix _f_Fking(const unsigned char color);
  void flag_(const unsigned char x, const unsigned char y);
  void _f_guard(const unsigned char x, const unsigned char y);
  void _f_move(const unsigned char x, const unsigned char y);
  void _f_placed(const unsigned char x, const unsigned char y);
  void _f_attack(const unsigned char x, const unsigned char y);
  Matrix _f_attack_Extra(const char color);
  Matrix _f_layout(const char color);
  bool chk_layout(const unsigned char x, const unsigned char y);
};
#endif
