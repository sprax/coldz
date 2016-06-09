/*****************************************************************************
 inGrid.c    S.R.Lines    93.8.17-9.21
 *****************************************************************************/
#include        <stdio.h>
#include        <stdlib.h>
#include        <string.h>
#include        <math.h>
#include        <un/unBug.h>
#include        <un/unTypes.h>
#include        <un/unFlags.h>
#include        <un/unMacro.h>
#include        <gl/gl.h>
#include        <gl/device.h>
#include	<m/mj.h>
#include	<m/rowNwts.h>
#include	<m/rowPclr.h>
#include	<m/rowStat.h>
#include	<m/rowZero.h>
#include	<winLoop.h>
#include	<winDraw.h>
#include	"mob.h"
#include	"blerp.h"

int 		GrdX = GTX, GrdY = GTY;      /* should be reset by makeGridTet */
static	int	Km = (GTX - 1), Jm = (GTY - 1), gridMade = 0;
#define MINAREA 16.0
#define rv0	12

extern	int     MoveGridA, MoveGridB, ResetGrid;
extern	int     UseFlowAB;
extern	long	InShowWid;
extern	long    InEditIndex;

void
drawGridTet1(pMD wwD, unt flags)
{
    pMOB	mB = wwD->mob[InEditIndex];
    int     j, k;
    GridV4  *gv;
    long 	wid = winget();

    WdrawP(wwD->dWin);	/* Wdraw does the winset */
    drawmode(OVERDRAW);
    if (flags & 2) {
        for (j = 1; j <= GrdY; j++) {
            for (k = 1; k <= GrdX; k++) {
                bgnline();
                gv = &mB->grdB[j][k];
                color(gv->clr);
                if (j < GrdY) v2f(mB->grdB[j][k - 1].xy);
                v2f(gv->xy);
                if (k < GrdX) v2f(mB->grdB[j - 1][k].xy);
                endline();
            }
        }
    }
    else {
        for (j = 1; j <= GrdY; j++) {
            for (k = 1; k <= GrdX; k++) {
                bgnline();
                gv = &mB->grdA[j][k];
                color(gv->clr);
                if (j < GrdY) v2f(mB->grdA[j][k - 1].xy);
                v2f(gv->xy);
                if (k < GrdX) v2f(mB->grdA[j - 1][k].xy);
                endline();
            }
        }
    }
    /* popmatrix(); */
    drawmode(NORMALDRAW);
    winset(wid);
}


void
drawGridTet2(Wimj *v, int device, int flags)
{
    pWmj    wA = (pWmj)v, wB = wA->next;
    WmjAct *ap = wA->act;
    pMD     wwD = (pMD)ap->data;
    int 	nA = ap->nA;
    int     nB = ap->nB;  pMOB  mB = wwD->mob[nB];
    /* pDmj    D   = &wA->D; int dX = D->x, dY = D->y, dW = D->w, dH = D->h; */
    int     j, k;
    GridV4  *gv;
    long 	widA, widB, wid = winget();

    /* prn("drawGridTet2: wA->id: %d,  wA->in: %d,  wA->next->id: %d"
      ,wA->id,wA->in,wA->next->id); */

    if (wA->in != nA) { SWAP(wA, wB, v); }
    widA = wA->id;
    widB = wB->id;

    WdrawP(wA);	/* Wdraw does the winset(wA->id); */
    /* pushmatrix(); scale(2.0, 2.0, 0.0); */
    drawmode(OVERDRAW);
    for (j = 1; j <= GrdY; j++) {
        for (k = 1; k <= GrdX; k++) {
            bgnline();
            gv = &mB->grdA[j][k];
            color(gv->clr);
            if (j < GrdY) v2f(mB->grdA[j][k - 1].xy);
            v2f(gv->xy);
            if (k < GrdX) v2f(mB->grdA[j - 1][k].xy);
            endline();
        }
    }
    /* popmatrix(); */
    drawmode(NORMALDRAW);

    WdrawP(wB);
    /* pushmatrix(); scale(2.0, 2.0, 0.0); */
    drawmode(OVERDRAW);
    for (j = 1; j <= GrdY; j++) {
        for (k = 1; k <= GrdX; k++) {
            bgnline();
            gv = &mB->grdB[j][k];
            color(gv->clr);
            if (j < GrdY) v2f(mB->grdB[j][k - 1].xy);
            v2f(gv->xy);
            if (k < GrdX) v2f(mB->grdB[j - 1][k].xy);
            endline();
        }
    }
    /* popmatrix(); */
    drawmode(NORMALDRAW);
    winset(wid);
}



