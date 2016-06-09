/* Functions for program profiling */

#include <sys/types.h>
#include <time.h>
#define CLOCK_TICKS_PER_SEC 1000000.0

typedef struct {
  long past;			/* last recorded time */
  long accumulated;		/* accumulated time */
  unsigned long invocations;	/* number of invocations */
} TIMER;

TIMER *alloc_timer();
void timer_clear();
void timer_reset();
void timer_mark();
void timer_stats();
void free_timer();


