/*****************************************************************************
 rowStat.c    S.R.Lines (spraxlo@{ai|media}.mit.edu)     93.6.21-7.29
 *****************************************************************************/
#include <stdio.h>
#include <math.h>
#include <un/unTypes.h>
#include <un/unBug.h>
#include <un/unMath.h>
#include <rowStat.h>


void fxyStat ( dbl *stdX, dbl *stdY, ppf fvX, ppf fvY
             , int sX, int sY, unt sW, unt sH, char *tag, int flags)
{
vxy     mimX[10], mimY[10];
int     crdX[10], crdY[10];

  if (flags   ) {
    shoStats4FL(stdX,mimX,crdX,fvX,sX,sY,sW,sH,tag);
    shoStats4FL(stdY,mimY,crdY,fvY,sX,sY,sW,sH,tag);
  }
  else {
    shoStats4(stdX, fvX, sX, sY, sW, sH, tag);
    shoStats4(stdY, fvY, sX, sY, sW, sH, tag);
  }
}



void
shoStats4FL ( dbl *stdM, vxy *mM, int *xy, flt **row
            , int sX, int sY, unt sW, unt sH, char *tag)
{
  rowStd4(stdM, row, sX, sY, sW, sH);
  fprintf(stderr,"StdStats (%s): % .3f % .3f % .3f % .3f % .3f % .3f\n"
         ,tag, stdM[1], stdM[2], stdM[3], stdM[4], stdM[5], stdM[6]);
  shoMinMaxFL(mM, xy, row, sX, sY, sW, sH, tag);
}



void
shoMinMaxFL(vxy *mM, int *xy, flt **row, int sX, int sY, unt sW, unt sH, char *tag)
{
  vxyMinMaxFL( mM, xy, row, sX, sY, sW, sH);
  fprintf(stderr,"MinMax(F):  % .4f at (%4d, %4d)  &  % .4f at (%4d, %4d)\n"
         ,mM[0].v, mM[0].x,mM[0].y, mM[1].v,mM[1].x,mM[1].y);
  fprintf(stderr,"MinMax(L):  % .4f at (%4d, %4d)  &  % .4f at (%4d, %4d)\n"
         ,mM[0].v, xy[0],  xy[1],   mM[1].v,  xy[2],  xy[3]);
}


void
shoStats2(dbl *stdM, flt **row, int sX, int sY, unt sW, unt sH, char *tag)
{
  rowStd2(stdM,row,sX,sY,sW,sH);
  fprintf( stderr,"%s (shoStats4): % .3f % .3f  % .3f % .3f\n"
         , tag, stdM[1], stdM[2], stdM[3], stdM[4]);
}

void
shoStats4(dbl *stdM, flt **row, int sX, int sY, unt sW, unt sH, char *tag)
{
  rowStd4(stdM,row,sX,sY,sW,sH);
  fprintf( stderr,"%s (shoStats4): % .3f % .3f % .3f % .3f % .3f % .3f\n"
         , tag, stdM[1], stdM[2], stdM[3], stdM[4], stdM[5], stdM[6]);
}



void
rowStdN (double *std, flt **src, int sX, int sY, unt sW, unt sH, int maxOrder)
{
  switch (maxOrder) {
    case 4:	rowStd4 (std, src, sX, sY, sW, sH);
    default: die("rowStdN: NOIMP for maxOrder %d\n",maxOrder);
  }
}

void
rowVarN (double *var, flt **src, int sX, int sY, unt sW, unt sH, int maxOrder)
{
  switch (maxOrder) {
    case 4:	rowVar4(var, src, sX, sY, sW, sH);
    default: die("rowVarN: NOIMP for maxOrder %d\n",maxOrder);
  }
}


void
rowStd2 (double *std, flt **src, int sX, int sY, unt sW, unt sH)
{   
    double   mom[6];
    rowVar2( mom, src, sX, sY, sW, sH);
    varStd2( std, mom);
}

void
rowStd4 (double *std, flt **src, int sX, int sY, unt sW, unt sH)
{   
    double   mom[8];
    rowVar4( mom, src, sX, sY, sW, sH);
    varStd4( std, mom);
}


double rowMeanFlt (ppf src, int sX, int sY, unt sW, unt sH)
{   
int 	j,k;
float	*sp;
double	rov, mov=0.0;

  for (src += sY, j = sH; j-- ; mov += (rov/sW))
    for (sp = sX+*src++, rov = 0.0, k = sW; k--; )
      rov  += *sp++;
  return(mov/(dbl)sH);
}

void rowVar2 (double var[5], flt **src, int sX, int sY, unt sW, unt sH)
{   
    int j,k;
    float *sp;
    double v1, ro1, ro2;
    double Min = src[sY][sX], Max = Min, mo1=0.0, mo2=0.0;

    for (src += sY, j = sH; j; j--) {
        sp = *src++ + sX;
        ro1 = ro2 = 0.0;
        for (k = sW; k; k--)  {
            ro1  += (v1 = *sp++);
            ro2  += v1*v1;
            if      (v1 > Max)  Max = v1;
            else if (v1 < Min)  Min = v1;
        }
        mo1 += (ro1 / sW);
        mo2 += (ro2 / sW);
    }
    *var++ = 5.0;
    *var++ = (mo1 /= sH);
    *var++ = (mo2 /= sH);
    *var++ =  Min;
    *var++ =  Max;
}

