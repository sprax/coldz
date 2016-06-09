/*****************************************************************************
 rowBord.c    S.R.Lines     93.7.2-22
#undef		 NDEBUG
#include	<unMath.h>
 *****************************************************************************/
#include	<assert.h>
#include	<un/unBug.h>
#include	<un/unTypes.h>
#include	<rowBord.h>

void
rowBordFlt(ppf src, int sX, int sY, unt sW, unt sH, unt bord, unt rtype)
{
rpfl	dp,dq,sp,sq;
int	sM = sW-1, sN = sH-1, dX = sX-bord; 
unt     dR = sX+sW+bord;
flt	fs,ft;
unt     j, k;
     
  src += sY;

  /*wrn("rowBordFlt: sW:%u sM:%d  sN:%d   dX:%d  dR:%d",sW,sM,sN,dX,dR);*/

  switch(rtype) {

    case  BD_RFLX_ODD:			/* odd reflection: edge skipped */ 
      for (j = 0; j < sH; j++)
        for (dp = sp = src[j]+sX, dq = sq = dp+sM, k = 0; k < bord; k++) {
          *--dp  = *++sp; 
          *++dq  = *--sq; 
        }
      for (j = 1; j <= bord; j++) {
        k  = dX;
        dp = &src[0 -j][k];
        sp = &src[   j][k];
        dq = &src[sN+j][k];
        sq = &src[sN-j][k];
        for ( ; k < dR; k++) {
          *dp++ = *sp++; 
          *dq++ = *sq++; 
        }
      }
      break;

    case  BD_RFLX_EVEN:                  /* even reflection: edge copied */
      for (j = 0; j < sH; j++)
        for (dp = sp = src[j]+sX, dq = sq = dp+sM, k = 0; k < bord; k++) {
          *--dp  = *sp++;
          *++dq  = *sq--;
        }
      for (j = 0; j < bord; j++) {
        k  = dX;
        dp = &src[-1-j][k];
        sp = &src[   j][k];
        dq = &src[sH+j][k];
        sq = &src[sN-j][k];
        for ( ; k < dR; k++) {
          *dp++ = *sp++;
          *dq++ = *sq++;
        }
      }
      break;
 
    case  BD_COPY:			/* copy  edge values */
      for (j = 0; j < sH; j++) {
        for (dp = src[j]+sX, fs=*dp, dq = dp+sM, ft=*dq, k=0; k < bord; k++) {
          *--dp = fs;
          *++dq = ft;
        }
      }
      for (j = 1; j <= bord; j++) {
        sp = src[ 0]+dX;
        sq = src[sN]+dX;
        for (dp = src[0-j]+dX, dq = src[sN+j]+dX, k = dX; k < dR; k++) {
          *dp++ = *sp++;
          *dq++ = *sq++;
        }
      }
      break;

#if 0000

   case  7:                             /* copy  edge values */
     for (j=0; j < sH; j++)
       for (m=1; m <= bord; m++) {
         dp[ -m] = dp[0];
         dp[r+m] = dp[r];;
       }
     for (m=1; m <= bord; m++)
       for (k=dX; k < dR; k++) {
         src[ -m][k] = src[0][k];
         src[t+m][k] = src[t][k];
       }
     break;

   case  3:				/* dRap around (left=right) */
     for (j=0; j < sH; j++) {
       for (m=1; m <= bord; m++) {
         dp[ -m] = dp[r-m];
         dp[r+m] = dp[  m];
       }
       fv1 = (dp[0] + dp[r])/2; dp[0] = dp[r] = fv1;
     }
     for (m=1; m <= bord; m++) {
       for (k=dX; k < dR; k++) {
         src[ -m][k] = src[t-m][k];
         src[t+m][k] = src[  m][k];
       }
       fv1 = (src[0][k] + src[t][k])/2; src[0][k] = src[t][k] = fv1;
     }
     break;
   case  4:				/* copy averaged edge values */
     for (j=0; j < sH; j++) {
       fv1 = (dp[0]*2.0F + dp[  1])/3.0F;
       fv2 = (dp[r]*2.0F + dp[r-1])/3.0F;
       for (m=1; m <= bord; m++) {
         dp[ -m] = fv1;
         dp[r+m] = fv2;
       }
     }
     for (k=dX; k < dR; k++) {
       fv1 = (src[0][k]*2.0F + src[1  ][k])/3.0F;
       fv2 = (src[t][k]*2.0F + src[t-1][k])/3.0F;
       for (m=1; m <= bord; m++) {
         src[ -m][k] = fv1;
         src[t+m][k] = fv2;
       }
     }
     break;
   case  5: 				/* zero out border */
     for (j=0; j < sH; j++)
       for (m=1; m <= bord; m++) {
         dp[ -m] = 0.0;
         dp[r+m] = 0.0;
       }
     for (m=1; m <= bord; m++)
       for (k=dX; k < dR; k++) {
         src[ -m][k] = 0.0;
         src[t+m][k] = 0.0;
       }
     break;
   case  6:				/* dRap around */
     for (j=0; j < sH; j++)
       for (m=1; m <= bord; m++) {
         dp[ -m] = dp[r-m+1];
         dp[r+m] = dp[  m-1];
       }
     for (m=1; m <= bord; m++)
       for (k=dX; k < dR; k++) {
         src[ -m][k] = src[t-m+1][k];
         src[t+m][k] = src[  m-1][k];
       }
     break;
   case  8:				/* weird reflection */
     for (j=0; j < sH; j++)
       for (m=1; m <= bord; m++) {
         dp[ -m] = 2*dp[0] - dp[  m];
         dp[r+m] = 2*dp[r] - dp[r-m];
       }
     for (m=1; m <= bord; m++)
       for (k=dX; k < dR; k++) {
         src[ -m][k] = 2*src[0][k] - src[  m][k];
         src[t+m][k] = 2*src[t][k] - src[t-m][k];
       }
     break;
#endif /* 0000 */
    default: die("rowBordFlt: NOIMP for case %d",rtype);
  }
}
/*****************************************************************************/


