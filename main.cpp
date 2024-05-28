#include "./src/lib/binaryMatrix.hpp"
#include "./src/lib/board.hpp"
int main() {
  Matrix a, b;
  a.setto(true);
  a.print();
  b.print();
  b = a;
  b ^= b;
  b.set(4, 5);
  b ^= a;
  b.print();
  board main;
  main.print();
}