void rowVar4 (double *var, flt **src, int sX, int sY, unt sW, unt sH)
{   
    int j,k;
    float *sp;
    double v1, v2, v3, v4, ro1, ro2, ro3, ro4;
    double Min = **src, Max = Min, mo1=0.0, mo2=0.0, mo3=0.0, mo4=0.0;

    for (src += sY, j = sH; j; j--) {
        sp = *src++ + sX;
        ro1 = ro2 = ro3 = ro4 = 0.0;
        for (k = sW; k; k--)  {
            ro1  += (v1 = *sp++);
            ro2  += (v2 = v1*v1);
            ro3  += (v3 = v2*v1);
            ro4  += (v4 = v3*v1);
            if      (v1 > Max)  Max = v1;
            else if (v1 < Min)  Min = v1;
        }
        mo1 += (ro1 / sW);
        mo2 += (ro2 / sW);
        mo3 += (ro3 / sW);
        mo4 += (ro4 / sW);
    }
    var[0] = 7.0;
    var[1] = (mo1 /= sH);
    var[2] = (mo2 /= sH);
    var[3] = (mo3 /= sH);
    var[4] = (mo4 /= sH);
    var[5] =  Min;
    var[6] =  Max;
}


void varStd2 (double *std, double *mom)		/* OK if std == mom */
{   /* Get mean, std. dev., etc., from moments about zero */

    double mean = mom[1];
    *std++ = 5;
    *std++ = mean;
    *std++ = sqrt(mom[2] - mean*mean);
    *std++ = mom[3] - mean;
    *std++ = mom[4] - mean;
}


void varStd4 (double *std, double *mom)		/* OK if std == mom */
{   /* Get mean, std. dev., etc., from moments about zero */

    double mean  = mom[1];
    double mean2 = mean*mean;
    std[0] = 7;
    std[1] = mean;
    std[2] = sqrt(mom[2] - mean2);
    std[3] = cbrt(mom[3] - 3.0*mom[2]*mean + 2.0*mean2*mean);
    std[4] = sqrt(sqrt(mom[4] - 4.0*mom[3]*mean + 6.0*mom[2]*mean2 - 3.0*mean2*mean2));
    /*****
    std[5] = mom[5] - mean;
    std[6] = mom[6] - mean;
    *****/
}


int MeanVarianceFlt (flt **src, unt sW, unt sH, dbl *mean, dbl *vari)
{   /* mean & variance computed row by row for greater accuracy */
    double   val, rave, rvar, ave = 0.0, var = 0.0;
    register flt *sp;
    int j,k;

    for (j = sH; j; j--) {
        for (rave = rvar = 0.0, sp = *src++, k = sW; k; k--)  {
            rave += (val = *sp++);
            rvar += (val*val);
        }
        ave += (rave / sW);
        var += (rvar / sW);
        /* fprintf(stderr,"MVF %4d: ave,var = %8.3f %8.3f\n",j,ave,var); */
    }
    *mean = (ave /= sH);
    *vari = (var /  sH - ave*ave);
    return(0);
}



int MeanVarianceUnc (unc **src, unt sW, unt sH, dbl *mean, dbl *vari)
{   /* mean & variance computed row by row for greater accuracy */
    double   val, rave, rvar, ave = 0.0, var = 0.0;
    register unc *sp;
    register int j,k;

    for (j = sH; j; j--) {
        for (rave = rvar = 0.0, sp = *src++, k = sW; k; k--)  {
            rave += (val = *sp++);
            rvar += val*val;
        }
        ave += (rave / sW);
        var += (rvar / sW);
    }
    *mean = (ave /= sH);
    *vari = (var /  sH - ave*ave);
    return(0);
}




void vxyMinMax (vxy minmax[2], flt **src, int sX, int sY, unt sW, unt sH)
{
    unt j,k;
    float *sp;
    double val;
    double Max, Min;
    vxy *min = &minmax[0], *max = &minmax[1];
    
    max->v = min->v = Min = Max = **src;
    max->x = min->x = sX;
    max->y = min->y = sY;
    sW += sX;
    sH += sY;
    for (j = sY, src += j; j < sH; j++) {
        for (k = sX, sp = *src++ + k; k < sW; k++)  {
            val = *sp++;
            if (val > Max)  {
                max->v = Max = val;
                max->x = k;
                max->y = j;
            } else
            if (val < Min)  {
                min->v = Max = val;
                min->x = k;
                min->y = j;
            }   
        }
    }
}





void vxyMinMaxFL (vxy minmax[2], int xyL[4], flt **src, int sX, int sY, unt sW, unt sH) 
{
    unt j,k;
    int MxL, MyL, mxL, myL;
    float *sp;
    double val, Max, Min;
    vxy *min = &minmax[0], *max = &minmax[1];
    
    min->v = max->v = Min = Max = **src;
    min->x = max->x = mxL = MxL = sX;
    min->y = max->y = myL = MyL = sY;
    sW += sX;
    sH += sY;
    for (src += sY, j = sY; j < sH; j++) {
        for (k = sX, sp = *src++ + k; k < sW; k++)  {
            val = *sp++;
            if (val >= Max)  {
                MxL = k;
                MyL = j;
                if (val > Max)  {
                    max->v = Max = val;
                    max->x = k;
                    max->y = j;
                }
            }
            else if (val <= Min)  {
                mxL = k;
                myL = j;
                if (val < Min)  {
                    min->v = Min = val;
                    min->x = k;
                    min->y = j;
                }
            }   
        }
    }
    xyL[0] = mxL;
    xyL[1] = myL;
    xyL[2] = MxL;
    xyL[3] = MyL;
}



