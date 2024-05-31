#include "./src/lib/binaryMatrix.hpp"
#include "./src/lib/board.hpp"
#include "./src/lib/consts.hpp"
#include "./src/lib/timer.hpp"
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
typedef struct {
  Matrix moves;
  unsigned char x, y;
  piece *used;
} selection;
void render(ALLEGRO_BITMAP *bitMap[3][6], board *brd, selection *selected) {
  static const ALLEGRO_COLOR colors[2] = {al_map_rgb(255, 0, 0),
                                          al_map_rgb(0, 255, 0)};
  for (int y = 0; y < 8; y++)
    for (int x = 0; x < 8; x++) {
      al_draw_filled_rectangle(
          TILE_SIZE * x, TILE_SIZE * y, (TILE_SIZE * x) + TILE_SIZE,
          (TILE_SIZE * y + TILE_SIZE), colors[(x + y) & 1]);

      if (brd->layout[x][y]) {
        al_draw_bitmap(bitMap[brd->layout[x][y]->color][brd->layout[x][y]->typ],
                       TILE_SIZE * x, TILE_SIZE * y, 0);
      }
      if (selected->x == x && selected->y == y)
        al_draw_bitmap(bitMap[2][1], TILE_SIZE * x, TILE_SIZE * y, 0);
      if (selected->moves.value(y, x))
        al_draw_bitmap(bitMap[2][0], TILE_SIZE * x, TILE_SIZE * y, 0);
    }
  al_flip_display();
}
int main() {
  al_init();
  al_init_primitives_addon();
  al_install_mouse();
  al_install_keyboard();
  al_init_image_addon();
  al_init_font_addon();
  al_init_ttf_addon();
  al_install_audio();

  ALLEGRO_DISPLAY *display = al_create_display(D_WIDTH, D_HEIGHT);
  ALLEGRO_FONT *font = al_load_font("./src/ttf/Sans.ttf", 30, 0);
  ALLEGRO_BITMAP *textures[3][6] = {
      {al_load_bitmap("./src/img/wp.png"), al_load_bitmap("./src/img/wn.png"),
       al_load_bitmap("./src/img/wb.png"), al_load_bitmap("./src/img/wr.png"),
       al_load_bitmap("./src/img/wq.png"), al_load_bitmap("./src/img/wk.png")},
      {al_load_bitmap("./src/img/bp.png"), al_load_bitmap("./src/img/bn.png"),
       al_load_bitmap("./src/img/bb.png"), al_load_bitmap("./src/img/br.png"),
       al_load_bitmap("./src/img/bq.png"), al_load_bitmap("./src/img/bk.png")},
      {al_load_bitmap("./src/img/mark.png"),
       al_load_bitmap("./src/img/marked.png")}};

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

  selection selected;
  board *mainboard = new board;
  Timer maintimer(mainboard, 1200);

  mainboard->layout[3][3] = new piece(WHITE, QUEEN);

  while (active) {
    al_wait_for_event(queue, &event);
    switch (event.type) {
    case ALLEGRO_EVENT_TIMER:
      // al_clear_to_color(al_map_rgb(0, 255, 0));
      al_draw_text(font, al_map_rgb(0, 255, 0), 0, 0, 0, "abdc");
      render(textures, mainboard, &selected);
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

      printf("%d,%d\n", x, y);

      if (mainboard->flag(x, y) && mainboard->layout[x][y]) {
        mainboard->print();
        selected.moves.clear();
        selected.moves =
            (mainboard->flags[mainboard->layout[x][y]->color][MOVE] |
             mainboard->flags[mainboard->layout[x][y]->color][ATTACK]);
        selected.x = x;
        selected.y = y;
        selected.used = mainboard->layout[x][y];
      } else
        selected.x = -1;
      selected.moves.print();
      break;
    case 42:
      active = 0;
      break;
    default:
      // printf("> %d\n", event.type);
      break;
    }
    // selected.moves.print();
  }
  al_uninstall_audio();
  al_uninstall_mouse();
  al_uninstall_keyboard();
  al_destroy_timer(timer);
  al_destroy_event_queue(queue);
  al_destroy_display(display);
  for (int x = 0; x < 6; x++)
    for (int y = 0; y < 3; y++)
      al_destroy_bitmap(textures[y][x]);
}
