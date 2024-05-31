#include "binaryMatrix.hpp"
#include <stdio.h>
Matrix::Matrix() { this->clear(); }
Matrix::~Matrix() {}
void Matrix::set(unsigned const char x, unsigned const char y) {
  if (x < 8 && y < 8)
    this->mx[y] |= (1 << x);
}
void Matrix::set(unsigned const char x, unsigned const char y,
                 const bool value) {
  if (value)
    this->set(x, y);
  else
    this->mx[x] &= (~(1 << y));
}
bool Matrix::value(unsigned const char x, unsigned const char y) {
  return (bool)(this->mx[x] & (1 << y));
}
unsigned char Matrix::allValues() {
  unsigned char a = 0;
  for (int x = 0; x < 8; x++)
    for (int y = 0; y < 8; y++)
      a += this->value(x, y);
  return a;
}
Matrix Matrix::operator&(const Matrix b) {
  Matrix a;
  for (int x = 0; x < 8; x++)
    a.mx[x] = this->mx[x] & b.mx[x];
  return a;
}
void Matrix::operator=(const Matrix b) {
  for (int x = 0; x < 8; x++)
    this->mx[x] = b.mx[x];
}
void Matrix::operator&=(const Matrix b) {
  for (int x = 0; x < 8; x++)
    this->mx[x] &= b.mx[x];
}
Matrix Matrix::operator^(const Matrix b) {
  Matrix a;
  for (int x = 0; x < 8; x++)
    a.mx[x] = this->mx[x] ^ b.mx[x];
  return a;
}
void Matrix::operator^=(const Matrix b) {
  for (int x = 0; x < 8; x++)
    this->mx[x] ^= b.mx[x];
}
Matrix Matrix::operator|(const Matrix b) {
  Matrix a;
  for (int x = 0; x < 8; x++)
    a.mx[x] = this->mx[x] | b.mx[x];
  return a;
}
void Matrix::operator|=(const Matrix b) {
  for (int x = 0; x < 8; x++)
    this->mx[x] |= b.mx[x];
}
void Matrix::setto(const bool value) {
  if (value)
    for (int x = 0; x < 8; x++)
      this->mx[x] = -1;
  else
    this->clear();
}
void Matrix::clear() {
  for (int x = 0; x < 8; x++)
    this->mx[x] = 0;
}
void Matrix::print() {
  puts(" -----------------");
  for (int x = 0; x < 8; x++) {
    printf("| ");
    for (int y = 0; y < 8; y++) {
      if (this->value(x, y))
        printf("\033[1;31m1 \033[0m");
      else
        printf("\033[1;34m0 \033[0m");
    }
    printf("|\n");
  }
  puts(" -----------------");
}
void Matrix::print(char text[]) {
  printf("|\033[1;5;36m%s\033[0m|\n", text);
  puts(" -----------------");
  for (int x = 0; x < 8; x++) {
    printf("| ");
    for (int y = 0; y < 8; y++) {
      if (this->value(x, y))
        printf("\033[1;31m1 \033[0m");
      else
        printf("\033[1;34m0 \033[0m");
    }
    printf("|\n");
  }
  puts(" -----------------");
}
