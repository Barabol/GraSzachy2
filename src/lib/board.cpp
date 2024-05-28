#include "board.hpp"
#include <stdio.h>
board::board() { this->clear(); }
board::~board() {
  for (int x = 0; x < 8; x++)
    for (int y = 0; y < 0; y++) {
      if (this->layout[x][y])
        this->layout[x][y]->~piece();
      this->layout[x][y] = nullptr;
    }
}
void board::operator=(board b) {}
void board::clear() {
  this->playing = WHITE;
  this->flags[0].clear();
  this->flags[1].clear();
  char content[8] = {ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK};
  for (int x = 0; x < 8; x++)
    for (int y = 0; y < 0; y++) {
      if (this->layout[x][y])
        this->layout[x][y]->~piece();
      this->layout[x][y] = nullptr;
    }
  for (int x = 0; x < 8; x++) {
    this->layout[x][0] = new piece(BLACK, content[x]);
    this->layout[x][1] = new piece(BLACK, PAWN);
    this->layout[x][2] = nullptr;
    this->layout[x][3] = nullptr;
    this->layout[x][4] = nullptr;
    this->layout[x][5] = nullptr;
    this->layout[x][6] = new piece(WHITE, PAWN);
    this->layout[x][7] = new piece(WHITE, content[7 - x]);
  }
}
void board::print() {
  char symbol = ' ';
  char kolorek = 0;
  const char BG[4][7] = {
      {"\x1b[46m"}, {"\x1b[42m"}, {"\x1b[41m"}, {"\x1b[105m"}};
  const char PC[2][8] = {{"\x1b[1;37m"}, {"\x1b[1;30m"}};

  printf("\x1b[44;1;31m   ");
  for (int x = 0; x < 8; x++)
    printf("\x1b[44;1;31m%c ", 'A' + x);
  puts("\x1b[0m");

  for (int y = 0; y < 8; y++) {
    printf("\x1b[44;1;31m %c \x1b[0m", '8' - y);
    for (int x = 0; x < 8; x++) {

      if (this->layout[x][y] != nullptr) {
        symbol = this->layout[x][y]->symbol();
        kolorek = this->layout[x][y]->color;
        if (this->layout[x][y]->color)
          symbol += 32;
      }
      printf("%s%s%c \x1b[0m", BG[(x + y) & 1], PC[kolorek], symbol);
      symbol = ' ';
    }
    puts("");
  }
}

piece::piece(char kolor, char typ) {
  this->color = kolor;
  this->typ = typ;
  notMoved = true;
}
piece::~piece() {}
char piece::symbol() {
  switch (this->typ) {
  case PAWN:
    return 'P';
  case KNIGHT:
    return 'N';
  case BISHOP:
    return 'B';
  case ROOK:
    return 'R';
  case QUEEN:
    return 'Q';
  case KING:
    return 'K';
  }
  return '?';
}
// trzeba to zmienić bo tak będzie w chuj nie wygodnie )-:
void board::_f_guard(char color) {}
void board::_f_move(char color) {}
void board::_f_attack(char color) {}
void board::flag(int flagType, char color) {
  switch (flagType) {
  case MOVE:
    _f_move(color);
    break;
  case ATTACK:
    _f_attack(color);
    break;
  case GUARD:
    _f_guard(color);
    break;
  }
}
