#include "board.hpp"
#include "consts.hpp"
#ifndef __CHESS_BOT__
#define __CHESS_BOT__
typedef struct {
  char x, y;
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
};
#endif
