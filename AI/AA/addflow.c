/*****************************************************************************
 addflow.c    S.R.Lines    94.4.20 - 96.6.27
 *****************************************************************************/
#define		UNTIME  0
#define		UNBUG	0
#include	<assert.h>
#include	<stddef.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<assert.h>
#include	<errno.h>
#include	<math.h>
#include	<un/unFlags.h>
#include	<un/unBug.h>
#include	<un/unMacro.h>
#include	<un/unSys.h>
#include	<un/unMem.h>
#include	<un/unMath.h>
#include	<un/unTypes.h>
#include	<un/unTime.h>
#include	<un/unUsage.h>
#include	<un/unParse.h>
#include	<libim.h>
#include	<m/rowClip.h>
#include	<m/rowSGI.h>
#include	"mj.h"
#include        <winLoop.h>
#include	"new.h"
#include	"rowFimg.h"

unsigned long Options = 0;

void
rowAddFlowBtoA ( ppf fAX, ppf fAY, ppf fBX, ppf fBY, int dX, int dY, unt dW, unt dH, dbl trs)
{
pfl     pX, pY, sp;
int	dx1 = dX + dW - 1;
int	dy1 = dY + dH - 1, dP = dW+1;
int 	jA,kA, jT,kT;
flt 	xM,yM, xT,yT;

  for (jA = dY, fAX += jA, fAY += jA, fBX += jA, fBY += jA; jA <= dy1; jA++) {
    for (kA = dX, pX = *fAX++ + kA, pY = *fAY++ + kA; kA <= dx1; pX++, pY++, kA++) {
      yT = (flt)(jA - trs * *pY);
      jT = IFLOOR(yT);
      if (dY <= jT && jT < dy1) {
        xT = (flt)(kA - trs * *pX);
        kT = IFLOOR(xT);
        if (dX <= kT && kT < dx1) {

          xT  -=  kT;
          xM   =  1.0F - xT;
          yT  -=  jT;
          yM   =  1.0F - yT;

          sp   =  fBX[jT] + kT;
          *pX +=  xM*yM * *sp
               +  xT*yM *  sp[1]
               +  xM*yT *  sp[dW]
               +  xT*yT *  sp[dP];

          sp   =  fBY[jT] + kT;
          *pY +=  xM*yM * *sp
               +  xT*yM *  sp[1]
               +  xM*yT *  sp[dW]
               +  xT*yT *  sp[dP];
        }
      }
/*
      else { add some running average to flow field?  @@ what to do? }
 */
    }
  }
}


void
rowAddBtoA ( ppf sA, ppf sB, int dX, int dY, unt dW, unt dH)
{
register flt *ap, *bp;
unt k;
  for (sA += dY, sB += dY; dH; dH--) {
    for (ap = *sA++ + dX, bp = *sB++ + dX, k = dW; k; k--) {
      *ap++ += *bp++;
    }
  }
}


void
rowAplusBtoC ( ppf sum, ppf sA, ppf sB
             , int dX, int dY, unt dW, unt dH
             , int sX, int sY, unt sW, unt sH)
{
  unt k;
  register flt *dp, *ap, *bp;
  for (sum += dY, sA += sY, sB += sY; dH; dH--) {
    for (dp = *sum++ + dX, ap = *sA++ + sX, bp = *sB++ + sX, k = dW; k; k--) {
      *dp++ = *ap++ + *bp++;
    }
  }
}

#if 0	/* rowPclr.c */
void
rowPclrFF ( ppf dst, ppf src
          , int dX, int dY, unt dW, unt dH
          , int sX, int sY, unt sW, unt sH
          , dbl factor, dbl mean, dbl rms, dbl lo, dbl hi)
{
  unt k;
  flt ii = hi, oo = lo, fac = 1.0/(factor*rms);
  register flt *dp;
  register flt *sp;
  register flt  sv;
  for (dst += dY, src += sY; dH; dH--) {
    for (dp = *dst++ + dX, sp = *src++ + sX, k = dW; k; k--) {
      sv = 127.5F * (1.0F + (*sp++ - mean)*fac);
      if      (sv > ii)  *dp++ = ii;
      else if (sv < oo)  *dp++ = oo;
      else               *dp++ = sv;
    }
  }
}
#endif

