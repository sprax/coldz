/*****************************************************************************
 jkRead.c    S.R.Lines    93.8.17-9.21
 REDRAWICONIC @@
 *****************************************************************************/
#define         UNTIME  0
#define         UNBUG   0
#include        <stddef.h>
#include        <stdio.h>
#include        <stdlib.h>
#include        <string.h>
#include        <errno.h>
 //#include        <un/unistd.h>
#include        <un/unFlags.h>
#include        <un/unBug.h>
#include        <un/unTypes.h>
#include        <un/unMem.h>
#include        <un/unTime.h>
#include        <un/unMacro.h>
#include        <m/rowSGI.h>
/********
********/
#include	"mj.h"
#include	"ggExtern.h"
extern	int	readFaceLbl(LblFace *lbl, char *name);

int jkReadFiles(ppu inpU[][4], LblFace sLbl[], char *iname[], pDmj dm)
{
    int     j, iX, iY;
    unt     w, h, ch, iW = 0, iH = 0, chan = 4;
    int     skipExtraction = 0;

    iX = 0;
    iY = 0;
    for (j = 0; j < NumSrc; j++) {
        iname[j] = ArgStr[j + 1];

        ch = sgiReadUn(inpU[j], &w, &h, 0, 0, 2048, 2048, iname[j], 0);    /** READ IMAGES **/
        if (j) {
            if (ch != chan) {
                warn("sgiReadUn sez %s is %d chan, but %s is %d chan", iname[0], chan, iname[j], ch);
                chan = MIN(chan, ch);
            }
            if (iW != w || iH != h) {
                warn("Size of input image %d is %d x %d, tho image 0 was %d x %d", j, w, h, w, h);
                iW = MIN(iW, w), iH = MIN(iH, h);
            }
        }
        else iW = w, iH = h, chan = ch;

        if (!(readFaceLbl(&sLbl[j], iname[j])))            /** READ LABELS **/
            skipExtraction = 1;
    }
    dm->l = iX, dm->t = iY, dm->w = iW, dm->h = iH, dm->d = chan;
    return skipExtraction;
}


int jkReadTriPairs(float tris[][12], int maxPrs, char *path, int yMax, unsigned flags)
{
#define SLEN 256
    char    q, str[SLEN];
    int     j, line, entry, numPtPrs;
    float   *tp;
    double  X1, Y1, X2, Y2, X3, Y3, x1, y1, x2, y2, x3, y3;
    FILE    *fi;

    errno_t err = fopen_s(&fi, path, "r");
    if (err) {
        /*warn("jkReadTriPairs [%s]: can't open %s, returning 0",__FILE__,path);*/
        return 0;
    }

    for (line = entry = numPtPrs = 0; fgets(str, SLEN, fi) != NULL && numPtPrs < maxPrs; line++) {
        if ((j = strlen(str)) < 32 || (q = str[0]) == '#' || q == '/' || q == ';' || q == '*') {
            if (flags > 1000) fprintf(stderr, "[%3d]: %s", j, str);
        }
        else {
            entry++;
            if (12 != (j = sscanf_s(str, " %lf , %lf , %lf , %lf , %lf , %lf,  %lf , %lf , %lf , %lf , %lf , %lf "
                , &X1, &Y1, &X2, &Y2, &X3, &Y3, &x1, &y1, &x2, &y2, &x3, &y3))
                && 12 != (j = sscanf_s(str, " { %lf , %lf , %lf , %lf , %lf , %lf,  %lf , %lf , %lf , %lf , %lf , %lf } "
                    , &X1, &Y1, &X2, &Y2, &X3, &Y3, &x1, &y1, &x2, &y2, &x3, &y3)))
                die("jkReadTriPairs: parse error at entry %d, text line %d, sscanf %d:\n%s", entry, line, j, str);
            else {
                if (flags > 1000) fprintf(stderr, "%3d: {\
        % 7.2f % 7.2f   % 7.2f % 7.2f   % 7.2f % 7.2f\
        % 7.2f % 7.2f   % 7.2f % 7.2f   % 7.2f % 7.2f }\n"
                    , entry, X1, Y1, X2, Y2, X3, Y3, x1, y1, x2, y2, x3, y3);
                tp = tris[numPtPrs++];

                tp[0] = (flt)X1;
                tp[2] = (flt)X2;
                tp[4] = (flt)X3;
                tp[6] = (flt)x1;
                tp[8] = (flt)x2;
                tp[10] = (flt)x3;

                if (flags & MJ_INVTY) {
                    tp[1] = (flt)(yMax - Y1);		/* yMax is usually (height - 1) */
                    tp[3] = (flt)(yMax - Y2);
                    tp[5] = (flt)(yMax - Y3);
                    tp[7] = (flt)(yMax - y1);
                    tp[9] = (flt)(yMax - y2);
                    tp[11] = (flt)(yMax - y3);
                }
                else {
                    tp[1] = (flt)Y1;
                    tp[1] = (flt)Y1;
                    tp[3] = (flt)Y2;
                    tp[5] = (flt)Y3;
                    tp[7] = (flt)y1;
                    tp[9] = (flt)y2;
                    tp[11] = (flt)y3;
                }
            }
        }
    }
    fclose(fi);
    return numPtPrs;
}

