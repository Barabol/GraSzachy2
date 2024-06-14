#include "chessBot.hpp"
#include "binaryMatrix.hpp"
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
  if (usedBoard->szach[usedBoard->playing] == -1) {
#ifdef DEBUG
    puts("szach mat z bota");
#endif
    return;
  }
  if (usedBoard->szach[usedBoard->playing] && this->kingMustMove()) {
#ifdef DEBUG
    puts("król uciekł z szachu");
#endif
    return;
  }
  if (this->unguardedAttack()) {
#ifdef DEBUG
    puts("atak na nie broniony");
#endif
    return;
  }
  if (this->materialAnichilation()) {
#ifdef DEBUG
    puts("bonus materiału");
#endif
    return;
  }
  if (this->kindaSafeMove()) {
#ifdef DEBUG
    puts("??");
#endif
    return;
  }
  this->krazyMove();
}
char bot::kingMustMove() {
  piece *king = usedBoard->layout[usedBoard->kings[usedBoard->playing][0]]
                                 [usedBoard->kings[usedBoard->playing][1]];
  this->makeMove(selected, usedBoard, usedBoard->kings[usedBoard->playing][0],
                 usedBoard->kings[usedBoard->playing][1]);
  moveLoc kingMoves[10];
  for (int x = 0; x < 10; x++) {
    kingMoves[x].x = -1;
    kingMoves[x].y = -1;
    kingMoves[x].boneBreaker = 0;
  }
  char boneBreakers = 0;
  char helper = 0;
  if (selected->moves.allValues()) {
    for (int x = 0; x < 8; x++)
      for (int y = 0; y < 8; y++) {
        if (selected->moves.value(x, y)) {
          kingMoves[helper].x = x;
          kingMoves[helper].y = y;
          if (usedBoard->layout[x][y] &&
              usedBoard->layout[x][y]->color != king->color) {
            kingMoves[helper].boneBreaker = true;
            boneBreakers++;
          }
          helper++;
        }
      }
  }
  if (!helper)
    return 0;
  if (boneBreakers)
    for (int x = 0; x < 10; x++)
      if (kingMoves[x].boneBreaker)
        if (this->makeMove(selected, usedBoard, kingMoves[x].x, kingMoves[x].y))
          return 1;
  for (int x = 0; x < 10; x++)
    if (kingMoves[x].x != -1 &&
        this->makeMove(selected, usedBoard, kingMoves[x].x, kingMoves[x].y))
      return 1;
  return 0;
}
void bot::krazyMove() {
  static unsigned char recursionLimit = RECURSION_LIMIT;
  moveLoc holder;
  usedBoard->flagAll();
  holder.x = std::rand() & 7;
  holder.y = std::rand() & 7;
#ifdef DEBUG
  printf("bot: %d,%d\n", holder.x, holder.y);
#endif
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
char bot::materialAnichilation() {
  usedBoard->flagAll();
  char value;
  Matrix holder = usedBoard->flags[usedBoard->playing][ATTACK] &
                  (usedBoard->flags[(~usedBoard->playing) & 1][GUARD] &
                   usedBoard->flags[usedBoard->playing][ATTACK]);
  if (!holder.allValues())
    return 0;
  usedBoard->clearAllFlags();
  usedBoard->flagAll((~usedBoard->playing) & 1);
  for (int x = 0; x < 8; x++)
    for (int y = 0; y < 8; y++) {
      if (usedBoard->layout[x][y] &&
          usedBoard->playing == usedBoard->layout[x][y]->color) {
        value = usedBoard->layout[x][y]->value();
        usedBoard->flag(x, y);
        if ((usedBoard->flags[usedBoard->playing][ATTACK] & holder)
                .allValues()) {
          this->makeMove(selected, usedBoard, x, y);
          for (int z = 0; z < 8; z++)
            for (int v = 0; v < 8; v++)
              if (holder.value(z, v) &&
                  usedBoard->layout[z][v]->value() > value) {
                if (this->makeMove(selected, usedBoard, z, v)) {
                  return 1;
                }
              }
        }
        usedBoard->clearAllFlags();
        usedBoard->flagAll((~usedBoard->playing) & 1);
      }
    }
  return 0;
}
char bot::kindaSafeMove() {
  usedBoard->flagAll();
  Matrix holder = usedBoard->flags[usedBoard->playing][ATTACK];
  Matrix holder2;
  for (int x = 0; x < 8; x++)
    for (int y = 0; y < 8; y++) {
      if (usedBoard->layout[x][y] &&
          usedBoard->playing == usedBoard->layout[x][y]->color) {
        usedBoard->flag(x, y);
        if ((usedBoard->flags[usedBoard->playing][MOVE] & holder).allValues()) {
          holder2 = usedBoard->flags[usedBoard->playing][MOVE];
          this->makeMove(selected, usedBoard, x, y);
          for (int z = 0; z < 8; z++)
            for (int v = 0; v < 8; v++)
              if (holder.value(z, v)) {
                if (this->makeMove(selected, usedBoard, z, v))
                  return 1;
              }
        }
        usedBoard->clearAllFlags();
        usedBoard->flagAll((~usedBoard->playing) & 1);
      }
    }
  return 0;
}
char bot::unguardedAttack() {
  usedBoard->flagAll();
  Matrix holder = usedBoard->flags[usedBoard->playing][ATTACK] ^
                  (usedBoard->flags[(~usedBoard->playing) & 1][GUARD] &
                   usedBoard->flags[usedBoard->playing][ATTACK]);
  if (!holder.allValues())
    return 0;
  usedBoard->clearAllFlags();
  usedBoard->flagAll((~usedBoard->playing) & 1);
  for (int x = 0; x < 8; x++)
    for (int y = 0; y < 8; y++) {
      if (usedBoard->layout[x][y] &&
          usedBoard->playing == usedBoard->layout[x][y]->color) {
        usedBoard->flag(x, y);
        if ((usedBoard->flags[usedBoard->playing][ATTACK] & holder)
                .allValues()) {
          this->makeMove(selected, usedBoard, x, y);
          for (int z = 0; z < 8; z++)
            for (int v = 0; v < 8; v++)
              if (holder.value(z, v)) {
                if (this->makeMove(selected, usedBoard, z, v))
                  return 1;
              }
        }
        usedBoard->clearAllFlags();
        usedBoard->flagAll((~usedBoard->playing) & 1);
      }
    }
  return 0;
}
