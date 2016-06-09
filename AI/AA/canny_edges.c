/*****************************************************************
	canny_edges.c
	Delivery code for Canny Edge Detection.
******************************************************************/

#include <math.h> 
#include <stdio.h>
#include <stdlib.h>


/***********************
	Prototypes
************************/

void canny_edges(unsigned char *image, long w, long h, double dsigma, 
		 double lo_frac, double hi_frac);




/*********  Other internal prototypes ***************/

static void canny_of_canny(long *magmax,long *hthresh,long *lthresh,long *xsize,long *ysize,
	short *shortim,long *windowsize,double *sigma,long *bordermode, double *hfrac,
	double *lfrac,long *pflag, short *gx,short *gy,short *mag,long *hist,
	long *histsize,unsigned char *nms,unsigned char *edgemap,float *gm,
	float *gmp,short *temp);
static void magnitude_of_canny(short *gx,short *gy,long xsize,long ysize,
			       short *mag,long *max);
static void non_max_supp_of_canny(short *mag,short *gradx,short *grady,long ncols,long nrows,
			unsigned char *result); 


static void copyimage_of_canny(long *intimage, long ncols, long nrows, short *shortimage);
static void gauss_filter_of_canny(short *inimage, long inx, long iny,
				  long direction, long boundary, 
				  long masksize, float sigma, short *grad,
				  long *outx, long *outy,
				  float *gmask, float *gprimemask, short *tempimage);
static void find_edges_of_canny(unsigned char *map,short *mag,long xsize,
				long ysize,long maxmag,
				float hpixel_fraction,
				float lpixel_fraction,long *hgram,
				long hsize, long *actual_hthresh,
				long *actual_lthresh);

static void correlate_image_of_canny(short *image_ptr, long incols,
				     long inrows, float filter[20], 
				     long windowsize, long direction, long boundery,
				     short *result, long *outcols, 
				     long *outrows, long *maxval, long *minval, long *status);
static void make_gaussian_mask_of_canny(float gmask[20], float gprimemask[20], long masksize, 
					float sigma, float maxresponse);

static void crop_image_of_canny(unsigned char *imageptr, long *cols,
				long *rows, long cropcornerax, long cropcorneray, 
				long cropcornerbx, long cropcornerby);
static long max_of_canny(long x, long y);
static long min_of_canny(long x, long y);
static long nearestint_of_canny(float x);
static short nearestshort_of_canny(float x);
static void order_of_canny(long *small, long *big);
static void fporder_of_canny(float *big, float *small);
static float fpclip_of_canny(float x, float bound1, float bound2);

static void clear_borders_of_canny(unsigned char *charimage, long xsize, long ysize);
static void follow_edges_of_canny(unsigned char *edgemapptr, short *edgemagptr);

static void cleanup_of_canny(unsigned char *map, long xsize, long ysize);



static void histogram_of_canny(short *theimage, long xsize,long ysize,
			       long pixelmax,long pixelmin,
			       long hgram[], long histsize);
static void get_histogram_threshold_of_canny(long hgram[], long histsize, long pixelmax,
					     long pixelmin, float fraction,
					     long zflag, float *ht, float *lt);

static void thin_of_canny(unsigned char *edges, int height, int width);
static void addmult_of_canny(int row, int col,int label);
static char neighbor_of_canny(int r,int c,int d);




/***********************************************************
	Here's the main procedure to perform edge detection
************************************************************/

#define MAX_MASK 256		/* Mask Mask size - 1D */
/*		DEFINE the DEFAULT values for input parameters... 
		that are used if those parameters AREN'T SPECIFIED */
#define DEFAULT_SIGMA 1.0
#define DEFAULT_MASKSIZE 9
#define DEFAULT_LFRAC 0.5
#define DEFAULT_HFRAC 0.9


void canny_edges(image, w, h, dsigma, lo_frac, hi_frac)
	unsigned char *image;		/*	The byte image.  Will contain edge map */
	long w, h;					/*  Image Dimensions */
	double dsigma;				/*  Size of smoothing kernal */
	double lo_frac, hi_frac;	/*  Fractiles for threshold with hysteresis */
{
	short *shortim, *temp, *gx, *gy, *mag;
	unsigned char *nms;
	long xsize, ysize, windowsize, bordermode, nrc;
	float gm[MAX_MASK], gmp[MAX_MASK];
	double sigma, hfrac, lfrac;
	long pflag = 0, magmax;
	long hist[257];
	long histsize = 256, hthresh, lthresh, lastint;
	long i;
	
	sigma = dsigma;
	windowsize = DEFAULT_MASKSIZE;
	if ((lo_frac < 0.0) || (lo_frac > 1.0) || (hi_frac < 0.0) || (hi_frac > 1.0) ||
		(hi_frac <= lo_frac)){
		lfrac = DEFAULT_LFRAC;
		hfrac = DEFAULT_HFRAC;
	}
	else {
		lfrac = lo_frac;	hfrac = hi_frac;
	};
	xsize = w;	ysize = h;	nrc = w * h;
	shortim = (short *) malloc(sizeof(short) * nrc * 5);
	if (shortim == (short *) '\0') {
		fprintf(stderr, "Error in canny_edges():  Cannot allocate workspace\n");
		exit(-1);
	};
	temp = shortim + nrc;	gx = temp + nrc;	gy = gx + nrc;	mag = gy + nrc;
	nms = (unsigned char *) malloc(sizeof(unsigned char *) * nrc);
	if (nms == (unsigned char *) '\0'){
		fprintf(stderr, "Error in canny_edges():  Cannot allocate workspace\n");
		free(shortim);
		exit(-1);
	};
	for (i=0; i<nrc; i++) shortim[i] = (short) image[i];
	bordermode = 1;
	hthresh = 0;	lthresh = 0;	magmax = 1;	lastint = 32767; pflag = 0;
	histsize = 256;
	*gx = (short) '\0';	*gy = (short) '\0';
	canny_of_canny(&magmax, &hthresh, &lthresh, &xsize, &ysize, shortim, &windowsize,
		       &sigma, &bordermode, &hfrac, &lfrac, &pflag, gx, gy, mag, hist, 
		       &histsize, nms, image, gm, gmp, temp);
	free(nms);
	free(shortim);
	thin_of_canny(image, (int) h, (int) w);
	
}






/*******************************************************************
	canny_top_level
********************************************************************/

#define NOEDGE 0
#define POSSIBLE_EDGE 254
#define EDGE 255

/*----------------*/
/* CONVOLVE IMAGE */
/*--------------- */

/* POSSIBLE VALUES OF THE FILTERTYPE PARMETER */

#define NOSYMMETRY      0
#define SYMMETRIC       1
#define ANITSYMMETRIC   2

/* POSSIBLE VALUES OF THE BOUNDERY PARAMETER */

#define ZERO      0    /* ZEROES THE REGION OUTSIDE OF THE IMAGE */
#define WRAP      1    /* THE FILTER IS CIRCULAR */
#define MAKESMALL 2    /* RETURNS A SMALLER IMAGE. */
#define EXTEND    3    /* EXTENDS THE BOUNDARY PIXELS */
#define MASKIT    4    /* MASKS THE BOUNDARY TO ZERO */

/* POSSIBLE VALUES OF THE STATUS PARAMETER */

#define SUCCESS        1    /* SUCCESSFULL EXECUTION */
#define NO_SUCH_FILTER 2  /* IF THE FILTER TYPE PARAMETER IS NOT ONE OF THE ALLOWED VALS. */                                                  
#define EVENWINDOWSIZE  3  /* IF THE FILTER IS EVENSIZED */
#define NOSUCHDIRECTION 4 /* Direction is not XDIR of YDIR */
#define NOSUCHBOUNDERY 5  /* Nonexistant boundery option specified */

