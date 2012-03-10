
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef __APPLE__
#include <mach/mach_time.h>
#endif

#include "mftimer.h"

mftimer *mftimer_create(void) {
  mftimer init = {{0, 0}, 0};
  mftimer *timer = NULL;
  timer = malloc(sizeof(mftimer));
  if (timer == NULL) {
    printf("mftimer creation failed: memory allocation failed\n");
  } else {
    memcpy(timer, &init, sizeof(mftimer));
  }
  return timer;
}

void mftimer_delete(mftimer *timer) {
  free(timer);
}

void mftimer_init(mftimer *timer) {
#ifdef __APPLE__
  mach_timebase_info(&(timer->timebase));
#endif /* __APPLE__ */
}

u64 mftimer_now(mftimer *timer) {
#ifdef __APPLE__
  return (mach_absolute_time() * timer->timebase.numer) / timer->timebase.denom;
#endif /* __APPLE__ */
}

void mftimer_reset(mftimer *timer) {
#ifdef __APPLE__
  timer->reference = mach_absolute_time();
#endif /* __APPLE__ */
}

void mftimer_update(mftimer *timer, mfclock *clock) {
#ifdef __APPLE__
  u32 numer = timer->timebase.numer;
  u32 denom = timer->timebase.denom;
  u64 time = mach_absolute_time();
  u64 elapsed_ns = (((time - timer->reference) * numer) / denom);
  timer->reference = time;

  u32 hour = clock->hour,
      min  = clock->min,
      sec  = clock->sec,
      msec = clock->msec;

  u64 elapsed_ms = elapsed_ns / NS_PER_MS;
  if (elapsed_ms) {
    msec += elapsed_ms;

    if ((msec  / MS_PER_SEC) >= 1) {
      sec++;
      msec = 0;
    
      if ((sec / SEC_PER_MIN) >= 1) {
        min++;
        sec = 0;
      
        if ((min / MIN_PER_HOUR) >= 1) {
          hour++;
          min = 0;
  
          clock->hour = hour;
        }
      
        clock->min = min;
      }
    
      clock->sec = sec;
    }

    clock->msec = msec;

  }
  
#endif /* __APPLE__ */
}

  u64 mftimer_elapsed(mftimer *timer) {
#ifdef __APPLE__
  return ((mach_absolute_time() - timer->reference) * timer->timebase.numer) / timer->timebase.denom;
#endif /* __APPLE__ */
}

  u64 mftimer_elapsed_us(mftimer *timer) {
#ifdef __APPLE__
  return (((mach_absolute_time() - timer->reference) * timer->timebase.numer) / timer->timebase.denom) / NS_PER_US;
#endif /* __APPLE__ */
}

  u64 mftimer_elapsed_ms(mftimer *timer) {
#ifdef __APPLE__
  return (((mach_absolute_time() - timer->reference) * timer->timebase.numer) / timer->timebase.denom) / NS_PER_MS;
#endif /* __APPLE__ */
}

  u32 mftimer_elapsed_s(mftimer *timer) {
#ifdef __APPLE__
  return (((mach_absolute_time() - timer->reference) * timer->timebase.numer) / timer->timebase.denom) / NS_PER_SEC;
#endif /* __APPLE__ */
}

  u64 mftimer_elapsed_reset(mftimer *timer) {
#ifdef __APPLE__
  u64 now = mach_absolute_time();
  u64 elapsed = now - timer->reference;
  timer->reference = now;
  return (elapsed * timer->timebase.numer) / timer->timebase.denom;
#endif /* __APPLE__ */
}

  void mftimer_wait(mftimer *timer, u64 time_in_ns) {
#ifdef __APPLE__
  mach_wait_until(mftimer_now(timer) + time_in_ns);
#endif /* __APPLE__ */
}
