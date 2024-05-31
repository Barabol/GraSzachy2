#include "board.hpp"
bool board::chk_layout(unsigned char x, unsigned char y, bool negated) {
  if (y > 7 || y < 0 || x > 7 || x < 0)
    return false;
  if (this->layout[x][y]) {
    if (negated)
      return (bool)(!this->layout[x][y]);
    return (bool)(this->layout[x][y]);
  }
  return false;
}
void board::_f_move(unsigned char x, unsigned char y) {
  const char Kholder[4][2] = {{2, 1}, {2, -1}, {-2, 1}, {-2, -1}};
  piece *used = this->layout[x][y];
  switch (used->typ) {
  case PAWN:
    if (used->color == WHITE) {
      this->flags[used->color][MOVE].set(x, y - 1);
      if (y == 6 && !this->layout[x][y - 2])
        this->flags[used->color][MOVE].set(x, y - 2);
    } else {
      this->flags[used->color][MOVE].set(x, y + 1);
      if (y == 1 && !this->layout[x][y + 2])
        this->flags[used->color][MOVE].set(x, y + 2);
    }
    break;
  case KNIGHT:
    for (int z = 0; z < 4; z++) {
      if (this->chk_layout(x + Kholder[z][0], y + Kholder[z][1], true))
        this->flags[used->color][MOVE].set(x + Kholder[z][0],
                                           y + Kholder[z][1]);
      if (this->chk_layout(x + Kholder[z][1], y + Kholder[z][0], true))
        this->flags[used->color][MOVE].set(x + Kholder[z][1],
                                           y + Kholder[z][0]);
    }
    break;
  case QUEEN:
  case BISHOP:
    for (int z = x + 1, v = y + 1; z < 8 && v < 8; z++, v++) {
      if (chk_layout(z, v, false))
        break;
      this->flags[used->color][MOVE].set(z, v);
    }
    for (int z = x - 1, v = y + 1; z > -1 && v < 8; z--, v++) {
      if (chk_layout(z, v, false))
        break;
      this->flags[used->color][MOVE].set(z, v);
    }
    for (int z = x + 1, v = y - 1; z < 8 && v > -1; z++, v--) {
      if (chk_layout(z, v, false))
        break;
      this->flags[used->color][MOVE].set(z, v);
    }
    for (int z = x - 1, v = y - 1; z > -1 && v > -1; z--, v--) {
      if (chk_layout(z, v, false))
        break;
      this->flags[used->color][MOVE].set(z, v);
    }
    if (used->typ == BISHOP)
      break;
  case ROOK:
    for (int z = x + 1; z < 8; z++) {
      if (chk_layout(z, y, false))
        break;
      this->flags[used->color][MOVE].set(z, y);
    }
    for (int z = y + 1; z < 8; z++) {
      if (chk_layout(x, z, false))
        break;
      this->flags[used->color][MOVE].set(x, z);
    }
    for (int z = x - 1; z > -1; z--) {
      if (chk_layout(z, y, false))
        break;
      this->flags[used->color][MOVE].set(z, y);
    }
    for (int z = y - 1; z > -1; z--) {
      if (chk_layout(x, z, false))
        break;
      this->flags[used->color][MOVE].set(x, z);
    }
    break;
  case KING:

    break;
  }
}
void board::_f_guard(unsigned char x, unsigned char y) {
  piece *used = this->layout[x][y];
  const char Kholder[4][2] = {{2, 1}, {2, -1}, {-2, 1}, {-2, -1}};
  switch (used->typ) {
  case PAWN:
    break;
  case KNIGHT:
    for (int z = 0; z < 4; z++) {
      if (this->chk_layout(x + Kholder[z][0], y + Kholder[z][1], false) &&
          this->layout[x + Kholder[z][0]][y + Kholder[z][1]]->color ==
              used->color)
        this->flags[used->color][GUARD].set(x + Kholder[z][0],
                                            y + Kholder[z][1]);
      if (this->chk_layout(x + Kholder[z][1], y + Kholder[z][0], false) &&
          this->layout[x + Kholder[z][1]][y + Kholder[z][0]]->color ==
              used->color)
        this->flags[used->color][GUARD].set(x + Kholder[z][1],
                                            y + Kholder[z][0]);
    }
    break;
  case QUEEN:
  case BISHOP:
    for (int z = x + 1, v = y + 1; z < 8 && v < 8; z++, v++) {
      if (chk_layout(z, v, false)) {
        if (this->layout[z][v]->color == used->color)
          this->flags[used->color][GUARD].set(z, v);
        break;
      }
    }
    for (int z = x - 1, v = y + 1; z > -1 && v < 8; z--, v++) {
      if (chk_layout(z, v, false)) {
        if (this->layout[z][v]->color == used->color)
          this->flags[used->color][GUARD].set(z, v);
        break;
      }
    }
    for (int z = x + 1, v = y - 1; z < 8 && v > -1; z++, v--) {
      if (chk_layout(z, v, false)) {
        if (this->layout[z][v]->color == used->color)
          this->flags[used->color][GUARD].set(z, v);
        break;
      }
    }
    for (int z = x - 1, v = y - 1; z > -1 && v > -1; z--, v--) {
      if (chk_layout(z, v, false)) {
        if (this->layout[z][v]->color == used->color)
          this->flags[used->color][GUARD].set(z, v);
        break;
      }
    }

    if (used->typ == BISHOP)
      break;
  case ROOK:
    for (int z = x + 1; z < 8; z++) {
      if (chk_layout(z, y, false) && this->layout[z][y]->color != used->color)
        break;
      if (chk_layout(z, y, false) && this->layout[z][y]->color == used->color) {
        this->flags[used->color][GUARD].set(z, y);
        break;
      }
    }
    for (int z = x - 1; z > -1; z--) {
      if (chk_layout(z, y, false) && this->layout[z][y]->color != used->color)
        break;
      if (chk_layout(z, y, false) && this->layout[z][y]->color == used->color) {
        this->flags[used->color][GUARD].set(z, y);
        break;
      }
    }
    for (int z = y + 1; z < 8; z++) {
      if (chk_layout(x, z, false) && this->layout[x][z]->color != used->color)
        break;
      if (chk_layout(x, z, false) && this->layout[x][z]->color == used->color) {
        this->flags[used->color][GUARD].set(x, z);
        break;
      }
    }
    for (int z = y - 1; z > -1; z--) {
      if (chk_layout(x, z, false) && this->layout[x][z]->color != used->color)
        break;
      if (chk_layout(x, z, false) && this->layout[x][z]->color == used->color) {
        this->flags[used->color][GUARD].set(x, z);
        break;
      }
    }
    break;
  case KING:

    break;
  }
}
void board::_f_attack(unsigned char x, unsigned char y) {
  piece *used = this->layout[x][y];
  const char Kholder[4][2] = {{2, 1}, {2, -1}, {-2, 1}, {-2, -1}};
  switch (used->typ) {
  case PAWN:
    break;
  case KNIGHT:
    for (int z = 0; z < 4; z++) {
      if (this->chk_layout(x + Kholder[z][0], y + Kholder[z][1], false) &&
          this->layout[x + Kholder[z][0]][y + Kholder[z][1]]->color !=
              used->color)
        this->flags[used->color][ATTACK].set(x + Kholder[z][0],
                                             y + Kholder[z][1]);
      if (this->chk_layout(x + Kholder[z][1], y + Kholder[z][0], false) &&
          this->layout[x + Kholder[z][1]][y + Kholder[z][0]]->color !=
              used->color)
        this->flags[used->color][ATTACK].set(x + Kholder[z][1],
                                             y + Kholder[z][0]);
    }
    break;
  case QUEEN:
  case BISHOP:
    for (int z = x + 1, v = y + 1; z < 8 && v < 8; z++, v++) {
      if (chk_layout(z, v, false)) {
        if (this->layout[z][v]->color != used->color)
          this->flags[used->color][ATTACK].set(z, v);
        break;
      }
    }
    for (int z = x - 1, v = y + 1; z > -1 && v < 8; z--, v++) {
      if (chk_layout(z, v, false)) {
        if (this->layout[z][v]->color != used->color)
          this->flags[used->color][ATTACK].set(z, v);
        break;
      }
    }
    for (int z = x + 1, v = y - 1; z < 8 && v > -1; z++, v--) {
      if (chk_layout(z, v, false)) {
        if (this->layout[z][v]->color != used->color)
          this->flags[used->color][ATTACK].set(z, v);
        break;
      }
    }
    for (int z = x - 1, v = y - 1; z > -1 && v > -1; z--, v--) {
      if (chk_layout(z, v, false)) {
        if (this->layout[z][v]->color != used->color)
          this->flags[used->color][ATTACK].set(z, v);
        break;
      }
    }

    if (used->typ == BISHOP)
      break;
  case ROOK:
    for (int z = x + 1; z < 8; z++) {
      if (chk_layout(z, y, false) && this->layout[z][y]->color == used->color)
        break;
      if (chk_layout(z, y, false) && this->layout[z][y]->color != used->color) {
        this->flags[used->color][ATTACK].set(z, y);
        break;
      }
    }
    for (int z = x - 1; z > -1; z--) {
      if (chk_layout(z, y, false) && this->layout[z][y]->color == used->color)
        break;
      if (chk_layout(z, y, false) && this->layout[z][y]->color != used->color) {
        this->flags[used->color][ATTACK].set(z, y);
        break;
      }
    }
    for (int z = y + 1; z < 8; z++) {
      if (chk_layout(x, z, false) && this->layout[x][z]->color == used->color)
        break;
      if (chk_layout(x, z, false) && this->layout[x][z]->color != used->color) {
        this->flags[used->color][ATTACK].set(x, z);
        break;
      }
    }
    for (int z = y - 1; z > -1; z--) {
      if (chk_layout(x, z, false) && this->layout[x][z]->color == used->color)
        break;
      if (chk_layout(x, z, false) && this->layout[x][z]->color != used->color) {
        this->flags[used->color][ATTACK].set(x, z);
        break;
      }
    }

    break;
  case KING:

    break;
  }
}
void board::_f_placed(unsigned char x, unsigned char y) {
  this->flags[this->layout[x][y]->color][PLACED].set(x, y);
}
void board::flag(unsigned char x, unsigned char y) {
  if (!this->layout[x][y])
    return;
  for (int z = 0; z < 4; z++)
    this->flags[this->layout[x][y]->color][z].clear();
  if (x > 7 || y > 7)
    return;
  if (this->layout[x][y]) {
    _f_placed(x, y);
    _f_attack(x, y);
    _f_move(x, y);
    _f_guard(x, y);
  }
}
void board::flag_(unsigned char x, unsigned char y) {
  if (x > 7 || y > 7)
    return;
  if (this->layout[x][y]) {
    _f_placed(x, y);
    _f_attack(x, y);
    _f_move(x, y);
    _f_guard(x, y);
  }
}
void board::flagAll(unsigned char color) {
  for (int x = 0; x < 4; x++)
    this->flags[color][x].clear();
  for (int y = 0; y < 8; y++)
    for (int x = 0; x < 8; x++) {
      if (this->layout[x][y] && this->layout[x][y]->color == color)
        flag_(x, y);
    }
}
void board::flagAll() {
  for (int x = 0; x < 4; x++) {
    this->flags[WHITE][x].clear();
    this->flags[BLACK][x].clear();
  }
  for (int y = 0; y < 8; y++)
    for (int x = 0; x < 8; x++) {
      if (this->layout[x][y])
        flag_(x, y);
    }
}
