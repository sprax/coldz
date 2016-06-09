/* Driver for routine CHSTWO */

#include <stdio.h>
#include <math.h>
#include "nr.h"

#define NBINS 10
#define NPTS 2000

main()
{
	float chsq,df,prob,x;
	int i,ibin,idum=(-17),j;
	static float bins1[NBINS+1],bins2[NBINS+1];

	for(j=1;j<=NBINS;j++) {
		bins1[j]=0.0;
		bins2[j]=0.0;
	}
	for(i=1;i<=NPTS;i++) {
		x=expdev(&idum);
		ibin=(x*NBINS/3.0)+1;
		if (ibin <= NBINS) ++bins1[ibin];
		x=expdev(&idum);
		ibin=(x*NBINS/3.0)+1;
		if (ibin <= NBINS) ++bins2[ibin];
	}
	chstwo(bins1,bins2,NBINS,-1,&df,&chsq,&prob);
	printf("\n%15s %15s\n","dataset 1","dataset 2");
	for(i=1;i<=NBINS;i++)
		printf("%13.2f %15.2f\n",bins1[i],bins2[i]);
	printf("\n%18s %12.4f\n","chi-squared:",chsq);
	printf("%18s %12.4f\n","probability:",prob);
}