void
surfGridWLS(Wimj *v, int device, int flags)
{
    pWmj    tW = (pWmj)v;
    WmjAct *ap = tW->act;
    pMD     wwD = (pMD)ap->data;
    /* int     nA  =  ap->nA; */
    int     nB = ap->nB; pMOB    mB = wwD->mob[nB];
    ppf     wts = wwD->wts;
    pDmj    fD = wwD->fD;  int fX = fD->x, fY = fD->y, fW = fD->w, fH = fD->h;
    ppf    *bvX = wwD->bvX;	/* *bvY = wwD->bvY; */
    /*
    pDmj    sD  = wwD->sD;  int sX = sD->x, sY = sD->y, sW = sD->w, sH = sD->h;
    ppf    *fvX = wwD->fvX, *fvY = wwD->fvY;
    */
    flt     vwts[GTN], xcrd[GTN], ycrd[GTN], xval[GTN], yval[GTN], recB[4], tetB[4][2];
    int     j, k, m, n, q;
    int     tX, tY, bX, bY, bW, bH, mX, mY, mW, mH;
    flt     *xyA, *xyB, rX = 1.0 / tW->zX, rY = 1.0 / tW->zY, xA, yA, xB, yB;
    dbl	areaB;
    long    winid = winget();

    /* wrn("BEG surfGridWLS:  nA: %d   nB: %d ", nA, nB); */

    rowZeroF(FeG[0], fX, fY, fW, fH);

    for (j = 2; j < Jm - 1; j++) {
        for (k = 2; k < Km - 1; k++) {

            mX = mY = 16384, tX = tY = -16384, q = 0;

            for (m = -2; m < 4; m++) {
                for (n = -2; n < 4; q++, n++) {
                    xyA = mB->grdA[j + m][k + n].xy;  xA = xyA[0] * rX;  yA = xyA[1] * rY;
                    xyB = mB->grdB[j + m][k + n].xy;  xB = xyB[0] * rX;  yB = xyB[1] * rY;
                    xcrd[q] = xB;   xval[q] = xB - xA;
                    ycrd[q] = yB;   yval[q] = yB - yA;

                    if (m == 0) {
                        if (n == 0) {
                            tetB[0][0] = xB, tetB[0][1] = yB;
                            mX = MIN(xA, xB);
                            mY = MIN(yA, yB);
                        }
                        else if (n == 1)	tetB[1][0] = xB, tetB[1][1] = yB;
                    }
                    else if (m == 1) {
                        if (n == 1) {
                            tetB[2][0] = xB, tetB[2][1] = yB;
                            mW = MAX(xA, xB) - mX;
                            mH = MAX(yA, yB) - mY;
                        }
                        else if (n == 0)	tetB[3][0] = xB, tetB[3][1] = yB;
                    }

                    /*
                    if (xA < xB) { if (xA < mX)  mX = xA; if (xB > tX)  tX = xB; }
                    else         { if (xB < mX)  mX = xB; if (xA > tX)  tX = xA; }
                    if (yA < yB) { if (yA < mY)  mY = yA; if (yB > tY)  tY = yB; }
                    else         { if (yB < mY)  mY = yB; if (yA > tY)  tY = yA; }
                    */
                }
            }
            areaB = rectBoundsFromPoly(recB, tetB, 4);
            /*
            if (areaB < MINAREA) wrn("surfGridWLS %d %d: area B %.2f %.2f very small",j,k,areaB);
            */
            bX = recB[0];   bY = recB[1];  bW = recB[2] - bX;  bH = recB[3] - bY;

            /* if (j == k)  prn("surfGridWLS: calling surfWLSO for jk: %3d %3d   (%d)", j,k, ++z); */
#if 1
            surfWLSO(FeX[0], wts, vwts, xcrd, ycrd, xval, bX, bY, bW, bH, 36, 4, 0.02, tetB);
            copyTetIntoRec(FeX[1], FeX[0], bX, bY, bW, bH, 1, tetB);
#else
            surfWLSO(FeX[1], wts, vwts, xcrd, ycrd, xval, mX, mY, mW, mH, 16, 5, 1.00, tetB);
            surfWLSO(FeX[1], wts, vwts, xcrd, ycrd, xval, k*gX, j*gY, gX, gY, 5, 16, 1.00, tetB);
#endif
            /* warpTetToTetBoxBilRgb(wwD->dsrc,wts,mB->rgbi,mX,mY,mW,mH,dD,sD,tetA,tetB); */
        }
    }

    if (1) {
        dbl     stdX[10];
        shoStats4(stdX, FeX[1], fX, fY, fW, fH, "surfGridWLS stats");
        rowPclrFF(FeG[0], FeX[1], fX, fY, fW, fH, fX, fY, fW, fH, 3.0, stdX[1], stdX[2], 0.0, 255.0);
        showF(FeG[0], fX, fY, fW, fH, 1, NULL, "surfGridWLS", 2.0, 5);
        shoStats4(stdX, bvX[1], fX, fY, fW, fH, "surfGridWLS stats");
        rowPclrFF(FeG[1], bvX[1], fX, fY, fW, fH, fX, fY, fW, fH, 3.0, stdX[1], stdX[2], 0.0, 255.0);
        showF(FeG[1], fX, fY, fW, fH, 1, NULL, "orig bvX[1]", 2.0, 7);
    }

    winset(winid);
}