/* POSSIBLE VALUES OF THE DIRECTION PARAMETER */

#define XDIR  1
#define YDIR  2

#define TWO_PI 2*3.1415927


static long x_of_canny[8] = {1,1,0,-1,-1,-1,0,1}, y_of_canny[8] = {0,1,1,1,0,-1,-1,-1};
static long xdim_of_canny,ydim_of_canny;
static long thelothresh_of_canny;
/* debug*/
static long bugflag_of_canny = 0;  /* A bug flag of 1 causes all kinds of debug info..  */

/* ------------------------------------------------------------------ */
/* The top level routine performing a Canny edge detection.           */
/* The parameters are described below.                                */
/* ------------------------------------------------------------------ */

    
static void canny_of_canny(magmax,hthresh,lthresh,xsize,ysize,shortim,windowsize,sigma,
			   bordermode,hfrac,lfrac,pflag,
			   gx,gy,mag,hist,histsize,nms,edgemap,gm,gmp,temp)
/* INPUT PARAMETERS */
    
    long *xsize;             /* The x-dimension of the image */
    long *ysize;             /* The y-dimension of the image */
    short *shortim,*temp;   /* scratch space */
    long *windowsize;        /* The size of the window to be used in the filtering MUST BE ODD!  */
    float *gm,*gmp;         /* arrays to hold masks */
    double *sigma;          /* The sigma of the gaussian in pixels. g(x) = (1/(sqrt(two_PI)*sigma))*exp(x*x/(2*sigma*sigma) */ 
                            /* RANGE -- (0.0,somebignumber) */
    long *bordermode;        /* Specifies how the filtering routine treats th borders.   */
                            /* The options for bordermode are 0: Use zero outside image bounds,,  */
                            /* 1: WRAP, 3: Extends the boudary pixels, */
                            /* 4: Masks the invalid boundary region to zero.*/ 
    double *hfrac;           /* Chooses the upper threshold in hysteretic thresholding to cause the  seed pixels to be chosen from */
                            /* the upper (hfrac*100) percent of the pixels in the magnitiude (edginess) image */
                            /* RANGE -- [0.0,1.0] */
    double *lfrac;           /* Mutiplies the upper threshold to get the lower hysteretic threshold */
                            /* RANGE -- [0.0, 1.0] */
    long *pflag;              /* 1 -- print progress. 0 -- suppress printing of progress. */
    
/* Output Parameters */
    unsigned char *edgemap;  /* An array of char with 255 values at the edgepoints and 0 elsewhere */
    
/* Input/Output parameters */
    short *gx;              /* Pointer to array of short.  If not nil, will be used to compute x-gradient */
    short *gy;              /* Same as gx except its the y gradient.  If the pointer is nil, canny routine will allocate */
                            /* temporary storage for these routines. */
    short *mag;             /* Same as gx and gy except for its for the magnitude image. */
    long  *magmax;           /* storage  for the maximum gradient magnitude. */
    long  *hist;              /* An array of 256 integers used for historgramming which will contain a histogram of the mag image. */
    long  *histsize;          /* Size of the histogram. */
    long *hthresh, *lthresh; /* If not nil, the values of the actual thresholds used in the hysteretic thresholding will be returned. */
    unsigned char  *nms;    /* Same as above except it is a pointer to an array of unsigned char. The result of non-maximal */
                            /* supression applied to the magnitude image is placed here, if not nil. */
{    
    short *mgx,*mgy,*mmag,*shortimage;
    long *mhist,mhthresh,mlthresh, mhistsize,dummyoutx,dummyouty,mxsize,mysize,mmagmax;
    //unsigned char *iptr, *optr;
    short *temp_image;
    float *gmask,*gpmask;
    

    if(*pflag)
    {
        fprintf(stderr,"The non-array input arguments are: \n\nxsize - %d\nysize - %d\nsigma - %f\nwindowsize - %d\n",*xsize,*ysize,*sigma,*windowsize);
        fprintf(stderr,"lfrac - %f\nhfrac - %f\n\n",*lfrac,*hfrac);
        fprintf(stderr,"\nbordermode - %d\nhistsize - %d\n\n",*bordermode,*histsize);
    }

    mxsize = *xsize;
    mysize = *ysize;

    mhistsize = *histsize;

    /* array arguments are assumed to have the following sizes  */
    /*                                                          */
    /*      pointer     bytes       type        # elements      */
    /*      -------     -----       ----        ----------      */
    /*      shortim     2*npixels   short       nrows*ncols     */
    /*      gx          2*npixels   short       nrows*ncols     */
    /*      gy          2*npixels   short       nrows*ncols     */
    /*      gy          2*npixels   short       nrows*ncols     */
    /*      mag         2*npixels   short       nrows*ncols     */
    /*      nms         npixels     unsign ch   nrows*ncols     */
    /*      hist        4*histsize  int         histsize        */
    /*      gm          4*windowsz  float       windowsize      */
    /*      gmp         4*windowsz  float       windowsize      */
    /*      temp        2*npixels   short       nrows*ncols     */

    shortimage = shortim;
    mgx = gx;
    mgy = gy;
    mmag = mag;
    mhist = hist;
    temp_image = temp;
    gmask = gm;
    gpmask = gmp;

    /* BEGIN COMPUTATION */ 

   /* copyimage_of_canny(image,mxsize,mysize,shortimage);   */

    shortimage = shortim;
                           
    if(*pflag)
        fprintf(stderr,"Beginning computation of the x gradient image.\n");

    gauss_filter_of_canny(shortimage,mxsize,mysize,XDIR,
                 *bordermode,*windowsize,*sigma,mgx,
                 &dummyoutx,&dummyouty,gmask,gpmask,temp_image);

    shortimage = shortim;
    temp_image = temp;
    gmask = gm;
    gpmask = gmp;

    if(*pflag)
    {
        fprintf(stderr,"Done with computation of the x gradient image.\n\n");
        fprintf(stderr,"Beginning computation of the y gradient image.\n");
    }

    gauss_filter_of_canny(shortimage,mxsize,mysize,YDIR,
                 *bordermode,*windowsize,*sigma,mgy,
                 &dummyoutx,&dummyouty,gmask,gpmask,temp_image);
         
    shortimage = shortim;
    temp_image = temp;
    gmask = gm;
    gpmask = gmp;

    if(*pflag)
    {
        fprintf(stderr,"Done with computation of the y gradient image.\n\n");
        fprintf(stderr,"Beginning computation of the magnitude image from the two gradient images.\n");
    }

    magnitude_of_canny(mgx,mgy,mxsize,mysize,mmag,&mmagmax);

    if(*pflag)
        fprintf(stderr,"Done computing magnitude.\n\nStarting non-maximal supression.\n");

    non_max_supp_of_canny(mmag,mgx,mgy,mxsize,mysize,edgemap);

/*	COMMENTED OUT BY JMM Apr.7, 1989 - since the nms array is
currently never printed out.


	/  * This code (following) originally copied edgemap from the
	non_max_supp (above) to nms to save the results before the
	final edge pixels were chosen.  It can be commented out IF
	noone wants to use this nms mask later. *  /

    if (nms != (unsigned char *) 0L)
    {
fprintf(stderr,"JOE copying NMS NOW FOR DEBUG or OTHER PURPOSES\n");
        for(i=0, iptr = edgemap, optr = nms; i<mxsize*mysize;i++, iptr++, optr++)
            *optr = *iptr;
    }
*/
    if(*pflag)
        fprintf(stderr,"Done with non-maximal supression.\n\nStarting hysteretic thresholding.\n");

    find_edges_of_canny(edgemap,mmag,mxsize,mysize,mmagmax,*hfrac,*lfrac,
               mhist,mhistsize,&mhthresh,&mlthresh);

    if(*pflag)
        fprintf(stderr,"Done with hysterectic thresholding.\n\nDone with Canny edge detection.\n");

    /* set output vars */
    
    if (magmax != (long *) 0L)
    {
        *magmax = mmagmax;
    }


    if (hthresh != (long *) 0L)
    {
        *hthresh =  mhthresh;
    }

    if (lthresh != (long *) 0L) 
    {
        *lthresh =  mlthresh; 
    }

}



