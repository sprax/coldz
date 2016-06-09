/*
 *	blend - 
 *		Blend two images.
 *
 *				Paul Haebetli - 1987
 */
#include <stdio.h>
#include <math.h>
#include "SGI/port.h"
#include "SGI/image.h"

short rowbuf1[8192];
short rowbuf2[8192];

main(argc,argv)
int argc;
char **argv;
{
    register IMAGE *iimage1, *iimage2, *oimage;
    register int y, z;
    int xsize, ysize, zsize1, zsize2;
    int ozsize;
    float p;

    if( argc<5 ) {
	fprintf(stderr,"usage: blend inimage1 inimage2 outimage param\n");
	exit(1);
    } 
    if( (iimage1=iopen(argv[1],"r")) == NULL ) {
	fprintf(stderr,"blend: can't open input file %s\n",argv[1]);
	exit(1);
    }
    if( (iimage2=iopen(argv[2],"r")) == NULL ) {
	fprintf(stderr,"blend: can't open input file %s\n",argv[2]);
	exit(1);
    }
    xsize = MIN(iimage1->xsize,iimage2->xsize);
    ysize = MIN(iimage1->ysize,iimage2->ysize);
    zsize1 = iimage1->zsize;
    zsize2 = iimage2->zsize;
    ozsize = MAX(zsize1,zsize2);

    printf("oimage = iopen(%s,\"w\",RLE(BPP(iimage1->type==%d))==%d,\n\t\
		  iimage1->dim==%d,xsize==%d,ysize==%d,ozsize==%d)\n" 
    ,argv[3],iimage1->type,RLE(BPP(iimage1->type)),iimage1->dim,xsize,ysize,ozsize);

    oimage = iopen(argv[3],"w",RLE(BPP(iimage1->type)),
					iimage1->dim,xsize,ysize,ozsize); 
    p = atof(argv[4]);
    isetname(oimage,iimage1->name);
    for(z=0; z<ozsize; z++)
	for(y=0; y<ysize; y++) {
	    getrow(iimage1,rowbuf1,y,z%zsize1);
	    getrow(iimage2,rowbuf2,y,z%zsize2);
	    blendrow(rowbuf1,rowbuf2,xsize,p);
	    putrow(oimage,rowbuf2,y,z);
	}
    iclose(oimage);
    exit(0);
}

blendrow(buf1,buf2,n,p)
register short *buf1;
register short *buf2;
register int n;
float p;
{
    int p1, p2, div; 

    p2 = 255*p;
    p1 = 255-p2;
    if(p2>255)
	p2 = 255;
    if(p1>255)
	p1 = 255;
    div = p1+p2;
    if(div == 0)
	div = 255;
    while (n--) {
	*buf2 = ((p1 * *buf1)+(p2 * *buf2))/div;
	if(*buf2>255) 
	    *buf2 = 255;
	if(*buf2<0)
	    *buf2 = 0;
	buf1++;
	buf2++;
    }
}