void
warpGrids(Wimj *v, int device, int flags, GridV4  grdA[GTY + 1][GTX + 1])
{
    pWmj    tW = (pWmj)v;
    WmjAct *ap = tW->act;
    pMD     wwD = (pMD)ap->data;
    pMOB    mB = wwD->mob[0];
    flt     *pxy, recA[4], recB[4], tetA[4][2], tetB[4][2];
    int 	j = 0, k = 0;
    pxy = mB->grdA[j - 1][k - 1].xy;  tetA[0][0] = pxy[0];  tetA[0][1] = pxy[1];
    return;
}

void
warpGridTet(Wimj *v, int device, int flags, dbl trs)
{
    pWmj    tW = (pWmj)v;
    WmjAct *ap = tW->act;
    pMD     wwD = (pMD)ap->data;
    int     nA = ap->nA;	pMOB    mA = wwD->mob[nA];
    int 	nB = ap->nB; pMOB    mB = wwD->mob[nB];
    int     j, k;
    pDmj    sD = wwD->sD;  int  sX = sD->x, sY = sD->y, sW = sD->w, sH = sD->h;
    pDmj    dD = wwD->dD;/*int  dX = dD->x, dY = dD->y, dW = dD->w, dH = dD->h;*/
    pDmj    fD = wwD->fD;  int  fX = fD->x, fY = fD->y, fW = fD->w, fH = fD->h;
    ppf    *bvX = wwD->bvX, *bvY = wwD->bvY;
    ppf    *fvX = wwD->fvX, *fvY = wwD->fvY;
    ppf	wts = wwD->wts;
    /*
    ppf     boX = wwD->bvX[nB], boY = wwD->bvY[nB];
    GridV4  *gA, *gB;
    */
    int     aX, aY, aW, aH, bX, bY, bW, bH, mX, mY, mW, mH;
    flt     *pxy, recA[4], recB[4], tetA[4][2], tetB[4][2];
    flt     rX = 1.0 / tW->zX, rY = 1.0 / tW->zY;
    dbl     areaA, areaB, omt = 1.0 - trs, srs = trs*rX;
    long    winid = winget();

    /* wrn("BEG warpGridTet:  nA: %d   nB: %d ", nA, nB); */

    switch (flags) {
        case  0:
        case  1:
        case  2:  rowZeroF(wwD->dsrc, sX * 3, sY, sW * 3, sH); break;
        case  8:  rowZeroF(fvX[nB], fX, fY, fW, fH); rowZeroF(fvY[nB], fX, fY, fW, fH); break;
        case  9:  rowZeroF(bvX[nB], fX, fY, fW, fH); rowZeroF(bvY[nB], fX, fY, fW, fH); break;
        default:  warn("warpGridTet: NOIMP for flags: %d", flags);
    }
    rowZeroF(wts, sX, sY, sW, sH);

    for (j = 1; j <= GrdY; j++) {
        for (k = 1; k <= GrdX; k++) {

            pxy = mB->grdA[j - 1][k - 1].xy;  tetA[0][0] = pxy[0] * rX;  tetA[0][1] = pxy[1] * rY;
            pxy = mB->grdA[j - 1][k].xy;  tetA[1][0] = pxy[0] * rX;  tetA[1][1] = pxy[1] * rY;
            pxy = mB->grdA[j][k].xy;  tetA[2][0] = pxy[0] * rX;  tetA[2][1] = pxy[1] * rY;
            pxy = mB->grdA[j][k - 1].xy;  tetA[3][0] = pxy[0] * rX;  tetA[3][1] = pxy[1] * rY;

            if (flags == 2) {
                pxy = mB->grdB[j - 1][k - 1].xy;  tetB[0][0] = omt*tetA[0][0] + trs*pxy[0] * rX;
                tetB[0][1] = omt*tetA[0][1] + trs*pxy[1] * rY;
                pxy = mB->grdB[j - 1][k].xy;  tetB[1][0] = omt*tetA[1][0] + trs*pxy[0] * rX;
                tetB[1][1] = omt*tetA[1][1] + trs*pxy[1] * rY;
                pxy = mB->grdB[j][k].xy;  tetB[2][0] = omt*tetA[2][0] + trs*pxy[0] * rX;
                tetB[2][1] = omt*tetA[2][1] + trs*pxy[1] * rY;
                pxy = mB->grdB[j][k - 1].xy;  tetB[3][0] = omt*tetA[3][0] + trs*pxy[0] * rX;
                tetB[3][1] = omt*tetA[3][1] + trs*pxy[1] * rY;
            }
            else {
                pxy = mB->grdB[j - 1][k - 1].xy;  tetB[0][0] = pxy[0] * rX;  tetB[0][1] = pxy[1] * rY;
                pxy = mB->grdB[j - 1][k].xy;  tetB[1][0] = pxy[0] * rX;  tetB[1][1] = pxy[1] * rY;
                pxy = mB->grdB[j][k].xy;  tetB[2][0] = pxy[0] * rX;  tetB[2][1] = pxy[1] * rY;
                pxy = mB->grdB[j][k - 1].xy;  tetB[3][0] = pxy[0] * rX;  tetB[3][1] = pxy[1] * rY;
            }

            areaA = rectBoundsFromPoly(recA, tetA, 4);
            areaB = rectBoundsFromPoly(recB, tetB, 4);
            /*
            if (areaA < MINAREA || areaB < MINAREA)
              wrn("warpGridTet %d %d: area A or B %.2f %.2f very small",j,k,areaA,areaB);
            */
            aX = recA[0];   aY = recA[1];  aW = recA[2] - aX;  aH = recA[3] - aY;
            bX = recB[0];   bY = recB[1];  bW = recB[2] - bX;  bH = recB[3] - bY;
            mX = MIN(aX, bX);
            mY = MIN(aY, bY);
            mW = MAX(recA[2], recB[2]) - mX;
            mH = MAX(recA[3], recB[3]) - mY;
            switch (flags) {
                case  0:
                    warpTetToTetBoxBilRgb(wwD->dsrc, wts, mB->rgbi, mX, mY, mW, mH, dD, sD, tetA, tetB); break;
                case  1:
                case  2:
                    warpTetToTetBoxBilRgb(wwD->dsrc, wts, mA->rgbi, mX, mY, mW, mH, dD, sD, tetB, tetA); break;
                case  8:
                    warpTetToTetBoxBilFxy(fvX[nB], fvY[nB], wts, fvX[0], fvY[0], mX, mY, mW, mH, dD, sD, tetA, tetB);
                    break;
                case  9:
                    warpTetToTetBoxBilFxy(bvX[nB], bvY[nB], wts, bvX[0], bvY[0], mX, mY, mW, mH, dD, sD, tetB, tetA);
                    break;
            }
        }
    }
    switch (flags) {
        case  0:
        case  1:
        case  2: rgbNwtsFlt(wwD->dsrc, wwD->wts, sX, sY, sW, sH, 0.05);
            /* rgbPlugFlt(dst,wts,sX,sY,sW,sH,0.05,127.5); */
            if (!InShowWid) makeShowWin(wwD, "gridWarp");
            showNewFrameShow(wwD);
            break;
        case  8: rowNwtsVxy(fvX[nB], fvY[nB], wts, sX, sY, sW, sH, 0.05); break;
        case  9: rowNwtsVxy(bvX[nB], bvY[nB], wts, sX, sY, sW, sH, 0.05); break;
    }
    winset(winid);
}



