
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <ncurses.h>

#include "mftimer.h"
#include "mfplayer.h"
#include "mfenemies.h"
#include "mfworld.h"
#include "mfgame.h"

u32 hash(const s8 *str) {
  u32 len = strlen(str),
      h = 0;
  for (int i = 0; i < len; i++)
    h = 31 * h + str[i];
  return h;
}

mfgame *init_game(s32 mx, s32 my) {
  mftimer timer = {{0, 0}, 0};
  mftimer_init(&timer);
  srand(hash("minefield") + (u32)mftimer_now(&timer));
  mfgame *game = mfgame_create(mfworld_create(mfplayer_create(), mfenemies_create()));
  game->running = true;
  game->mx = mx;
  game->my = my;
  mfplayer_set_position(game->world->player, mx / 2, (4 * my) / 5);
  //mfenemies_init(game->world->enemies, mx, my);
  return game;
}

void init_ncurses(s32 *max_x, s32 *max_y) {
  initscr();
  cbreak();
  noecho();
  timeout(0); //don't block on input
  curs_set(0); //hide cursor
  getmaxyx(stdscr, *max_y, *max_x);
}

void shutdown_ncurses() {
  refresh();
  endwin();
}

s32 main(void) {

  s32 max_x, max_y;
  init_ncurses(&max_x, &max_y);
  mfgame *game = init_game(max_x, max_y);

  mfgame_run(game);
  timeout(-1);
  getch();
  getch();

  shutdown_ncurses();
  mfgame_delete(game);
  return 0;
}