/* ------------------------------------------------------------------ */   
/* Mag takes two images and produces their magnitude                  */
/* ------------------------------------------------------------------ */

static void magnitude_of_canny(gx,gy,xsize,ysize,mag, max)
    short *gx,*gy,*mag;
    long xsize,ysize,*max;
{
    short *xpixelptr,*ypixelptr,*magpixelptr;
    long pixelcount,themax = 0;
    float gradx,grady;


    for(pixelcount = 0, xpixelptr = gx, ypixelptr = gy, magpixelptr = mag;
        pixelcount < xsize*ysize;
        pixelcount++, xpixelptr++,ypixelptr++,magpixelptr++)
    {
        gradx = (float) *xpixelptr;
        grady = (float) *ypixelptr;

        if ((*magpixelptr = nearestshort_of_canny(sqrt((float) (gradx*gradx + grady*grady)))) > themax)
            themax = *magpixelptr;
    }
    *max = themax;
}
 
/* ------------------------------------------------------------------ */
/* The following procedure calculates the non-maximal suppression of  */
/* an image.  The required input is the magnitude image, and the two  */
/* gradient images. Edgemap is and image for the result.              */
/* ------------------------------------------------------------------ */

static void non_max_supp_of_canny(mag,gradx,grady,ncols,nrows,result) 
    short *mag, *gradx, *grady;
    long   ncols, nrows; 
    unsigned char *result;
{
    long rowcount, colcount,count;
    short *magrowptr,*magptr;
    short *gxrowptr,*gxptr;
    short *gyrowptr,*gyptr,z1,z2;
    short m00,gx,gy;
    float mag1,mag2,xperp,yperp;
    unsigned char *resultrowptr, *resultptr;
    

    /* Zero the edges of the result image */

    for(count = 0,resultrowptr = result, resultptr = result + ncols*(nrows - 1); 
        count < ncols;
        resultptr++,resultrowptr++,count++)
    {
        *resultrowptr = *resultptr = (unsigned char) 0;
    }

    for(count = 0, resultptr = result, resultrowptr = result + ncols - 1;
        count < nrows;
        count++, resultptr += ncols, resultrowptr += ncols)
    {
        *resultptr = *resultrowptr = (unsigned char) 0;
    }

    /* Suppress non-maximum points */

    for(rowcount = 1, magrowptr = mag + ncols + 1, gxrowptr = gradx + ncols + 1, gyrowptr = grady + ncols + 1, resultrowptr = result + ncols + 1;
        rowcount < nrows - 2; 
        rowcount++, magrowptr += ncols, gyrowptr += ncols, gxrowptr += ncols, resultrowptr += ncols)
    {   
        for(colcount = 1, magptr = magrowptr, gxptr = gxrowptr, gyptr = gyrowptr, resultptr = resultrowptr; 
            colcount < ncols - 2; 
            colcount++, magptr++,gxptr++,gyptr++, resultptr++)
        {   
            m00 = *magptr;
            if(m00 == 0)
            {
                *resultptr = (unsigned char) NOEDGE; 
   /* Commented out by Joe Miller Nov. 89 - a BUG ?  Reported to me by
	ramin@scotty.Standfor.edu...     at least its a potential error
 	I don't know where it came from.
                xperp = -(gx = *gxptr)/((float)m00);
                yperp = (gy = *gyptr)/((float)m00);
 */
            }
            else
            {
                xperp = -(gx = *gxptr)/((float)m00);
                yperp = (gy = *gyptr)/((float)m00);
            }

            if (gx >= 0)
            {
                if (gy >= 0) 
                {
                    if (gx >= gy)
                    {  
                        /* 111 */
                        /* Left point */
                        z1 = *(magptr - 1);
                        z2 = *(magptr - ncols - 1);

                        mag1 = (m00 - z1)*xperp + (z2 - z1)*yperp;
                        
                        /* Right point */
                        z1 = *(magptr + 1);
                        z2 = *(magptr + ncols + 1);

                        mag2 = (m00 - z1)*xperp + (z2 - z1)*yperp;
                        /* JOE CHANGED THIS ERROR  mag2 = (m00 - z1)*xperp + (z1 - z2)*yperp; */
                    }
                    else
                    {    
                        /* 110 */
                        /* Left point */
                        z1 = *(magptr - ncols);
                        z2 = *(magptr - ncols - 1);

                        mag1 = (z1 - z2)*xperp + (z1 - m00)*yperp;

                        /* Right point */
                        z1 = *(magptr + ncols);
                        z2 = *(magptr + ncols + 1);

                        mag2 = (z1 - z2)*xperp + (z1 - m00)*yperp; 
                    }
                }
                else
                {
                    if (gx >= -gy)
                    {
                        /* 101 */
                        /* Left point */
                        z1 = *(magptr - 1);
                        z2 = *(magptr + ncols - 1);

                        mag1 = (m00 - z1)*xperp + (z1 - z2)*yperp;
            
                        /* Right point */
                        z1 = *(magptr + 1);
                        z2 = *(magptr - ncols + 1);

                        mag2 = (m00 - z1)*xperp + (z1 - z2)*yperp;
                    }
                    else
                    {    
                        /* 100 */
                        /* Left point */
                        z1 = *(magptr + ncols);
                        z2 = *(magptr + ncols - 1);

                        mag1 = (z1 - z2)*xperp + (m00 - z1)*yperp;

                        /* Right point */
                        z1 = *(magptr - ncols);
                        z2 = *(magptr - ncols + 1);

                        mag2 = (z1 - z2)*xperp  + (m00 - z1)*yperp; 
                    }
                }
            }
            else
            {
                if ((gy = *gyptr) >= 0)
                {
                    if (-gx >= gy)
                    {          
                        /* 011 */
                        /* Left point */
                        z1 = *(magptr + 1);
                        z2 = *(magptr - ncols + 1);

                        mag1 = (z1 - m00)*xperp + (z2 - z1)*yperp;

                        /* Right point */
                        z1 = *(magptr - 1);
                        z2 = *(magptr + ncols - 1);

                        mag2 = (z1 - m00)*xperp + (z2 - z1)*yperp;
                    }
                    else
                    {
                        /* 010 */
                        /* Left point */
                        z1 = *(magptr - ncols);
                        z2 = *(magptr - ncols + 1);

                        mag1 = (z2 - z1)*xperp + (z1 - m00)*yperp;

                        /* Right point */
                        z1 = *(magptr + ncols);
                        z2 = *(magptr + ncols - 1);

                        mag2 = (z2 - z1)*xperp + (z1 - m00)*yperp;
                    }
                }
                else
                {
                    if (-gx > -gy)
                    {
                        /* 001 */
                        /* Left point */
                        z1 = *(magptr + 1);
                        z2 = *(magptr + ncols + 1);

                        mag1 = (z1 - m00)*xperp + (z1 - z2)*yperp;

                        /* Right point */
                        z1 = *(magptr - 1);
                        z2 = *(magptr - ncols - 1);

                        mag2 = (z1 - m00)*xperp + (z1 - z2)*yperp;
                    }
                    else
                    {
                        /* 000 */
                        /* Left point */
                        z1 = *(magptr + ncols);
                        z2 = *(magptr + ncols + 1);

                        mag1 = (z2 - z1)*xperp + (m00 - z1)*yperp;

                        /* Right point */
                        z1 = *(magptr - ncols);
                        z2 = *(magptr - ncols - 1);

                        mag2 = (z2 - z1)*xperp + (m00 - z1)*yperp;
                    }
                }
            } 

            /* Now determine if the current point is a maximum point */

            if ((mag1 >= 0.0) || (mag2 > 0.0))
            {
                *resultptr = (unsigned char) NOEDGE;
            }
            else
            {
                *resultptr = (unsigned char) POSSIBLE_EDGE;
            }
        } 
    }
}