#define FLINES(V,fxy,j,k)	{\
bgnline(); if (k)     v2f(V[j][k-1].xy); v2f(fxy); if (j)     v2f(V[j-1][k].xy); endline(); \
bgnline(); if (j<GrdY) v2f(V[j+1][k].xy); v2f(fxy); if (k<GrdX) v2f(V[j][k+1].xy); endline(); }


static	long	xAo, yAo, xBo, yBo, jm, km;

void
markGridTet(void *v, int device, int flags)
{
    pWmj    wA = (pWmj)v;
    pWmj    wB = wA->next;
    WmjAct *ap = wA->act;
    pMD     wwD = (pMD)ap->data;
    int     nB = ap->nB;  pMOB  mB = wwD->mob[nB];
    long    orgX, orgY, wd, ht;
    long    xAn, yAn, xBn, yBn;
    long    widA = wA->id, widB = wB->id;
    ppf     floX = mB->foX, floY = mB->foY;
    int     j, k, jB, kB;	unt clr;
    flt     zX = wA->zX, zY = wA->zY;
    dbl     rX = 1.0 / zX, rY = 1.0 / zY;
    flt 	*xyA, *xyB, fxy[2], dx, dy, dist2, mind2 = 10e6;

    if (!flags)  return;

    getsize(&wd, &ht);     wd -= 1, ht -= 1;
    getorigin(&orgX, &orgY);
    xAn = (getvaluator(MOUSEX) - orgX);
    yAn = (getvaluator(MOUSEY) - orgY);

    /*wrn("THIS IS markGridTet: xAn,yAn == %d , %d", xAn,yAn);*/

    dx = xAn - xAo;
    dy = yAn - yAo;
    dist2 = dx*dx + dy*dy;
    /* if (dist2 > 33.0)  **  if close, pick prev. */
    for (j = 0; j <= GrdY; j++) {
        for (k = 0; k <= GrdX; k++) {
            xyA = mB->grdA[j][k].xy;
            dx = xAn - xyA[0];
            dy = yAn - xyA[1];
            dist2 = dx*dx + dy*dy;
            if (dist2 < mind2) {
                mind2 = dist2;
                jm = j;
                km = k;
            }
        }
    }
    /* Corner points can't move at all... */
    if (jm == 0) {
        if (km == 0)  jm = km = 1;
        else if (km == GrdX) jm = 1, km = Km;
    }
    else if (jm == GrdY) {
        if (km == 0)  jm = Jm, km = 1;
        else if (km == GrdX) jm = Jm, km = Km;
    }
    j = jm, k = km;

    winset(widB);
    drawmode(OVERDRAW);
    color(BLACK);
    circi(xBo, yBo, rv0);	/* Erase mark (in B) from last touched grid node */
    xyB = mB->grdB[jm][km].xy;
    xBo = xyB[0];
    yBo = xyB[1];
    if (MoveGridB) FLINES(mB->grdB, xyB, jm, km);

    winset(widA);
    cursoff();
    drawmode(OVERDRAW);
    color(BLACK);
    circi(xAo, yAo, rv0);	/* Erase mark (in A) from last touched grid node */
    xyA = mB->grdA[jm][km].xy;
    clr = mB->grdA[jm][km].clr;
    xAo = xyA[0];
    yAo = xyA[1];
    FLINES(mB->grdA, xyA, jm, km);

    while (getbutton(device)) {
        xAn = getvaluator(MOUSEX) - orgX;
        yAn = getvaluator(MOUSEY) - orgY;

        if (km == 0) xAn = 0; else if (km == GrdX)       xAn = wd;
        else if (xAn < 1) xAn = 1; else if (xAn >= wd)  xAn = wd - 1;

        if (jm == 0) yAn = 0; else if (jm == GrdY)       yAn = ht;
        else if (yAn < 1) yAn = 1; else if (yAn >= ht)  yAn = ht - 1;

        if (xAn != xAo || yAn != yAo) {

            if (MoveGridB) {
                if (UseFlowAB) {
                    kB = xAn*rX + 0.5;
                    jB = yAn*rY + 0.5;
                    xBn = xAn - zX * floX[jB][kB];
                    yBn = yAn - zY * floY[jB][kB];
                }
                else {
                    xBn = xyB[0] + (xAn - xyA[0]);  /* make proportional to spacing in B */
                    yBn = xyB[1] + (yAn - xyA[1]);
                }
                if (xBn < 0)  xBn = 0;  else if (xBn > wd)  xBn = wd;
                if (yBn < 0)  yBn = 0;  else if (yBn > ht)  yBn = ht;
                winset(widB);
                color(BLACK);
                fxy[0] = xBo, fxy[1] = yBo;
                FLINES(mB->grdB, fxy, jm, km);
                color(clr);
                fxy[0] = xBn, fxy[1] = yBn;
                FLINES(mB->grdB, fxy, jm, km);
                xBo = xBn, yBo = yBn;
                winset(widA);
            }

            color(BLACK);
            fxy[0] = xAo, fxy[1] = yAo;
            FLINES(mB->grdA, fxy, jm, km);
            color(clr);
            fxy[0] = xAn, fxy[1] = yAn;
            FLINES(mB->grdA, fxy, jm, km);

            xAo = xAn, yAo = yAn;
        }
    }
    xyA[0] = xAn;	/* mB->grdA[jm][km].xy[0] = xAn; */
    xyA[1] = yAn;	/* mB->grdA[jm][km].xy[1] = yAn; */
    if (MoveGridB) {
        mB->grdB[jm][km].xy[0] = xBn;
        mB->grdB[jm][km].xy[1] = yBn;
    }

    winset(widB);
    xBo = xyB[0] + 0.5;
    yBo = xyB[1] + 0.5;
    color(clr); circi(xBo, yBo, rv0);	/* Mark (in B) last grid node moved */
    drawmode(NORMALDRAW);

    winset(widA);
    color(clr); circi(xAo, yAo, rv0);	/* Mark (in A) last grid node moved */
    drawmode(NORMALDRAW);
    drawGridTet2(wA, device, (flags | 1));
    curson();
}

