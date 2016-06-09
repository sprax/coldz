/******************************************************************************
 any2sgi.c    S.R.Lines    93.5.22
 NB: Why does using RLE vs. VERBATIM in iopen create split images?!!!!!!!!
 *****************************************************************************/
#include	<stdio.h>
#include	"beymer/image.h"
#include	"SGI/image.h"
typedef	char	ichar;	/* un/signed doesn't affect results on SGI */

static	int Sputrow(register IMAGE *image,ichar *buffer,unsigned y,unsigned z);
extern	int bitstorow(char *row,ichar *cbuf,int xsize);
extern	int iclose(register IMAGE *image);
extern	int img_seek(IMAGE *image, unsigned y, unsigned z);
extern	int img_write(IMAGE *image, char *buffer, long count);
extern	int cvtshorts(register unsigned short *buffer, register long n);
extern	int i_errhdlr();





int Swrite_sgi(image_t *img, char *path, int mode)
{
  int y, xsize = img->width, ysize = img->height;
  IMAGE *oimage;
  /* oimage = iopen(path,"w",RLE(BPP(1)),3,img->width,img->height,3);splits.*/
  oimage = iopen(path,"w",(VERBATIM(1)),3,img->width,img->height,3);
  if ( ! oimage ) {
    fprintf(stderr,"\n%s write_sgi: err iopen(%s)\n",__FILE__,path);
    return(-1);
  }
  switch(img->type) {
    case IMG_1BIT:  { register ichar **rowp = (ichar **) img->image;
                      ichar cbuf[8192];		/* kludge @@ */
		      for (y = 0; y < ysize; y++) {
                        bitstorow(rowp[y],cbuf,xsize);
                        Sputrow(oimage,cbuf,(ysize-1)-y,0);
		      }
                    } break;
    case IMG_U8BIT: { register ichar **rowp = (ichar **) img->image;
                      for (y = 0; y < ysize; y++) {
                        Sputrow(oimage,rowp[y],(ysize-1)-y,0);
			Sputrow(oimage,rowp[y],(ysize-1)-y,1);
                        Sputrow(oimage,rowp[y],(ysize-1)-y,2);
                      }
		    } break;
    case IMG_S8BIT: { register ichar **rowp = (ichar **) img->image;
		      for (y = 0; y < ysize; y++) {
                        Sputrow(oimage,rowp[y],(ysize-1)-y,0);
                        Sputrow(oimage,rowp[y],(ysize-1)-y,1);
                        Sputrow(oimage,rowp[y],(ysize-1)-y,2);
		      }
                    } break;
    case IMG_S32BIT:
    case IMG_FLOAT:
    default: fprintf(stderr,"IMG_TYPE %d not (yet) supported!\n",img->type);
	     exit(-1);
  }
  iclose(oimage);
  return(0);
}






int main (argc, argv)
int argc;
char *argv[];
{
  image_t *image;

  if (argc != 3) {
    fprintf (stderr, "usage: %s input output\n", argv[0]);
    return(-1);
  }
  if ((image = read_image (argv[1])) == NULL) {
    fprintf(stderr,"%s: error opening file %s for input.\n",argv[0],argv[1]);
    return(-1);
  }
  return( Swrite_sgi(image, argv[2], 0) );
}

static int
Sputrow(register IMAGE *image,ichar *buffer,unsigned y,unsigned z)
{
    /***************** register unsigned short 	*sptr; ***************/
    register ichar 	*sptr;
    register ichar      *cptr;
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
		cptr = (ichar *)image->tmpbuf;
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
		/* @@ cast for ph's type mismatch: */
		if (img_write(image,(char *)image->tmpbuf,cnt) != cnt) 
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
                    /* @@ my casts: */
		    cvtshorts((unsigned short *)buffer,cnt);
		if (img_write(image,buffer,cnt) != cnt) {
		    if(image->dorev)	
                        /* @@ should fix this: */
			cvtshorts(buffer,cnt);
		    return -1;
		} else {
		    if(image->dorev)	
			cvtshorts((unsigned short *)buffer,cnt);
		    return image->xsize;
		}
		/* NOTREACHED */

	    default:
		i_errhdlr("Sputrow: weird bpp\n");
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
		i_errhdlr("Sputrow: weird bpp\n");
	}
    } else 
	i_errhdlr("Sputrow: weird image type\n");

    return 0;	/* hmmm. */
}


