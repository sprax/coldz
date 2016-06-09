
#include <stdio.h>
#include <unWorks.h>

main()
{
    char *pchar;
    unc  *punc;
    int  *pint;
    float *pflt;
    double *pdbl;

    char *cp = (char *)435;
    int fcnt = -9;
    if (fcnt) fcnt -= 1;
    pchar = (char *)malloc(32); printf("pchar:\t%ld,\t&%ld\n",pchar,&pchar);
    punc = (unc *)malloc(32); printf("punc:\t%ld,\t&%ld\n",punc,&punc);
    pint = (int *)malloc(32); printf("pint:\t%ld,\t&%ld\n",pint,&pint);
    pflt = (flt *)malloc(32); printf("pflt:\t%ld,\t&%ld\n",pflt,&pflt);
    pdbl = (dbl *)malloc(32); printf("pdbl:\t%ld,\t&%ld\n",pdbl,&pdbl);

}

