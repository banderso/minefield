
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "mfenemies.h"
#include "mflog.h"

mfenemies *mfenemies_create(void) {
  mfenemies init = { 0, 0, ENEMY_COUNT, 0, {0}, {0}, {0}, {0}, {0} };
  mfenemies *enemies = NULL;
  enemies = malloc(sizeof(mfenemies));
  if (enemies == NULL) {
    printf("mfenemies creation failed: memory allocation failed\n");
  } else {
    memcpy(enemies, &init, sizeof(mfenemies));
  }
  return enemies;
}

void mfenemies_delete(mfenemies *enemies) {
  free(enemies);
}

/*
void mfenemies_init(mfenemies *enemies, s32 mx, s32 my) {
  u32 *xps = enemies->xpos;
  u32 *yps = enemies->ypos;
  bool *alv = enemies->alive;
  for (int i = 0, len = enemies->count; i < len; i++) {
    xps[i] = rand();
  }
}
*/

u32 mfenemies_update_position(mfenemies *enemies, u32 mx, u32 my, mfgravity gravity, u64 dt) {
  u64 alive = enemies->alive;
  s32 *xpos = enemies->xpos;
  s32 *ypos = enemies->ypos;
  u32 *speed = enemies->speed;

  u32 d = 0, dt_ms = dt / NS_PER_MS;
  //  bool dead[ENEMY_COUNT] = {false};

  s32 len = enemies->count;
  for (s32 i = 0; i < len; i++) {
    if (alive_chk(alive, i)) {
      d = speed[i] * dt_ms;
      switch (gravity) {
        case UP:
          ypos[i] -= d;
          alive = alive_cond_flip(alive, ((ypos[i] / 1000) <= 0), i);
          //dead[i] = ((ypos[i] / 1000) <= 0);
          break;
        case DOWN:
          ypos[i] += d;
          alive = alive_cond_flip(alive, ((ypos[i] / 1000) > my), i);
          //dead[i] = ((ypos[i] / 1000) > my);
          break;
        case LEFT:
          xpos[i] -= d;
          alive = alive_cond_flip(alive, ((xpos[i] / 1000) <= 0), i);
          //dead[i] = ((xpos[i] / 1000) <= 0);
          break;
        case RIGHT:
          xpos[i] += d;
          alive = alive_cond_flip(alive, ((xpos[i] / 1000) > mx), i);
          //dead[i] = ((xpos[i] / 1000) > mx);
          break;
      }
    }
  }

  u32 alive_cnt = enemies->alive_cnt,
      total_death = 0;
  u8 *size = enemies->size;
  for (s32 i = 0, len = ENEMY_COUNT; i < len; i++) {
    /*if (dead[i]) {
      alive = alive_set(alive, i);
      alive_cnt--;
      total_death += size[i];
    }
    */
    alive_cnt -= alive_chk(alive, i);
    total_death = total_death + (size[i] * !alive_chk(alive, i));
  }

  enemies->alive = alive;
  enemies->alive_cnt = alive_cnt;
  return total_death;
}

bool mfenemies_check_position(mfenemies *enemies, u32 x, u32 y) {
  u8 *size = enemies->size;
  s32 *xpos = enemies->xpos;
  s32 *ypos = enemies->ypos;
  u64 alive = enemies->alive;
  bool hit = false;
  s32 ex = 0, ey = 0, exx = 0, eyy = 0;;
  u8 s = 0;
  for (u32 i = 0, len = enemies->count; i < len; i++) {
    if (alive_chk(alive, i)) {
      ex = (xpos[i] / 1000);
      ey = (ypos[i] / 1000);
      s = size[i];
      if (s == 1) {
        if ((hit = (ex == x && ey == y))) break;
      } else {
        exx = ex + 1;
        eyy = ey + 1;
        if ((hit = ((ex == x || exx == x) &&
                    (ey == y || eyy == y)))) break;
      }
    }
  }
  return hit;
}

