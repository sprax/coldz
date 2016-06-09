/*****************************************************************************
 inParm.c    S.R.Lines    95.06.04
#define 	UNBUG	  0
#define 	UNTIME    0
#include        "mob.h"
 *****************************************************************************/
#include        <stdio.h>
#include        <assert.h>
#include        <un/unBug.h>
#include        <un/unMacro.h>
#include        <un/unTypes.h>
#include        <un/unFlags.h>
#include        <m/mj.h>
#include        "inExtern.h"
#include        "inParm.h"

flt     InFrax[MAXPARM];
flt     InCwts[MAXPARM];

void
clrsFromXYZ (flt clrs[], dbl tX, dbl tY, dbl tZ, int numSrc)
{
int 	j;
dbl 	dd;

  switch (numSrc) {
    case  1:
    case  2:  
    case  3:
              clrs[0]  = (flt)(1.0 - tX);
              clrs[1]  = (flt)tX;                    
              break;
#if 0
    case  3:
              if ((dd = tX + tY) > 1.0) {       /* lower left tri only */
                dd = (dd - 1.0)*0.5;
                tX -= dd, tY -= dd;
              }
              clrs[0]  = 1.0 - tX - tY;
              clrs[1]  =       tX     ;
              clrs[2]  =            tY;         
              break;
#endif

    case  6:  tX = (tX - 0.5)*2.0;
              if (tX < 0.0)  {
                tX = -tX;
                clrs[6] = -1.0;
              }
              else clrs[6] = 1.0;
              /* no break; */

    case  4:  clrs[0]  = (flt)((1.0 - tX)*(1.0 - tY));
              clrs[1]  = (flt) (      tX *(1.0 - tY));
              clrs[2]  = (flt)(tX        *       tY);                 
              clrs[3]  = (flt)((1.0 - tX)*       tY);
              break;

    case  9:  tX = (tX - 0.5)*2.0;
              if (tX < 0.0)  {
                tX = -tX;
                clrs[9] = -1.0;
              }
              else clrs[9] = 1.0;

              tY = (tY - 0.5)*2.0;
              if (tY < 0.0)  {
                tY = -tY;
                clrs[10] = -1.0;
              }
              else clrs[10] = 1.0;

              /* no break; */

              clrs[0]  = (1.0 - tX)*(1.0 - tY);
              clrs[1]  =        tX *(1.0 - tY);
              clrs[2]  = tX        *       tY;
              clrs[3]  = (1.0 - tX)*       tY;
              break;


#if 0
    case 12:  
              tX = (tX - 0.5)*2.0;
              if (tX < 0.0)  {
                tX = -tX;
                clrs[12] = -1.0;
              }
              else clrs[12] = 1.0;

              if (tY < 0.666667) {
                tY = (tY*1.5 - 0.5)*2.0;
                if (tY < 0.0)  {
                  tY = -tY;
                  clrs[13] = -1.0;
                }
                else clrs[13] = 1.0;
              }
              else {
                tY = (tY - 0.666667)*3.0;
                if (tY < 0.0) {
                  wrn("inParm.c: clrsFromXYZ: tY: %f -> 0", tY);
                  tY = 0.0;
                }
                else if (tY > 1.0) { 
                  wrn("inParm.c: clrsFromXYZ: tY: %f -> 1", tY);
                  tY = 1.0;
                }
                clrs[13] = 3.0;
              }

              clrs[0]  = (1.0 - tX)*(1.0 - tY);
              clrs[1]  =        tX *(1.0 - tY);
              clrs[2]  = tX        *       tY;
              clrs[3]  = (1.0 - tX)*       tY;
              break;

#else
    case 12:  tX = (tX - 0.5)*2.0;
              if (tX < 0.0)  {
                tX = -tX;
                clrs[12] = -1.0;
              }
              else clrs[12] = 1.0;
              clrs[0] = (1.0 - tX)*(1.0 - tY)*(1.0 - tZ);
              clrs[1] = (1.0 - tX)*       tY *(1.0 - tZ);
              clrs[2] = (1.0 - tX)*       tY *       tZ;
              clrs[3] = (1.0 - tX)*(1.0 - tY)*       tZ;

              clrs[4] =        tX *(1.0 - tY)*(1.0 - tZ);
              clrs[5] =        tX *       tY *(1.0 - tZ);
              clrs[6] =        tX *       tY *       tZ;
              clrs[7] =        tX *(1.0 - tY)*       tZ;
              break;
#endif

    case  8:  clrs[0] = (1.0 - tX)*(1.0 - tY)*(1.0 - tZ);
              clrs[1] =        tX *(1.0 - tY)*(1.0 - tZ);
              clrs[2] = (1.0 - tX)*       tY *(1.0 - tZ);
              clrs[3] =        tX *       tY *(1.0 - tZ);
              clrs[4] = (1.0 - tX)*(1.0 - tY)*       tZ;
              clrs[5] =        tX *(1.0 - tY)*       tZ;
              clrs[6] = (1.0 - tX)*       tY *       tZ;
              clrs[7] =        tX *       tY *       tZ;
              break;

    default:  die("clrsFromXYZ: NOIMP for numSrc: %d", numSrc);
  }

  for (dd = j = 0; j < numSrc; j++)   dd += clrs[j];

  if (Devopts & O_WARN) {
    printf("XYZ: %.2f %.2f %.2f (%.2f) " ,tX,tY,tZ, dd);
    for (j = 0; j < numSrc; j++)
      printf(" %.2f",clrs[j]);
    printf("\n");
  }

  dd -= 1.0; dd  = ABS(dd);
  assert(dd < 0.001);
}





