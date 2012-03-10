
#ifndef __MFGAME_H__
#define __MFGAME_H__

#include "mfdefs.h"
#include "mfworld.h"

typedef struct {
  u32 mx, //max screen x
    my, //max screen y
    score;
  bool running;
  mfclock clock;
  mfworld * const world;
} mfgame;

extern mfgame *mfgame_create(mfworld *world);
extern void mfgame_delete(mfgame *game);

extern s8 *mfgame_to_string(const mfgame *game);
extern void mfgame_print(mfgame *game);
extern mfgame *mfgame_update(mfgame *game, u64 dt);
extern mfgame *mfgame_render(mfgame *game, u64 elapsed_time, u64 processed_time);
extern void mfgame_run(mfgame *game);

#endif /* __MFGAME_H__ */
