
#include <stdarg.h>
#include <stdio.h>

#include "mftimer.h"
#include "mflog.h"
#include "mffilesystem.h"

mffile *mflog_handle = NULL;
mftimer *timer = NULL;

void mflog_setup(void) {
  timer = mftimer_create();
  mftimer_init(timer);
  mftimer_reset(timer);
  mflog_handle = mffilesystem_open("minefield.log", "a");

  mflog("\n\n");
  mflog("*** [Logging to filesystem] ***");
}

void mflog_shutdown(void) {
  mffilesystem_close(mflog_handle);
}

s32 mflog(const s8 *format, ...) {
  if (mflog_handle == NULL) return -1;
  va_list ap;
  va_start(ap, format);

  fprintf(mflog_handle->file, "\n[%lu] ", mftimer_elapsed_ms(timer));
  vfprintf(mflog_handle->file, format, ap);
  fflush(mflog_handle->file);

  va_end(ap);

  return 0;
}