void
clrsFromXY (flt clrs[], dbl tX, dbl tY, int numSrc)
{
flt tf;
  switch (numSrc) {
    case  1:
    case  2:  clrs[0]  = (1.0 - tX);
              clrs[1]  = tX;                    break;
    case  3:
              if ((tf = tX + tY) > 1.0) {       /* lower left tri only */
                tf = (tf - 1.0)*0.5;
                tX -= tf, tY -= tf;
              }
              clrs[0]  = 1.0 - tX - tY;
              clrs[1]  =       tX     ;
              clrs[2]  =            tY;         break;
    case  4:  clrs[0]  = (1.0 - tX)*(1.0 - tY);
              clrs[1]  = (1.0 - tY)*tX;
              clrs[2]  = (1.0 - tX)*tY;
              clrs[3]  = tX*tY;                 break;
    default:  die("clrsFromXY: need 0 < numSrc:%d < 5", numSrc);
  }

/*
wrn("clrsFromXY %d: %.3f %.3f --> %.3f %.3f %.3f %.3f"
        ,numSrc, tX,tY,clrs[0],clrs[1],clrs[2],clrs[3]);
*/
}


int
parmFromNtweens8(flt parm[][MAXPARM], int nx, int ny, int nz, int numSrc)
{
int     jx,jy,jz, q, ntweens = nx*ny;
dbl 	tx,ty,tz; flt *cp;

  for ( q = jz = 0;  jz < nz; jz++) {
        if (nz > 1)  tz = jz/(nz - 1.0);	else  tz = 0.5;
    for (   jy = 0;  jy < ny; jy++) {
        if (ny > 1)  ty = jy/(ny - 1.0);	else  ty = 0.5;
      for ( jx = 0;  jx < nx; jx++) {
        if (nx > 1)  tx = jx/(nx - 1.0);	else  tx = 0.5;
        q = jz*ny*nx + jy*nx + (jy % 2) ? nx-1-jx : jx;
        clrsFromXYZ(parm[q],tx,ty,tz,numSrc);
        cp = parm[q];
/*
prn("pFN8 %d: xyz: %.3f %.3f %.3f\n\t\
clrs: %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f"
,q,tx,ty,tz,cp[0],cp[1],cp[2],cp[3],cp[4],cp[5],cp[6],cp[7]);
*/
      }
    }
  }
  return ntweens;
}


