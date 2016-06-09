/* fixed length encoding routines */

#include <stdio.h>
#include <math.h>
#include <un/unMath.h>

/* Global declarations */
FILE *Out;		/* file buffers */
long Outbytes;		/* counter for size calculation */
long Tabbytes;		/* counter for size of tables */
long Codebytes;		/* counter for size of code */

int squeeze_0(data,blocksize,numblocks,fileptr)
short *data;
int blocksize,numblocks;
FILE *fileptr;
{
	register short *p=data,*fp=p+numblocks*blocksize,s;
	short minv= *p,maxv= *p;
	unsigned char nbits;

	Out = fileptr; 
        for (; p < fp; p++) {
	 if (*p < minv)
            minv = *p;
         else if (*p > maxv)
            maxv = *p;
        }
	fflushbits();   Outbytes = 0;
	nbits = (unsigned char) ceil(log2((double) maxv-minv+1));
	fwrite(&minv,2,1,Out);
	fwrite(&nbits,1,1,Out);
	Outbytes += Tabbytes = 3;
	for (p=data, fp=p+numblocks*blocksize; p < fp; p++) {
          s = *p - minv;
	  putbits(s,nbits);
	}

        fflushbits();

	Codebytes = Outbytes - Tabbytes;
#ifdef DEBUG
	printf("Number of code bytes = %d\n",Codebytes);
#endif
	fflush(Out);
	return(Outbytes);
}

