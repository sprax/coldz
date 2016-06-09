/*****************************************************************************
 rowStat.h    S.R.Lines (spraxlo@{ai|media}.mit.edu)     93.6.21-7.29
 *****************************************************************************/
#ifndef im_rowStat_h__
#define im_rowStat_h__

#define MAX_STAT 12

typedef	struct vxy {
	double	v;
	int 	x;
	int 	y;
}	vxy;

double rowMeanFlt (ppf src, int sX, int sY, unt sW, unt sH);

void rowStdN (double *std, flt **src, int sX, int sY, unt sW, unt sH, int maxOrder);
void rowVarN (double *std, flt **src, int sX, int sY, unt sW, unt sH, int maxOrder);

void rowStd2 (double *std, flt **src, int sX, int sY, unt sW, unt sH);
void rowStd4 (double *std, flt **src, int sX, int sY, unt sW, unt sH);

void rowVar2 (double *var, flt **src, int sX, int sY, unt sW, unt sH);
void rowVar4 (double *var, flt **src, int sX, int sY, unt sW, unt sH);

void varStd2 (double *std, double *mom);
void varStd4 (double *std, double *mom);


int MeanVarianceUnc (unc **src, unt sW, unt sH, dbl *mean, dbl *vari);
int MeanVarianceFlt (flt **src, unt sW, unt sH, dbl *mean, dbl *vari);

void
shoStats4FL( dbl *stdM, vxy *mM, int *xy, flt **row
           , int sX, int sY, unt sW, unt sH, char *tag);

void
shoStats4(dbl *stdM, flt **row, int sX, int sY, unt sW, unt sH, char *tag);
void
shoStats2(dbl *stdM, flt **row, int sX, int sY, unt sW, unt sH, char *tag);


void
shoMinMaxFL( vxy *mM, int *xy, flt **row
           , int sX, int sY, unt sW, unt sH, char *tag);

void
vxyMinMax ( vxy minmax[2], flt **src
          , int sX, int sY, unt sW, unt sH);

void
vxyMinMaxFL ( vxy minmax[2], int xH[4], flt **src
            , int sX, int sY, unt sW, unt sH);

void fxyStat ( dbl *stdX, dbl *stdY, ppf fvX, ppf fvY
             , int sX, int sY, unt sW, unt sH, char *tag, int flags);

#endif	/* im_rowStat_h__ */