extern double GloX,GloY,GloZ;


int
parmFromNtweens4(flt parm[][MAXPARM], int horz, int vert, int numSrc)
{
dbl     begX=TrsBeg, lenX=TrsEnd;
dbl     begY=TrsBeg, lenY=TrsEnd;
int     j,k,z, ntweens = horz*vert;

  for (z = j = 0; j < vert; j++) {
    for (k = 0; k < horz; k++) {
      if (vert > 1)  GloY = begY + j*lenY/(vert - 1.0);
      else           GloY = begY +   lenY/2.0;
      if (horz > 1)  GloX = begX + k*lenX/(horz - 1.0);
      else           GloX = begX +   lenX/2.0;
#if 1
      if (Devopts & O_WARN)
        z = (j % 2) ? j*horz + horz-1-k : j*horz + k;     /* smoother reversals? */
      else
        z = j*horz + k;
#else
      z = j*horz + k;
#endif
      clrsFromXY(parm[z],GloX,GloY,4);
    }
  }
  return ntweens;
}


int
parmFromNtweens2(flt parm[][MAXPARM], int ntweens, int numSrc)
{
/**
dbl     begX=TrsBeg, lenX=TrsEnd, trsX;
dbl     begY=TrsBeg, lenY=TrsEnd, trsY;
**/
int     z;
dbl  	clr, range  = TrsEnd - TrsBeg;

  TrsMed = TrsBeg + range*TrsMed;

  switch (ntweens) {
    case  1: parm[0][0] = 0.5001; parm[0][1] = 0.4999;  break;
    case  2: if (Devopts & O_BOTH) {
               parm[0][0] = 0.5001; parm[0][1] = 0.4999;
               parm[1][0] = 0.4999; parm[1][1] = 0.5001;
             }
             else {
               parm[0][0] = 0.75;   parm[0][1] = 0.25;
               parm[1][0] = 0.25;   parm[1][1] = 0.75;
             }
             break;
    default: for (z = 0; z < ntweens; z++) {
               /* trs = TrsBeg + clr*range; */
               clr = (dbl)z/(ntweens-1.0);
               parm[z][0] = 1.0 - clr;   parm[z][1] = clr;
             }
             break;
  }
  return ntweens;
}



#if 00 /* moved temporarily to winWarp.c */

int
parmReadXYZ (flt parm[][MAXPARM], char *path, int numSrc)
{
#define SLEN 256
char    q, str[SLEN];
dbl     tx,ty,tz;
int     j, line, tween;
float   *cp;
FILE    *fi;

  if (! (fi = fopen_s(path,"r"))) {
    warn("parmReadXYZ [%s]: can't open %s, returning 0",__FILE__,path);
    return 0;
  }

  for (line = tween = 0; fgets(str,SLEN,fi) != NULL; line++) {
    if ((j=strlen(str)) < 12 || (q=str[0]) == '#' || q == '/' || q == ';' || q == '*') {
      fprintf(stderr,"[%3d]: %s",j,str);
    }
    else {
      if (3 != (j=sscanf(str, " %lf %lf %lf "         , &tx, &ty, &tz))
      &&  3 != (j=sscanf(str, " %*[^:]: %lf %lf %lf "  , &tx, &ty, &tz)))
        die("parmReadXYZ: parse error at tween %d, line %d, sscanf %d:\n%s"
		,tween,line,j,str);
      else {
        fprintf(stderr,"%3d: % 7.2f  % 7.2f  % 7.2f \n"
		,tween,tx,ty,tz);

        clrsFromXYZ(parm[tween],tx,ty,tz,numSrc);
        cp = parm[tween];
        prn("readXYZ_C %d: xyz: %.3f %.3f %.3f\n\t\
        clrs: %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f"
        ,j,tx,ty,tz,cp[0],cp[1],cp[2],cp[3],cp[4],cp[5],cp[6],cp[7]);

        tween++;
      }
    }
  }
  fclose(fi);
  return tween;
}
#endif

