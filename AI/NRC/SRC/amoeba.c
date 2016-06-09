#include <math.h>

#define ITMAX 500
#define ALPHA 1.0
#define BETA 0.5
#define GAMMA 2.0

void amoeba(p,y,ndim,ftol,funk,iter)
float **p,y[],ftol,(*funk)();
int ndim,*iter;
{
	int mpts,j,inhi,ilo,ihi,i;
	float yprr,ypr,rtol;
	float *pr,*prr,*pbar,*vector();
	void nrerror(),free_vector();

	pr=vector(1,ndim);
	prr=vector(1,ndim);
	pbar=vector(1,ndim);
	mpts=ndim+1;
	*iter=0;
    	for (;;) {
		ilo=1;
		ihi = y[1]>y[2] ? (inhi=2,1) : (inhi=1,2);
		for (i=1;i<=mpts;i++) {
			if (y[i] < y[ilo]) ilo=i;
			if (y[i] > y[ihi]) {
				inhi=ihi;
				ihi=i;
			} else if (y[i] > y[inhi])
				if (i != ihi) inhi=i;
		}
		rtol=2.0*fabs(y[ihi]-y[ilo])/(fabs(y[ihi])+fabs(y[ilo]));
		if (rtol < ftol) break;
		if ((*iter)++ == ITMAX) nrerror("Too many iterations in AMOEBA");
		for (j=1;j<=ndim;j++) pbar[j]=0.0;
		for (i=1;i<=mpts;i++)
			if (i != ihi)
				for (j=1;j<=ndim;j++) pbar[j] += p[i][j];
		for (j=1;j<=ndim;j++) {
			pbar[j] /= ndim;
			pr[j]=(1.0+ALPHA)*pbar[j]-ALPHA*p[ihi][j];
		}
		ypr=(*funk)(pr);
		if (ypr <= y[ilo]) {
			for (j=1;j<=ndim;j++)
				prr[j]=GAMMA*pr[j]+(1.0-GAMMA)*pbar[j];
			yprr=(*funk)(prr);
			if (yprr < y[ilo]) {
				for (j=1;j<=ndim;j++) p[ihi][j]=prr[j];
				y[ihi]=yprr;
			} else {
				for (j=1;j<=ndim;j++) p[ihi][j]=pr[j];
				y[ihi]=ypr;
			}
		} else if (ypr >= y[inhi]) {
			if (ypr < y[ihi]) {
				for (j=1;j<=ndim;j++) p[ihi][j]=pr[j];
				y[ihi]=ypr;
			}
			for (j=1;j<=ndim;j++)
				prr[j]=BETA*p[ihi][j]+(1.0-BETA)*pbar[j];
			yprr=(*funk)(prr);
			if (yprr < y[ihi]) {
				for (j=1;j<=ndim;j++) p[ihi][j]=prr[j];
				y[ihi]=yprr;
			} else {
				for (i=1;i<=mpts;i++) {
					if (i != ilo) {
						for (j=1;j<=ndim;j++) {
							pr[j]=0.5*(p[i][j]+p[ilo][j]);
							p[i][j]=pr[j];
						}
						y[i]=(*funk)(pr);
					}
				}
			}
		} else {
			for (j=1;j<=ndim;j++) p[ihi][j]=pr[j];
			y[ihi]=ypr;
		}
	}
	free_vector(pbar,1,ndim);
	free_vector(prr,1,ndim);
	free_vector(pr,1,ndim);
}

#undef ALPHA
#undef BETA
#undef GAMMA
#undef ITMAX

