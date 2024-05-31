#include "board.hpp"
void board::move(char px, char py, char x, char y) {
  if (this->layout[x][y]) {
    this->points[this->playing] += this->layout[x][y]->value();
    this->layout[x][y]->~piece();
    this->layout[x][y] = this->layout[px][py];
    this->layout[px][py] = nullptr;
  } else {
    this->layout[x][y] = this->layout[px][py];
    this->layout[px][py] = nullptr;
  }
}
