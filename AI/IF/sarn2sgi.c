/*****************************************************************************
 sarn2sgi.c -- Sarnoff img to SGI rgb    S.R.Lines (< Paul Haeberli)  93.5.24
 *****************************************************************************/
#include "SGI/image.h"
#include "SGI/rasterfile.h"	/* update? */
#include "sarnoff.h"

#define MAXWIDTH	8192

char	cbuf[4*MAXWIDTH];
short	rbuf[MAXWIDTH];
short	gbuf[MAXWIDTH];
short	bbuf[MAXWIDTH];
unsigned char rmap[256];
unsigned char gmap[256];
unsigned char bmap[256];
#ifndef VERBOSE
#define VERBOSE      1
#endif
static int Options = 0;


int
putrowch(image,buffer,y,z) 
register IMAGE	*image;
unsigned char	*buffer;	/* was short -- sprax */
unsigned  	y, z;
{
    /***************** register unsigned short 	*sptr; ***************/
    register unsigned char 	*sptr;
    register unsigned char      *cptr;
    register unsigned int x;
    register unsigned long min, max;
    register long cnt;

    if( !(image->flags & (_IORW|_IOWRT)) )
	return -1;
    if(image->dim<3)
	z = 0;
    if(image->dim<2)
	y = 0;
    if(ISVERBATIM(image->type)) {
	switch(BPP(image->type)) {
	    case 1: 
		min = image->min;
		max = image->max;
		cptr = (unsigned char *)image->tmpbuf;
		sptr = buffer;
		for(x=image->xsize; x--;) { 
		    *cptr = *sptr++;
		    if (*cptr > max) max = *cptr;
		    if (*cptr < min) min = *cptr;
		    cptr++;
		}
		image->min = min;
		image->max = max;
		img_seek(image,y,z);
		cnt = image->xsize;
		if (img_write(image,image->tmpbuf,cnt) != cnt) 
		    return -1;
		else
		    return cnt;
		/* NOTREACHED */

	    case 2: 
		min = image->min;
		max = image->max;
		sptr = buffer;
		for(x=image->xsize; x--;) { 
		    if (*sptr > max) max = *sptr;
		    if (*sptr < min) min = *sptr;
		    sptr++;
		}
		image->min = min;
		image->max = max;
		img_seek(image,y,z);
		cnt = image->xsize<<1;
		if(image->dorev)	
		    cvtshorts(buffer,cnt);
		if (img_write(image,buffer,cnt) != cnt) {
		    if(image->dorev)	
			cvtshorts(buffer,cnt);
		    return -1;
		} else {
		    if(image->dorev)	
			cvtshorts(buffer,cnt);
		    return image->xsize;
		}
		/* NOTREACHED */

	    default:
		i_errhdlr("putrowch: weird bpp\n");
	}
    } else if(ISRLE(image->type)) {
	switch(BPP(image->type)) {
	    case 1: 
		min = image->min;
		max = image->max;
		sptr = buffer;
		for(x=image->xsize; x--;) { 
		    if (*sptr > max) max = *sptr;
		    if (*sptr < min) min = *sptr;
		    sptr++;
		}
		image->min = min;
		image->max = max;
		cnt = img_rle_compact(buffer,2,image->tmpbuf,1,image->xsize);
		img_setrowsize(image,cnt,y,z);
		img_seek(image,y,z);
		if (img_write(image,image->tmpbuf,cnt) != cnt) 
		    return -1;
		else
		    return image->xsize;
		/* NOTREACHED */

	    case 2: 
		min = image->min;
		max = image->max;
		sptr = buffer;
		for(x=image->xsize; x--;) { 
		    if (*sptr > max) max = *sptr;
		    if (*sptr < min) min = *sptr;
		    sptr++;
		}
		image->min = min;
		image->max = max;
		cnt = img_rle_compact(buffer,2,image->tmpbuf,2,image->xsize);
		cnt <<= 1;
		img_setrowsize(image,cnt,y,z);
		img_seek(image,y,z);
		if(image->dorev)
		    cvtshorts(image->tmpbuf,cnt);
		if (img_write(image,image->tmpbuf,cnt) != cnt) {
		    if(image->dorev)
			cvtshorts(image->tmpbuf,cnt);
		    return -1;
		} else {
		    if(image->dorev)
			cvtshorts(image->tmpbuf,cnt);
		    return image->xsize;
		}
		/* NOTREACHED */

	    default:
		i_errhdlr("putrowch: weird bpp\n");
	}
    } else 
	i_errhdlr("putrowch: weird image type\n");
}


