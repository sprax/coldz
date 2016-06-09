/*****************************************************************************
 manCor.c    S.R.Lines    94.8.17-9.13
 *****************************************************************************/
#define         UNTIME  0
#define         UNBUG   0
#include        <stdio.h>
#include        <stdlib.h>
#include        <un/unFlags.h>
#include        <un/unBug.h>
#include        <un/unTypes.h>
 /**********
 **********/
#include        <gl/gl.h>
#include	<m/mj.h>
#include	<winLoop.h>
#include	<winDraw.h>
#include        <m/mxyFwd.h>
#include        "ggExtern.h"
#include	"modFnTri.h"
#include	"manCor.h"

int drawTris(void *vp)
{
    pGlst   G = (pGlst)vp;
    int     j = G->glen / 3;
    punl    rgba = G->rgba;
    flt     *fxy = G->glxy;
    long    sfactr = BF_SA, dfactr = BF_SA;

    while (j--) {
        blendfunction(sfactr, dfactr);
        bgnpolygon();
        cpack(rgba[0] & 0x7FFFFFFF); v2f(&fxy[0]);
        cpack(rgba[1] & 0x7FFFFFFF); v2f(&fxy[2]);
        cpack(rgba[2] & 0x7FFFFFFF); v2f(&fxy[4]);
        endpolygon();
        blendfunction(BF_ONE, BF_ZERO);
        cpack(rgba[0]); circ(fxy[0], fxy[1], 2.0);
        cpack(rgba[1]); circ(fxy[2], fxy[3], 2.0);
        cpack(rgba[2]); circ(fxy[4], fxy[5], 2.0);
        fxy += 6, rgba += 3;
    }
    return G->glen / 3;
}

int drawTrisOld(void *vp) /* Example function for G->glfn */
{
    pGlst   G = (pGlst)vp;
    int     j = G->glen;
    flt     *fxy = G->glxy;
    for (j = G->glen; j--; fxy += 2) {
        cpack(FcBLU);  circf(fxy[0], fxy[1], 1.0);
        cpack(FcGRN);  circ(fxy[0], fxy[1], 2.0);
        cpack(FcRED);  circ((Coord)fxy[0], (Coord)fxy[1], 3.3F);
    }
    return G->glen;
}

