/* fixed length decoding routines */

#include <stdio.h>
#include <math.h>

/* Global declarations */
FILE *In;		/* file buffer */

short getbits();

unsqueeze_0(data,blocksize,numblocks,fileptr)
short *data;
int blocksize,numblocks;
FILE *fileptr;
{
	register short *p=data,*fp=p+numblocks*blocksize,s;
	short minv;
	unsigned char nbits;

	In = fileptr;	
	fflushbits();
	fread(&minv,2,1,In);
	fread(&nbits,1,1,In);
	for (p=data, fp=p+numblocks*blocksize; p < fp; p++) {
	  s = getbits(nbits);
          *p = s + minv;
	}
	fflushbits();
}


