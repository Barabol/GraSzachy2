#include "board.hpp"
#ifndef __timer__
#define __timer__
class Timer {
public:
  Timer(board *brd, unsigned int time_);
  bool operator--();
  ~Timer();
  void setto(unsigned int time_);
  unsigned int operator[](char a);

private:
  unsigned int times[2];
  bool *playing;
};
#endif
