// #include "./src/lib/binaryMatrix.hpp"
#include "./src/lib/board.hpp"
#include "./src/lib/consts.hpp"
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/bitmap.h>
#include <allegro5/bitmap_draw.h>
#include <allegro5/bitmap_io.h>
#include <allegro5/color.h>
#include <allegro5/display.h>
#include <allegro5/drawing.h>
#include <allegro5/events.h>
#include <allegro5/keyboard.h>
#include <allegro5/mouse.h>
#include <allegro5/timer.h>
#include <stdio.h>
void loadImg(ALLEGRO_BITMAP *bitMap[2][6]) {
  char path[] = "../img/wp.png";
  char col[2];
  col[WHITE] = 'w';
  col[BLACK] = 'b';
  char typ[6];
  typ[PAWN] = 'p';
  typ[KNIGHT] = 'n';
  typ[BISHOP] = 'b';
  typ[ROOK] = 'r';
  typ[QUEEN] = 'q';
  typ[KING] = 'k';

  for (int y = 0; y < 6; y++) {
    path[7] = col[WHITE];
    path[8] = typ[y];
    bitMap[WHITE][y] = al_load_bitmap(path);
  }

  for (int y = 0; y < 6; y++) {
    path[7] = col[BLACK];
    path[8] = typ[y];
    bitMap[BLACK][y] = al_load_bitmap(path);
  }
}
void render(ALLEGRO_BITMAP *bitMap[2][6], board *brd) {
  static const ALLEGRO_COLOR colors[2] = {al_map_rgb(255, 0, 0),
                                          al_map_rgb(0, 255, 0)};
  for (int x = 0; x < 8; x++)
    for (int y = 0; y < 8; y++) {
      al_draw_filled_rectangle(
          TILE_SIZE * x, TILE_SIZE * y, (TILE_SIZE * x) + TILE_SIZE,
          (TILE_SIZE * y + TILE_SIZE), colors[(x + y) & 1]);

      if (brd->layout[x][y] &&
          bitMap[brd->layout[x][y]->color][brd->layout[x][y]->typ])
        al_draw_bitmap(bitMap[brd->layout[x][y]->color][brd->layout[x][y]->typ],
                       TILE_SIZE * x, TILE_SIZE * y, 0);
    }
  puts("a");
}
int main() {
  al_init();
  al_install_mouse();
  al_install_keyboard();
  al_init_image_addon();
  al_init_font_addon();
  al_init_ttf_addon();
  al_install_audio();

  ALLEGRO_DISPLAY *display = al_create_display(D_WIDTH, D_HEIGHT);
  ALLEGRO_FONT *font = al_load_font("./src/ttf/Sans.ttf", 30, 0);
  ALLEGRO_BITMAP *textures[2][6];
  loadImg(textures);

  ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
  ALLEGRO_TIMER *timer = al_create_timer(1.00 / FRAMES);
  al_start_timer(timer);
  ALLEGRO_EVENT event;

  al_register_event_source(queue, al_get_timer_event_source(timer));
  al_register_event_source(queue, al_get_keyboard_event_source());
  al_register_event_source(queue, al_get_display_event_source(display));
  al_register_event_source(queue, al_get_mouse_event_source());

  bool active = 1;
  char x, y;

  board *mainboard = new board;

  while (active) {
    al_wait_for_event(queue, &event);
    switch (event.type) {
    case ALLEGRO_EVENT_TIMER:
      // al_clear_to_color(al_map_rgb(0, 255, 0));
      al_draw_text(font, al_map_rgb(0, 255, 0), 0, 0, 0, "abdc");
      render(textures, mainboard);
      break;
    case 10:
      switch (event.keyboard.keycode) {
      case 59:
        active = 0;
        break;
      case 18:
        // board.clear(1200);
        break;
      default:
        printf("--%d--\n", event.keyboard.keycode);
      }
      break;
    case 20: // move
      break;
    case 21: // click

      x = (int)(event.mouse.x / TILE_SIZE);
      y = (int)(event.mouse.y / TILE_SIZE);
      break;
    case 42:
      active = 0;
      break;
    default:
      // printf("> %d\n", event.type);
      break;
    }
  }
}
