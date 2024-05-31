#include "./src/lib/binaryMatrix.hpp"
#include "./src/lib/board.hpp"
#include <stdio.h>
int main() {
  board main;
  main.layout[3][3] = new piece(WHITE, QUEEN);
  main.print();
  main.flagAll(WHITE);
  main.flagAll(BLACK);
  main.flags[WHITE][ATTACK].print("białe");
  main.flags[BLACK][MOVE].print("czarne");
  main.flags[WHITE][PLACED].print();
  (main.flags[WHITE][ATTACK] | main.flags[WHITE][PLACED]).print();
}