void
manCorTri(ppf dcT, ppf dfT, ppf dwT, ppf scA, ppf scB, ppf sgA, ppf sgB
    , ppf sRGB[], ppf baX, ppf baY, ppf abX, ppf abY
    , int dX, int dY, unt dW, unt dH
    , int sX, int sY, unt sW, unt sH
    , int fX, int fY, unt fW, unt fH
    , int pvim, dbl trans, dbl cfrac, dbl inFill, dbl init, unt flags
    , char **argStr, flt triP[][12], int numTri)
{
    static	flt 	fA[MAXPRS], fB[MAXPRS], fC[MAXPRS];
    static	unl	*gp, sA[MAXPRS], sB[MAXPRS], sC[MAXPRS];
    static	flt	triT[MAXTRI][12];
    long	j;
    dbl	trs = 0.5, clr = 0.5;
    flt 	*dp, *sp;
    pGlst	gA = NULL, gB = NULL, gC = NULL, gD = NULL, gE = NULL, gF = NULL;

    wrn("begin manCorTri with this many TRIANGLES: %d", numTri);
    Zoom = 3.0;
    wrn("manCorTri: dD: %d %d %u %u  sD: %d %d %u %u", dX, dY, dW, dH, sX, sY, sW, sH);

    gA = (pGlst)calloc(1, sizeof(Glst));
    gp = gA->rgba = &sA[0]; gA->glfn = &drawTris;
    gA->stat = numTri; gA->glen = numTri * 3;
    dp = gA->glxy = &fA[0];
    for (j = numTri; j--; gp += 3, dp += 6) {
        sp = triP[j];
        dp[0] = sp[0] * Zoom;  dp[1] = sp[1] * Zoom;
        dp[2] = sp[2] * Zoom;  dp[3] = sp[3] * Zoom;
        dp[4] = sp[4] * Zoom;  dp[5] = sp[5] * Zoom;
        gp[0] = FcGRN;
        gp[1] = FcRED;
        gp[2] = FcBLU;
    }
    if (Options & O_VERB)
        showF(sRGB[0], dX, dY, dW, dH, 3, gA, "sRGB-A", Zoom, 0);

    gB = (pGlst)calloc(1, sizeof(Glst));
    gp = gB->rgba = &sB[0]; gB->glfn = &drawTris;
    gB->stat = gB->glen = numTri * 3;
    dp = gB->glxy = &fB[0];
    for (j = numTri; j--; gp += 3, dp += 6) {
        sp = &triP[j][6];
        dp[0] = sp[0] * Zoom;  dp[1] = sp[1] * Zoom;
        dp[2] = sp[2] * Zoom;  dp[3] = sp[3] * Zoom;
        dp[4] = sp[4] * Zoom;  dp[5] = sp[5] * Zoom;
        gp[0] = FcYEL;
        gp[1] = FcCYN;
        gp[2] = FcMAG;
    }
    if (Options & O_VERB)
        showF(sRGB[1], dX, dY, dW, dH, 3, gB, "sRGB-B", Zoom, 0);

    gC = (pGlst)calloc(1, sizeof(Glst));
    gp = gC->rgba = &sC[0]; gC->glfn = &drawTris;
    gC->stat = numTri; gC->glen = numTri * 6;
    dp = gC->glxy = &fC[0];
    for (j = numTri; j--; gp += 3, dp += 6) {
        sp = triP[j];
        dp[0] = sp[0] * Zoom;  dp[1] = sp[1] * Zoom;
        dp[2] = sp[2] * Zoom;  dp[3] = sp[3] * Zoom;
        dp[4] = sp[4] * Zoom;  dp[5] = sp[5] * Zoom;
        gp[0] = FcGRN;
        gp[1] = FcRED;
        gp[2] = FcBLU;
        dp += 6, sp += 6, gp += 3;
        dp[0] = sp[0] * Zoom;  dp[1] = sp[1] * Zoom;
        dp[2] = sp[2] * Zoom;  dp[3] = sp[3] * Zoom;
        dp[4] = sp[4] * Zoom;  dp[5] = sp[5] * Zoom;
        gp[0] = FcYEL;
        gp[1] = FcCYN;
        gp[2] = FcMAG;
    }
    gD = (pGlst)calloc(1, sizeof(Glst));  gD->glxy = fC;
    gD->rgba = sC; gD->glfn = &drawList; gD->stat = gD->glen = gC->glen;
    gE = (pGlst)calloc(1, sizeof(Glst));  gE->glxy = fC;
    gE->rgba = sC; gE->glfn = &drawList; gE->stat = gE->glen = gC->glen;
    gF = (pGlst)calloc(1, sizeof(Glst));  gF->glxy = fC;
    gF->rgba = sC; gF->glfn = &drawList; gF->stat = gF->glen = gC->glen;

    if (Options & O_VERB) {
        ixyFwd2(dcT, dwT, scA, scB, baX, baY, dX, dY, dW, dH, sX, sY, sW, sH, pvim, trs, clr, inFill, 127.5);
        showF(dcT, dX, dY, dW, dH, 3, gC, "ixyFwd2  OPT-FLOW  0.5", Zoom, 0);
    }

    if (baX && baY) {
        for (j = 0; j < numTri; j++) {
            sp = triP[j];
            dp = triT[j];
            dp[0] = sp[0];	dp[6] = sp[0] - sp[6];
            dp[1] = sp[1];	dp[7] = sp[1] - sp[7];
            dp[2] = sp[2];	dp[8] = sp[2] - sp[8];
            dp[3] = sp[3];	dp[9] = sp[3] - sp[9];
            dp[4] = sp[4];	dp[10] = sp[4] - sp[10];
            dp[5] = sp[5];	dp[11] = sp[5] - sp[11];
        }
        triModFn2(baX, baY, triT, numTri);
    }
    if (abX && abY) {
        wrn("manCor: abX & abY are %lu %lu, so look for reverse MC file...", (unl)abX, (unl)abY);
    }

    /******
    if (Options & O_VERB)
      showF(dcT,dX,dY,dW,dH,3,gD,"ixyFwd2  MODIFIED  0.5",0, Zoom);
    ixyFwd2(dcT,dwT,scA,scB,baX,baY,dX,dY,dW,dH,sX,sY,sW,sH,pvim,trs,clr,inFill,127.5);
    showF(dcT,dX,dY,dW,dH,3,gE,"ixyFwd2  MANUAL CORRESP  0.5",0, Zoom);
    *******/
#if 1
#else
    winLoopSgi();
#endif
    Zoom = MvZoom;
}



