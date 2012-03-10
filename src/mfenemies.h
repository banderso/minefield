
#ifndef __MFENEMIES_H__
#define __MFENEMIES_H__

#include <stdbool.h>

#include "mfdefs.h"

#define ENEMY_COUNT 50
typedef struct {
  const u32 count;
  s64 next_update;
  u32 alive_cnt;
  u8 size[ENEMY_COUNT];
  s32 xpos[ENEMY_COUNT];
  s32 ypos[ENEMY_COUNT];
  u32 speed[ENEMY_COUNT];
  s32 update[ENEMY_COUNT];
  bool alive[ENEMY_COUNT];
} mfenemies;

extern mfenemies *mfenemies_create(void);
extern void mfenemies_delete(mfenemies *enemies);
//extern void mfenemies_init(mfenemies *enemies, s32 mx, s32 my);
extern u32 mfenemies_update_position(mfenemies *enemies, u32 mx, u32 my, mfgravity gravity, u64 dt);
extern bool mfenemies_check_position(mfenemies *enemies, u32 x, u32 y);
extern void mfenemies_activate(mfenemies *enemies, u32 mx, u32 my, mfgravity gravity);
extern void mfenemies_activate_enemy(mfenemies *enemies, u32 x, u32 y);
extern s8 *mfenemies_enemy_to_string(const mfenemies *enemies, u32 num);
extern s8 *mfenemies_to_string(const mfenemies *enemies);

#endif /* __MFENEMIES_H__ */
