/* Driver for routine LUBKSB */

#include <stdio.h>
#include "nr.h"
#include "nrutil.h"

#define NP 20

main()
{
	int j,k,l,m,n;
	static int indx[NP+1];
	float p,**a,**b,**c;
	static float x[NP+1];
	FILE *fp;

	a=matrix(1,NP,1,NP);
	b=matrix(1,NP,1,NP);
	c=matrix(1,NP,1,NP);
	if (fp=fopen("matrx1.dat","r")) {
		while (!feof(fp)) {
			fscanf(fp,"%*[^\n] %*[^\n] %d %d %*[^\n]",&n,&m);
			for(k=1;k<=n;k++)
				for(l=1;l<=n;l++) fscanf(fp,"%f ",&a[k][l]);
			fscanf(fp,"%*[^\n]");
			for(l=1;l<=m;l++)
				for(k=1;k<=n;k++) fscanf(fp,"%f ",&b[k][l]);
			/* Save matrix a for later testing */
			for(l=1;l<=n;l++)
				for(k=1;k<=n;k++) c[k][l]=a[k][l];
			/* Do lu decomposition */
			ludcmp(c,n,indx,&p);
			/* Solve equations for each right-hand vector */
			for(k=1;k<=m;k++) {
				for(l=1;l<=n;l++) x[l]=b[l][k];
				lubksb(c,n,indx,x);
				/* Test results with original matrix */
				printf("right-hand side vector:\n");
				for(l=1;l<=n;l++)
					printf("%12.6f",b[l][k]);
				printf("\n%s%s\n","result of matrix applied",
					" to sol'n vector");
				for(l=1;l<=n;l++) {
					b[l][k]=0.0;
					for(j=1;j<=n;j++)
						b[l][k] += (a[l][j]*x[j]);
				}
				for(l=1;l<=n;l++)
					printf("%12.6f",b[l][k]);
				printf("\n*********************************\n");
			}
			printf("press RETURN for next problem:\n");
			getchar();
		}
		fclose(fp);
		free_matrix(c,1,NP,1,NP);
		free_matrix(b,1,NP,1,NP);
		free_matrix(a,1,NP,1,NP);
	} else
		printf("Data file MATRX1.DAT not found\n");
}

