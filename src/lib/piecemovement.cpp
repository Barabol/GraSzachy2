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
  // return;
  char other = (~this->playing) & 1;
  char moves[3] = {-1, 0, 1};
  this->flagAll(other);
  for (int z = 0; z < 4; z++)
    this->flags[this->playing][z].clear();
  for (int z = 0; z < 3; z++)
    for (int q = 0; q < 3; q++) {
      if (!(z + q))
        continue;
      if (x + moves[z] > -1 && y + moves[q] > -1 && x + moves[z] < 8 &&
          y + moves[q] < 8) {
        if (!this->flags[other][ATTACK].value(x + moves[z] < 8, y + moves[q]) &&
            !this->flags[other][GUARD].value(x + moves[z] < 8, y + moves[q])) {
          if (this->layout[x + moves[z] < 8][y + moves[q]] &&
              this->layout[x + moves[z] < 8][y + moves[q]]->color == other)
            this->flags[this->playing][ATTACK].set(x + moves[z] < 8,
                                                   y + moves[q]);
          else
            this->flags[this->playing][MOVE].set(x + moves[z] < 8,
                                                 y + moves[q]);
        }
      }
    }
}
void board::_f_Fking(const unsigned char color) {
  for (int y = 0; y < 8; y++)
    for (int x = 0; x < 8; x++) {
    }
}