#if 0
void rowFreeVoid(void **rows, int iX, int iY)
{
  if (rows) {
    if   (rows[iY] + iX)  free(rows[iY] + iX);
    if   (rows+iY)        free(rows+iY);
    else                  free(rows);
  }
  rows = NULL;
}
#endif


void writeFlows ( ppf vX, ppf vY, int fX, int fY, unt fW, unt fH
             , const char *path, const char ext, unt flags)
{
  if (Options & O_TIME) {
    prn("writing %s.Fimg[XY]", path);
    rowWriteFimg(vX, fX, fY,fW,fH,       path, (ext=='F' ? "sFX":"sBX"), flags|6);
    rowWriteFimg(vY, fX, fY,fW,fH,       path, (ext=='F' ? "sFY":"sBY"), flags|2);
  } else {
    prn("writing %s.F[XY]", path);
    rowWriteFlt( vX, fX,fY,fW,fH,0,NULL,path, (ext=='F' ? "rFX":"rBX"), flags);
    rowWriteFlt( vY, fX,fY,fW,fH,0,NULL,path, (ext=='F' ? "rFY":"rBY"), flags);
  }
}


static int Image_border = 0;

void readFlows ( ppf *vX, ppf *vY, int fX, int fY, unt fW, unt fH
           , unt hdrBytes, const char *path, const char ext, unt flags)
{
  if (Options & O_TIME) {
    unt sw = fW - 2*Image_border, sh = fH - 2*Image_border;
    prn("reading FIMAGE %s.sF[XY]", path);
    rowReadFimg(vX,fX,fY,fW,fH,sw,sh,0,     path, (ext=='F' ? "sFX":"sBX"), flags|6);
    rowReadFimg(vY,fX,fY,fW,fH,sw,sh,0,     path, (ext=='F' ? "sFY":"sBY"), flags|2);
  } else {
    prn("reading RAW BINARY %s.F[XY]", path);
    rowReadFlt( vX,fX,fY,fW,fH,      0,NULL,path, (ext=='F' ? "rFX":"rBX"), flags);
    rowReadFlt( vY,fX,fY,fW,fH,      0,NULL,path, (ext=='F' ? "rFY":"rBY"), flags);
  }
}

