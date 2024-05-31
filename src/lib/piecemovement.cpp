#include "board.hpp"
void board::move(char px, char py, char x, char y) {
  piece *used = this->layout[px][py];
  if (used->typ == KING) {

    return;
  }
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
void board::kinglogic(char x, char y) {
  char other = (~this->playing) & 1;
  this->flagAll(other);
  for (int z = 0; z < 4; z++)
    this->flags[this->playing][z].clear();
}
void board::_f_Fking(unsigned char color) {
  for (int y = 0; y < 8; y++)
    for (int x = 0; x < 8; x++) {
    }
}
