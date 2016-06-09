/*****************************************************************************
 rowFlow.h -- Optical Flow interface.    S.R.Lines    93.6.27-9.9
 Wrap images up in pyramids and call coarse_fine (lkflowlib.c) to
 get optical flow fields.
#include <unTypes.h>
 *****************************************************************************/
#include <sarnoff/imgdec.h>

void
gaussFlow ( ppf floX, ppf floY, ppf cnfd, ppf srcA, ppf srcB
          , int fX, int fY, unt fW, unt fH
          , int botlev, int finlev, int toplev);

void
newFlowBac ( ppf *floX, ppf *floY, ppf *conf, ppf srcA, ppf srcB
           , int sX, int sY, unt sW, unt sH     
           , int fX, int fY, unt fW, unt fH
           , int toplev, int botlev, int flags);

void
fxyFlowBac ( ppf abX, ppf abY, ppf dwT, ppf sgA, ppf sgB
           , int fX, int fY, unt fW, unt fH
           , int sX, int sY, unt sW, unt sH, dbl *stdX, dbl *stdY
	   , int pyrLevs);

void
rowFlowBac ( ppf floX, ppf floY, ppf conf, ppf srcA, ppf srcB
           , int sX, int sY, unt sW, unt sH
           , int fX, int fY, unt fW, unt fH
           , int toplev, int botlev, int flags);


void rowFlowFwd ( ppf floX, ppf floY, ppf conf, ppf srcA, ppf srcB
                , unt wide, unt tall, int toplev, int botlev, int flags);
void rowFlowBak ( ppf floX, ppf floY, ppf conf, ppf srcA, ppf srcB
                , unt wide, unt tall, int toplev, int botlev, int flags);

void makePyrF(FPYR pyr, int botlev, int toplev, int w, int h, ppf ptr);


#ifndef LAPLACIAN
#define LAPLACIAN 1
#endif


