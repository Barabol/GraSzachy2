#include "board.hpp"
void board::move(const char px, const char py, const char x, const char y) {
  piece *used = this->layout[px][py];
  if (this->layout[x][y]) {
    this->points[this->playing] += this->layout[x][y]->value();
    this->layout[x][y]->~piece();
    this->layout[x][y] = this->layout[px][py];
    this->layout[px][py] = nullptr;
  } else {
    this->layout[x][y] = this->layout[px][py];
    this->layout[px][py] = nullptr;
  }
  used->notMoved = false;
}
void board::kinglogic(const char x, const char y) {
  enum { Move, Attack };
  Matrix holder[2];
  holder[Move].clear();
  holder[Attack].clear();
  char color = this->layout[x][y]->color;
  if (x + 1 < 8) { // |x
    if (this->layout[x + 1][y]) {
      if (this->layout[x + 1][y]->color != color)
        holder[Attack].set(x + 1, y);
    } else
      holder[Move].set(x + 1, y);
  }
  if (y + 1 < 8) { // |y
    if (this->layout[x][y + 1]) {
      if (this->layout[x][y + 1]->color != color)
        holder[Attack].set(x, y + 1);
    } else
      holder[Move].set(x, y + 1);
  }
  if (x - 1 > -1) {
    if (this->layout[x - 1][y]) {
      if (this->layout[x - 1][y]->color != color)
        holder[Attack].set(x - 1, y);
    } else
      holder[Move].set(x - 1, y);
  }
  if (y - 1 > -1) {
    if (this->layout[x][y - 1]) {
      if (this->layout[x][y - 1]->color != color)
        holder[Attack].set(x, y - 1);
    } else
      holder[Move].set(x, y - 1);
  }

  if (x + 1 < 8 && y + 1 < 8) { //
    if (this->layout[x + 1][y + 1]) {
      if (this->layout[x + 1][y + 1]->color != color)
        holder[Attack].set(x + 1, y + 1);
    } else
      holder[Move].set(x + 1, y + 1);
  }
  if (x - 1 > -1 && y + 1 < 8) { //
    if (this->layout[x - 1][y + 1]) {
      if (this->layout[x - 1][y + 1]->color != color)
        holder[Attack].set(x - 1, y + 1);
    } else
      holder[Move].set(x - 1, y + 1);
  }
  if (x - 1 > -1 && y - 1 > -1) {
    if (this->layout[x - 1][y - 1]) {
      if (this->layout[x - 1][y - 1]->color != color)
        holder[Attack].set(x - 1, y - 1);
    } else
      holder[Move].set(x - 1, y - 1);
  }
  if (y - 1 > -1 && x + 1 < 8) {
    if (this->layout[x + 1][y - 1]) {
      if (this->layout[x + 1][y - 1]->color != color)
        holder[Attack].set(x + 1, y - 1);
    } else
      holder[Move].set(x + 1, y - 1);
  }
  for (int z = 0; z < 4; z++)
    this->flags[(~color) & 1][z].clear();
  this->flagAll((~color) & 1);
  this->_f_Fking((~color) & 1);
  Matrix non;
  non.clear();
  non |= this->_f_Fking((~color) & 1);
  non |= this->_f_layout((~color) & 1);
  non.print("non");
  this->flags[color][MOVE] |= (holder[Move] ^ (holder[Move] & non));
  this->flags[color][ATTACK] |=
      (holder[Attack] ^ (holder[Attack] & this->flags[(~color) & 1][GUARD]));
}
Matrix board::_f_Fking(const unsigned char color) {
  Matrix holder;
  holder.clear();
  int x, y;
  for (y = 0; y < 8; y++)
    for (x = 0; x < 8; x++)
      if (this->layout[x][y] && this->layout[x][y]->typ == KING &&
          this->layout[x][y]->color == color)
        goto exit;
exit:
  if (x + 1 < 8) {
    if (this->layout[x + 1][y]) {
      if (this->layout[x + 1][y]->color == color)
        this->flags[color][GUARD].set(x + 1, y);
    } else
      holder.set(x + 1, y);
  }
  if (y + 1 < 8) {
    if (this->layout[x][y + 1]) {
      if (this->layout[x][y + 1]->color == color)
        this->flags[color][GUARD].set(x, y + 1);
    } else
      holder.set(x, y + 1);
  }
  if (x - 1 > -1) {
    if (this->layout[x - 1][y]) {
      if (this->layout[x - 1][y]->color == color)
        this->flags[color][GUARD].set(x - 1, y);
    } else
      holder.set(x - 1, y);
  }
  if (y - 1 > -1) {
    if (this->layout[x][y - 1]) {
      if (this->layout[x][y - 1]->color == color)
        this->flags[color][GUARD].set(x, y - 1);
    } else
      holder.set(x, y - 1);
  }

  if (x + 1 < 8 && y + 1 < 8) {
    if (this->layout[x + 1][y + 1]) {
      if (this->layout[x + 1][y + 1]->color == color)
        this->flags[color][GUARD].set(x + 1, y + 1);
    } else
      holder.set(x + 1, y + 1);
  }
  if (x - 1 > -1 && y + 1 < 8) {
    if (this->layout[x - 1][y + 1]) {
      if (this->layout[x - 1][y + 1]->color == color)
        this->flags[color][GUARD].set(x - 1, y + 1);
    } else
      holder.set(x - 1, y + 1);
  }
  if (x - 1 > -1 && y - 1 > -1) {
    if (this->layout[x - 1][y - 1]) {
      if (this->layout[x - 1][y - 1]->color == color)
        this->flags[color][GUARD].set(x - 1, y - 1);
    } else
      holder.set(x - 1, y - 1);
  }
  if (y - 1 > -1 && x + 1 < 8) {
    if (this->layout[x + 1][y - 1]) {
      if (this->layout[x + 1][y - 1]->color == color)
        this->flags[color][GUARD].set(x + 1, y - 1);
    } else
      holder.set(x + 1, y - 1);
  }
  return holder;
}
