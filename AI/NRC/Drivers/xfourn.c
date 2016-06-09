/* Driver for routine FOURN */

#include <stdio.h>
#include "nr.h"

#define NDIM 3
#define NDAT2 1024

main()
{
	int i,isign,j,k,l,ll,ndum;
	static int nn[NDIM+1];
	static float data[NDAT2+1];

	ndum=2;
	for(i=1;i<=NDIM;i++) {
		ndum *= 2;
		nn[i]=ndum;
	}
	for(k=1;k<=nn[1];k++) {
		for(j=1;j<=nn[2];j++) {
			for(i=1;i<=nn[3];i++) {
				l=k+(j-1)*nn[1]+(i-1)*nn[2]*nn[1];
				ll=2*l-1;
				data[ll]=ll;
				data[ll+1]=ll+1;
			}
		}
	}
	isign=1;
	fourn(data,nn,NDIM,isign);
	isign = -1;
	fourn(data,nn,NDIM,isign);
	printf("Double 3-dimensional transform\n\n");
	printf("%22s %24s %20s\n",
		"Double transf.","Original data","Ratio");
	printf("%10s %13s %12s %13s %11s %13s\n\n",
		"real","imag.","real","imag.","real","imag.");
	for(i=1;i<=4;i++) {
		k=2*(j=2*i);
		l=k+(j-1)*nn[1]+(i-1)*nn[2]*nn[1];
		ll=2*l-1;
		printf("%12.2f %12.2f %10d %12d %14.2f %12.2f\n",
			data[ll],data[ll+1],ll,ll+1,data[ll]/ll,
			data[ll+1]/(ll+1));
	}
	printf("\nThe product of transform lengths is: %4d\n",nn[1]*nn[2]*nn[3]);
}

