/****************************************************************************
 va.c -- varags exp.    S.R.Lines (spraxlo@ai)    93.6.21
 ****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

static int flags[4] = { 0,1,2,3 };

#ifdef DBLARG /*+++++++++++++++++++++++++++++++++++++++++++++++++*/
/*
  The following dbug routine works only if its prototype is
  neither seen nor inferred by the compiler, or else all its
  arguments are (cast to) the proper type.
  In K&R or "traditional" C, it works with a variable number of args;
  for other compilers, the arg list may have to be padded.

int dbug(int flag,pch fmt, dbl a1,dbl a2,dbl a3,dbl a4,dbl a5);
*/

int dbug(flag,fmt,a1,a2,a3,a4,a5)
int flag; char *fmt; double a1,a2,a3,a4,a5;
{
  if (flags[flag])
    fprintf(stderr,fmt,a1,a2,a3,a4,a5);
}
#endif /* DBLARG ------------------------------------------------*/


#ifdef VARARG /*+++++++++++++++++++++++++++++++++++++++++++++++++*/
#include <varargs.h>
/*
  The following varargs dbug routine works only with no prototype,
  at least in THINK C.
*/
int dbug(va_alist)
va_dcl
{
va_list args;
int flag;
char *fmt;
  va_start(args);
  flag = va_arg(args,int);
  fmt  = va_arg(args,char *);
  if (flags[flag])
    vfprintf(stderr,fmt,args);
  va_end(args);
}
#endif /* VARARG ------------------------------------------------*/

#define STDARG 1
#ifdef  STDARG /*+++++++++++++++++++++++++++++++++++++++++++++++++*/
#include <stdarg.h>
/*
  The following dbug routine works if ... is allowed in prototypes,
  or if its prototype is neither seen nor inferred by the compiler.
*/

void dbug(int flag, const char *fmt, ...)
/* int dbug(fmt) const char *fmt; */
{
va_list args;
  va_start(args,fmt);
  fprintf(stderr,"(int) args[0] == %d\n",(int)(* (((int *)args)) ));
  if (flags[flag])
    vfprintf(stderr,fmt,args);
  va_end(args);
}
#endif /* STDARG ------------------------------------------------*/


int main(int argc, char *argv[])
{
int mod, save;
float floo;
char ts[80], ss[80];
  printf("Welcome to VARGLAND.\n");
  
  for (;;)  {
    for (save = 0; !save; )  {
      printf("Enter a number:  ");
      fflush(stdout); gets(ts);
      if (ts[0] == 'q')  exit(0);
      save = sscanf(ts," %d",&mod);
    }
    dbug(3,"Your number is:  %d\n",mod);
    for (save = 0; !save; )  {
      printf("Enter a float and a string:  ");
      fflush(stdout); gets(ts);
      if (ts[0] == 'q')  exit(0);
      save = sscanf(ts," %g %s",&floo,ss);
    }
    dbug(1,"Float & string are: %g  %s\n",floo,ss,0,0);
  }
}

