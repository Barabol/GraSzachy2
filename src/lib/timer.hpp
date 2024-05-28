#include "board.hpp"
#ifndef __timer__
#define __timer__
class timer {
public:
  timer(board brd, unsigned int time_);
  bool operator--();
  ~timer();
  void setto(unsigned int time_);

private:
  unsigned int times[2];
  bool *playing;
};
#endif