void main (int argc, char **argv, char **envp)
{
char	sts[256], *cp, *pathA="fA", *pathB="fB", *pathC=NULL, ext = 'F';
void	*header = NULL;
int	iX =   0, iY =   0, j, shw = 0, bkwd = 0;
unt	iW = 272, iH = 272, hB = 512;
dbl	stdAX[10], stdAY[10], stdBX[10], stdBY[10];
////dbl stdCX[10], stdCY[10];
ppf	vAX,vAY, vBX,vBY, vCX,vCY;

  j=0;
  ++j; if (argc > j)  pathA = argv[j];
  ++j; if (argc > j)  pathB = argv[j]; else { 
    prn("Usage: %s fA-B fB-C [fA-C bkwd hdrBytes w h x y show]",argv[0]);
    prn("Defaults:  bkwd:%d hdrBytes:%d w:%d h:%d x:%d y:%d show:%d"
       , bkwd, hB, iW, iH, iX, iY, shw );
    exit(1);
  }
  ++j; if (argc > j)  pathC = argv[j];
  ++j; if (argc > j) {bkwd = atoi(argv[j]); prn("bkwd == %d", bkwd); }
  ++j; if (argc > j)  hB = atoi(argv[j]);
  ++j; if (argc > j)  iW = atoi(argv[j]);
  ++j; if (argc > j)  iH = atoi(argv[j]);
  ++j; if (argc > j)  iX = atoi(argv[j]);
  ++j; if (argc > j)  iY = atoi(argv[j]);
  ++j; if (argc > j)  shw++; 

  if (! pathC) {
    sprintf_s(sts, 256, "%s-%s", pathA, pathB);
    pathC = sts;
  }
  if ( hB > 0) { 
    Options = O_TIME; 			/* @@ total hack - kludge! */
    callocAss(header,unc,hB); 
  }
  if (bkwd) {
    ext = 'B';
    cp = pathA; pathA = pathB; pathB = cp;
    prn("bkwd: %d,  pathA:%s  pathB:%s", bkwd, pathA, pathB);
  }

  readFlows(&vAX,&vAY,iX,iY,iW,iH,hB,pathA,ext,0);
  readFlows(&vBX,&vBY,iX,iY,iW,iH,hB,pathB,ext,0);
  vCX = rowMalFlt(iX,iY,iW,iH);
  vCY = rowMalFlt(iX,iY,iW,iH);

  if (shw) {
    shoStats4(stdAX,vAX,iX,iY,iW,iH,"vAX stats");
    rowPclrFF( vCX,vAX,iX,iY,iW,iH,iX,iY,iW,iH, 3.0, stdAX[1], stdAX[2], 0.0, 255.0);
    showF(vCX,iX,iY,iW,iH,1,NULL,"X-flow A",1.0, 0);
  }
  if (shw) {
    shoStats4(stdAY,vAY,iX,iY,iW,iH,"vAY stats");
    rowPclrFF( vCY,vAY,iX,iY,iW,iH,iX,iY,iW,iH, 3.0, stdAY[1], stdAY[2], 0.0, 255.0);
    showF(vCY,iX,iY,iW,iH,1,NULL,"Y-flow A",1.0, 0);
  }
  if (shw) {
    shoStats4(stdBX,vBX,iX,iY,iW,iH,"vBX stats");
    rowPclrFF( vCX,vBX,iX,iY,iW,iH,iX,iY,iW,iH, 3.0, stdBX[1], stdBX[2], 0.0, 255.0);
    showF(vCX,iX,iY,iW,iH,1,NULL,"X-flow B",1.0, 0);
  }
  if (shw) {
    shoStats4(stdBY,vBY,iX,iY,iW,iH,"vBY stats");
    rowPclrFF( vCY,vBY,iX,iY,iW,iH,iX,iY,iW,iH, 3.0, stdBY[1], stdBY[2], 0.0, 255.0);
    showF(vCY,iX,iY,iW,iH,1,NULL,"Y-flow B",1.0, 0);
  }

#if 1

  rowAddFlowBtoA(vAX,vAY,vBX,vBY,iX,iY,iW,iH,1.0);

  writeFlows(vAX,vAY,iX,iY,iW,iH,pathC,ext,0);

  if (shw) {
    shoStats4(stdAX,vAX,iX,iY,iW,iH,"AX = AX+BX stats");
    rowPclrFF(vCX,vAX,iX,iY,iW,iH,iX,iY,iW,iH, 3.0, stdAX[1], stdAX[2], 0.0, 255.0);
    showF(vCX,iX,iY,iW,iH,1,NULL,"X-flow A + B",1.0, 0);
  }
  if (shw) {
    shoStats4(stdAY,vAY,iX,iY,iW,iH,"AY = AY+BY stats");
    rowPclrFF(vCY,vAY,iX,iY,iW,iH,iX,iY,iW,iH, 3.0, stdAY[1], stdAY[2], 0.0, 255.0);
    showF(vCY,iX,iY,iW,iH,1,NULL,"Y-flow A + B",1.0, 0);
  }

//WINLOOP:
  winLoop(0);    /* Should exit if no windows are up. */
#endif
}


