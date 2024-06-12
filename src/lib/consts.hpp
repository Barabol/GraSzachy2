#ifndef __CONSTANTS__
#define __CONSTANTS__

#include "binaryMatrix.hpp"
#include "board.hpp"

#define D_WIDTH 1280
#define D_HEIGHT 720
#define FRAMES 15
#define TILE_SIZE 90
#define RECURSION_LIMIT 130

typedef struct {
  Matrix moves;
  char x, y;
  piece *used;
} selection;
/*-----BUILD-FLAGS------*/

// po prostu informacje debugowe
//                            UWAGA
// z uwagi na inny sposób wyświetlania kolorów na systemie windows
// informacje nie są wyświetlane "poprawnie"
#define DEBUG

// automatycznie zmienia kto gra gdy jest odznaczone zmiana gracza jest gdy
// kliknie się przycisk "q"
// #define MANUAL_ROUND_CHANGE

// do debugowania biała królowa na start na polu D5
#define BONUS_QUEEN

// po odznaczeniu PROMOTION_CHOICE pionek zawsze awansuje na królową
#define PROMOTION_CHOICE

// po kliknięciu przycisku "r" restartuje całą plansze jak i timer
#define RESTART_KEYBIND

// po kliknięciu przycisku "i" bot dokonuje ruchu
#define MANUAL_AI_MOVE

/*----------------------*/
#endif
