
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "mfworld.h"

mfworld *mfworld_create(mfplayer *player, mfenemies *enemies) {
  mfworld init = {player, enemies, DOWN, (MS_PER_SEC * 30)};
  mfworld *world = NULL;
  world = malloc(sizeof(mfworld));
  if (world == NULL) {
    printf("mfworld creation failed: memory allocation failed\n");
  } else {
    memcpy(world, &init, sizeof(mfworld));
  }
  return world;
}

void mfworld_delete(mfworld *world) {
  mfplayer_delete(world->player);
  mfenemies_delete(world->enemies);
  free(world);
}

void mfworld_update(mfworld *world, u64 dt) {
  const mfgravity opts[4][2] = {{/*UP,*/   LEFT, RIGHT},
                                {/*DOWN,*/ LEFT, RIGHT},
                                {UP,   DOWN, /*LEFT*/},
                                {UP,   DOWN, /*RIGHT*/}}; 
  //  return;
  if (world->update < 0) {
    const mfgravity g = world->gravity;
    world->gravity = opts[g][(rand() % 2)];
    world->update = MS_PER_SEC * ((rand() % (30 - 10) + 1) + 10);
    //world->update = NS_PER_SEC * 30;
  } else {
    s64 dt_ms = dt / NS_PER_MS;
    //world->update -= ((dt_ms) ? dt_ms : 2);
    world->update -= dt_ms;
  }
}

s8 *mfworld_to_string(const mfworld *world) {
  s8 *ps = mfplayer_to_string(world->player);
  s8 *es = mfenemies_to_string(world->enemies);

  const s8 format[] = "{%s, %s}";
  s32 size = snprintf(NULL, 0, format, ps, es) + 1;
  s8 *str = malloc(sizeof(s8) * size);
  if (str != NULL) {
    snprintf(str, size, format, ps, es);
  }
  free(ps);
  free(es);
  return str;
}
