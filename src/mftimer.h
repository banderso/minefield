
#ifndef __TIMER_H__
#define __TIMER_H__

#ifdef __APPLE__

#include <mach/mach_time.h>

#endif

#include "mfdefs.h"

typedef struct {
#ifdef __APPLE__
  mach_timebase_info_data_t timebase;
  u64 reference;
  mfclock clock;
#endif
} mftimer;

extern mftimer *mftimer_create(void);
extern void mftimer_delete(mftimer *timer);

extern void mftimer_init(mftimer *timer);
extern u64 mftimer_now(mftimer *time);
extern void mftimer_reset(mftimer *timer);
extern void mftimer_update(mftimer *timer, mfclock *clock);
extern u64 mftimer_elapsed(mftimer *timer);
extern u64 mftimer_elapsed_us(mftimer *timer);
extern u64 mftimer_elapsed_ms(mftimer *timer);
extern u32 mftimer_elapsed_s(mftimer *timer);
extern u64 mftimer_elapsed_reset(mftimer *timer);
extern void mftimer_wait(mftimer *timer, u64 time_in_ns);

#endif /* __TIMER_H__ */
