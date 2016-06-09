/* Functions for program profiling */


#include "profile.h"
#include <stdio.h>
#include <malloc.h>

TIMER *alloc_timer()
{
  TIMER *t;
  t = (TIMER *) malloc(sizeof(TIMER));
  t->accumulated = t->invocations = 0;
  t->past = clock();
  return t;
}

void timer_clear(t)
TIMER *t;
{
  t->accumulated = t->invocations = 0;
  t->past = clock(); 
}

void timer_reset(t)
TIMER *t;
{
  t->past = clock();
}

void timer_mark(t)
TIMER *t;
{
  time_t present;

  present = clock();
  t->accumulated += present - t->past;
  t->invocations++;
  t->past = present;
}

void timer_stats(t,s)
TIMER *t;
char *s;
{
  printf("%s: Number of invocations %d,  Total time: %f secs\n",
    s, t->invocations, (double) t->accumulated/CLOCK_TICKS_PER_SEC);
  printf("%s: Avg time per invocation: %f secs\n",
    s, (double) t->accumulated/(double) t->invocations/CLOCK_TICKS_PER_SEC);
}

void free_timer(t)  /* @@ Originally, t had no type -- spraxlo */
TIMER *t;
{
  free(t);
}

