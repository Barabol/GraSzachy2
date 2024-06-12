#include "chessBot.hpp"
#include "board.hpp"
#include "consts.hpp"
#include <ctime>
#include <random>
#include <stdio.h>
bot::bot(class board *usedBoard, selection *selected,
         char (*makeMove)(selection *, board *, char, char)) {
  this->usedBoard = usedBoard;
  this->makeMove = makeMove;
  std::srand(time(nullptr));
}
void bot::getMove() {
  static unsigned char recursionLimit = RECURSION_LIMIT;
  moveLoc holder;
  usedBoard->flagAll();
  holder.x = std::rand() & 7;
  holder.y = std::rand() & 7;
  printf("bot: %d,%d\n", holder.x, holder.y);
  if (!usedBoard->flags[usedBoard->playing][PLACED].value(holder.x, holder.y) &&
      recursionLimit) {
    recursionLimit--;
    return this->getMove();
  }
  this->makeMove(selected, usedBoard, holder.x, holder.y);
  if (!selected->moves.allValues()) {
    recursionLimit--;
    return this->getMove();
  }
kochamGoto:
  holder.x = std::rand() & 7;
  holder.y = std::rand() & 7;
  if (!selected->moves.value(holder.x, holder.y))
    goto kochamGoto;
  this->makeMove(selected, usedBoard, holder.x, holder.y);
  recursionLimit = RECURSION_LIMIT;
}
