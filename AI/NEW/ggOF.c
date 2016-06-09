/*****************************************************************************
 ggOF.c -- Optical Flow interface.    S.R.Lines    93.6.27-7.3
 *****************************************************************************/
#include	<stdio.h>
#include	<stdlib.h>
#include	"sarnoff.h"
#include	"ggOF.h"

#define Err(ss,ii) {fprintf(stderr,"Err: %s %d\n",ss,ii);exit(-1);}

extern	FILTER rf,ef;
#ifdef	MAC
#undef	DEFAULT_PYR_FILTER
#define DEFAULT_PYR_FILTER "pyrg.fir"
#else
#ifndef DEFAULT_PYR_FILTER
#define DEFAULT_PYR_FILTER "/home/vu/spraxlo/c/rha/src/pyrg.fir"
#endif
#endif

/*  new_lsqcf: Deals only with sarnoff fimg's, no Beymer images of type IMG_FLOAT.
    dx,dy should be the flow in fimg2's frame from fimg2 back to fimg1;
    cf (confidences) are ignored.
*/
void
new_lsqcf (FIMAGE *dx_arg, FIMAGE *dy_arg, FIMAGE *cf_arg
          ,FIMAGE *fimg1,  FIMAGE *fimg2, int toplev, int botlev)
{
  FPYR	pyr1,pyr2,dx,dy,cf;
  int	finlev=0, wide, tall;
  char	*filtfile = DEFAULT_PYR_FILTER;
   
#ifdef VERBOSE
  printf("%s mot_lsqcf: DEFAULT_PYR_FILTER %s\n",__FILE__,filtfile);
#endif
  getpyrfilters(filtfile,&rf,&ef);

  if ((wide=fimg1->w) != fimg2->w || (tall=fimg1->h) != fimg2->h)
    Err("mot_lsqcf: image size mismatch",wide*tall);
  
  /********************************************************
  print_fimage(dx_arg, "dx_arg at beginning of new_lsqcf");
  print_fimage(dy_arg, "dy_arg at beginning of new_lsqcf");
  print_fimage(fimg1,  "fimg1, at beginning of new_lsqcf");
  print_fimage(fimg2,  "fimg2, at beginning of new_lsqcf");
  ********************************************************/

/* Set up pyramids for coarse to fine estimation */
   def_fpyr(pyr1,botlev,wide,tall);
   pyr1[botlev].ptr = fimg1->ptr;

   def_fpyr(pyr2,botlev,wide,tall);
   pyr2[botlev].ptr = fimg2->ptr;

#ifdef LAPLACIAN
   alloc_fpyr(pyr1,botlev+1,toplev+1);
   alloc_fpyr(pyr2,botlev+1,toplev+1);
#else
   alloc_fpyr(pyr1,botlev+1,toplev);
   alloc_fpyr(pyr2,botlev+1,toplev);
#endif
   def_fpyr  (dx,botlev,wide,tall);
   dx[botlev].ptr = dx_arg->ptr;
   alloc_fpyr(dx,finlev+1,toplev);

   def_fpyr  (dy,botlev,wide,tall);
   dy[botlev].ptr = dy_arg->ptr;
   alloc_fpyr(dy,finlev+1,toplev);

   def_fpyr  (cf,botlev,wide,tall);
   cf[botlev].ptr = cf_arg->ptr;
   alloc_fpyr(cf,finlev+1,toplev);
   
   coarse_fine(pyr1,pyr2,dx,dy,cf,botlev,finlev,toplev); /* lkflowlib.c */

#ifndef UNBUG
#define UNBUG 0
#endif
#if	UNBUG > 6
  { int p,q,z;
    fprintf(stderr,"DX AFTER COARSE_FINE:::::::::::::::::::::::::::\n");
    for (z = 0; z < 25; z++) {
      p = rand() % tall;
      q = rand() % wide;
      fprintf(stderr,"dx_arg->ptr[%d][%d]=%.2g    ",p,q,dx_arg->ptr[p][q]);
    }
    fprintf(stderr,"\n");
  }
#endif

#ifdef LAPLACIAN
   free_fpyr(pyr1,botlev+1,toplev+1);	/* don't free bottom level, cuz */
   free_fpyr(pyr2,botlev+1,toplev+1);   /* the bottom level is the fimg */
#else
   free_fpyr(pyr1,botlev+1,toplev);
   free_fpyr(pyr2,botlev+1,toplev);
#endif
   free_fpyr(dx,finlev+1,toplev);	/* don't free fine level (0) */
   free_fpyr(dy,finlev+1,toplev);
   free_fpyr(cf,finlev+1,toplev);
}



    /***************************
    sarnoff_f2image(dstA,dx[j]);
    write_pix(dstA,"hand");
    xx("byebye",0);
    ****************************/


