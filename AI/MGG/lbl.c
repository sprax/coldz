/******************************************************************************
 lbl.c  -- convert file.labels to file.lbl
#include <unTypes.h>
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>

#define	SLEN 256
static char Ss[SLEN];

static void usage (char **argv)
{
  fprintf(stderr,"\nusage: %s inpath xsize [ysize] [outpath]\n",argv[0]);
}


int main (int argc, char **argv, char **envp)
{
FILE	*ifp, *ofp;
char 	opath[SLEN], name[SLEN], *ipath = argv[1];
double	ww, hh, wide = 300.0, tall = 300.0, dx,dy, sx,sy;

  if ( ! (ifp = fopen(ipath,"r"))) {
    fprintf(stderr,"fopen failed on <%s>\n",ipath);
    usage(argv);
    exit(-1);
  }
  if (argc > 2) { wide  = atof(argv[2]); tall = wide; }
  if (argc > 3) { tall  = atof(argv[3]); }
  if (argc > 4) { strcpy(opath,argv[4]); }
  else  { sprintf(opath,"%s.lbl",ipath); }

  if ( ! (ofp = fopen(opath,"w"))) {
    fprintf(stderr,"fopen failed on <%s>\n",opath);
    exit(-1);
  }

  
  fprintf(stderr,"%s --> %s, %f %f\n", ipath,opath,wide,tall);

  ww = wide - 1.0;
  hh = tall - 1.0;
  while (fgets (Ss, SLEN, ifp) != NULL) {
    if (sscanf (Ss, "%s %lf %lf", name, &sx, &sy) == 3) {
      dx = sx/ww, dy = (tall - 1.0 - sy)/hh;
      fprintf(ofp   ,"%s	%.8f	%.8f\n",name,dx,dy);
      fprintf(stderr,"%s	%.8f	%.8f  (from  %g  %g)\n"
	,name,dx,dy,(double)sx,(double)sy);
    }
    else fprintf(ofp,"%s\n",Ss);
  }
  fclose(ifp);
  fclose(ofp);
return 0;
}