/*******************************************************************************
	gauss.c
********************************************************************************/

/* This routine convolves an image with a filter along one direction */
/* ie. x or y.   */

static void correlate_image_of_canny(image_ptr, incols, inrows, filter, windowsize,
				     direction, boundery, 
				     result, outcols, outrows, maxval, minval, status)
    short *image_ptr; 
    short *result;
    long inrows, incols, windowsize, boundery, direction;
    float filter[20];
    long *outcols, *outrows, *maxval, *minval, *status;
{
    long rowcount,colcount,findex,halfwindow,currentpixel,thecol,zeroflag = 0;
    long therow,totalpixels, windowcolpixels,themaxval = 0, theminval = 0;
    short *inbegrowptr, *inendrowptr; 
    short *inbegcolptr, *inendcolptr, *inposptr1, *inposptr2; 
    short *outrowptr, *outcolptr, *outposptr; 
    float currentresult;

/* DEBUG PRINTOUT HERE... - JMM
fprintf(stderr,"COR_IMAGE: col %d row %d \n",incols,inrows);
	To get cols and rows passed */

    if (windowsize % 2 == 0) 
    {  
        /* Do not allow even sized filters */
        *status = EVENWINDOWSIZE;
        return;
    } 
    
    /* Calculate the values of some variables */

    halfwindow = windowsize/2;  /* half the size of the filter window */
    totalpixels = inrows * incols;    /* The total number of pixels in the image */
    windowcolpixels = halfwindow * incols;  /* The number of pixels in the scan lines between two pixels that  */
                                                /* are halfwindow apart vertically. */


    switch(direction)
    {                        
        case XDIR: 
        /* Filter is in the X direction */
            /* Convolution in the x direction, convolve across cols */
            for(rowcount = 0, inbegrowptr = image_ptr, inendrowptr = image_ptr + incols - 1, outrowptr = result; 
                rowcount < inrows; rowcount++, inbegrowptr += incols, outrowptr += incols)
            {
                for(colcount = 0, inposptr1 = inbegrowptr, outposptr = outrowptr; 
                    colcount < incols ; 
                    colcount++, inposptr1++, outposptr++)
                { 
                    currentresult = 0;
                    zeroflag = 0;
                    /* scan filter */
                    for(findex = 0, inposptr2 = inposptr1 - halfwindow, thecol = colcount - halfwindow; 
                        findex < windowsize; 
                        findex++, inposptr2++, thecol++)
                    {
				/* This section works on the LEFT side
				of the image - JMM Apr. 6, 89 */
                        if(thecol < 0)
                        {      
                            switch(boundery)
                            {
                                case ZERO:
                                    currentpixel = 0;
                                    break;
                                case WRAP:
                                    currentpixel = (long) (*(inposptr2 + incols));
                                    break;
                                case MAKESMALL:
                                    currentpixel = 0;
                                    break; 
                                case EXTEND:
                                    currentpixel = (long) *(inbegrowptr); /* lefthand pixel of the current row */
                                    break;
                                case MASKIT:
                                    currentpixel = 0;
                                    zeroflag = 1;
                                    break;
                                default:
                                    *status = NOSUCHBOUNDERY;
                                    return;
                            }
                        }
                        else
                        {
				/* This section works on the right side
				of the image - JMM Apr. 6, 89 */
                            if(thecol >= incols)
                            {
                                switch(boundery)
                                {
                                case ZERO:
                                    currentpixel = 0;
                                    break;
                                case WRAP:
                                   currentpixel = (long) (*(inposptr2 - incols));
                                    break;
                                case MAKESMALL:
                                    currentpixel = 0;
                                    break; 
                                case EXTEND:
                                    currentpixel = (long) *(inendrowptr); /* lefthand pixel of the current row */
                                    break; 
                                case MASKIT:
                                    currentpixel = 0;
                                    zeroflag = 1;
                                    break;
                                default:
                                    *status = NOSUCHBOUNDERY;
                                    return;
                                }
                            }
                            else
                                currentpixel = (long) *inposptr2;
                        }  
                        if (!zeroflag) 
                            currentresult += filter[findex]*currentpixel;
                    }   /* End filter scan loop */
                    if (themaxval < (long)currentresult)
                        themaxval = (long)currentresult;
                    if (theminval > (long)currentresult)
                        theminval = (long)currentresult;
                    *outposptr = nearestshort_of_canny(currentresult);
                }   /* End column loop */
            }   /* End row loop */ 

            *outrows = inrows;
            *outcols = incols; 
            *maxval = themaxval;
            *minval = theminval;




            if (boundery == MAKESMALL)
            {
                crop_image_of_canny((unsigned char *) result,outcols,outrows,halfwindow,0,incols - halfwindow,inrows - 1);
            }
            break;  /* Break out of direction switch statement */


        /* Filter in the Y direction */
        case YDIR: 
            /* Convolution in the Y direction, convolve across rows */
            for(colcount = 0,inbegcolptr = image_ptr,inendcolptr = image_ptr + totalpixels - incols, outcolptr = result; 
                colcount < incols; 
                colcount++, inbegcolptr++, inendcolptr++, outcolptr++)
            {
                for(rowcount = 0,inposptr1 = inbegcolptr,outposptr = outcolptr; 
                    rowcount < inrows; 
                    rowcount++, inposptr1 += incols, outposptr += incols)
                {    
                    zeroflag = 0;
                    currentresult = 0;
                    /* scan filter */
                    for(findex = 0, inposptr2 = inposptr1 - windowcolpixels, therow = rowcount - halfwindow; 
                        findex < windowsize; 
                        findex++, inposptr2 += incols, therow++)
                    {
                        if(therow < 0)
                        {
				/*  TOP of IMAGE is processed here -JMM */
                            switch(boundery)
                            {
                                case ZERO:
                                    currentpixel = 0;
                                    break;
                                case WRAP:
                                    currentpixel = (long) (*(inposptr2 + totalpixels));   /* totalpixels = nrow*ncols */
                                    break;
                                case MAKESMALL:
                                    currentpixel = 0;
                                    break; 
                                case EXTEND:
                                    currentpixel = (long) (*(inbegcolptr)); /* top pixel of the current column */
                                    break;
                                case MASKIT:
                                    currentpixel = 0;
                                    zeroflag = 1;
                                    break;
                                default:
                                    *status = NOSUCHBOUNDERY;
                                    return;
                            }
                        } /* end of IF */
                        else
                        {
                            if(therow >= inrows)
                            {
                                switch(boundery)
                                {
                                case ZERO:
                                    currentpixel = 0;
                                    break;
                                case WRAP:
                                    currentpixel = (long) (*(inposptr2 - totalpixels));   /* totalpixels = nrow*ncols */
                                    break;
                                case MAKESMALL:
                                    currentpixel = 0;
                                    break; 
                                case EXTEND:
                                    currentpixel = (long) (*(inendcolptr));  /* bottom pixel of the current column */
                                    break;
                                case MASKIT:
                                    currentpixel = 0;
                                    zeroflag = 1;
                                    break;
                                default:
                                    *status = NOSUCHBOUNDERY;
                                    return;
                                }
                            }

			/* THEY FORGOT THIS ELSE !!  - JMM */
			    else
                                currentpixel = (long) *inposptr2;
                        } /* end of else */ 
                        if (!zeroflag)
                            currentresult += filter[findex]*currentpixel;
                    }   /* End filter scan loop */
                    if (themaxval < currentresult) 
                        themaxval = currentresult;
                    if (theminval > currentresult) 
                        theminval = currentresult;
                    *outposptr = nearestshort_of_canny(currentresult);
                }   /* End row loop */
            }   /* End column loop */

            *outrows = inrows;
            *outcols = incols;
            *maxval = themaxval;
            *minval = theminval;

            if (boundery == MAKESMALL)
            {    
                /* shift image to upper left corner and set the outrow and outcols parameters appropriately */

                crop_image_of_canny((unsigned char *) result,outcols,outrows,0,halfwindow,incols - 1,inrows - halfwindow);
            }      
            break;

        default:
            *status = NOSUCHDIRECTION; 
            return;
    }   /* End direction switch */
}
                                 