void mfenemies_activate(mfenemies *enemies, u32 mx, u32 my, mfgravity gravity) {
  const u32 tenth_of_enemies = enemies->count / 10;
  u32 active_num = tenth_of_enemies;
  u8 *size = enemies->size;
  s32 *xpos = enemies->xpos;
  s32 *ypos = enemies->ypos;
  u32 *speed = enemies->speed;
  u64 alive = enemies->alive;
  u32 alive_cnt = enemies->alive_cnt;
  for (u32 i = 0, len = enemies->count; i < len; i++) {
    if (active_num != 0 && !(alive_chk(alive, i))) {
      active_num--;
      alive_cnt++;
      alive = alive_set(alive, i);
      size[i] = (rand() % 2 + 1);
      speed[i] = ((rand() % (20 - 2) + 1) + 2);
      switch (gravity) {
        case UP:
          xpos[i] = (rand() % mx + 1) * 1000;
          ypos[i] = my * 1000;
          break;
        case DOWN:
          xpos[i] = (rand() % mx + 1) * 1000;
          ypos[i] = 0;
          break;
        case LEFT:
          xpos[i] = mx * 1000;
          ypos[i] = (rand() % my + 1) * 1000;
          break;
        case RIGHT:
          xpos[i] = 0;
          ypos[i] = (rand() % my + 1) * 1000;
          break;
      }
    }
  }
  enemies->alive = alive;
  enemies->alive_cnt = alive_cnt;
}

void mfenemies_activate_enemy(mfenemies *enemies, u32 x, u32 y) {
  u64 alive = enemies->alive;
  s32 enemy = -1;
  for (s32 i = 0, len = enemies->count; i < len; i++) {
    if (!(alive_chk(alive, i))) {
      enemy = i;
      alive = alive_set(alive, i);
      break;
    }
  }
  if (enemy < 0) return;
  enemies->alive = alive;
  enemies->update[enemy] = 0;
  enemies->xpos[enemy] = x;
  enemies->ypos[enemy] = y;
  enemies->alive_cnt++;
}

s8 *mfenemies_enemy_to_string(const mfenemies *enemies, u32 num) {
  if (num >= enemies->count) return NULL;

  const u32 x = enemies->xpos[num];
  const u32 y = enemies->ypos[num];
  const u32 a = alive_chk(enemies->alive, num);
  const s8 format[] = "{%d, %d, %d}";
  s32 size = snprintf(NULL, 0, format, x, y, a) + 1;
  s8 *str = malloc(sizeof(s8) * (size));
  if (str != NULL) {
    snprintf(str, size, format, x, y, a);
  }
  return str;
}

s8 *mfenemies_to_string(const mfenemies *enemies) {
  const u32 count = enemies->count;
  s8 *strs[ENEMY_COUNT] = {NULL};
  size_t strs_length[ENEMY_COUNT] = {0};

  size_t total_length = 0;
  s8 *en_str = NULL;
  for (int i = 0; i < count; i++) {
    en_str = mfenemies_enemy_to_string(enemies, i);
    strs[i] = en_str;
    strs_length[i] = strlen(en_str);
    total_length += strs_length[i];
  }
  en_str = NULL;

  s8 *cat_str = malloc(sizeof(s8) * (total_length + (2 * (count - 1)) + 1));
  if (cat_str != NULL) {
    *cat_str = '\0';
    for (int i = 0; i < count; i++) {
      strncat(cat_str, strs[i], strs_length[i]);
      if (i < (count - 1))
        strncat(cat_str, ", ", 2);
      free(strs[i]);
      strs[i] = NULL;
    }
  } else {
    return NULL;
  }

  const s8 format[] = "{%s}";
  s32 size = snprintf(NULL, 0, format, cat_str) + 1;
  s8 *str = malloc(sizeof(s8) * size);
  if (str != NULL) {
    snprintf(str, size, format, cat_str);
  }
  free(cat_str);
  return str;
}