void
moveGridTet(void *v, int device, int flags)
{
    pWmj    wB = (pWmj)v;
    pWmj    wA = wB->next;
    WmjAct *ap = wB->act;
    pMD     wwD = (pMD)ap->data;
    int     nB = ap->nB;  pMOB  mB = wwD->mob[nB];   unt clr;
    long    widA = wA->id, widB = wB->id;
    long    j, k, orgX, orgY, wd, ht;
    ppf     bloX = mB->boX, bloY = mB->boY;
    long    xAn, yAn, xBn, yBn, xm, xM, ym, yM;
    flt 	*xyA, *xyB, fxy[2], dx, dy, dist2, mind2 = 10e6;
    flt     zX = wA->zX, zY = wA->zY;
    dbl     rX = 1.0 / zX, rY = 1.0 / zY;
    pDmj	sD = wwD->sD;
    int     sw = sD->w - 1, sh = sD->h - 1;


    if (!flags)  return;

    getsize(&wd, &ht);     wd -= 1, ht -= 1;
    getorigin(&orgX, &orgY);
    xBn = (getvaluator(MOUSEX) - orgX);
    yBn = (getvaluator(MOUSEY) - orgY);

    /*wrn("THIS IS moveGridTet: xBn,yBn == %d , %d", xBn,yBn);*/

    dx = xBn - xBo;
    dy = yBn - yBo;
    dist2 = dx*dx + dy*dy;
    /* if (dist2 > 33.0)  ** if close, pick prev. */
    for (j = 0; j <= GrdY; j++) {
        for (k = 0; k <= GrdX; k++) {
            xyB = mB->grdB[j][k].xy;
            dx = xBn - xyB[0];
            dy = yBn - xyB[1];
            dist2 = dx*dx + dy*dy;
            if (dist2 < mind2) {
                mind2 = dist2;
                jm = j;
                km = k;
            }
        }
    }
    /* Corner points can't move at all... */
    if (jm == 0) {
        if (km == 0)  jm = km = 1;
        else if (km == GrdX) jm = 1, km = Km;
    }
    else if (jm == GrdY) {
        if (km == 0)  jm = Jm, km = 1;
        else if (km == GrdX) jm = Jm, km = Km;
    }

    cursoff();
    winset(widA);
    drawmode(OVERDRAW);
    color(BLACK);
    circi(xAo, yAo, rv0);		/* Erase mark (in A) from last moved */
    clr = mB->grdB[jm][km].clr;
    xyA = mB->grdA[jm][km].xy;
    xAo = xyA[0];
    yAo = xyA[1];
    if (MoveGridA) FLINES(mB->grdA, xyA, jm, km);

    winset(widB);
    drawmode(OVERDRAW);
    color(BLACK);
    circi(xBo, yBo, rv0);		/* Erase mark (in B) from last moved */
    xyB = mB->grdB[jm][km].xy;
    xBo = xyB[0];
    yBo = xyB[1];
    FLINES(mB->grdB, xyB, jm, km);

    if (ResetGrid) {
        xAn = xBn = (flt)(km*sw*zX) / GrdX + 0.5;
        yAn = yBn = (flt)(jm*sh*zY) / GrdY + 0.5;
        goto END_MOVE;
    }

    /* determine limits of this grid point's mobility */

    if (jm == 0)	ym = yM = 0;
    else if (jm == GrdY)	ym = yM = ht;
    else {
        xyB = mB->grdB[jm - 1][km].xy;	ym = xyB[1] + 1;
        xyB = mB->grdB[jm + 1][km].xy;	yM = xyB[1] - 1;
    }
    if (km == 0)	xm = xM = 0;
    else if (km == GrdX)	xm = xM = wd;
    else {
        xyB = mB->grdB[jm][km - 1].xy;	xm = xyB[0] + 1;
        xyB = mB->grdB[jm][km + 1].xy;	xM = xyB[0] - 1;
    }

    while (getbutton(device)) {
        xBn = getvaluator(MOUSEX) - orgX;
        yBn = getvaluator(MOUSEY) - orgY;

        if (xBn < xm)  xBn = xm;   else if (xBn > xM)  xBn = xM;
        if (yBn < ym)  yBn = ym;   else if (yBn > yM)  yBn = yM;

        if (xBn != xBo || yBn != yBo) {

            if (MoveGridA) {
                if (UseFlowAB) {
                    k = xBn*rX + 0.5;
                    j = yBn*rY + 0.5;
                    xAn = xBn - zX * bloX[j][k];
                    yAn = yBn - zY * bloY[j][k];
                }
                else {
                    xAn = xyA[0] + (xBn - xyB[0]);  /* make proportional to spacing in B */
                    yAn = xyA[1] + (yBn - xyB[1]);
                }
                if (xAn < 0)  xAn = 0;  else if (xAn > wd)  xAn = wd;
                if (yAn < 0)  yAn = 0;  else if (yAn > ht)  yAn = ht;
                winset(widA);
                color(BLACK);
                fxy[0] = xAo, fxy[1] = yAo;
                FLINES(mB->grdA, fxy, jm, km);
                color(clr);
                fxy[0] = xAn, fxy[1] = yAn;
                FLINES(mB->grdA, fxy, jm, km);
                xAo = xAn, yAo = yAn;
                winset(widB);
            }

            color(BLACK);
            fxy[0] = xBo, fxy[1] = yBo;
            FLINES(mB->grdB, fxy, jm, km);

            color(clr);
            fxy[0] = xBn, fxy[1] = yBn;
            FLINES(mB->grdB, fxy, jm, km);

            xBo = xBn, yBo = yBn;
        }
    }

END_MOVE:

    mB->grdB[jm][km].xy[0] = xBn;
    mB->grdB[jm][km].xy[1] = yBn;
    if (MoveGridA) {
        mB->grdA[jm][km].xy[0] = xAn;
        mB->grdA[jm][km].xy[1] = yAn;
    }

    winset(widA);
    xyA = mB->grdA[jm][km].xy;
    xAo = xyA[0];
    yAo = xyA[1];
    color(clr); circi(xAo, yAo, rv0);	/* Mark (in A) last grid node moved */
    drawmode(NORMALDRAW);

    winset(widB);
    color(clr); circi(xBo, yBo, rv0);	/* Mark (in B) last grid node moved */
    drawmode(NORMALDRAW);
    drawGridTet2(wB, device, (flags | 2));
    curson();
}


