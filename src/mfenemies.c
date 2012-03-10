
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "mfenemies.h"

mfenemies *mfenemies_create(void) {
  mfenemies init = { ENEMY_COUNT, 0, 0, {0}, {0}, {0}, {0}, {0} };
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
  bool *alive = enemies->alive;
  s32 *xpos = enemies->xpos;
  s32 *ypos = enemies->ypos;
  u8 *size = enemies->size;
  u32 *speed = enemies->speed;
  s32 *update = enemies->update;
  u32 alive_cnt = enemies->alive_cnt;
 
  bool dead = false;
  u32 total_death = 0;
  for (s32 i = 0, len = enemies->count; i < len; i++) {
    if (alive[i]) {
      if (update[i] < 0) {
        update[i] = NS_PER_FRAME * speed[i];
        switch (gravity) {
          case UP:
            ypos[i] -= 2;
            dead = (ypos[i] < 0);
            break;
          case DOWN:
            ypos[i] += 2;
            dead = (ypos[i] > my);
            break;
          case LEFT:
            xpos[i] -= 2;
            dead = (xpos[i] < 0);
            break;
          case RIGHT:
            xpos[i] += 2;
            dead = (xpos[i] > mx);
            break;
        }
        if (dead) {
          dead = false;
          alive[i] = false;
          alive_cnt--;
          total_death += size[i];
        }
      } else {
        update[i] -= dt;
      }
    }
  }
  enemies->alive_cnt = alive_cnt;
  return total_death;
}

bool mfenemies_check_position(mfenemies *enemies, u32 x, u32 y) {
  u8 *size = enemies->size;
  s32 *xpos = enemies->xpos;
  s32 *ypos = enemies->ypos;
  bool *alive = enemies->alive;
  bool hit = false;
  s32 ex = 0, ey = 0, exx = 0, eyy = 0;;
  u8 s = 0;
  for (u32 i = 0, len = enemies->count; i < len; i++) {
    if (alive[i]) {
      ex = xpos[i];
      ey = ypos[i];
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
  bool *alive = enemies->alive;
  u32 alive_cnt = enemies->alive_cnt;
  for (u32 i = 0, len = enemies->count; i < len; i++) {
    if (active_num != 0 && !alive[i]) {
      active_num--;
      alive_cnt++;
      alive[i] = true;
      size[i] = (rand() % 2 + 1);
      speed[i] = ((rand() % (15 - 2) + 1) + 2);
      switch (gravity) {
        case UP:
          xpos[i] = (rand() % mx + 1);
          ypos[i] = my;
          break;
        case DOWN:
          xpos[i] = (rand() % mx + 1);
          ypos[i] = 0;
          break;
        case LEFT:
          xpos[i] = mx;
          ypos[i] = (rand() % my + 1);
          break;
        case RIGHT:
          xpos[i] = 0;
          ypos[i] = (rand() % my + 1);
          break;
      }
    }
  }
  enemies->alive_cnt = alive_cnt;
}

void mfenemies_activate_enemy(mfenemies *enemies, u32 x, u32 y) {
  bool *alive = enemies->alive;
  s32 enemy = -1;
  for (s32 i = 0, len = enemies->count; i < len; i++) {
    if (!alive[i]) {
      enemy = i;
      break;
    }
  }
  if (enemy < 0) return;
  alive[enemy] = true;
  enemies->update[enemy] = 0;
  enemies->xpos[enemy] = x;
  enemies->ypos[enemy] = y;
  enemies->alive_cnt++;
}

s8 *mfenemies_enemy_to_string(const mfenemies *enemies, u32 num) {
  if (num >= enemies->count) return NULL;

  const u32 x = enemies->xpos[num];
  const u32 y = enemies->ypos[num];
  const bool a = enemies->alive[num];
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
