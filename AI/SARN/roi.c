/* Utilities for Region of Interest */

#include "imgdec.h"
#include "imgmacro.h"
#define TRUE 1
#define FALSE 0

typedef struct roi {
  float x[4],y[4];
  int levels[MAXLEV];
  int nlev;
  struct roi *next;
} ROI;

roi2mask(pyr,botlev,finlev,toplev,roi)
BPYR pyr;
int botlev,finlev,toplev;
ROI *roi;
{
}

static line_check(x0,y0,x1,y1,x,y)
float x0,y0,x1,y1,x,y;
{
  return ((x1-x0)*(y-y0) - (x-x0)*(y1-y0) >= 0.0) ? 1 : 0;
}

getrectroimask(pyr,botlev,finlev,toplev,x,y,w,h)
BPYR pyr;
int botlev,finlev,toplev,x,y,w,h;
{
   int i,j,l=x,r=x+w-1,b=y,t=y+h-1,lev;

   for (lev=botlev; lev <= toplev; lev++) {
     double left,bottom,right,top;
     if (lev >= finlev)
       for (j=0; j < pyr[lev].h; j++)
         for (i=0; i < pyr[lev].w; i++)
           if (i >= l && i <= r && j >= b && j <= t)
             pyr[lev].ptr[j][i] = 255;
           else
             pyr[lev].ptr[j][i] = 0;
     left = ceil(((double) l)/2.0);
     bottom = ceil(((double) b)/2.0);
     right = floor(((double) r)/2.0);
     top = floor(((double) t)/2.0);
     l = nint(left);  r = nint(right);  b = nint(bottom);  t = nint(top);
   }
}


#if 0

ROI *getroi(file)
char *file;
{
  ROI *roi = NULL;
  FILE *stream;

  if (!(stream = fopen_s(file,"r")))
    return (ROI *) NULL;
}

getroimask(pyr,botlev,finlev,toplev,roifile)
BPYR pyr;
int botlev,finlev,toplev;
char *roifile;
{
  ROI *roi,getroi();

  roi = getroi(roifile)
  roi2mask(pyr,botlev,finlev,toplev,roi);
  for (p=roi; p; p=q) {
    q = p->next; free(p);
  }
}

#endif

getroimask(pyr,botlev,finlev,toplev,roifile)
BPYR pyr;
int botlev,finlev,toplev;
char *roifile;
{
  char line[256],*s,*l;
  float x[4],y[4],xs[4],ys[4],scale;
  int i,j,k,k1,k2,lev;
  int line_check(),inside;
  int reg;
  FILE *stream;

/* Format for ROI file */
/*  Name Region=(x1 y1 x2 y2 x3 y3 x4 y4) Levels={ X, Y, Z ... } */

  if (!roifile) {
    for (lev=finlev; lev <= toplev; lev++)
      bset(pyr[lev],255);
    return(1);
  } else
    for (lev=finlev; lev <= toplev; lev++)
      bset(pyr[lev],0);

  if (!(stream = fopen(roifile,"r")))
    die("Error: couldn't open %s for reading\n",roifile);

  for (reg=0; fgets(line,256,stream); reg++) {
    if (!(s = strchr(line,'('))) 
      die("init_roi(): syntax error at line %d of file %s\n",reg+1,roifile);
    if (sscanf(++s,"%f %f %f %f %f %f %f %f", &x[0],&y[0],&x[1],&y[1],&x[2],&y[2],&x[3],&y[3]) != 8)
      die("init_roi(): syntax error at line %d of file %s\n",reg+1,roifile);
    for (k=0; k < 4; k++) {
      k1 = (k+1)%4;   k2 = (k+2)%4;
      if (!line_check(x[k],y[k],x[k1],y[k1],x[k2],y[k2]))
        die("init_roi(): Region %d of file %s is concave\n",reg,roifile);
    }
    if (!(s=strchr(line,'}')))
      die("init_roi(): syntax error at line %d of file %s\n",reg+1,roifile);
    else
      *s = '\0';
    for (l=strchr(line,'{'), s=strtok(++l,","); s; s=strtok(NULL,",")) {
      lev = atoi(s);      scale = 1 << lev;
      if (lev < finlev || lev > toplev)
        continue;
      for (k=0; k < 4; k++) {
        xs[k] = x[k]/scale;  ys[k] = y[k]/scale;
      }
      for (j=0; j < pyr[lev].h; j++)
        for (i=0; i < pyr[lev].w; i++) {
          for (k=0, k1=(k+1)%4, inside=TRUE; inside && k < 4; k++, k1 = (k+1)%4)
            if (!line_check(xs[k],ys[k],xs[k1],ys[k1],(float) i,(float) j))
              inside = FALSE;
          if (inside) pyr[lev].ptr[j][i] = 255;
        }
    }
  }
  fclose(stream);
  return(0);	/* added return --spraxlo@ai */
}



