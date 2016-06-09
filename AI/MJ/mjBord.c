/*****************************************************************************
 mjBord.c    S.R.Lines     94.4.25
#include	<unMath.h>
 *****************************************************************************/
#include	<assert.h>
#include	<un/unBug.h>
#include	<un/unTypes.h>
#include	<mjBord.h>

void
mjBordFlt ( ppf img[]
, int dL, int dT, unt dW, unt dH
, int cL, int cT, unt cW, unt cH
, int sL, int sT, unt sW, unt sH, int rtype, int dD)
{
  while(dD--)
    mjBordFltBrd(img[dD],dL,dT,dW,dH,cL,cT,cW,cH,sL,sT,sW,sH,rtype);
}

void
mjBordFltBrd ( ppf img
, int dL, int dT, unt dW, unt dH
, int cL, int cT, unt cW, unt cH
, int sL, int sT, unt sW, unt sH, int rtype)
{
unt 	mW = cW-1;
register int j,k;	/**** wl= -border, wr = sW+border; ****/
register pfl pD,pS;
int 	bL,bT,bR,bB, icH = cH;
ppf	vM;	

 switch(rtype) {

   case  0:
   case  1:				/* odd reflection: edge skipped */ 

     wrn("rowBordFltBrd: %3d %3d %3u %3u"   ,dL,dT,dW,dH);
     wrn("             , %3d %3d %3u %3u"   ,cL,cT,cW,cH);
     wrn("             , %3d %3d %3u %3u %d",sL,sT,sW,sH,rtype);

     bL = dL-cL;
     bR = dW-dL-cW;
     bT = (bL-cL)-(sW-sL);  if (bT > 0)  bL -= bT;
     bB = bR-mW-sL;         if (bB > 0)  bR -= bB;
     for (vM = img-cT, j = 0; j < icH; j++ ) {
       for (pD = pS = *vM   - cL, k = bL ; k-- ; )  *--pD = *++pS;
       for (pD = pS = *vM++ + mW, k = bR ; k-- ; )  *++pD = *--pS;
     }
     bT = dT-cT;     bL = sH-sT+cT-1;  if (bT > bL)  bT  = bL;
     bB = dH-dT-cH;  bR = cH+sT-1;     if (bB > bR)  bB  = bR;  
     bR = (dW-dL)-(sW-sL);	       if (bR > 0 )  dW -= bR;
				/* How far R of R edge would ptr go? */
     for (vM = img-cT,   j = 1; j <= bT; j++ )
       for (pD=&vM[-j][-dL], pS=&vM[ j][-dL], k = dW; k-- ;)  *pD++ = *pS++;
     for (vM = img+cH-1, j = 1; j <= bB ; j++ )
       for (pD=&vM[ j][-dL], pS=&vM[-j][-dL], k = dW; k-- ;)  *pD++ = *pS++;
     break;

#if 0

   case  2:				/* even reflection: edge copied */ 
     for (vM = img; sH-- ; ) {
       for (pD = pS =   *vM   , m = L ; m-- ; )  *--pD = *pS++;
       for (pD = pS = r+*vM++ , m = R ; m-- ; )  *++pD = *pS--;
     }
     vM = img;
     do { pD=&vM[ -T][kB]; pS=&vM[--T][kB]; k = kW; while(k--) *pD++ = *pS++;
     } while (T);
     vM = img + mH;
     do { pD=&vM[B--][kB]; pS=&vM[ -B][kB]; k = kW; while(k--) *pD++ = *pS++;
     } while (B);
     break;

   case  3:				/* wrap around (left=right) */
     for (j=0; j < sH; j++) {
       for (m=1; m <= border; m++) {
         img[j][ -m] = img[j][r-m];
         img[j][r+m] = img[j][  m];
       }
       fv1 = (img[j][0] + img[j][r])/2; img[j][0] = img[j][r] = fv1;
     }
     for (m=1; m <= border; m++) {
       for (k=wl; k < wr; k++) {
         img[ -m][k] = img[mH-m][k];
         img[mH+m][k] = img[  m][k];
       }
       fv1 = (img[0][k] + img[mH][k])/2; img[0][k] = img[mH][k] = fv1;
     }
     break;
   case  4:				/* copy averaged edge values */
     for (j=0; j < sH; j++) {
       fv1 = (img[j][0]*2.0F + img[j][  1])/3.0F;
       fv2 = (img[j][r]*2.0F + img[j][r-1])/3.0F;
       for (m=1; m <= border; m++) {
         img[j][ -m] = fv1;
         img[j][r+m] = fv2;
       }
     }
     for (k=wl; k < wr; k++) {
       fv1 = (img[0][k]*2.0F + img[1  ][k])/3.0F;
       fv2 = (img[mH][k]*2.0F + img[mH-1][k])/3.0F;
       for (m=1; m <= border; m++) {
         img[ -m][k] = fv1;
         img[mH+m][k] = fv2;
       }
     }
     break;
   case  5: 				/* zero out border */
     for (j=0; j < sH; j++)
       for (m=1; m <= border; m++) {
         img[j][ -m] = 0.0;
         img[j][r+m] = 0.0;
       }
     for (m=1; m <= border; m++)
       for (k=wl; k < wr; k++) {
         img[ -m][k] = 0.0;
         img[mH+m][k] = 0.0;
       }
     break;
   case  6:				/* wrap around */
     for (j=0; j < sH; j++)
       for (m=1; m <= border; m++) {
         img[j][ -m] = img[j][r-m+1];
         img[j][r+m] = img[j][  m-1];
       }
     for (m=1; m <= border; m++)
       for (k=wl; k < wr; k++) {
         img[ -m][k] = img[mH-m+1][k];
         img[mH+m][k] = img[  m-1][k];
       }
     break;
   case  7:				/* copy  edge values */
     for (j=0; j < sH; j++)
       for (m=1; m <= border; m++) {
         img[j][ -m] = img[j][0];
         img[j][r+m] = img[j][r];;
       }
     for (m=1; m <= border; m++)
       for (k=wl; k < wr; k++) {
         img[ -m][k] = img[0][k];
         img[mH+m][k] = img[mH][k];
       }
     break;
   case  8:				/* weird reflection */
     for (j=0; j < sH; j++)
       for (m=1; m <= border; m++) {
         img[j][ -m] = 2*img[j][0] - img[j][  m];
         img[j][r+m] = 2*img[j][r] - img[j][r-m];
       }
     for (m=1; m <= border; m++)
       for (k=wl; k < wr; k++) {
         img[ -m][k] = 2*img[0][k] - img[  m][k];
         img[mH+m][k] = 2*img[mH][k] - img[mH-m][k];
       }
     break;
#endif
    default: die("rowBordFlt: NOIMP for case %d",rtype);
  }
}

