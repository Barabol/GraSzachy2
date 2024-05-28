#include "timer.hpp"
timer::timer(board brd, unsigned int time_) {
  this->playing = &(brd.playing);
  this->setto(time_);
}
timer::~timer() {}
bool timer::operator--() {
  if (*this->playing) {
    if (this->times[1])
      this->times[1]--;
    else
      return true;
  } else {
    if (this->times[0])
      this->times[0]--;
    else
      return true;
  }
  return false;
}
void timer::setto(unsigned int time_) {
  this->times[0] = time_;
  this->times[0] = time_;
}
