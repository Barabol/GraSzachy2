#include "board.hpp"
#include "consts.hpp"
#ifndef __CHESS_BOT__
#define __CHESS_BOT__
typedef struct {
  char x, y, boneBreaker;
} moveLoc;
class bot {
public:
  bot(class board *usedBoard, selection *selected,
      char (*makeMove)(selection *, board *, char, char));
  void getMove();

private:
  board *usedBoard;
  selection *selected;
  char (*makeMove)(selection *selected, board *mainboard, char x, char y);
  char ostatniRatunek();
  char kingMustMove();
  char materialAnichilation();
  char unguardedAttack();
  void krazyMove();
  char kindaSafeMove();
  typedef struct {
    char x, y, mx, my, rato;
  } MoveListing;
};
#endif