void
makeGridTet(Wimj *v, int ycells, int xcells, unt flags)
{
    static  WmjAct  gridMark, gridMove;
    pWmj    wA = (pWmj)v;
    WmjAct *ap = wA->act;
    pMD     wwD = (pMD)ap->data;
    int     nB = ap->nB;  pMOB  mB = wwD->mob[nB];
    pWmj    wB = wA->next;
    pDmj	sD = wwD->sD;
    int 	sW = sD->w, sH = sD->h;
    int 	j, k, jB, kB, sw = sW - 1, sh = sH - 1;
    ppf     floX = mB->foX;
    ppf     floY = mB->foY;
    flt 	xB, yB, xO, yO, sR, sT;
    dbl     zX = wA->zX, zY = wA->zY;
    GridV4	*vA, *vB, *rA, *rB;
    long	wd, ht;

    if (xcells < 5 || xcells > GTX)  xcells = GTX;
    if (ycells < 5 || ycells > GTY)  ycells = GTY;
    GrdX = xcells;
    GrdY = ycells;
    Jm = GrdY - 1;
    Km = GrdX - 1;

    gridMark = gridMove = *ap;
    gridMark.func = &markGridTet;
    gridMove.func = &moveGridTet;
    wA->act = &gridMark;
    wB->act = &gridMove;
    wB->draw = wA->draw = &drawGridTet2;

    getsize(&wd, &ht);     wd -= 1, ht -= 1;

    for (j = 0; j <= GrdY; j++) {
        rA = mB->grdA[j];
        rB = mB->grdB[j];
        for (k = 0; k <= GrdX; k++) {
            vA = rA + k;
            vB = rB + k;
            vB->clr = vA->clr = k % 3 + 1;

            xB = kB = (flt)(k*sw) / GrdX + 0.5;	/* init flt coords of A-grid w/ int vals */
            yB = jB = (flt)(j*sh) / GrdY + 0.5;	/* so B-grid needs no round-off or interp */
            vA->xy[0] = xB*zX;	/* store zoomed coords in grid for better res */
            vA->xy[1] = yB*zX;

            sR = sW - Km, sT = sH - Jm;
            if (k && k < GrdX) {
                xB = xB - floX[jB][kB];
                if (xB <= xO)  xB = xO + 0.2;	/* prevents foldover in X */
                if (xB < k)  xB = k;  else if (xB > sR + k)  xB = sR + k;
            }
            if (j && j < GrdY) {
                yB = yB - floY[jB][kB];
                yO = mB->grdB[j - 1][k].xy[1] / zY;
                if (yB <= yO)  yB = yO + 0.2;
                if (yB < j)  yB = j;  else if (yB > sT + j)  yB = sT + j;
            }
            xO = xB;
            vB->xy[0] = xB*zY;		/* no round off */
            vB->xy[1] = yB*zY;
        }
    }
    /* drawGridTet2(wA,0,0); */
    ++gridMade;
}


