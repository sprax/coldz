/******************************************************************************
 iosgi.c    S.R.Lines    93.5.25
 As of 5.25, this file needs Paul Haeberli's SGI libs, which in turn use GL.
 The next step is probably just to grab the needed functions and insert them
 here, prefixed by sgi_.

 NB: Why does using RLE vs. VERBATIM in iopen create split images?!!!!!!!!
 *****************************************************************************/
#include	<sys/types.h>
#include	<stdio.h>
#include	"beymer/image.h"
#include	"SGI/image.h"
#include	"imagic.h"

#define	rgst	register
typedef	char	ichar;	/* un/signed doesn't affect results on SGI */

image_t *
read_sgi(char *path, int mode)
{
    fprintf(stderr,"\n%s read_sgi: NOT YET IMPLEMENTED HERE.\n"
    ,__FILE__);
    return(NULL);
}


#ifndef	SGI /*____________________ Until GL dependence removed ___________*/


int write_sgi(image_t *image, char *path, int mode)
{
    fprintf(stderr,"\n%s write_sgi: NOT YET IMPLEMENTED HERE, cf. iosgi.c\n"
    ,__FILE__);
    return(-1);
}


#else	/* defined(SGI) __________ Until GL dependence removed ___________*/


static int sgi_putrow(rgst IMAGE *image,ichar *buffer,unsigned y,unsigned z);


int
write_sgi(image_t *img, char *path, int mode) /* mode ignored for now */
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
    case IMG_1BIT:  { rgst ichar **rowp = (ichar **) img->image;
                      ichar cbuf[8192];		/* kludge @@ */
		      for (y = 0; y < ysize; y++) {
                        bitstorow(rowp[y],cbuf,xsize);
                        sgi_putrow(oimage,cbuf,(ysize-1)-y,0);
		      }
                    } break;
    case IMG_U8BIT: { rgst ichar **rowp = (ichar **) img->image;
                      for (y = 0; y < ysize; y++) {
                        sgi_putrow(oimage,rowp[y],(ysize-1)-y,0);
			sgi_putrow(oimage,rowp[y],(ysize-1)-y,1);
                        sgi_putrow(oimage,rowp[y],(ysize-1)-y,2);
                      }
		    } break;
    case IMG_S8BIT: { rgst ichar **rowp = (ichar **) img->image;
		      for (y = 0; y < ysize; y++) {
                        sgi_putrow(oimage,rowp[y],(ysize-1)-y,0);
                        sgi_putrow(oimage,rowp[y],(ysize-1)-y,1);
                        sgi_putrow(oimage,rowp[y],(ysize-1)-y,2);
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



static int
sgi_putrow(rgst IMAGE *image,ichar *buffer,unsigned y,unsigned z)
{
    /***************** rgst unsigned short 	*sptr; ***************/
    rgst ichar 	*sptr;
    rgst ichar      *cptr;
    rgst unsigned int x;
    rgst unsigned long min, max;
    rgst long cnt;

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
		i_errhdlr("sgi_putrow: weird bpp\n");
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
		i_errhdlr("sgi_putrow: weird bpp\n");
	}
    } else 
	i_errhdlr("sgi_putrow: weird image type\n");
}


#endif	/* defined(SGI) */