/*-----------------------------------------------------*/    
/* This routine loads gmask with a Gaussian mask with  */
/* sigma in pixels as the spreading factor.  It also   */
/* loads gprimemask with the derivative of a Gaussian. */
/* The masksize must be odd. Maxresponse is the        */ 
/* maximum response desired to a perfect unit step     */
/* edge.                                               */
/* --------------------------------------------------- */

#define NINTEGRATEPTS 11  /* Number of points to integrate over. */

static void make_gaussian_mask_of_canny(gmask, gprimemask, masksize, sigma, maxresponse)
     float gmask[20],gprimemask[20],sigma, maxresponse;
     long masksize;
{
    long i, maskcenter, count, findex;
    float delta, currentx,gconst,gprimeconst;
    
    if (masksize % 2 == 0)
    {
        printf("Even masksize in make_gaussian_mask (in gauss.c).\n");
        exit(-1); 
    }

    maskcenter = masksize/2; 
    delta = 1.0/(NINTEGRATEPTS - 1);
    
    gconst = 1.0*maxresponse/((sqrt(TWO_PI)) * sigma);
    gprimeconst = maxresponse/(sigma*sigma);


    for(i = -maskcenter, findex = 0;findex < masksize;i++, findex++)
    {   
        gmask[findex] = 0;
        gprimemask[findex] = 0;
        
        /* Loop NINTEGRATEPTS times to perform the integration. */
        for(count = 1, currentx = i - .5; count <= NINTEGRATEPTS; count++, currentx += delta)
        {
            gmask[findex] += gconst * exp(-currentx*currentx/(2*sigma*sigma));
            gprimemask[findex] += gprimeconst * currentx * exp(-currentx*currentx/(2*sigma*sigma)); 
        }
        gmask[findex] /= NINTEGRATEPTS;
        gprimemask[findex] /= NINTEGRATEPTS;
    }
}


static void gauss_filter_of_canny(inimage, inx, iny, direction, boundary, masksize,
             sigma, grad, outx, outy,
             gmask,gprimemask,tempimage)
    short *inimage,*grad;
    long inx,iny,direction,masksize,*outx,*outy,boundary;
    float sigma;
    float *gmask, *gprimemask;
    short *tempimage;
{
    long orthogdir,status,max,min;

    /* Allocate space for the mask off the stack */
    /* and load mask with Gaussian and its derivative */ 

    make_gaussian_mask_of_canny(gmask, gprimemask, masksize, sigma, 4.0);

    /* Load orthogdir with the direction orthoganal to direction */

    switch(direction)
    {
        case XDIR:
            orthogdir = YDIR;
            break;
        case YDIR:
            orthogdir = XDIR;
            break;
        default:
            fprintf(stderr,"No such direction in gauss_filter (in gauss.c).\n");
            exit(-1);
            break;
    } 
status=0;
    

    correlate_image_of_canny(inimage,inx,iny,gmask,masksize,orthogdir,boundary,tempimage,outx,outy,&max,&min,&status);


    correlate_image_of_canny(tempimage,*outx,*outy,gprimemask,masksize,direction,boundary,grad,outx,outy,&max,&min,&status);
}
    
/* ----------------------------------------------------- */
/* copyimage copys an array of INTEGER to an array - JMM */
/* of short.                                             */
/* JMM: Modified Mar. 2, 1989 to copy integers - NOT u.c.*/
/* ----------------------------------------------------- */

static void copyimage_of_canny(charimage,ncols,nrows,shortimage)
    long *charimage;
    short *shortimage;
    long ncols, nrows;
{
    long i;
    long *inptr;
    short *outptr;

    for(i=0,inptr = charimage,outptr = shortimage;i<ncols*nrows;i++,inptr++,outptr++)
    {
        *outptr = (short) *inptr;
    }
}


/*********************************************************************************
	general.c
**********************************************************************************/

/* Max and min do the obvious thing on integers. */

static long max_of_canny(x,y) long x,y; { return((x > y) ? x : y); }
static long min_of_canny(x,y) long x,y; { return((x < y) ? x : y); }

/*  Nearestint takes a float argument and rounds to the nearest integer. */

static long nearestint_of_canny(x) 
    float x; 
{   
    if (x >= 0) 
        return((long) (x + .5));
    else
        return((long) (x - .5));
}

static short nearestshort_of_canny(x)
    float x;
{
    if (x >= 0) 
        return((short) (x + .5));
    else
        return((short) (x - .5));
}


/* This routines takes two integers in small and big and return them so that */
/* the smaller is in small and the larger is in big */

static void order_of_canny(small, big)
    long *small, *big;
{   
    long temp;

    if ((temp = *small) > *big)
    {
        *small = *big;
        *big = temp;
    }
}

/* Same thing for floats */
 
static void fporder_of_canny(big,small)
    float *big,*small;
{
    float b,s;

    if ((b = *big) < (s = *small))
    {
        *small = b;
        *big = s;
    }
}  

/* Clip a floating point number to bound1 and bound2 (they need not be ordered)  */

static float fpclip_of_canny(x,bound1,bound2)
    float x,bound1,bound2;
{ 
    fporder_of_canny(&bound1,&bound2);
    
    if (x > bound1) 
       return(bound1);

    if (x < bound2)
        return(bound2);

    return(x);
}

/******************************************************************************
	hthresh.c
*******************************************************************************/

/* -------------------------------------------------------------------------- */
/* clear borders sets the edge pixels of a character array to zero            */
/* -------------------------------------------------------------------------- */                                                                              

static void clear_borders_of_canny(charimage,xsize,ysize)
    unsigned char *charimage;
    long xsize,ysize;
{
    unsigned char *rowptr,*rowendptr, *colptr, *colendptr;
    long i;

    for(i=0, rowptr = charimage, rowendptr = charimage + xsize - 1;
        i < ysize;
        i++,rowptr += xsize, rowendptr += xsize)
    {
        *rowptr = (unsigned char) NOEDGE;
        *rowendptr = (unsigned char) NOEDGE;
    }

    for(i=0,colptr = charimage, colendptr = charimage + xsize*(ysize - 1);
        i < xsize;
        i++, colptr++, colendptr++)
    {
        *colptr = (unsigned char) NOEDGE;
        *colendptr = (unsigned char) NOEDGE;
    } 
}


/* ------------------------------------------------------------------------ */
/* follow_edges is a recursive routine that traces edgs along all paths     */
/* whose magnitude values remain above some specifyable lower threshhold.   */
/* This lower threshold is a floating number between 0.0 and 1.0 and        */
/* defines the threshold in terms of the fraction of the largest value of   */
/* the magnitude.                                                           */
/* ------------------------------------------------------------------------ */


