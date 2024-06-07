#include "board.hpp"
#include "consts.hpp"
#include <stdio.h>
char board::move(const char px, const char py, const char x, const char y) {
  char retval = NONE;
  piece *used = this->layout[px][py];

  if (this->layout[x][y]) {
    this->points[this->playing] += this->layout[x][y]->value();
    this->layout[x][y]->~piece();
    this->layout[x][y] = this->layout[px][py];
    this->layout[px][py] = nullptr;
    retval = CAPTURE;
    this->enPassant.passantable = nullptr;
  } else {
    if (used->typ == KING && used->notMoved) {
      if (x == 2) {
        this->layout[3][y] = this->layout[0][y];
        this->layout[0][y] = nullptr;
        retval = CASTLE_LONG;
        this->layout[3][y]->notMoved = false;
      } else if (x == 6) {
        this->layout[5][y] = this->layout[7][y];
        this->layout[7][y] = nullptr;
        retval = CASTLE_SHORT;
        this->layout[5][y]->notMoved = false;
      }
    }
    if (used->typ == PAWN && !used->notMoved &&
        ((px - x) == 1 || (x - px) == 1)) {
      this->points[this->playing] +=
          this->layout[this->enPassant.x][this->enPassant.y]->value();
      this->enPassant.passantable->~piece();
      this->layout[this->enPassant.x][this->enPassant.y] = nullptr;
    }

    if (used->typ == PAWN && (y - py == -2 || y - py == 2)) {
      this->enPassant.x = x;
      this->enPassant.y = y;
      this->enPassant.passantable = used;
    } else
      this->enPassant.passantable = nullptr;
    this->layout[x][y] = this->layout[px][py];
    this->layout[px][py] = nullptr;
  }
  if (used->typ == PAWN && (y == 7 || y == 0)) {
    retval = PROMOTION;
    if (this->promotionFunction)
      used->typ = this->promotionFunction(used->color);
    else
      used->typ = QUEEN;
#ifdef DEBUG
    puts("awans");
#endif
  }
  used->notMoved = false;
  this->flagAll();
  if (this->flags[this->playing][ATTACK].value(
          this->kings[~(this->playing) & 1][0],
          this->kings[~(this->playing) & 1][1])) {
#ifdef DEBUG
    printf("szach\n");
#endif
  }
#ifndef MANUAL_ROUND_CHANGE
  this->switchPlayer();
#endif
  return retval;
}
void board::kinglogic(const char x, const char y) {
  enum { Move, Attack };
  Matrix holder[2];
  holder[Move].clear();
  holder[Attack].clear();
  char color = this->layout[x][y]->color;
  for (int z = 0; z < 4; z++)
    this->flags[color][z].clear();
  // roszada
  Matrix roszada;
  roszada.clear();
  this->flagAll(color);
  if (this->layout[x][y]->notMoved) {
    if (this->layout[x][y]->color == WHITE) {
      if (this->layout[0][7] && this->layout[0][7]->notMoved) {
        roszada.set(1, 7);
        roszada.set(2, 7);
        roszada.set(3, 7);
#ifdef DEBUG
        roszada.print("roszada");
#endif
        if (!((this->flags[this->playing][PLACED] |
               this->flags[(this->playing)][PLACED]) &
              roszada)
                 .allValues()) {
          roszada.set(4, 7);
          if (!(this->flags[(~this->playing) & 1][ATTACK] & roszada)
                   .allValues())
            holder[Move].set(2, 7);
        }
        roszada.clear();
      }
      if (this->layout[7][7] && this->layout[7][7]->notMoved) {
        roszada.set(5, 7);
        roszada.set(6, 7);
#ifdef DEBUG
        roszada.print("roszada");
#endif
        if (!((this->flags[this->playing][PLACED] |
               this->flags[(this->playing)][PLACED]) &
              roszada)
                 .allValues()) {
          roszada.set(4, 7);
          if (!(this->flags[(~this->playing) & 1][ATTACK] & roszada)
                   .allValues())
            holder[Move].set(6, 7);
        }
      }
    } else {
      if (this->layout[0][0] && this->layout[0][0]->notMoved) {
        roszada.set(1, 0);
        roszada.set(2, 0);
        roszada.set(3, 0);
#ifdef DEBUG
        roszada.print("roszada");
#endif
        if (!((this->flags[this->playing][PLACED] |
               this->flags[(this->playing)][PLACED]) &
              roszada)
                 .allValues()) {
          roszada.set(4, 0);
          if (!(this->flags[(~this->playing) & 1][ATTACK] & roszada)
                   .allValues())
            holder[Move].set(2, 0);
        }
        roszada.clear();
      }
      if (this->layout[7][0] && this->layout[7][0]->notMoved) {
        roszada.set(5, 0);
        roszada.set(6, 0);
#ifdef DEBUG
        roszada.print("roszada");
#endif
        if (!((this->flags[this->playing][PLACED] |
               this->flags[(this->playing)][PLACED]) &
              roszada)
                 .allValues()) {
          roszada.set(4, 0);
          if (!(this->flags[(~this->playing) & 1][ATTACK] & roszada)
                   .allValues())
            holder[Move].set(6, 0);
        }
      }
    }
  }
  for (int z = 0; z < 4; z++)
    this->flags[color][z].clear();
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
#ifdef DEBUG
  non.print("non");
#endif
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