int
writGridTet(Wimj *v, int ycells, int xcells, unt flags)
{
    pWmj    wA = (pWmj)v;
    WmjAct *ap = wA->act;
    pMD     wwD = (pMD)ap->data;
    int     nA = ap->nA;  pMOB  mA = wwD->mob[nA];
    int     nB = ap->nB;  pMOB  mB = wwD->mob[nB];
    int     j, k;
    flt     xA, yA, xB, yB;
    GridV4  *rA, *rB;
    flt     *xyA, *xyB, rX = 1.0 / wA->zX, rY = 1.0 / wA->zY;
    char    path[256];
    FILE    *fop;

    if (xcells < 5 || xcells > GTX)  xcells = GrdX;
    if (ycells < 5 || ycells > GTY)  ycells = GrdY;

    sprintf_s(path, 256, "%s-%s.grid.%dx%d", mA->name, mB->name, ycells, xcells);
    errno_t err = fopen_s(&fop, path, "w");
    if (err) {
        warn("writGridTet: can't open %s (w), returning 0", path);
        return 0;
    }

    for (j = 0; j <= GrdY; j++) {
        rA = mB->grdA[j];
        rB = mB->grdB[j];
        for (k = 0; k <= GrdX; k++) {
            xyA = rA[k].xy;  xA = xyA[0] * rX;  yA = xyA[1] * rY;
            xyB = rB[k].xy;  xB = xyB[0] * rX;  yB = xyB[1] * rY;
            if (fprintf(fop, "%12.5f %12.5f %12.5f %12.5f\n", xA, yA, xB, yB) == EOF) {
                wrn("writGridTet: fprintf error at jk: %3d %3d", j, k);
                return 0;
            }
        }
    }
    fclose(fop);
    return j*k;
}



