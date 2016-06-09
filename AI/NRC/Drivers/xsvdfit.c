/* Driver for routine SVDFIT */

/* #include <malloc.h> */
#include <stdio.h>
#include <math.h>
#include "nr.h"
#include "nrutil.h"

#define NPT 100
#define SPREAD 0.02
#define NPOL 5
#define MP NPT
#define NP NPOL
#define NCVM NPOL

main()
{
	int i,idum=(-911);
	float chisq,**cvm,**u,**v;
	static float x[NPT+1],y[NPT+1],sig[NPT+1];
	static float a[NPOL+1],w[NPOL+1];

	cvm=matrix(1,NCVM,1,NCVM);
	u=matrix(1,MP,1,NP);
	v=matrix(1,NP,1,NP);
	for(i=1;i<=NPT;i++) {
		x[i]=0.02*i;
		y[i]=1.0+x[i]*(2.0+x[i]*(3.0+x[i]*(4.0+x[i]*5.0)));
		y[i] *= (1.0+SPREAD*gasdev(&idum));
		sig[i]=y[i]*SPREAD;
	}
	svdfit(x,y,sig,NPT,a,NPOL,u,v,w,&chisq,fpoly);
	svdvar(v,NPOL,w,cvm);
	printf("\npolynomial fit:\n\n");
	for(i=1;i<=NPOL;i++)
		printf("%12.6f %s %10.6f\n",a[i],"  +-",sqrt(cvm[i][i]));
	printf("\nChi-squared %12.6f\n",chisq);
	svdfit(x,y,sig,NPT,a,NPOL,u,v,w,&chisq,fleg);
	svdvar(v,NPOL,w,cvm);
	printf("\nLegendre polynomial fit:\n\n");
	for(i=1;i<=NPOL;i++)
		printf("%12.6f %s %10.6f\n",a[i],"  +-",sqrt(cvm[i][i]));
	printf("\nChi-squared %12.6f\n",chisq);
	free_matrix(v,1,NP);
	free_matrix(u,1,MP);
	free_matrix(cvm,1,NCVM);
}

