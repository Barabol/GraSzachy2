#include "./src/lib/binaryMatrix.hpp"
#include "./src/lib/board.hpp"
#include "./src/lib/chessBot.hpp"
#include "./src/lib/consts.hpp"
#include "./src/lib/timer.hpp"
#include <allegro5/allegro.h>
#include <allegro5/allegro_acodec.h>
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
char makeMove(selection *selected, board *mainboard, char x, char y) {
  char retval = 0;
  if (mainboard->szach[mainboard->playing] == -1)
    return 0;
  if (mainboard->flag(x, y) && mainboard->layout[x][y] &&
      mainboard->layout[x][y]->color == mainboard->playing) {
    if (mainboard->layout[x][y]->typ == KING)
      mainboard->kinglogic(x, y);
#ifdef DEBUG
    mainboard->print();
#endif
    selected->moves.clear();
    selected->x = x;
    selected->y = y;
    selected->used = mainboard->layout[x][y];
    if (mainboard->szach[mainboard->playing] && selected->used->typ != KING) {
      selected->moves =
          (mainboard->flags[mainboard->layout[x][y]->color][MOVE] |
           mainboard->flags[mainboard->layout[x][y]->color][ATTACK]) &
          mainboard->kingFlagging(mainboard->playing);
    } else
      selected->moves =
          (mainboard->flags[mainboard->layout[x][y]->color][MOVE] |
           mainboard->flags[mainboard->layout[x][y]->color][ATTACK]);
    mainboard->isPinned(x, y);
    if (mainboard->layout[x][y]->pinned) {
      selected->moves &= mainboard->isPinned(x, y);
#ifdef DEBUG
      printf("pinowany: %d\n", mainboard->isPinned(x, y).allValues());
#endif
    }
  } else if (selected->moves.value(x, y)) {
    mainboard->move(selected->x, selected->y, x, y);
    retval = 1;
    // al_flush_event_queue(queue);
    if (selected->used->typ == KING) {
      mainboard->kings[selected->used->color][0] = x;
      mainboard->kings[selected->used->color][1] = y;
    }
    selected->x = -1;
    selected->moves.clear();
  } else {
    selected->x = -1;
    selected->moves.clear();
  }
#ifdef DEBUG
  selected->moves.print();
#endif
  return retval;
}
char promotion(char color) {
  ALLEGRO_BITMAP *textures[2][6] = {
      {al_load_bitmap("./src/img/wp.png"), al_load_bitmap("./src/img/wn.png"),
       al_load_bitmap("./src/img/wb.png"), al_load_bitmap("./src/img/wr.png"),
       al_load_bitmap("./src/img/wq.png"), al_load_bitmap("./src/img/wk.png")},
      {al_load_bitmap("./src/img/bp.png"), al_load_bitmap("./src/img/bn.png"),
       al_load_bitmap("./src/img/bb.png"), al_load_bitmap("./src/img/br.png"),
       al_load_bitmap("./src/img/bq.png"), al_load_bitmap("./src/img/bk.png")},
  };
  char holder = QUEEN;
  al_draw_filled_rectangle(0, 300, 720, 420, al_map_rgb(0, 0, 0));
  for (int x = 1; x < 5; x++) {
    al_draw_filled_rectangle((72 * x) + (90 * (x - 1)), 315,
                             (72 * x) + (90 * x), 405, al_map_rgb(50, 50, 50));
    al_draw_bitmap(textures[color][x], (72 * x) + (90 * (x - 1)), 315, 0);
  }
  al_flip_display();
  ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
  ALLEGRO_EVENT event;
  al_register_event_source(queue, al_get_mouse_event_source());
  char gut;
get_gut: // każdy dobry program WYMAGA goto
  al_wait_for_event(queue, &event);
  if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
    if (event.mouse.x > 72 && event.mouse.x < 72 + 90 && event.mouse.y > 315 &&
        event.mouse.y < 405) {
      holder = KNIGHT;
      gut = true;
    } else if (event.mouse.x > 234 && event.mouse.x < 234 + 90 &&
               event.mouse.y > 315 && event.mouse.y < 405) {
      holder = BISHOP;
      gut = true;
    } else if (event.mouse.x > 396 && event.mouse.x < 396 + 90 &&
               event.mouse.y > 315 && event.mouse.y < 405) {
      holder = ROOK;
      gut = true;
    } else if (event.mouse.x > 558 && event.mouse.x < 558 + 90 &&
               event.mouse.y > 315 && event.mouse.y < 405) {
      holder = QUEEN;
      gut = true;
    }
  }
  if (gut == false)
    goto get_gut;
  for (int x = 0; x < 2; x++)
    for (int y = 0; y < 6; y++)
      al_destroy_bitmap(textures[x][y]);
  al_flush_event_queue(queue);
  al_destroy_event_queue(queue);
  return holder;
}
void render(ALLEGRO_BITMAP *bitMap[3][6], board *brd, selection *selected) {
  static const ALLEGRO_COLOR colors[2] = {
      al_map_rgb(brd->colors[0][0], brd->colors[0][1], brd->colors[0][2]),
      al_map_rgb(brd->colors[1][0], brd->colors[1][1], brd->colors[1][2])};
  for (int y = 0; y < 8; y++)
    for (int x = 0; x < 8; x++) {
      al_draw_filled_rectangle(
          TILE_SIZE * x, TILE_SIZE * y, (TILE_SIZE * x) + TILE_SIZE,
          (TILE_SIZE * y + TILE_SIZE), colors[(x + y) & 1]);

      if (brd->layout[x][y]) {
        al_draw_bitmap(bitMap[brd->layout[x][y]->color][brd->layout[x][y]->typ],
                       TILE_SIZE * x, TILE_SIZE * y, 0);
      }
      if (selected->x != -1) {
        if (selected->x == x && selected->y == y)
          al_draw_bitmap(bitMap[2][1], TILE_SIZE * x, TILE_SIZE * y, 0);
        if (selected->moves.value(x, y))
          al_draw_bitmap(bitMap[2][0], TILE_SIZE * x, TILE_SIZE * y, 0);
      }
    }
  al_flip_display();
  // promotion(WHITE);
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
  al_init_acodec_addon();
  al_reserve_samples(6);

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
#ifndef NO_MUSIC
  ALLEGRO_SAMPLE *tileSoundz = al_load_sample("./src/sfx/move.flac");
  ALLEGRO_SAMPLE_INSTANCE *tileSound = al_create_sample_instance(tileSoundz);

  ALLEGRO_SAMPLE *gameoverz = al_load_sample("./src/sfx/gameover.flac");
  ALLEGRO_SAMPLE_INSTANCE *gameover = al_create_sample_instance(gameoverz);

  ALLEGRO_SAMPLE *selectSoundz = al_load_sample("./src/sfx/click.flac");
  ALLEGRO_SAMPLE_INSTANCE *selectSound =
      al_create_sample_instance(selectSoundz);

  ALLEGRO_SAMPLE *soundtrackz = al_load_sample("./src/sfx/soundtrack.flac");
  ALLEGRO_SAMPLE_INSTANCE *soundtrack = al_create_sample_instance(soundtrackz);

  al_attach_sample_instance_to_mixer(tileSound, al_get_default_mixer());
  al_attach_sample_instance_to_mixer(gameover, al_get_default_mixer());
  al_attach_sample_instance_to_mixer(selectSound, al_get_default_mixer());
  al_attach_sample_instance_to_mixer(soundtrack, al_get_default_mixer());
  al_set_sample_instance_playmode(soundtrack, ALLEGRO_PLAYMODE_LOOP);
  al_play_sample_instance(soundtrack);

  char muzyczka = 1;
#endif
  bool active = 1;
  char x, y;

  selection *selected = new selection;
  board *mainboard = new board;
  mainboard->setBoardColor(0, 240, 217, 181);
  mainboard->setBoardColor(1, 181, 136, 99);
  bot mainbot(mainboard, selected, makeMove);
  moveLoc botMovement;

  Timer maintimer(mainboard, DEFAULT_TIME);
#ifdef PROMOTION_CHOICE
  mainboard->setPfunction(promotion);
#endif
#ifdef BONUS_QUEEN
  mainboard->layout[3][3] = new piece(WHITE, QUEEN);
#endif
  while (active) {
    al_wait_for_event(queue, &event);

    switch (event.type) {
    case ALLEGRO_EVENT_TIMER:
#ifndef NO_MUSIC
      if (mainboard->szach[0] == -1 || mainboard->szach[1] == -1) {
        if (muzyczka) {
          al_play_sample_instance(gameover);
          muzyczka = 0;
          continue;
        } else
          al_stop_sample_instance(soundtrack);
      }
#endif
      al_clear_to_color(al_map_rgb(0, 0, 0));
      --maintimer;

      al_draw_textf(font, al_map_rgb(0, 255, 0), 800, 30, 0, "Czas: %d:%d",
                    maintimer[0] / 60, maintimer[0] % 60);
      al_draw_textf(font, al_map_rgb(0, 255, 0), 800, 570, 0, "Materiał: %d",
                    mainboard->points[WHITE] - mainboard->points[BLACK]);
      al_draw_textf(font, al_map_rgb(0, 255, 0), 800, 90, 0, "Materiał: %d",
                    mainboard->points[BLACK] - mainboard->points[WHITE]);
      al_draw_textf(font, al_map_rgb(0, 255, 0), 800, 640, 0, "Czas: %d:%d",
                    maintimer[1] / 60, maintimer[1] % 60);
      render(textures, mainboard, selected);
      break;
    case 10:
      switch (event.keyboard.keycode) {
      case 9:
#ifdef MANUAL_AI_MOVE
        mainbot.getMove();
#endif
        break;
      case 59:
        active = 0;
        break;
      case 17:
#ifdef MAUNAL_ROUND_CHANGE
        mainboard->switchPlayer();
        mainboard->clearAllFlags();
        mainboard->clearAllFlags();
        mainboard->flagAll();
        selected.x = -1;
#endif
        // board.clear(1200);
        break;
      case 18:
#ifdef RESTART_KEYBIND
#ifndef NO_MUSIC
        al_play_sample_instance(soundtrack);
        muzyczka = 1;
#endif
        mainboard->clear();
        maintimer.setto(DEFAULT_TIME);
        mainboard->clearAllFlags();
        selected->x = -1;
#endif
        break;
      default:
#ifdef DEBUG
        printf("--%d--\n", event.keyboard.keycode);
#endif
        break;
      }
      break;
    case 20: // move

      break;
    case 21: // click
      x = (int)(event.mouse.x / TILE_SIZE);
      y = (int)(event.mouse.y / TILE_SIZE);
#ifndef NO_MUSIC
      if (mainboard->layout[x][y] &&
          mainboard->layout[x][y]->color == mainboard->playing)
        al_play_sample_instance(selectSound);
      if (makeMove(selected, mainboard, x, y))
        al_play_sample_instance(tileSound);
#endif
#ifdef NO_MUSIC
      makeMove(selected, mainboard, x, y);
#endif
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
  mainboard->~board();
  maintimer.~Timer();
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
