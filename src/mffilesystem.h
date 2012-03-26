
#ifndef __MFFILESYSTEM_H__
#define __MFFILESYSTEM_H__

#include "mfdefs.h"

#include <stdarg.h>

typedef struct {
  void *file;
  u32 size;
} mffile;

extern void mffilesystem_setup(void);
extern void mffilesystem_shutdown(void);

extern mffile *mffilesystem_open(const s8 *filename, s8 *mode);
extern void mffilesystem_close(mffile *file);

//s32 mffilesystem_write(mffile *file, const s8 *format, ...);

#endif /* __MFFILESYSTEM_H__ */
