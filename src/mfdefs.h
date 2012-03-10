
#ifndef __MFDEFS_H__
#define __MFDEFS_H__

#include <stdbool.h>

#define s8 char
#define u8 unsigned char
#define s16 short
#define u16 unsigned short
#define s32 int
#define u32 unsigned int
#define s64 long
#define u64 unsigned long

#define NS_PER_US 1000
#define NS_PER_MS 1000000
#define NS_PER_SEC 1000000000

#define US_PER_MS 1000
#define US_PER_SEC 1000000

#define MS_PER_SEC 1000

#define SEC_PER_MIN 60
#define MIN_PER_HOUR 60

#define NS_PER_FRAME (NS_PER_SEC / 60)
#define MS_PER_FRAME (ms_PER_SEC / 60);

typedef enum {UP, DOWN, LEFT, RIGHT} mfgravity;

typedef struct {u32 x, y;} u32Vec2;
typedef struct {u32 hour, min, sec, msec;} mfclock;

#endif /* __MFDEFS_H__ */