int
readGridTet(Wimj *v, int ycells, int xcells, unt flags)
{
    pWmj    wA = (pWmj)v;
    WmjAct *ap = wA->act;
    pMD     wwD = (pMD)ap->data;
    int     nA = ap->nA;  pMOB  mA = wwD->mob[nA];
    int     nB = ap->nB;  pMOB  mB = wwD->mob[nB];
    /*
    pDmj    sD   = wwD->sD; int     sW = sD->w, sH = sD->h;
    */
    int     j, k, nr;
    flt     xA, yA, xB, yB, ff;
    dbl     zX = wA->zX, zY = wA->zY;
    GridV4  *vA, *vB, *rA, *rB;
    char    q, path[256], str[256];
    FILE    *fip;

    prn("BEG readGridTet: ycells,xcells,GTY,GTX:  %d %d %d %d", ycells, xcells, GTY, GTX);


    if (xcells < 5 || xcells > GTX)  xcells = GTX;
    if (ycells < 5 || ycells > GTY)  ycells = GTY;
    GrdX = xcells;
    GrdY = ycells;
    Jm = GrdY - 1;
    Km = GrdX - 1;

    if (!gridMade)  
        makeGridTet(wA, GrdY, GrdX, 0);

    if (flags)
        sprintf_s(path, 256, "%s-%s.grid.%dx%d", mB->name, mA->name, ycells, xcells);
    else
        sprintf_s(path, 256, "%s-%s.grid.%dx%d", mA->name, mB->name, ycells, xcells);
    errno_t err = fopen_s(&fip, path, "r");
    if (err) {
        warn("readGridTet: can't open %s, returning 0", path);
        return 0;
    }

    for (j = 0; j <= GrdY; j++) {
        rA = mB->grdA[j];
        rB = mB->grdB[j];
        for (k = 0; k <= GrdX; k++) {
            vA = rA + k;
            vB = rB + k;
            vB->clr = vA->clr = k % 3 + 1;
            if (fgets(str, SLEN, fip) == NULL) {
                warn("readGridTet: NULL line at jk: %3d %3d", j, k);
                return 0;
            }
            if (strlen(str) < 3 || (q = str[0]) == '#' || q == '/' || q == ';' || q == '*')
                continue;
            if (4 != (nr = sscanf(str, " %f %f %f %f ", &xA, &yA, &xB, &yB))) {
                wrn("readGridTet: parse error at jk: %3d %3d    sscanf %d <%s>", j, k, nr, str);
                return 0;
            }
            if (flags) { SWAP(xA, xB, ff); SWAP(yA, yB, ff); }
            vA->xy[0] = xA*zX;
            vA->xy[1] = yA*zX;
            vB->xy[0] = xB*zY;
            vB->xy[1] = yB*zY;
        }
    }
    fclose(fip);
    drawGridTet2(wA, 0, 0);
    return j*k;
}

