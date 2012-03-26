
#ifndef __MFLOG_H__
#define __MFLOG_H__

#include "mfdefs.h"

extern void mflog_setup(void);
extern void mflog_shutdown(void);
extern s32 mflog(const s8 *format, ...);

#endif /* __MFLOG_H__ */