static void follow_edges_of_canny(edgemapptr,edgemagptr)
    unsigned char *edgemapptr;
    short *edgemagptr;
{   
    short *tempmagptr;
    unsigned char *tempmapptr;
    long i;


/*debug*/      if (bugflag_of_canny) 
/*debug*/      {  
/*debug*/          printf("%d %d %d %d %d\n%d %d %d %d %d\n%d %d %d %d %d\n%d %d %d %d %d\n%d %d %d %d %d\n\n",
/*debug*/                            *(edgemapptr-2*xdim_of_canny-2),*(edgemapptr-2*xdim_of_canny-1),*(edgemapptr-2*xdim_of_canny),*(edgemapptr-2*xdim_of_canny+1),*(edgemapptr-2*xdim_of_canny+2),
/*debug*/                      *(edgemapptr-xdim_of_canny-2),*(edgemapptr-xdim_of_canny-1),*(edgemapptr-xdim_of_canny),*(edgemapptr-xdim_of_canny+1),*(edgemapptr-xdim_of_canny+2),
/*debug*/                      *(edgemapptr-2),*(edgemapptr-1),*(edgemapptr),*(edgemapptr+1),*(edgemapptr+2),
/*debug*/                      *(edgemapptr+xdim_of_canny-2),*(edgemapptr+xdim_of_canny-1),*(edgemapptr+xdim_of_canny),*(edgemapptr+xdim_of_canny+1),*(edgemapptr+xdim_of_canny+2),
/*debug*/                      *(edgemapptr+2*xdim_of_canny-2),*(edgemapptr+2*xdim_of_canny-1),*(edgemapptr+2*xdim_of_canny),*(edgemapptr+2*xdim_of_canny+1),*(edgemapptr+2*xdim_of_canny+2));
    
/*debug*/          printf("%d %d %d %d %d\n%d %d %d %d %d\n%d %d %d %d %d\n%d %d %d %d %d\n%d %d %d %d %d\n\n",
/*debug*/                            *(edgemagptr-2*xdim_of_canny-2),*(edgemagptr-2*xdim_of_canny-1),*(edgemagptr-2*xdim_of_canny),*(edgemagptr-2*xdim_of_canny+1),*(edgemagptr-2*xdim_of_canny+2),
/*debug*/                      *(edgemagptr-xdim_of_canny-2),*(edgemagptr-xdim_of_canny-1),*(edgemagptr-xdim_of_canny),*(edgemagptr-xdim_of_canny+1),*(edgemagptr-xdim_of_canny+2),
/*debug*/                      *(edgemagptr-2),*(edgemagptr-1),*(edgemagptr),*(edgemagptr+1),*(edgemagptr+2),
/*debug*/                      *(edgemagptr+xdim_of_canny-2),*(edgemagptr+xdim_of_canny-1),*(edgemagptr+xdim_of_canny),*(edgemagptr+xdim_of_canny+1),*(edgemagptr+xdim_of_canny+2),
/*debug*/                      *(edgemagptr+2*xdim_of_canny-2),*(edgemagptr+2*xdim_of_canny-1),*(edgemagptr+2*xdim_of_canny),*(edgemagptr+2*xdim_of_canny+1),*(edgemagptr+2*xdim_of_canny+2));
 
/*debug*/      }




    for(i=0;i<8;i++)
    {   
        tempmapptr = edgemapptr - y_of_canny[i]*xdim_of_canny + x_of_canny[i];
        tempmagptr = edgemagptr - y_of_canny[i]*xdim_of_canny + x_of_canny[i];
                                  
        if( (*tempmapptr == POSSIBLE_EDGE) && (*tempmagptr > thelothresh_of_canny))
        {    
            *tempmapptr = (unsigned char) EDGE;
 
/*debug*/      if (bugflag_of_canny) 
/*debug*/      {  
/*debug*/          printf("%d %d %d %d %d\n%d %d %d %d %d\n%d %d %d %d %d\n%d %d %d %d %d\n%d %d %d %d %d\n\n",
/*debug*/                            *(tempmapptr-2*xdim_of_canny-2),*(tempmapptr-2*xdim_of_canny-1),*(tempmapptr-2*xdim_of_canny),*(tempmapptr-2*xdim_of_canny+1),*(tempmapptr-2*xdim_of_canny+2),
/*debug*/                      *(tempmapptr-xdim_of_canny-2),*(tempmapptr-xdim_of_canny-1),*(tempmapptr-xdim_of_canny),*(tempmapptr-xdim_of_canny+1),*(tempmapptr-xdim_of_canny+2),
/*debug*/                      *(tempmapptr-2),*(tempmapptr-1),*(tempmapptr),*(tempmapptr+1),*(tempmapptr+2),
/*debug*/                      *(tempmapptr+xdim_of_canny-2),*(tempmapptr+xdim_of_canny-1),*(tempmapptr+xdim_of_canny),*(tempmapptr+xdim_of_canny+1),*(tempmapptr+xdim_of_canny+2),
/*debug*/                      *(tempmapptr+2*xdim_of_canny-2),*(tempmapptr+2*xdim_of_canny-1),*(tempmapptr+2*xdim_of_canny),*(tempmapptr+2*xdim_of_canny+1),*(tempmapptr+2*xdim_of_canny+2));
                                    
/*debug*/          printf("Came from (%d, %d).\n",-x_of_canny[i],-y_of_canny[i]);
/*debug*/          printf("%d %d %d %d %d\n%d %d %d %d %d\n%d %d %d %d %d\n%d %d %d %d %d\n%d %d %d %d %d\n\n",
/*debug*/                            *(tempmagptr-2*xdim_of_canny-2),*(tempmagptr-2*xdim_of_canny-1),*(tempmagptr-2*xdim_of_canny),*(tempmagptr-2*xdim_of_canny+1),*(tempmagptr-2*xdim_of_canny+2),
/*debug*/                      *(tempmagptr-xdim_of_canny-2),*(tempmagptr-xdim_of_canny-1),*(tempmagptr-xdim_of_canny),*(tempmagptr-xdim_of_canny+1),*(tempmagptr-xdim_of_canny+2),
/*debug*/                      *(tempmagptr-2),*(tempmagptr-1),*(tempmagptr),*(tempmagptr+1),*(tempmagptr+2),
/*debug*/                      *(tempmagptr+xdim_of_canny-2),*(tempmagptr+xdim_of_canny-1),*(tempmagptr+xdim_of_canny),*(tempmagptr+xdim_of_canny+1),*(tempmagptr+xdim_of_canny+2),
/*debug*/                      *(tempmagptr+2*xdim_of_canny-2),*(tempmagptr+2*xdim_of_canny-1),*(tempmagptr+2*xdim_of_canny),*(tempmagptr+2*xdim_of_canny+1),*(tempmagptr+2*xdim_of_canny+2));
 
/*debug*/      }



            follow_edges_of_canny(tempmapptr,tempmagptr);
        }
    } 
}

/* -------------------------------------------------------------------------- */
/* This routine finds edges that are above some hi threshhold and calls       */
/* follow edges.                                                              */
/* -------------------------------------------------------------------------- */