pWmj
showItWithTri
(ppu inpU[4], LblFace *lbl, pDmj iD, char *wtitle, AffTrf *aft, pGlst *pGL
    , flt triP[][12], int numTri, unt flags)
{
    static  unl     sA[MAXPRS], sB[MAXPRS];
    long    j;      unl *gp;
    flt     *dp, *sp;                                        /* yT = iD->h - 1.0; */
    pGlst   pG = *pGL = (pGlst)calloc(1, sizeof(Glst));

    if (flags) gp = &sB[0]; else gp = &sA[0];
    pG->rgba = gp;
    pG->glxy = dp = (pfl)calloc(MAXPRS, sizeof(flt));
    pG->stat = numTri;
    pG->glen = numTri * 3;
    pG->glfn = &drawTris;

    wrn("begin showItWithTri: TRIANGLES: %d", numTri);

    for (j = 0; j < numTri; j++, gp += 3, dp += 6) {
        if (flags) {
            sp = &triP[j][6];
            gp[0] = FcYEL;
            gp[1] = FcCYN;
            gp[2] = FcMAG;
        }
        else {
            sp = &triP[j][0];
            gp[0] = FcGRN;
            gp[1] = FcRED;
            gp[2] = FcBLU;
        }
#if 1
        dp[0] = sp[0] * Zoom;  dp[1] = sp[1] * Zoom;
        dp[2] = sp[2] * Zoom;  dp[3] = sp[3] * Zoom;
        dp[4] = sp[4] * Zoom;  dp[5] = sp[5] * Zoom;
#else
        dp[0] = sp[0] * Zoom;  dp[1] = (yT - sp[1])*Zoom;
        dp[2] = sp[2] * Zoom;  dp[3] = (yT - sp[3])*Zoom;
        dp[4] = sp[4] * Zoom;  dp[5] = (yT - sp[5])*Zoom;
#endif
    }
    return showU(inpU, iD->x, iD->y, iD->w, iD->h, iD->d, pG, wtitle, 0);
}


pWmj
showItWithDots(ppu src[4], LblFace *lbl, pDmj sD, char *wtitle, AffTrf *aft, pGlst *pGL)
{
    static  unl     *gp, sA[MAXPRS];
    int     sW = sD->w, sH = sD->h;
    pfl     dp;
    pGlst   pG = *pGL = (pGlst)calloc(1, sizeof(Glst));
    pG->rgba = gp = &sA[0]; pG->glfn = &drawTrisOld; /* drawTrisOld */
    pG->glxy = dp = (pfl)calloc(MAXPRS, sizeof(flt));
    pG->stat = 3;
    pG->glen = 3 * 3;
    gp[0] = FcCLR[0 % 6];
    gp[1] = FcCLR[1 % 6];
    gp[2] = FcCLR[2 % 6];
    gp[3] = FcCLR[3 % 6];
    dp[0] = sW * lbl->lex*Zoom;
    dp[1] = sH * lbl->ley*Zoom; wrn("DOTS 1: %.3f %.3f --> %.3f %.3f", lbl->lex, lbl->ley, dp[0], sH - dp[1]);
    dp[2] = sW * lbl->rex*Zoom;
    dp[3] = sH * lbl->rey*Zoom; wrn("DOTS 2: %.3f %.3f --> %.3f %.3f", lbl->rex, lbl->rey, dp[2], sH - dp[3]);
    dp[4] = sW * lbl->nox*Zoom;
    dp[5] = sH * lbl->noy*Zoom; wrn("DOTS 3: %.3f %.3f --> %.3f %.3f", lbl->nox, lbl->noy, dp[4], sH - dp[5]);
    dp[6] = sW * lbl->mex*Zoom;
    dp[7] = sH * lbl->mey*Zoom; wrn("DOTS 4: %.3f %.3f --> %.3f %.3f", lbl->mex, lbl->mey, dp[6], sH - dp[7]);
    return showU(src, sD->x, sD->y, sD->w, sD->h, sD->d, *pGL, wtitle, 0);
}


