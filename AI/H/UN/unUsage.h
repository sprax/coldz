/*****************************************************************************
 unUsage.h -- varargs usage macro.    S.R.Lines (spraxlo@ai.mit.edu)   93.7.11
 Use this macro to define a simple varargs usage function, e.g:

   static char *ProgName;
   USAGE(ProgName,"inFile outFile [options]","<explanation>","<more info>")
   main(int argc, char **argv) {
     ProgName=argv[0];
     if (argc<3) usage("No files!\n");
     printf("\n%s: %s %s %s\n",ProgName,argv[1],argv[2],(argc>3?argv[3]:""));
   }
#include	<stdio.h>
#include	<stdlib.h>
 *****************************************************************************/
#ifndef unUsage_h__
#define unUsage_h__

#include	<stdarg.h>

extern void Usage (char *fmt, ...);

#define USAGE(progName,argStr,strA,strB)		\
void Usage (char *fmt, ...)				\
{							\
  if (*fmt)  {						\
    char errs[512];					\
    va_list  args;					\
    va_start(args,fmt);					\
    vsprintf(errs,fmt,args);				\
    fprintf(stderr,"\n    %s\n", errs);			\
    va_end(args);					\
  }							\
  fprintf(stderr,"\nUsage:  %s %s\n",progName,argStr);	\
  fprintf(stderr,"%s\n",strA);				\
  fprintf(stderr,"%s\n",strB);  			\
  exit(-1);						\
}	/* NB: Put no semi-colon after "USAGE(...)"  */	\

#endif	/* unUsage_h__ */


