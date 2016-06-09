/*
 *	Display image defines . . 
 *
 *  			Paul Haeberli - 1988
 *
 */
#ifndef DISPIMGDEF
#define DISPIMGDEF

typedef struct dispimage {
    struct dispimage *next;
    int 		type;
    int 		xsize;
    int 		ysize;
    unsigned char 	*data;
} DISPIMAGE;

#define DI_WRITEPIXELS 		0
#define DI_WRITERGB 		1
#define DI_RECTWRITE 		2
#define DI_LRECTWRITE		3

extern  DISPIMAGE       *makedisp(IMAGE *image);
DISPIMAGE *makedisprgn();
DISPIMAGE *readdispimage();

/* dispimg.c prototypes */
short	**makedittab(int levels, int mult, int add);
long	imagetype(IMAGE *image);
void	setimagemode(IMAGE *image);
int	prefdrawmode(int type);
void	drawimage(DISPIMAGE *di, int xorg, int yorg);
DISPIMAGE *makedisprgn(IMAGE *image, unsigned int x1, unsigned int x2
        , unsigned int y1, unsigned int y2, int show, int xorg, int yorg);
DISPIMAGE *makedisp(IMAGE *image);
void	bwtowp(unsigned short *bw, unsigned short *wp, int n, int y);
void    makedittabs(void);
void rgbtowp(register unsigned short *r, register unsigned short *g
        , register unsigned short *b, register short *wp, register int n
        , register int y);
void	wptowp(unsigned short *sptr, unsigned short *dptr, int n);
DISPIMAGE *readdispimage(char *name);
void	ditrectzoom(float xzoom, float yzoom);
void	ditlrectwrite(int x1, int y1, int x2, int y2, unsigned long *buf);
void	ditlrow(int x, int y, unsigned char *buf, unsigned short *sbuf, int n);
void	ditpnt2i(int x, int y, long c);
void	replrow(long *buf, long *mbuf, int n, int mag);

#endif



