/* Driver for routine QROMO */

#include <stdio.h>
#include <math.h>
#include "nr.h"

#define PIO2 1.5707963

static float func(x)
float x;
{
	return(sqrt(x)/sin(x));
}

main()
{
	float midsql();
	float a=0.0,b=PIO2,result;

	printf("Improper integral:\n\n");
	result=qromo(func,a,b,midsql);
	printf("Function: sqrt(x)/sin(x)      Interval: (0,pi/2)\n");
	printf("Using: MIDSQL                 Result: %8.4f\n",result);
}

