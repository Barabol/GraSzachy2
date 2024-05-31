#include "./src/lib/binaryMatrix.hpp"
#include "./src/lib/board.hpp"
int main() {
  board main;
  main.print();
  main.flagAll(WHITE);
  main.flagAll(BLACK);
  main.flags[WHITE][MOVE].print("białe");
  main.flags[BLACK][MOVE].print("czarne");
  main.flags[WHITE][PLACED].print();
}
