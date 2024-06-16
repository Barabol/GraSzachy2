#include "timer.hpp"
#include "consts.hpp"
Timer::Timer(board *brd, unsigned int time_) {
  this->playing = &(brd->playing);
  this->setto(time_);
  this->szach = &(brd->szach[0]);
}
Timer::~Timer() {}
unsigned int Timer::operator[](char a) {
  if (a)
    return this->times[0];
  else
    return this->times[1];
}
bool Timer::operator--() {
  if (this->szach[0] == -1 || this->szach[1] == -1)
    return false;
  if (!this->times[0])
    this->szach[0] = -1;
  if (!this->times[1])
    this->szach[1] = -1;

  if (frameCounter > 0) {
    frameCounter--;
    if (*this->playing) {
      if (!this->times[1])
        return true;
      else
        return false;
    } else {
      if (!this->times[0])
        return true;
      else
        return false;
    }
  } else
    frameCounter = FRAMES;
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
  this->times[1] = time_;
}
