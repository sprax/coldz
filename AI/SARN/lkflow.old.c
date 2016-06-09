/**************************************************************
 Lucas-Kanade optical flow algorithm: Amnon Shashua, March 1993
 From RH.
 File name chaned to lkflowlib and global defs made into extern
 decls by spraxlo.
 **************************************************************/

#include "sarnoff/imgdec.h"
#include "sarnoff/imgmacro.h"
#include <string.h>

#define LAPLACIAN
/* #define DEBUG */

/* img1, img2 are input images. dx,dy are the flow components returned as
output.
*/

extern int Image_border;
extern FILTER rf,ef;


#define BEFORE		/* smoothing of residual velocity */
#define NBLUR	1	/* Number of smoothing levels */
#define LIMIT		/* limit residual velocities (only gradient method) */
#define EXTEND


void
coarse_fine ( FPYR pyr1, FPYR pyr2, FPYR dx, FPYR dy
            , FPYR conf, int botlev, int finlev, int toplev)
{
   FIMAGE warped;
   int lev;
   extern FILTER rf,ef;

/* Construct pyramids */
   reduce(pyr1,botlev,toplev+1,rf);
   reduce(pyr2,botlev,toplev+1,rf);
   expand(pyr1,botlev,toplev+1,ef,-1);
   expand(pyr2,botlev,toplev+1,ef,-1);


/* Start coarse to fine motion estimation */
   for (lev=toplev; lev >= finlev; lev--) {
#ifdef DEBUG
     fprintf(stderr,"Estimating velocities on level %d\n",lev);
#endif
     if (lev != toplev) {	/* expand previous levels velocity estimate */
       fmultiply(dx[lev+1],2.0);
       fmultiply(dy[lev+1],2.0);
       expand(dx,lev,lev+1,ef,0);
       expand(dy,lev,lev+1,ef,0);
     } else {
       fset(dx[lev],0.0);
       fset(dy[lev],0.0);
     }
     warped.w = pyr1[lev].w; warped.h = pyr1[lev].h; alloc_fimage(&warped);
     bilinear_warp(pyr1[lev],warped,dx[lev],dy[lev],-1.0);
     lsq_lsqgrad_fast(warped,pyr2[lev],dx[lev],dy[lev],conf[lev]);
#ifdef BEFORE
/* Blur velocity fields */
     blur(dx[lev],NBLUR);
     blur(dy[lev],NBLUR); 
#endif
     if (lev != toplev) {	/* add in previous estimates to current */
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


void
coarse_fine_mask(pyr1,pyr2,mask,dx,dy,conf,botlev,finlev,toplev)
FPYR pyr1,pyr2,mask,dx,dy,conf;
int botlev,finlev,toplev;
{
   FIMAGE warped;
   int lev;
   extern FILTER rf,ef;

/* Construct pyramids */
   reduce(pyr1,botlev,toplev+1,rf);
   reduce(pyr2,botlev,toplev+1,rf);
   reduce(mask,botlev,toplev,rf);
   expand(pyr1,botlev,toplev+1,ef,-1);
   expand(pyr2,botlev,toplev+1,ef,-1);


/* Start coarse to fine motion estimation */
   for (lev=toplev; lev >= finlev; lev--) {
#ifdef DEBUG
     fprintf(stderr,"Estimating velocities on level %d\n",lev);
#endif
     if (lev != toplev) {	/* expand previous levels velocity estimate */
       fmultiply(dx[lev+1],2.0);
       fmultiply(dy[lev+1],2.0);
       expand(dx,lev,lev+1,ef,0);
       expand(dy,lev,lev+1,ef,0);
     } else {
       fset(dx[lev],0.0);
       fset(dy[lev],0.0);
     }
     warped.w = pyr1[lev].w; warped.h = pyr1[lev].h; alloc_fimage(&warped);
     bilinear_warp(pyr1[lev],warped,dx[lev],dy[lev],-1.0);
     lsq_lsqgrad_fast_mask(warped,pyr2[lev],mask[lev],
			   dx[lev],dy[lev],conf[lev]);
#ifdef BEFORE
/* Blur velocity fields */
     blur(dx[lev],NBLUR);
     blur(dy[lev],NBLUR); 
#endif
     if (lev != toplev) {	/* add in previous estimates to current */
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

void
lkflow ( FIMAGE img1, FIMAGE img2, FIMAGE *dx_out, FIMAGE *dy_out
       , int botlev, int finlev, int toplev)
{

   char filtfile[MAX_STR_LEN];
   FPYR pyr1,pyr2,conf,dx,dy;
/*    int botlev=0,toplev=4,lev,finlev=0;  */

   strcpy(filtfile,DEFAULT_PYR_FILTER);	/* default filter */
   getpyrfilters(filtfile,&rf,&ef);


/* Set up pyramids for coarse to fine estimation */

   def_fpyr(pyr1,botlev,img1.w,img1.h);
   alloc_fpyr(pyr1,botlev+1,toplev+1);
   pyr1[botlev].ptr = img1.ptr;
   def_fpyr(pyr2,botlev,img2.w,img2.h);
   alloc_fpyr(pyr2,botlev+1,toplev+1);
   pyr2[botlev].ptr = img2.ptr;
   def_fpyr(dx,botlev,img1.w,img1.h);
   alloc_fpyr(dx,finlev,toplev);
   def_fpyr(dy,botlev,img1.w,img1.h);
   alloc_fpyr(dy,finlev,toplev);
   def_fpyr(conf,botlev,img1.w,img1.h);
   alloc_fpyr(conf,finlev,toplev);

/* coarse to fine motion estimation */
   coarse_fine(pyr1,pyr2,dx,dy,conf,botlev,finlev,toplev);

   free_fpyr(pyr1,botlev+1,toplev+1);
   free_fpyr(pyr2,botlev+1,toplev+1);
   free_fpyr(dx,finlev+1,toplev);
   free_fpyr(dy,finlev+1,toplev);
   free_fpyr(conf,finlev,toplev);

   dx_out->ptr = dx[0].ptr;
   dy_out->ptr = dy[0].ptr;
}