int jkTrfTriPairs(float tris[][12], int numTri, AffTrf *aT)
{
    int     jt;
    float   *tp;
    dbl	dx, dy;
    dbl	s0 = 1.0 / aT[0].scale;	/* Problems often come from using reciprocal.	*/
    dbl	s1 = 1.0 / aT[1].scale;	/* This scale is src_dist/dst_dist.		*/
    dbl 	diag0 = aT[0].cosa*s0, offd0 = aT[0].sina*s0;
    dbl	xdst0 = aT[0].xdst, ydst0 = aT[0].ydst;
    dbl	xsrc0 = aT[0].xsrc, ysrc0 = aT[0].ysrc;
    dbl 	diag1 = aT[1].cosa*s1, offd1 = aT[1].sina*s1;
    dbl	xdst1 = aT[1].xdst, ydst1 = aT[1].ydst;
    dbl	xsrc1 = aT[1].xsrc, ysrc1 = aT[1].ysrc;

#define KLUDGE	0
#if   	KLUDGE
    dbl kyoff = 0.0;	 ydst0 -= kyoff, ydst1 -= kyoff;	/* @@ KLUDGE */
    wrn("jkTrfTriPairs [%s]: KLUDGED??  scales: %.3f %.3f -> %.3f %.3f  diffs: %.3f %.3f"
        , __FILE__, aT[0].scale, aT[1].scale, s0, s1
        , ydst0*(1.0 - aT[0].scale), ydst1*(1.0 - aT[1].scale));
    affTrfPrint(&aT[0], "jkTrfTriPairs: aT[0]");
    affTrfPrint(&aT[1], "jkTrfTriPairs: aT[1]");
#endif

    for (jt = 0; jt < numTri; jt++) {

        tp = tris[jt];

        /**
        if (Options & O_VERB)
          printf(
    "B4: %5.1f %5.1f - %5.1f %5.1f - %5.1f %5.1f - %5.1f %5.1f - %5.1f %5.1f - %5.1f %5.1f\n"
        ,tp[0],tp[1],tp[2],tp[3],tp[4],tp[5],tp[6],tp[7],tp[8],tp[9],tp[10],tp[11]);
        **/

        dx = tp[0] - xsrc0;
        dy = tp[1] - ysrc0;
        tp[0] = (flt)(dx*diag0 + dy*offd0 + xdst0);
        tp[1] = (flt)(-dx*offd0 + dy*diag0 + ydst0);

        dx = tp[2] - xsrc0;
        dy = tp[3] - ysrc0;
        tp[2] = (flt)(dx*diag0 + dy*offd0 + xdst0);
        tp[3] = (flt)(-dx*offd0 + dy*diag0 + ydst0);

        dx = tp[4] - xsrc0;
        dy = tp[5] - ysrc0;
        tp[4] = (flt)(dx*diag0 + dy*offd0 + xdst0);
        tp[5] = (flt)(-dx*offd0 + dy*diag0 + ydst0);

        dx = tp[6] - xsrc1;
        dy = tp[7] - ysrc1;
        tp[6] = (flt)(dx*diag1 + dy*offd1 + xdst1);
        tp[7] = (flt)(-dx*offd1 + dy*diag1 + ydst1);

        dx = tp[8] - xsrc1;
        dy = tp[9] - ysrc1;
        tp[8] = (flt)(dx*diag1 + dy*offd1 + xdst1);
        tp[9] = (flt)(-dx*offd1 + dy*diag1 + ydst1);

        dx = tp[10] - xsrc1;
        dy = tp[11] - ysrc1;
        tp[10] = (flt)(dx*diag1 + dy*offd1 + xdst1);
        tp[11] = (flt)(-dx*offd1 + dy*diag1 + ydst1);

        /**
        if (Options & O_VERB)
          printf(
    "AF: %5.1f %5.1f - %5.1f %5.1f - %5.1f %5.1f - %5.1f %5.1f - %5.1f %5.1f - %5.1f %5.1f\n"
             ,tp[0],tp[1],tp[2],tp[3],tp[4],tp[5],tp[6],tp[7],tp[8],tp[9],tp[10],tp[11]);
        **/

    }
    return jt;
}

