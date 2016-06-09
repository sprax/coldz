/* Driver for routine AMOEBA */

/* #include <malloc.h> */
#include <stdio.h>
#include <math.h>
#include "nr.h"
#include "nrutil.h"

#define MP 4
#define NP 3
#define FTOL 1.0e-6
#define SQR(a) ((a)*(a))

float func(x)
float x[];
{
	return(0.6-bessj0(SQR(x[1]-0.5)+SQR(x[2]-0.6)+SQR(x[3]-0.7)));
}

main()
{
	int i,iter,j,ndim=NP;
	static float x[NP+1],y[MP+1];
	float **p;

	p=matrix(1,MP,1,NP);
	for(i=1;i<=MP;i++) {
		for(j=1;j<=NP;j++)
			x[j]=p[i][j]=(i == (j+1) ? 1.0 : 0.0);
		y[i]=func(x);
	}
	amoeba(p,y,ndim,FTOL,func,&iter);
	printf("\nIterations: %3d\n",iter);
	printf("Vertices of final 3-d simplex and\n");
	printf("function values at the vertices:\n\n");
	printf("%3s %10s %12s %12s %14s\n\n",
		"i","x[i]","y[i]","z[i]","function");
	for(i=1;i<=MP;i++) {
		printf("%3d ",i);
		for(j=1;j<=NP;j++) printf("%12.6f ",p[i][j]);
		printf("%12.6f\n",y[i]);
	}
	printf("\nTrue minimum is at (0.5,0.6,0.7)\n");
	free_matrix(p,1,MP,1,NP);
}

