/*****************************************************************************
 ggOF.h -- Optical Flow interface.    S.R.Lines    93.6.29
 *****************************************************************************/

void
new_lsqcf (FIMAGE *dx_arg, FIMAGE *dy_arg, FIMAGE *cf_arg
          ,FIMAGE *fimg1,  FIMAGE *fimg2, int toplev, int botlev);

#ifndef LAPLACIAN
#define LAPLACIAN 1
#endif

#ifndef VERBOSE
#define VERBOSE 0
#endif


