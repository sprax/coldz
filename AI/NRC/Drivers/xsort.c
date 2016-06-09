/* Driver for routine SORT */

#include <stdio.h>
#include "nr.h"

#define MAXSTR 80
#define NP 100

main()
{
	char txt[MAXSTR];
	int i,j;
	float a[NP+1];
	FILE *fp;

	if (fp=fopen("tarray.dat","r")) {
		fgets(txt,MAXSTR,fp);
		for(i=1;i<=NP;i++)
			fscanf(fp,"%f",&a[i]);
		fclose(fp);
		printf("\noriginal array:\n");
		for(i=0;i<=9;i++) {
			for(j=1;j<=10;j++)
				printf("%7.2f",a[10*i+j]);
			printf("\n");
		}
		sort(NP,a);
		printf("\nsorted array:\n");
		for(i=0;i<=9;i++) {
			for(j=1;j<=10;j++)
				printf("%7.2f",a[10*i+j]);
			printf("\n");
		}
	} else
		printf("Data file TARRAY.DAT not found\n");
}

