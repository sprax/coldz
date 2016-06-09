/******************************************************************************
 lkflowbac.h    S.R.Lines (spraxlo@ai.mit.edu)   93.9.16
 FORWARD Lucas-Kanade optical flow algorithm,
 based on lkflow.c by Amnon Shashua, MIT, March 1993,
 after Rajesh Hingorani, RCA Sarnoff Labs.
#define	DEBUG
 *****************************************************************************/
#ifndef 	LAPLACIAN
#define 	LAPLACIAN 88888
#endif

void
lkflow_bac ( FIMAGE img1, FIMAGE img2, FIMAGE *dx_out, FIMAGE *dy_out
           , int botlev, int finlev, int toplev);

void
coarse_fine_bac ( FPYR pyr1, FPYR pyr2, FPYR dx, FPYR dy
                , FPYR conf, int botlev, int finlev, int toplev);

void
coarse_fine_mask_bac ( FPYR pyr1, FPYR pyr2, FPYR mask, FPYR dx, FPYR dy, FPYR conf
                     , int botlev, int finlev, int toplev);



