#include <stdio.h>
#include <SGI/image.h>
#include <gl/gl.h>
#include <gl/device.h>

unsigned short rs[8192];
unsigned short gs[8192];
unsigned short bs[8192];

unsigned char rb[8192];
unsigned char gb[8192];
unsigned char bb[8192];

short colorbuff[4096]; 
IMAGE *image;
int x, y, xsize, ysize, zsize;
int xscreensize;
int yscreensize;
short val;

main(argc,argv)
int argc;
char **argv;
{
    long gid, sid, wide,tall, rgbi[4];
    char winStr[256];
    if( argc<2 ) {
	printf("usage: showimg infile\n");
	exit(1);
    } 
    xscreensize = getgdesc(GD_XPMAX);
    yscreensize = getgdesc(GD_YPMAX);
    if( (image=iopen(argv[1],"r")) == NULL ) {
	printf("showci: can't open input file %s\n",argv[1]);
	exit(1);
    }
    xsize = image->xsize;
    ysize = image->ysize;
    zsize = image->zsize;
    if( xsize > xscreensize || ysize > yscreensize ) {
	printf("showci: input image is too big %d %d",xsize,ysize);
	exit(1);
    }
    prefposition(1280-xsize,1279,0,ysize+99);
    /* prefsize(xsize,ysize+100); */
    sprintf(winStr,"%s  (%d, %d, %d) --%s"
           ,argv[1],xsize,ysize,zsize,argv[0]);
    gid = winopen(winStr);
    getsize(&wide,&tall);
    RGBmode();
    gconfig();
    cpack(0x00FF0000);
    clear();
    sleep(2);

    cpack(0x000000FF);
#if 0
    swinopen(gid);
    winposition(25,wide-25,tall-75,tall-25);
    RGBmode();
    gconfig();
    clear();
#else
    rectfi(25,tall-75,wide-25,tall-25);
#endif
    sleep(2);

    sid = swinopen(gid);
    winposition(0,wide-1,0,tall-1);
    RGBmode();
    gconfig();
    drawit();
    while(1) {
	if(qread(&val) == REDRAW) 
	    drawit();
    }
}

drawit()
{
    reshapeviewport();
    viewport(0,xsize-1,0,ysize-1);
    ortho2(-0.5,(float)xsize-0.5,-0.5,(float)ysize-0.5);
    for(y=0; y<ysize; y++) {
	if(zsize<3) {
	    getrow(image,rs,y,0);
	    compress(rs,rb,xsize);
	    cmov2i(0,y);
	    writeRGB(xsize,rb,rb,rb); 
	} else {
	    getrow(image,rs,y,0);
	    compress(rs,rb,xsize);
	    getrow(image,gs,y,1);
	    compress(gs,gb,xsize);
	    getrow(image,bs,y,2);
	    compress(bs,bb,xsize);
	    cmov2i(0,y);
	    writeRGB(xsize,rb,gb,bb); 
	}
    }
}

compress(sptr,bptr,n)
register unsigned short *sptr;
register unsigned char *bptr;
short n;
{
    while(n--) 
	*bptr++ = *sptr++;
}


