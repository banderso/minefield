
#ifndef __MFWORLD_H__
#define __MFWORLD_H__

#include "mfdefs.h"
#include "mfplayer.h"
#include "mfenemies.h"

typedef struct {
  mfplayer * const player;
  mfenemies * const enemies;
  mfgravity gravity;
  s64 update;
} mfworld;

extern mfworld *mfworld_create(mfplayer *player, mfenemies *enemies);
extern void mfworld_delete(mfworld *world);
extern void mfworld_update(mfworld *world, u64 dt);
extern s8 *mfworld_to_string(const mfworld *world);

#endif /* __MFWORLD_H__ */
