#include "timer.hpp"
Timer::Timer(board *brd, unsigned int time_) {
  this->playing = &(brd->playing);
  this->setto(time_);
}
Timer::~Timer() {}
bool Timer::operator--() {
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
void Timer::setto(unsigned int time_) {
  this->times[0] = time_;
  this->times[0] = time_;
}
