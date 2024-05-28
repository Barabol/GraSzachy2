#ifndef __BINARY_MATRIX__
#define __BINARY_MATRIX__
class Matrix {
public:
  Matrix();
  ~Matrix();
  void set(unsigned const char x, unsigned const char y);
  void set(unsigned const char x, unsigned const char y, const bool value);
  bool value(unsigned const char x, unsigned const char y);
  unsigned char allValues();
  Matrix operator&(const Matrix b);
  void operator=(const Matrix b);
  void operator&=(const Matrix b);
  Matrix operator^(const Matrix b);
  void operator^=(const Matrix b);
  Matrix operator|(const Matrix b);
  void operator|=(const Matrix b);
  void setto(const bool value);
  void print();
  void print(char text[]);
  void clear();
  char mx[8];

private:
};
#endif
