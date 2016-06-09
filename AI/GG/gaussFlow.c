/**************************************************************
  Lucas-Kanade flow, but with Gaussian pyramids instead of 
   laplacians or cannies
  - written by Tony Ezzat 4/28/95 
#include "imgdecTone.h"
#include "mot.h"
#include "fimgop.h"
 **************************************************************/
#include <sarnoff/imgdec.h>
#include "boolean.h"
#include <un/unBug.h>
#include <un/unTypes.h>
#include <m/rowMake.h>
#include "rowFlow.h"
//extern int  getpyrfilters(char *filename, FILTER *rf, FILTER *ef);
//extern void reduce(FPYR pyr, int botlev, int toplev, FILTER rf);
//extern void expand(FPYR pyr, int botlev, int toplev, FILTER ef, int mode);
//extern void free_fimage(FIMAGE img);
//extern void fset(FIMAGE image, double c);
//extern void free_fpyr(FPYR pyr, int botlev, int toplev);
//extern void alloc_fimage(FIMAGE *image);
//extern void bilinear_warp (FIMAGE in, FIMAGE out, FIMAGE vx, FIMAGE vy, double scale);

void coarseFineGauss ( FPYR pyr1, FPYR pyr2, FPYR dx, FPYR dy, FPYR pyrC
                , int botlev, int finlev, int toplev);



/*#define DEBUG */

extern int Image_border;
extern FILTER  rf, ef;        


#define BEFORE		/* smoothing of residual velocity */
#define NBLUR	1	/* Number of smoothing levels */
#define LIMIT		/* limit residual velocities (only gradient method) */
#define EXTEND


#if 00
void findflowFwdGauss ( FIMAGE imgA, FIMAGE imgB, FIMAGE *dx_out, FIMAGE *dy_out
			 , int botlev, int finlev, int toplev)
{
  getpyrfilters(DEFAULT_PYR_FILTER,&rf,&ef);
  gaussFlow(imgB,imgA,dx_out,dy_out,botlev,finlev,toplev);
}
#endif



void
gaussFlow ( ppf floX, ppf floY, ppf cnfd, ppf srcA, ppf srcB
          , int fX, int fY, unt fW, unt fH
          , int botlev, int finlev, int toplev)
{
static int first = 0;
/* FIMAGE img1,img2; */
   FPYR pyrA, pyrB, pyrX, pyrY, pyrC;
   /* char name[80]; */

ppf     dupA = rowDupFlt(srcA,fX,fY,fW,fH);
ppf     dupB = rowDupFlt(srcB,fX,fY,fW,fH);

  if (!first++)  wrn("gaussFlow: botlev %d,  finlev %d, toplev %d"
	 ,botlev,finlev,toplev);

   makePyrF(pyrA,botlev  ,toplev+1,fW,fH,dupA);
   makePyrF(pyrB,botlev  ,toplev+1,fW,fH,dupB);
   makePyrF(pyrX,finlev  ,toplev  ,fW,fH,floX);   /* for flows, botlev = finlev = 0 */
   makePyrF(pyrY,finlev  ,toplev  ,fW,fH,floY);
   makePyrF(pyrC,finlev  ,toplev  ,fW,fH,cnfd);


#if 00

/* First copy imgA imgB because coarse_fine modifies its images */
   alloc_fimage_like(&img1,imgA);       fcopy_img(imgA,img1);
   alloc_fimage_like(&img2,imgB);       fcopy_img(imgB,img2);

/* Set up pyramids for coarse to fine estimation */

   def_fpyr(pyrA,botlev,img1.w,img1.h,img1.t);
   alloc_fpyr(pyrA,botlev+1,toplev+1);
   pyrA[botlev].ptr = img1.ptr;
 
   def_fpyr(pyrB,botlev,img2.w,img2.h,img1.t);
   alloc_fpyr(pyrB,botlev+1,toplev+1);
   pyrB[botlev].ptr = img2.ptr;

   def_fpyr(pyrX,botlev,img1.w,img1.h,img1.t);
   alloc_fpyr(pyrX,finlev,toplev);

   def_fpyr(pyrY,botlev,img1.w,img1.h,img1.t);
   alloc_fpyr(pyrY,finlev,toplev);

#endif
 
/* Coarse to fine motion estimation */

   coarseFineGauss(pyrA,pyrB,pyrX,pyrY,pyrC,botlev,finlev,toplev);

#if 00

/* Set up the vector fields before returning */
   alloc_fimage_like(dx_out,img1);
   fcopy_img(pyrX[finlev],*dx_out);
   alloc_fimage_like(dy_out,img1);
   fcopy_img(pyrY[finlev],*dy_out);

#endif
   

/* Deallocate Resources */

   free_fpyr(pyrA,botlev+1,toplev+1);
   free_fpyr(pyrB,botlev+1,toplev+1);
   free_fpyr(pyrX,finlev+1,toplev);
   free_fpyr(pyrY,finlev+1,toplev);

#if 00
   free_fimage(img1);
   free_fimage(img2);
#endif
}



