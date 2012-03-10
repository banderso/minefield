
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ncurses.h>

#include "mfgame.h"
#include "mfplayer.h"
#include "mfenemies.h"
#include "mftimer.h"

mfgame *mfgame_create(mfworld *world) {
  mfgame init = {0, 0, 0, false, {0, 0, 0, 0}, world};
  mfgame *game = NULL;
  game = malloc(sizeof(mfgame));
  if (game == NULL) {
    printf("mfgame creation failed: memory allocation failed\n");
  } else {
    memcpy(game, &init, sizeof(mfgame));
  }
  return game;
}

void mfgame_delete(mfgame *game) {
  mfworld_delete(game->world);
  free(game);
}

s8 *mfgame_to_string(const mfgame *game) {
  s8 *ws = mfworld_to_string(game->world);

  const s8 format[] = "{%d, %s}";
  s32 size = snprintf(NULL, 0, format, game->score, ws) + 1;
  s8 *str = malloc(sizeof(s8) * size);
  if (str != NULL) {
    snprintf(str, size, format, game->score, ws);
  }
  free(ws);
  return str;
}

void mfgame_print(mfgame *game) {
  if (game == NULL) return;
  mfworld *world = game->world;
  mfplayer *player = world->player;
  mfenemies *enemies = world->enemies;

  // print score
  printf("Player score: %d\n", game->score);
  // print player
  printf("Position: (%d, %d)\n", player->pos.x, player->pos.y);
  printf("Dimention: %d x %d\n", player->dim.x, player->dim.y);
  printf("Alive: %s\n", ((player->alive) ? "yes" : "no"));
  // print enemies
  printf("Enemies:\n");
  s32 *xpos = enemies->xpos;
  s32 *ypos = enemies->ypos;
  bool *alive = enemies->alive;
  for (int i = 0; i < enemies->count; i++) {
    printf("%2d - Position: (%d, %d), Alive: %s\n", i, xpos[i], ypos[i], ((alive[i]) ? "yes" : "no"));
  }
}

mfgame *mfgame_update(mfgame *game, u64 dt) {
  mfworld *world = game->world;
  mfplayer *player = world->player;
  mfenemies *enemies = world->enemies;

  mfworld_update(world, dt);

  s32 dx = 0, dy = 0;
  s32 key = getch();
  if (key == 'a') dx = -1;
  if (key == 'd') dx = 1;
  if (key == 'w') dy = -1;
  if (key == 's') dy = 1;
  if (key == 'q') game->running = false;

  mfplayer_update_position(player, dx, dy, game->mx, game->my);

  game->score += mfenemies_update_position(enemies, game->mx, game->my, world->gravity, dt);
  mfenemies_activate(enemies, game->mx, game->my, world->gravity);
  bool hit = false;//mfenemies_check_position(enemies, mfp_x(player), mfp_y(player));

  if (game->running)
    game->running = !hit;

  return game;
}

mfgame * mfgame_render(mfgame *game, u64 elapsed_time, u64 processed_time) {
  mfworld *world = game->world;
  mfplayer *player = world->player;
  mfenemies *enemies = world->enemies;

  erase();
  if (player->alive) {
    attron(A_BOLD);
    mvaddch(player->pos.y, player->pos.x, '@');
    attroff(A_BOLD);
  }

  bool *alive = enemies->alive;
  u8 *size = enemies->size;
  s32 *xpos = enemies->xpos;
  s32 *ypos = enemies->ypos;
  s32 x = 0, y = 0, xx = 0, yy = 0;
  s8 enemy_grafx = 'M';
  for (int i = 0, len = enemies->count; i < len; i++) {
    if (alive[i]) {
      x = xpos[i];
      y = ypos[i];
      mvaddch(y, x, enemy_grafx);
      if (size[i] == 2) {
        xx = x + 1;
        yy = y + 1;
        mvaddch(y, xx, enemy_grafx);
        mvaddch(yy, x, enemy_grafx);
        mvaddch(yy, xx, enemy_grafx);
      }
    }
  }

  int row = 0;
  /*
  mvprintw(row++, 0, "     ns per second: %010lu", NS_PER_SEC);
  mvprintw(row++, 0, "      ns per frame: %010lu", NS_PER_FRAME);
  mvprintw(row++, 0, "    FrameTime (ns): %010lu", elapsed_time);
  mvprintw(row++, 0, "ProcessedTime (ns): %010lu", processed_time);
  //*/

  mfclock *clock = &(game->clock);
  mvprintw(row++, 0, "Time:     %02d:%02d:%02d.%03d", clock->hour, clock->min, clock->sec, clock->msec);
  mvprintw(row++, 0, "Gravity Update: %2d", (world->update / MS_PER_SEC));
  mfgravity g = world->gravity;
  mvprintw(row++, 0, "Gravity: %s (%d)", ((g == DOWN) ? "DOWN" : (g == UP) ? "UP" : (g == LEFT) ? "LEFT" : (g == RIGHT) ? "RIGHT" : "ERROR"), g);
  mvprintw(row++, 0, "Score: %d", game->score);
  
  if (!game->running) {
    s8 message[] = "GAME OVER";
    u32 ml = strlen(message);
    u32 mx = (game->mx / 2) - (ml / 2);
    u32 my = game->my / 2;
    mvprintw(my, mx, message);
  }

  refresh();

  return game;
}

void mfgame_run(mfgame *game) {
  mftimer timer = {{0, 0}, 0, {0, 0, 0, 0}};
  mftimer_init(&timer);
  mftimer_reset(&timer);
  u64 last = mftimer_now(&timer);
  u64 now = 0;
  u64 elapsed = 0;
  u64 processed = 0;
  while (game->running) {
    now = mftimer_now(&timer);
    elapsed = now - last;
    last = now;
    mftimer_wait(&timer, NS_PER_FRAME - elapsed);
    mftimer_update(&timer, &(game->clock));
    processed = mftimer_now(&timer) - now;
    mfgame_update(mfgame_render(game, elapsed, processed), elapsed);
  }
  mfgame_render(game, elapsed, processed);
}
