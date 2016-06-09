/*****************************************************************************
 unBug.c    S.R. Lines  (spraxlo@ai.mit.edu)   93.6.27
 NB: All library functions must be compiled, even if UNBUG is 0.
 This module is in unLib mainly to ensure its own portability.
 *****************************************************************************/
#include <stdarg.h>

#define   UNBUG 1	/* To unhide functions sharing same names as macros */
#include "unBug.h"

int unassume(char *x, char *file, int line)	/* For the macro assume(expr) */
{
  fprintf(stderr,"unassume (%s) in %s, line %d\n",x,file,line);
  return 0;    /* Unlike assert, just return false, no exit. */
}

void eqq(const char *fmt, ...)		/* Print and Exit */
{
#if UNBUG
    va_list  args;
    va_start(args,fmt);
    (void) vfprintf(stderr,fmt,args);
    va_end(args);
    exit(0);
#endif
}

void wqq(const char *fmt, ...)		/* Print only */ 
{
#if UNBUG
    static int tc = 0;
    va_list  args;
    va_start(args,fmt);
    (void)  fprintf(stderr,"KK %2d: ", ++tc);
    (void) vfprintf(stderr,fmt,args);
    va_end(args);
#endif
}

void die (const char *fmt, ...)
{
  va_list  args;
  va_start(args,fmt);
  (void)  fprintf(stderr,"die: ");
  (void) vfprintf(stderr,fmt,args);
  (void)  fprintf(stderr,"\n");
  va_end(args);
  exit(-1);
}

void warn (const char *fmt, ...)
{
  va_list  args;
  va_start(args,fmt);
  (void)  fprintf(stderr,"Warning -- ");
  (void) vfprintf(stderr,fmt,args);
  (void)  fprintf(stderr,"\n");
  va_end(args);
}

#define BUFLEN 4

void stay (const char *fmt, ...)
{
  char ts[BUFLEN];
  va_list  args;
  va_start(args,fmt);
  (void)  fprintf(stderr,"STAY -- ");
  (void) vfprintf(stderr,fmt,args);
  (void)  fprintf(stderr,"\n");
  va_end(args);
  fprintf(stderr,"...hit return to continue...");
  fflush( stderr);
  gets_s(ts, BUFLEN); 
}


