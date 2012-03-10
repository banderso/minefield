
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "mfplayer.h"

mfplayer *mfplayer_create(void) {
  mfplayer init = { {0, 0}, {1, 1}, 1 };
  mfplayer *player = NULL;
  player = malloc(sizeof(mfplayer));
  if (player == NULL) {
    printf("mfplayer creation failed: memory allocation failed\n");
  } else {
    memcpy(player, &init, sizeof(mfplayer));
  }
  return player;
}

void mfplayer_delete(mfplayer *player) {
  free(player);
}

void mfplayer_set_position(mfplayer *player, u32 x, u32 y) {
  player->pos.x = x;
  player->pos.y = y;
}

void mfplayer_horizontal_move(mfplayer *player, s32 dir, u32 max) {
  u32 px = player->pos.x;
  s32 npx = px + (dir * 2);
  player->pos.x = ((npx >= 0 && npx < max) ? npx : px);
}

void mfplayer_update_position(mfplayer *player, s32 dx, s32 dy, u32 mx, u32 my) {
  u32 px = mfp_x(player), py = mfp_y(player);
  s32 npx = px + (dx * 3);
  s32 npy = py + (dy * 2);
  mfp_x(player) = ((npx >= 0 && npx < mx) ? npx : px);
  mfp_y(player) = ((npy >= 0 && npy < my) ? npy : py);
}

s8 *mfplayer_to_string(const mfplayer *player) {
  u32 x = player->pos.x;
  u32 y = player->pos.y;
  u32 w = player->dim.x;
  u32 h = player->dim.y;
  bool a = player->alive;
  const s8 format[] = "{[%d, %d], [%d, %d], %d}";
  s32 size = snprintf(NULL, 0, format, x, y, w, h, a) + 1;
  s8 *str = malloc(sizeof(s8) * (size));
  if (str != NULL) {
    snprintf(str, size, format, x, y, w, h, a);
  }
  return str;
}