static void find_edges_of_canny(map,mag,xsize,ysize,maxmag,hpixel_fraction,
				lpixel_fraction,hgram,hsize,actual_hthresh,actual_lthresh)
    unsigned char *map;
    short *mag;
    long xsize, ysize,maxmag,*hgram,hsize,*actual_hthresh,*actual_lthresh;
    float hpixel_fraction,lpixel_fraction;
{
    long hthresh;
    long i;
    float hithresh,lothresh;
    short *magptr;
    unsigned char *mapptr;

/*	DEBUG code - JMM
	fprintf(stderr,"find_edges: START EDGE is %d NOEDGE %d\n",EDGE,NOEDGE);
*/

    xdim_of_canny = xsize;
    ydim_of_canny = ysize;

    /* Set the borders of the edge map to zero as this will insure that the recusive */
    /* procedure follow_edges will terminate without overrunning the edges           */
                              
/*debug    printf("\n clear borders \n");*/

    clear_borders_of_canny(map,xsize,ysize);
                                    

    /* Histogram the magenitude image to find whare the pixel values lie */ 

/*debug    printf("\n histogram \n"); */

    histogram_of_canny(mag,xsize,ysize,maxmag,0,hgram,hsize); 
    
    /* Use the histogram to get a pixel threshold below which h_pixel_fraction  */
    /* pixels lie */

/*debug    printf("\n hist threshold \n"); */

    get_histogram_threshold_of_canny(hgram,hsize,maxmag,0,hpixel_fraction,1,&hithresh,&lothresh);

    *actual_hthresh = hthresh = nearestint_of_canny(hithresh);

    /* Compute the lower threshold as l_pixel_fraction fraction of the hi threshold */
    /* computed from the histogram */

/*	DEBUG code - JMM
fprintf(stderr,"find_edges: before assigned thelothresh_of_canny is %d\n",thelothresh_of_canny);
*/
    *actual_lthresh = thelothresh_of_canny = nearestint_of_canny(hithresh*lpixel_fraction);
/*	DEBUG code - JMM
fprintf(stderr,"find_edges: thelothresh_of_canny is %d\n",thelothresh_of_canny);
*/

    /* This loop looks for pixels above hthresh and then calls follow_edges on that pixel. */

/*debug    printf("\n follow_edges \n"); */

    for(i = 0, magptr = mag, mapptr = map;i < xsize*ysize; i++, magptr++, mapptr++) 
    {
        if ((*mapptr == POSSIBLE_EDGE) && (*magptr >= hthresh))
        {
            *mapptr = EDGE;
/*	DEBUG code - JMM
fprintf(stderr,"fin_edges:  EDGE FOUND at offset i = %d\n",i);
*/
            follow_edges_of_canny(mapptr,magptr);
        }
    }

    /* Cleanup sets the remaining possible edges to nonedges */ 

/*debug    printf("\n clear borders \n"); */

    cleanup_of_canny(map,xsize,ysize);
}


/* ------------------------------------------------------------------------------- */
/* Cleanup sets the remaining possible edges to nonedges                           */
/* ------------------------------------------------------------------------------- */

static void cleanup_of_canny(map,xsize,ysize)
    unsigned char *map;
    long xsize,ysize;
{
    unsigned char *mapptr;
    long i;

    for(i=0, mapptr = map; i<xsize*ysize; i++, mapptr++)
    {
        if (*mapptr != (unsigned char) EDGE)
            *mapptr= (unsigned char) NOEDGE;
    }
}



/**************************************************************************
	image_utils.c
***************************************************************************/

/* This routine will take an image and return a ptr (in the same block of memory */
/* a pointer to a specified subimage.  cropcorner/ax/ay/bx/by specify opposing corners */
/* of the cropping rectangle.  The new image size is inserted in rows and cols. */

static void crop_image_of_canny(imageptr,cols,rows, cropcornerax, cropcorneray,
				cropcornerbx, cropcornerby)
    unsigned char *imageptr;
    long *rows, *cols, cropcornerax, cropcorneray, cropcornerbx, cropcornerby;
{

    long rowindex, colindex;
    unsigned char *srcrowptr, *destrowptr,*srcptr,*destptr;
    long vstepsrc, vstepdest;
    
    /* Make corner a be the one closest to the origin, and b the opposite. */
    order_of_canny(&cropcornerax,&cropcornerbx);
    order_of_canny(&cropcorneray,&cropcornerby);

    srcrowptr = imageptr + (*cols)*cropcorneray + cropcornerax;
    destrowptr = imageptr;

    vstepsrc = *cols;
    vstepdest = cropcornerbx - cropcornerax;
    
    /* Replace *rows and *cols by the values of the cropped image. */

    *rows = cropcornerby - cropcorneray;
    *cols = vstepdest;

    /* copy the subimage to the low memory portion of the original storage area. */

    for(rowindex = 0; rowindex < *rows; rowindex++, srcrowptr += vstepsrc, destrowptr += vstepdest)
    {
        for(colindex = 0, srcptr = srcrowptr, destptr = destrowptr; colindex < *cols; colindex++, srcptr++, destptr++)
        {
            *destptr = *srcptr;                         

        }
    }
}
          

/* ------------------------------------------------------------------------------------------------------- */
/* This routine computes a histogram of theimage and places it in thehistogram, a 256 element integer      */
/* array.                                                                                                  */
/* ------------------------------------------------------------------------------------------------------- */

static void histogram_of_canny(theimage,xsize,ysize,pixelmax,pixelmin,hgram,histsize)
    short *theimage;
    long xsize,ysize,hgram[],histsize,pixelmax,pixelmin;
{
    long i,index;
    short *iptr;
    float scale;

    order_of_canny(&pixelmin,&pixelmax); 

    scale = ((float) histsize)/(pixelmax - pixelmin + 1);     
                   
    /* Zero the histogram */

/*
#debug    printf("\n histogram: xsize = %d, ysize = %d, max = %d, min = %d,",
#debug            xsize, ysize, pixelmax, pixelmin);
#debug    printf(" scale = %d, histsize = %d\n", scale, histsize);
*/
    for(i=0;i<histsize;i++)
        hgram[i] = 0;

    for(i=0, iptr = theimage; i< xsize*ysize;i++,iptr++) 
    {     
        index = (long) ((*iptr - pixelmin) * scale); 

        hgram[index]++ ;
    }
  
}

/* ------------------------------------------------------------------------------------------------------ */
/* This routine finds a threshold above below which the given fraction of pixels lie                      */
/* ------------------------------------------------------------------------------------------------------ */

static void get_histogram_threshold_of_canny(hgram,histsize,pixelmax,pixelmin,
					     fraction,zflag,ht,lt)
    long hgram[],histsize,pixelmax,pixelmin,zflag;
    float fraction,*ht,*lt;
{
    long i,j, sum, partsum, halfindex,lstart,hstart;


    /* find total area */

    for(i = 0,sum = 0;i < histsize; sum += hgram[i],++i);

    /* Now find thresholds */

    if(zflag)
    {
        /* find the correct index */ 

        for(i=0, partsum=0; partsum < fraction*sum; partsum += hgram[i], i++);

        /* Compute the threshold */

        *ht = ((((float) (pixelmax - pixelmin))/histsize) * i)  + pixelmin; 
        *lt = 0.0;
    }
    else
    {   
        /* Find halfway index */
        for(halfindex = 0,partsum = 0;partsum < .5*sum; partsum += hgram[halfindex],halfindex++);

        halfindex--;

        lstart = (long) ((sum/2.0) - ((float)partsum) + hgram[halfindex] + 1.0);
        hstart = hgram[halfindex] - lstart - 1;



        /* now go down then up to find the required fraction */
        for(i = halfindex - 1, partsum = lstart;(partsum < .5*fraction*sum) && (i >= 0);partsum += hgram[i],i--);
        for(j = halfindex + 1, partsum = hstart;(partsum < .5*fraction*sum) && (j < histsize);partsum += hgram[j],j++);

        *lt = ((((float) (pixelmax - pixelmin))/histsize) * (i + 1))  + pixelmin; 
        *ht = ((((float) (pixelmax - pixelmin))/histsize) * (j - 1))  + pixelmin;
    }
}


/****************************************************************************
	thin.c
*****************************************************************************/

#define U	010
#define E	020
#define I	040
#define MM	0100
#define M	0200
#define	NMM	200
#define absval_of_canny(f) (((f)<0)?(-(f)):(f))

static int flagpass_of_canny[4] = {0,0,0,0};
static int flags_of_canny = 0;
static int flagv_of_canny = 0;
static int rinc_of_canny[10] = {0,0,-1,-1,-1,0,1,1,1,0};
static int cinc_of_canny[10] = {0,1,1,0,-1,-1,-1,0,1,1};
static int mrow_of_canny[NMM],mcol_of_canny[NMM],mlabel_of_canny[NMM],
  maxmult_of_canny,maxlabel_of_canny;