/*____________________________________MAIN_________________________*/
main(argc,argv)
int argc;
char **argv;
{
    IMAGE *image;  unsigned short itype;
    FILE *inf;
    int xsize, ysize, zsize, rowbytes;
    int j, y, depth, maplen, sarntype;
    unsigned char *fb, *fbptr;
    int fblength;
    sarnimg_header hdr;

    if (argc<3) {
	fprintf(stderr,"usage: sarn2sgi sarnimage irisimage [-verbose]\n");
	exit(1);
    }
    if (argc > 3 && strncmp(argv[3],"-v",2)==0)
	Options |= VERBOSE;
    if ( ! (inf = fopen(argv[1],"r"))) {
	fprintf(stderr,"p2sgi: can't open %s\n",argv[1]);
	exit(1);
    }
    if ((y=fread(&hdr,sizeof(sarnimg_header),1,inf)) != 1) {
	fprintf(stderr,"p2sgi: err fread sarnoff header of %s (%d)\n"
	       ,argv[1],y);
	exit(1);
    }
    /* NB: Sarnoff img format has no magic number. */
    xsize = (int) hdr.width;
    ysize = (int) hdr.height;
    depth = (int) hdr.depth * 8;	/* bytes to bits */
    sarntype = (int) hdr.type;
    if (Options & VERBOSE)  switch (sarntype) {
      case SARN_UBYTE: fprintf(stderr,"\tSarnType: SARN_UBYTE, ");  break;
      case SARN_BYTE:  fprintf(stderr,"\tSarnType: SARN_BYTE, ");   break;
      case SARN_FLOAT: fprintf(stderr,"\tSarnType: SARN_FLOAT, damn!\n");break;
      default: fprintf(stderr,"%s: SarnType %d unsupported! (cf. sarnoff.h)\n"
		      , argv[0], sarntype ); break;
    }
    rowbytes = hdr.width * hdr.depth;
    maplen = 0;	/* sarnpix has no colormap (unless in reserved hdr space) */
    if(depth != 8 && depth != 24 && depth != 1) {
	fprintf(stderr,"p2sgi: bad byte-depth is %d\n",hdr.depth);
	exit(1);
    }
    if (Options & VERBOSE) fprintf(stderr,"Listen..........depth==%d\n",depth);
    if (depth == 8)
	image = iopen(argv[2],"w",RLE(1),2,xsize,ysize);
    else if (depth == 24)
	image = iopen(argv[2],"w",VERBATIM(1),3,xsize,ysize,3);
    else {
        fprintf(stderr,"Sorry, NOIMP for depth == %d\n",depth);
        exit(1);
    }

    
    itype = image->type; 
    if (Options & VERBOSE)  fprintf(stderr
	,"image->type: %d, ISVERBATIM: %d, ISRLE: %d, BPP: %d\n"
        , itype, ISVERBATIM(itype), ISRLE(itype), BPP(itype) );
    for(y=0; y<ysize; y++) {
	switch(depth) {
	case 1:
	    fread(cbuf,rowbytes,1,inf);
	    bitstorow(cbuf,rbuf,xsize);
	    putrowch(image,rbuf,(ysize-1)-y,0);
	    break;
	case 8:
	    fread(cbuf,rowbytes,1,inf);
            for (j = 0; j < xsize; j++)  rbuf[j] = cbuf[j];
	    putrow(image,rbuf,(ysize-1)-y,0);
	    break;
	case 24: 
	    fread(cbuf,rowbytes,1,inf);
	    unswizzle24(cbuf,rbuf,gbuf,bbuf,xsize);
	    putrowch(image,rbuf,(ysize-1)-y,0);
	    putrowch(image,gbuf,(ysize-1)-y,1);
	    putrowch(image,bbuf,(ysize-1)-y,2);
	    break;
	case 32: 
	    fread(cbuf,rowbytes,1,inf);
	    unswizzle32(cbuf,rbuf,gbuf,bbuf,xsize);
	    putrowch(image,rbuf,(ysize-1)-y,0);
	    putrowch(image,gbuf,(ysize-1)-y,1);
	    putrowch(image,bbuf,(ysize-1)-y,2);
	    break;
	}/* endsw */
    }
    iclose(image);

    exit(0);
}

unswizzle24(cptr,rptr,gptr,bptr,n)
register unsigned char *cptr;
register unsigned short *rptr, *gptr, *bptr;
register int n;
{
    while(n--) {
	*rptr++ = *cptr++;
	*gptr++ = *cptr++;
	*bptr++ = *cptr++;
    }
}

unswizzle32(cptr,rptr,gptr,bptr,n)
register unsigned char *cptr;
register unsigned short *rptr, *gptr, *bptr;
register int n;
{
    while(n--) {
	cptr++;
	*bptr++ = *cptr++;
	*gptr++ = *cptr++;
	*rptr++ = *cptr++;
    }
}







cvtrow8(cptr,rptr,gptr,bptr,n)
register unsigned char *cptr;
register unsigned short *rptr, *gptr, *bptr;
register int n;
{
    while(n--) {
	*rptr++ = rmap[*cptr];
	*gptr++ = gmap[*cptr];
	*bptr++ = bmap[*cptr++];
    }
}

#if 0	/* not used */
#define	ESCAPE	(128)

getrledata(inf,rlelength,fb,fblength)
FILE *inf;
int rlelength;
unsigned char *fb;
int fblength;
{
    unsigned char *rledat, *expdat, *end;
    unsigned char c;
    int compcount, n, count, space;

    rledat = fb+(fblength-rlelength);
    expdat = fb;
    end = fb+fblength;
    compcount = rlelength;
    n = fread(rledat,1,rlelength,inf);
    if(n != rlelength) {
	fprintf(stderr,"error on read of RLE data\n");
	exit(1);
    }
    while(compcount) {
	compcount--;
        c = *rledat++;
        if(c == ESCAPE) {
	    if(compcount<=0) {
		fprintf(stderr,"getrledata: bad poop1\n");
		exit(1);
	    }
	    compcount--;
	    count = *rledat++;
	    if(count!=0) {
		if(compcount<=0) {
		    fprintf(stderr,"getrledata: bad poop2\n");
		    exit(1);
		}
		compcount--;
		c = *rledat++;
		count++;
		space = end-expdat;
		if(count>space)
		    count = space;
		while(count--) 
		    *expdat++ = c;
	    } else {
	       *expdat++ = ESCAPE;
	    }
	} else {
	    *expdat++ = c;
	}
    }
}

linebytes(xsize,depth)
int xsize, depth;
{
    return (((xsize*depth)+15) >> 3) & ~1;
}

#endif	/* not used */


