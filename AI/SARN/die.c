/****
 die.c
 ****/
#include <stdio.h>
#include <varargs.h>

/*****************************************************************************/

/* Funtion to print an error message on stderr and die */
/* usage: die(format,arg1,arg2,...,argn)               */

die(va_alist) va_dcl
{
  va_list args;
  char *format;

  va_start(args);
  format = va_arg(args,char *);
  (void) vfprintf(stderr,format,args);
  va_end(args);
  exit(-1);
}

/*****************************************************************************/