static int row_of_canny,col_of_canny;
static char *pic_of_canny, *pptr_of_canny;

static void thin_of_canny(edges,height,width)
     int height,width;
     unsigned char *edges;

{int frameno,r,c,change,trans,neigh,pass,first,gap,gapopen;
int i,j;
int mini,sumr,sumc,label,npt,Mfound;
float avgr,avgc,mindist,dist;
char *ppic;
char *temp;
unsigned char *temp2;

flagpass_of_canny[0]=1;
flagpass_of_canny[1]=1;
row_of_canny = height;
col_of_canny = width;
pic_of_canny = pptr_of_canny = (char * ) malloc(row_of_canny*col_of_canny*sizeof (char));

flagv_of_canny=0;   /* can be set to 1 for debugging */
frameno=0;

temp2 = edges;
temp=pic_of_canny;
for (i=0; i<height * width; i++)
  {*temp = (char)*temp2;
   temp++;
   temp2++;
 };

    for (pass=0;pass<4;pass++)
	if (flagpass_of_canny[pass]) {
	    
	    if (flagv_of_canny)
		fprintf(stderr,"thin: frame %d pass %d\n",frameno,pass);
	    do {
		maxlabel_of_canny = maxmult_of_canny = change = 0;

		ppic = pic_of_canny;

		/*  neighborhood:  432
				   501
			   678
	    	and 9 = 1 for modular arithmetic */

	    for (r=0;r<row_of_canny;r++) {
			for (c=0;c<col_of_canny;c++) {
			if ((*ppic&0377)!=0) {
			    switch(pass) {

			    case 0:
			    case 2:
				neigh = trans = 0;
				for (i=1;i<=8;i++) {
				  if (neighbor_of_canny(r,c,i)!=0)
						neigh++;
					if (((neighbor_of_canny(r,c,i)==0) ^
					    (neighbor_of_canny(r,c,i+1)==0)) != 0)
						trans++;
				}
				if (pass==0) {
				    if (trans==2 && neigh>2 && neigh<6) {
					*ppic++ = 0;
					change++;
				    }
				    else
					ppic++;
				}
				else {   /* pass 2 */
				    if (trans == 2 && neigh < 3)
					*ppic++ = E;
				    else if (neigh == 0)
					*ppic++ = I;
				    else if (trans >= 6)
					*ppic++ = M;
				    else if (neigh > 2)
					*ppic++ = MM;
				
				    else
					*ppic++ = U;
				  }
				continue;

			    case 1:
				neigh = trans = 0;
				for (i=1;i<=8;i++) {
					if (neighbor_of_canny(r,c,i)) {
						neigh++;
						first = i;
					}
					if (((neighbor_of_canny(r,c,i)==0) ^
					    (neighbor_of_canny(r,c,i+1)==0)) != 0)
						trans++;
				}
				if (neigh<2 || neigh>6 || 
				    (neigh==2 && trans<4) ||
				    (neigh==3 && trans>4)) {
					ppic++;
					continue;
				}
				gap = gapopen = 0;
				for (i=first+1;i<first+8;i++) {
				    j = ((i - 1) % 8) + 1;
				    if (gapopen) {
					if (neighbor_of_canny(r,c,j))
						gapopen = 0;
				    }
				    else {
					if (j%2==1 && neighbor_of_canny(r,c,j)==0) {
						gapopen++;
						gap++;
					}
				    }
				}
				if (gap == 1) {
					*ppic++ = 0;
					change++;
				}
				else
				  ppic++;
				continue;

			    case 3:
				if ((*ppic&0377)==MM || (*ppic&0377)==M) {
				    ppic++;
				    first = 0;
				    for (i=1;i<=8;i++) {
					if (neighbor_of_canny(r,c,i)==MM ||
					    neighbor_of_canny(r,c,i)==M) {
					    if (first++ == 0)
						addmult_of_canny(r,c,++maxlabel_of_canny);
					    addmult_of_canny(r+rinc_of_canny[i],
						c+cinc_of_canny[i],maxlabel_of_canny);
					}
				    }
				}
				else
					ppic++;
				continue;

			    default: 
				fprintf(stderr,"unknown pass!");
			    }
			}
			else
				ppic++;
	        }
	    }

	    if (pass == 3) {
		while (1) {
		    label = 0;
		    for (i=0;i<maxmult_of_canny;i++)
			if ((label = mlabel_of_canny[i]) != 0)
			    break;
		    if (label == 0)
			break;
		    Mfound = sumr = sumc = npt = 0;
		    for (i=0;i<maxmult_of_canny;i++)
			if (mlabel_of_canny[i] == label) {
			    sumr += mrow_of_canny[i];
			    sumc += mcol_of_canny[i];
			    if ((*(pic_of_canny + mrow_of_canny[i]*col_of_canny
				   +mcol_of_canny[i])&0377) == M)
				Mfound++;
			    npt++;
			}
		    if (Mfound) {
			for (i=0;i<maxmult_of_canny;i++) {
				if (mlabel_of_canny[i] == label)
					mlabel_of_canny[i] = 0;
			}
			continue;
		    }
		    if (npt < 2)
			fprintf(stderr,"only one M point!??");
		    avgr = ((float) sumr)/npt;
		    avgc = ((float) sumc)/npt;
		    mindist = 10000000.;
		    for (i=0;i<maxmult_of_canny;i++) {
			if (mlabel_of_canny[i] == label) {
			    dist = absval_of_canny(avgr-mrow_of_canny[i]) +
				absval_of_canny(avgc-mcol_of_canny[i]);
				    if (dist<mindist) {
				mindist = dist;
				mini = i;
			    }
			}
		    }
		    for (i=0;i<maxmult_of_canny;i++) {
			if (mlabel_of_canny[i]==label) {
			    if (i == mini) {
				*(pic_of_canny + mrow_of_canny[i]*col_of_canny +
				  mcol_of_canny[i]) = M;
				change++;
			    }
			    mlabel_of_canny[i] = 0;
			}
		    }
		}
	    }
		if (flagv_of_canny) fprintf(stderr,"thin: frame %d reversals = %d\n",frameno,change);
	    } while (change!=0 && flags_of_canny ==0 && (pass==0 || pass==1));
	}
	temp2 = edges;
	for (i=0; i<height * width; i++)
	  {*temp2 = (unsigned) *pic_of_canny;
 	  pic_of_canny++;
   	  temp2++;
 	};
 	free(pptr_of_canny);
}
/********************************************************************************************/

static void addmult_of_canny(row_of_canny,col_of_canny,label)

     int row_of_canny,col_of_canny,label;

{
	int i,oldlabel;

	oldlabel = 0;
	for (i=0;i<maxmult_of_canny;i++)
		if (mrow_of_canny[i]==row_of_canny && mcol_of_canny[i]==col_of_canny)
			oldlabel = mlabel_of_canny[i];
	if (oldlabel) {
		for (i=0;i<maxmult_of_canny;i++)
			if (mlabel_of_canny[i] == oldlabel)
				mlabel_of_canny[i] = label;
	}
	else {
		if (maxmult_of_canny >= NMM)
			fprintf(stderr,"M point overflow");
		mrow_of_canny[maxmult_of_canny] = row_of_canny;
		mcol_of_canny[maxmult_of_canny] = col_of_canny;
		mlabel_of_canny[maxmult_of_canny++] = label;
	}
}

/**************************************************************************************/

static char neighbor_of_canny(r,c,d)

     int r,c,d;

{
	int i,j;

	i = r + rinc_of_canny[d];
	j = c + cinc_of_canny[d];
	if (i<0 || i>=row_of_canny || j<0 || j>=col_of_canny)
		return(0);
	else
		return(pic_of_canny[i*col_of_canny+j]&0377);
}


