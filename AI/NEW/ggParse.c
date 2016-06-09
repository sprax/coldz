/*****************************************************************************
 ggParse.c    S.R.Lines  (spraxlo@{ai|media}, srl@athena)    93.10.19
 Convenience functions for parsing arglists like (argc,argv).
 NB: All arguments up to the first one beginning with a digit or '-'
     are taken as input image file pathnames; this rules out pathnames
     with a number.
 *****************************************************************************/
#if 1
#include	<stdio.h>
#include	<string.h>
#include	<stdlib.h>
#include	<ctype.h>
#include	<limits.h>
#include	<errno.h>
#include	<un/unBug.h>
#include	<un/unTypes.h>
#include	<un/unFlags.h>
#include	<un/unUsage.h>
#include	<un/unParse.h>
#include	<imagic.h>
#include	"ggUsage.h"
#include	"ggParse.h"
#define	MAXTWEENS 1000
#define MAXSRC 9        /* Only for declarations; not for loop control. */
#define MAXARG 16       /* Only for declarations; not for loop control. */
extern  char *ProgName;
#else
#include	"gg.h"
#endif

void
ggInitArgs ( unl *options, char **argStr, int *argInt, dbl *argDbl
            , int argc, char **argv, char **envp
            , int numSrc, int numStr, int numInt, int numDbl)
{
char 	dirname[256];
int 	j, q, z;
  z = (int) argStr[0];
  if (z < numSrc+2) {
    sprintf(dirname,"%s./%s",ProgName,argStr[1]);
    for (j = 2; j <= numSrc; j++) {
      strcat(dirname,".");
      strcat(dirname,argStr[j]);
    }
    argStr[numSrc+2] = dirname;
  }
  if (z < numSrc+1)
    argInt[MAXARG-1] = FF_SGI;	/* was: outFF */
  else if ((q = FFfromString(argStr[numSrc+1])) == FF_UNKNOWN)
    Usage("Unknown output file format (%d) -- see imagic.h",q);
  else argInt[MAXARG-1] = q;	/* was: outFF */

  z = argInt[0];
  if (z < 2)  argInt[2] = 1;	/* was: ntweens or vert */
  if (z < 1)  argInt[1] = 1;	/* was: ntweens or horz */
  else if ((q=argInt[1]) < 0 || q > MAXTWEENS) {
    warn("\tNtweens must be between 0 and %d, not %d -- reset to 1.",MAXTWEENS,q);
    argInt[1] = 1;		/* was: ntweens; warning should be in warpHun  */
  }
    
  z = (int) argDbl[0];		/* Not all warpH's use all dbl parms. */
  if (z < 6)  argDbl[6] = 1.00;	/* was: lenY */
  if (z < 5)  argDbl[5] = 0.00;	/* was: begY */
  if (z < 4)  argDbl[4] = 0.30;	/* was: inFill */
  if (z < 3)  argDbl[3] = 0.50;	/* was: trsMed */
  if (z < 2)  argDbl[2] = 1.00;	/* was: trsEnd, or lenX */
  if (z < 1)  argDbl[1] = 0.00;	/* was: trsBeg, or begX */

  /* warn("trsBeg & End: %f %f",argDbl[1],argDbl[2]); */
}

int ggParse ( unl *options, unl *devopt
            , char **argStr, int *argInt, dbl *argDbl
            , int argc, char **argv, char **envp
            , int numSrc, int maxStr, int maxInt, int maxDbl)
{
  char	*cp;
  int	j;
  dbl	tmpd;
  int 	numInt = 0, numDbl = 0, numStr = 0;
  
  if (argc < numSrc+1) {
    if (argv[1][0] == '-' && argv[1][1] == 'h')
      wrn("Help is not implemented...");  		/** Help(""); **/
    else
      Usage("\tAt least the first %d args to %s must be pathnames.",numSrc,argv[0]);
  }

  for (j = 1 ; j < argc; j++) {
    cp = argv[j];
    switch (argType(&tmpd, cp, j)) {
      case INT_:
        argInt[++numInt] = (int) tmpd;
        if (numInt > maxInt)
          Usage("Arg %d:\tToo many int parms!  (%s --> %d)",j,cp,argInt[numInt]);
        break;
      case DOUBLE_:
        argDbl[++numDbl] = tmpd;
        if (numDbl > maxDbl)
          Usage("Arg %d:\tToo many float parms!  (%s --> %g)",j,cp,argDbl[numDbl]);
        break;
      case STRING_:
      argStr[++numStr] = cp;
        if (numStr > maxStr)
          Usage("Arg %d:\tToo many string parms!  (%s)",j,cp);
        break;
      case OPTSTR_: 	/* Could differentiate between -options and +options */
          addUnFlags(options,devopt,++cp,0);
        break;
      case -1:
      default:
        Usage("Arg %d (%s): bad arg (string beginning with number?)\n",j,cp);
    }
  }
  argDbl[0] = (dbl)   numDbl;
  argInt[0] = (int)   numInt;
  argStr[0] = (char*) numStr;
  /* if (*options & O_VERB)  ggParsePrn(*options,argStr,argInt,argDbl); */
  return numStr;
}




void ggParsePrn (unl options, char **argStr, int *argInt, dbl *argDbl)
{
  int j,q;
  fprintf(stderr,"tstUnParse: options == %d\n", options);
  for (q = (int) argStr[0], j = 1; j <= q; j++)
    fprintf(stderr,"argStr[%2d]: \t %s\n", j, argStr[j]);
  for (q = (int) argInt[0], j = 1; j <= q; j++)
    fprintf(stderr,"argInt[%2d]: \t %d\n", j, argInt[j]);
  for (q = (int) argDbl[0], j = 1; j <= q; j++)
    fprintf(stderr,"argDbl[%2d]: \t %g\n", j, argDbl[j]);
}
