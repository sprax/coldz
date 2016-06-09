/****************************************************************************
 mainN.c    for inn, etc.    S.R.Lines    95.5.27
	warpMOB        *warpMob=inn;
 *****************************************************************************/
#include        "stdlib.h"
#include	"mainN.h"	/* GLOBAL variables initialized there. */
char           *ProgName="inn";                /* GLOBAL */
int		NumSrc=2, NumInt=6, NumDbl=6;   /* GLOBAL */
unl		Options=0;      /* X_GUI */     /* GLOBAL */
unl             Devopts=0;                      /* GLOBAL */
char 		Gstr[SLEN];
FILE		*LogFile=NULL;

int	InUndoIndex = 0;	/* FeX[2] holds copy for undo */
int	InRedoIndex = 0;	/* FeX[2] holds copy for undo */
ppf     FeX[NEX], FeY[NEX], FeG[NEX]; /* EXTRA GLOBAL, for now @@ */


int main (int argc, char **argv, char **envp)
{
static	pMOB 	mob[MAXSRC];
static  MobData	wwD;
Dmj     iD,fD,sD,dD;
int 	j;

#if 0
  death_trap = &segFault;
#endif

  NumSrc = jkParse(argc,argv,envp);

  /* inInit(); */

#if 0
  if (NumSrc == 8 || NumSrc >= 12)  Devopts |= O_PTRS;
#endif

  inReadAll(mob,&wwD,&dD,&sD,&iD,&fD,ArgStr);
  LogFile = jkLog(argc,argv,ArgStr,Ntweens,Options,TrsBeg,TrsEnd,dD.w,dD.h);

  for (j = 0; j < NEX; j++) {
    FeX[j] = rowMalFlt(fD.x,fD.y,fD.w,fD.h);
    FeY[j] = rowMalFlt(fD.x,fD.y,fD.w,fD.h);
    FeG[j] = rowMalFlt(fD.x,fD.y,fD.w,fD.h);
  }

  j = winWarpFwdN ( mob, &wwD, NumSrc );
  winLoop(0);

  fprintf(stderr,"mainN sez Bye-Bye!\n");
  return  j;
} /* ____________________________ END OF MAIN ______________________________ */














#if 0
void Usage (char *fmt, ...)
{
  if (*fmt)  {
    char errs[512];
    va_list  args;
    va_start(args,fmt);
    vsprintf(errs,fmt,args);
    fprintf(stderr,"\n    %s\n", errs);
    va_end(args);
  }
  fprintf(stderr,"\nUsage:  %s %s\n",progName,argStr);
  fprintf(stderr,"%s\n",strA);
  fprintf(stderr,"%s\n",strB);
  exit(-1);
}
#else

USAGE(ProgName,"imA imB [N] [Beg] [End] [Mid] [Inf] [-OFF] [-bchlmstuvwx]\n\
where:  A,B are image files (Sun,SGI,etc.)\n\
        N   (int>0) is the number of tweens to output (default: 0),\n\
        Beg (float) is the trs parm of first tween (default: 0.0)\n\
        End (float) is the trs parm of final tween (default: 1.0)\n\
        Mid (0.0<1.0) says where to switch from A=>B to B=>A (default: .75)\n\
        Inf (float) is the forward warping hole threshold (0.5)\n\
        OFF (string) is the output file format (\"sgi\", \"sun\", etc).\n\
        For a list of all other single letter optons, use -h.","\
FUNC: Two-image morphing using backward correspondences both ways,\n","")

#endif


