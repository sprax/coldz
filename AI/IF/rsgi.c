/******************************************************************************
sgi2raw    S.R.Lines (spraxlo@ai.mit.edu)    93.10.2
	   < tobin - Paul Haeberli - 1986
******************************************************************************/
#include	<stdlib.h>
#include	<string.h>
#include	<SGI/image.h>
#include	<SGI/gutil.h>
#include	<unTypes.h>
#include	<unBug.h>
#include	<unMem.h>
#include	<unUsage.h>
#include	"imagmem.h"
#include	"memFile.h"

void *readSGI(IMAGE *image, unsigned order, unsigned type);

static unsigned short rowbuf[8192];
static unsigned char charbuf[8192];

USAGE("sgi2raw","insgi outraw [-BIRfis]","\
where the options are I=Interleaved (default), P=Byte Planed, R=Row Banded\n\
and c=uchar (default), f=float, i=uint, s=ushort.","")

void main(int argc, char **argv)
{
IMAGE	*image;
char	*cp, cc;
unt 	type = MM_UNC, order = MM_INTERL;
void	*dst;

  if (argc < 2)  Usage("Need two filenames");
  if (argc > 3)  {
    cp = argv[3];
    if (*cp != '-')  Usage("options must begin with '-'");
    while ( (cc = *++cp) != '\0') {
      switch(cc) {
        case 'B': order = MM_PLANAR;	break;
        case 'I': order = MM_INTERL;	break;
        case 'R': order = MM_BANDED;	break;
        case 'c': type  = MM_UNC;	break;
        case 'f': type  = MM_FLT;	break;
        case 'i': type  = MM_UNT;	break;
        case 's': type  = MM_USH;	break;
        default:  Usage("Unrecognized option: %c", cc);
      }
    }
  }

  if ( ! (image=iopen(argv[1],"r")))
    die("Can't open input file %s\n",argv[1]);

  dst = readSGI(image,order,type);
  memWriteUnc(dst,image->xsize,image->ysize
             ,(type==MM_USHORT?2:1),0,argv[2],NULL);
}



void *readSGI(IMAGE *image, unsigned order, unsigned type)
{
int	xsize = image->xsize;
int	ysize = image->ysize;
int	zsize = image->zsize;
size_t	count = xsize*ysize*zsize;
int	y,z;

  switch (type) {
    case MM_USH: {
      ush *mem;
      mallocAss(mem,ush,count);
      for (z=0; z < zsize; z++) {
        for (y=0; y < ysize; y++) { 
          getrow(image,rowbuf,y,z);
          memcpy((void*)(mem+y*xsize),(void*)rowbuf,sizeof(short)*xsize);
        }
      }
      return mem;
    }
    case MM_CHR:
    case MM_UNC: {
      unc *mem;
      mallocAss(mem,unc,count);
      for (z=0; z < zsize; z++) {
        for (y=0; y < ysize; y++) { 
          getrow(image,rowbuf,y,z);
          stoc(rowbuf,charbuf,xsize);
          memcpy((void*)(mem+y*xsize),(void*)charbuf,sizeof(unc)*xsize);
        }
      }
      return mem;
    }
    default: die("readSGI: NOIMP for type %d",type); return NULL;
  }
}