void
coarseFineGauss ( FPYR pyr1, FPYR pyr2, FPYR dx, FPYR dy, FPYR pyrC
		, int botlev, int finlev, int toplev)
{
  FIMAGE warped;
  int lev;
  extern FILTER rf,ef;

  /* Construct pyramids */
  
  reduce(pyr1,botlev,toplev+1,rf);
  reduce(pyr2,botlev,toplev+1,rf);

/*
  for (lev=toplev; lev >= botlev; lev--) {
    sprintf_s(fname,"pyr1.%d.sgi",lev);
    write_fimage_sgi(fname,pyr1[lev]);

    sprintf_s(fname,"pyr2.%d.sgi",lev);
    write_fimage_sgi(fname,pyr2[lev]);
  }
*/

/* Start coarse to fine motion estimation */

#if 00
   for (lev=toplev; lev >= finlev; lev--) {
#else
   /* for (lev = toplev; lev >  finlev; lev--) { */
   for (lev = toplev; lev >= finlev; lev--) {
#endif

#ifdef DEBUG
     fprintf(stdout,"Estimating velocities on level %d\n",lev); fflush(stdout);
#endif

     if (lev != toplev) 
       {	
	 fmultiply(dx[lev+1],2.0);
	 fmultiply(dy[lev+1],2.0);
	 expand(dx,lev,lev+1,ef,0);
	 expand(dy,lev,lev+1,ef,0);
       } 
     else 
       {
	 fset(dx[lev],0.0);
	 fset(dy[lev],0.0);
       }

#if 00
     alloc_fimage_like(&warped,pyr1[lev]);
     warp_bkwd_bilinear(pyr1[lev],warped,dx[lev],dy[lev],1.0);
#else
     warped.w = pyr1[lev].w; warped.h = pyr1[lev].h; alloc_fimage(&warped);
     bilinear_warp(pyr1[lev],warped,dx[lev],dy[lev],-1.0);
#endif

     lsq_lsqgrad_fast(warped,pyr2[lev],dx[lev],dy[lev], pyrC[lev] );

#ifdef DEBUG
     sprintf_s(name,"dx%d.float",lev);
     write_fimage(name,dx[lev]);
     sprintf_s(name,"dy%d.float",lev);
     write_fimage(name,dy[lev]);
#endif


#ifdef BEFORE
     /* Blur velocity fields */
     blur(dx[lev],NBLUR);
     blur(dy[lev],NBLUR); 
#endif
     
     /* add in previous estimates to current */
     if (lev != toplev) {	
       expand(dx,lev,lev+1,ef,1);
       expand(dy,lev,lev+1,ef,1);
     }

#ifdef AFTER
/* Blur velocity fields */
     blur(dx[lev],NBLUR);
     blur(dy[lev],NBLUR);
#endif

     free_fimage(warped);
   }

}













