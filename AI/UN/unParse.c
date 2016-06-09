/*****************************************************************************
 unParse.c    S.R.Lines  (spraxlo@{ai|media}, srl@athena)    93.7.14-16
 Just some convenience functions for parsing arglists like (argc,argv).
 *****************************************************************************/
#include	<stdio.h>
#include	<stdlib.h>
#include	<ctype.h>
#include	<limits.h>
#include	<errno.h>
#include	<unTypes.h>

#undef		 UNBUG
#define		 UNBUG	0
#include	"unBug.h"
#include	"unFlags.h"
#include	"unUsage.h"
#include	"unParse.h"


int unParse ( unsigned long *options, char **argStr, int *argInt, dbl *argDbl
            , int argc, char **argv, char **envp)
{
  char	*cp;
  int	j,k;
  dbl	tmpd;
  int 	numInt = 0, numDbl = 0, numStr = 0;
  
  for (j=1 ; j < argc; j++) {
    cp = argv[j];
    switch (argType(&tmpd, cp, j)) {
      case INT_:    argInt[++numInt] = (int) tmpd; break;
      case DOUBLE_: argDbl[++numDbl] =       tmpd; break;
      case STRING_: argStr[++numStr] =       cp;   break;
      case OPTSTR_:
        for (k=(int)tmpd;k>0;k--)
          addUnFlag(options,*++cp); break;
      case -1:
      default:
#if 0
        usage("Arg %d (%s): bad arg (name beginning with number?)\n",j,cp);
#else
        die("unParse: Arg %d (%s): bad arg (name beginning with number?)\n",j,cp);
#endif
    }
  }
  argDbl[0] = (dbl)   numDbl;
  argInt[0] = (int)   numInt;
  argStr[0] = (char*) numStr;
  return 0;
}


int argType(double *dp, char *cp, int argno)
{
  int     intval;
  double  dblval;
  char   *pfrac, cc = *cp;
  
  if ( isdigit(cc) ||
     ((isdigit(cp[1]) || isdigit(cp[2])) && (cc == '.' || cc == '-' || cc == '+')))
  {                                     /* It seems to be a number. */
    intval = (int)strtol(cp,&pfrac,10);  /* Get its int & frac parts. */
    WQQ3("after strtol, pfrac is %d(%s)\n",(int)*pfrac,(*pfrac=='\0'?"NIL":pfrac));
    if (errno) {
      perror("strtol");
      fprintf(stderr,"Arg %d:\tstrtol got %d from %s, errno %d.\n",argno,intval,cp,errno);
      exit(-1);
    }
    if ((cc = *pfrac) == '\0') {      /* Indeed, it's an integer. */
      *dp = intval;
      WQQ3("argType: *dp set to int %d  --  %d\n",intval, (int)*dp);
      return INT_;
    }
    else if (cc == '.' || cc == 'e' || cc == 'E')  {
      dblval = strtod(cp,&pfrac);      /* Now it seems to be floating point */
      WQQ4("after strtod: dblval: %g, pfrac: %d(%s)\n",dblval,(int)*pfrac,(*pfrac=='\0'?"NIL":pfrac));
      if (errno) {
        perror("strtol");
        fprintf(stderr,"Arg %d:\tstrtod got %g from %s, errno %d.\n",argno,dblval,cp,errno);
        exit(-1);
      }
      if (*pfrac == '\0') {           /* Indeed it is a float or double. */
        *dp = dblval;
        WQQ3("argType: *dp set to dbl %g  --  %g\n",dblval, *dp);
        return DOUBLE_;
      }
    }
    /* Still here?  There would seem to be more characters after the number.  */
    /* It could be just a name beginning with a number, or maybe something    */
    /* like 4.0F, meaning 4 as a float, not a double.  Should this be parsed? */
    dp = NULL;
    return -1;
  }
  return STRING_;  /* Ok, let's just say it wasn't a number after all. */
}



int argUnion ( ctypes *arg, char *cp, int argno)
{
  int     intval;
  double  dblval;
  char   *pfrac, cc = *cp;
  
  if ( isdigit(cc) ||
     ((isdigit(cp[1]) || isdigit(cp[2])) && (cc == '.' || cc == '-' || cc == '+')))
  {                                     /* It seems to be a number. */
    intval = (int)strtol(cp,&pfrac,10);  /* Get its int & frac parts. */
    WQQ3("after strtol, pfrac is %d(%s)\n",(int)*pfrac,(*pfrac=='\0'?"NIL":pfrac));
    if (errno) {
      perror("strtol");
      fprintf(stderr,"Arg %d:\tstrtol got %d from %s, errno %d.\n",argno,intval,cp,errno);
      exit(-1);
    }
    if ((cc = *pfrac) == '\0') {      /* Indeed, it's an integer. */
      arg->intval = intval;
      WQQ3("argUnion: arg->intval set to int %d  --  %d\n",intval,arg->intval);
      return INT_;
    }
    else if (cc == '.' || cc == 'e' || cc == 'E')  {
      dblval = strtod(cp,&pfrac);      /* Now it seems to be floating point */
      WQQ4("after strtod: dblval: %g, pfrac: %d(%s)\n",dblval,(int)*pfrac,(*pfrac=='\0'?"NIL":pfrac));
      if (errno) {
        perror("strtol");
        fprintf(stderr,"Arg %d:\tstrtod got %g from %s, errno %d.\n",argno,dblval,cp,errno);
        exit(-1);
      }
      if (*pfrac == '\0') {           /* Indeed it is a float or double. */
        arg->dblval = dblval;
        WQQ3("argUnion: arg->dblval set to dbl %g  --  %g\n",dblval,arg->dblval);
        return DOUBLE_;
      }
    }
    /* Still here?  There would seem to be more characters after the number.  */
    /* It could be just a name beginning with a number, or maybe something    */
    /* like 4.0F, meaning 4 as a float, not a double.  Should this be parsed? */
    arg->dblval = NaN;
    return -1;
  }
  arg->chrptr = cp;
  return STRING_;  /* Ok, let's just say it wasn't a number after all. */
}
















