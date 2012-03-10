
#ifndef __MFPLAYER_H__
#define __MFPLAYER_H__

#include "mfdefs.h"

typedef struct {
  u32Vec2 pos, dim;
  bool alive;
} mfplayer;

extern mfplayer *mfplayer_create(void);
extern void mfplayer_delete(mfplayer *player);
extern void mfplayer_set_position(mfplayer *, u32, u32);
extern void mfplayer_horizontal_move(mfplayer *, s32, u32);
extern void mfplayer_update_position(mfplayer *, s32 dx, s32 dy, u32 mx, u32 my);
extern s8 *mfplayer_to_string(const mfplayer *player);

#define mfp_x(mfplayer) (mfplayer)->pos.x
#define mfp_y(mfplayer) (mfplayer)->pos.y
#define mfp_width(mfplayer) (myfplayer)->dim.x
#define mfp_height(mfplayer) (mfplayer)->dim.y
#define mfp_alive(mfplayer) (mfplayer)->alive

#endif /* __MFPLAYER_H__ */
