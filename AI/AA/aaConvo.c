/*******  aaConvo.c
          S.R. Lines
          9.9.92  (Orig. 21 July 92)
          NOTES: These functions normalize the positive integer convolution
          masks they take as arguments by dividing by their simple sums.
          Therefore, signed masks won't work properly here.  (Dividing by
          the absolute sums for signed masks doesn't usually work anyway.) 
          Consequently, range clipping is #defined away below because it is
          not needed for positive int masks cuz integer division truncates.
          Also, this code is "mature" and contains no bugs!   *:)   Otherwise:
#define   DEBUG 4
#include "aaBugsy.h"
********/

#define   TIMING  0
#define   RANGE(x,lo,hi)  (x)    /** Instead of: (x=MAX(x,lo),x=MIN(x,hi)) **/
#include <aa/aa.h>
#include <aa/aaConvo.h>

/*___________________________________________________________________________*
    ConvoSepGRAY/RGBI/RGBTI: use for 2-dimensional convolution when the filter
    is separable, i.e., the kernel (or "mask") possesses symmetries allowing
    separate convolutions in x & y.  For example, gaussian blurring.
    The dimensions of the destination are always less than those of the source
    by (filter_dimension - 1).  Hence, dwide = swide - mwide + 1, etc.
    So the source must already be padded (with zeros (black), or some average color,
    or by a wrap-around, etc.) before hand.
    NB: These functions are only for positive masks, since they normalize by dividing
    by the sum of all weights each mask.  For signed masks, modify these functions to
    take in the x- and y-denominators as arguments (NOT just the sum of abs. values!)
    Called by PxConvoSep.
 *___________________________________________________________________________*/

puc ConvoSepGRAY (dst,      src,            xmsk, ymsk,     swide,stall,mwide,mtall)
             puc  dst; pcuc src; const int *xmsk,*ymsk; int swide,stall,mwide,mtall;
{
    register int const *mp;   /* Register vars declared in order of frequency. */
    register pcuc sp;
    rgst int ival;
    register const int *mskE;
    register pcuc savep, savepE;
    register puc dp;
    puc tmp;
    int dwide = swide - mwide + 1; 
    int dtall = stall - mtall + 1, denom;

    BEGCLOCK("ConvoSepGRAY");  /* #ifdef TIMING, then clock this function (aa.h).*/
    
    if (! (tmp = (puc) malloc((size_t)dwide*stall)) || dst == src ||
        ! (dst = (puc)realloc(dst,(size_t)dwide*dtall))) {
        fprintf(stderr,"ConvoSepRGBI: bad malloc or src/dst pointers\n");
        return(NULL);  /* Clean up or exit is left to the caller. */
    }
    
    /*_________ X-convolution first: from src to tmp:________ */
    denom = 0, dp = tmp, savep = src, mp = xmsk, mskE = xmsk + mwide;
    do { denom += *mp++; } while (mp != mskE);
    for (; stall ; stall--, savep = sp)  {                      /* dude! */
        for (savepE = savep+dwide; savep != savepE; savep++)
        {
            ival = 0, sp = savep, mp = xmsk; 
            do { ival += *mp++ * *sp++; } while (mp != mskE);
            ival /= denom ;
            *dp++ = (unc) RANGE(ival,0,255);   /* RANGE  for debugging */
        }
    }

    /*_________ Y-convolution second: from tmp to dst:________ */
    denom = 0, dp = dst, savep = tmp, mp = ymsk, mskE = ymsk + mtall;
    do { denom += *mp++; } while (mp != mskE);
    for (; dtall; dtall--)  {
        for (savepE = savep+dwide; savep != savepE; savep++)
        {
            ival = 0, sp = savep, mp = ymsk; 
            do { ival += *mp++ * *sp; sp += dwide; } while (mp != mskE);
            ival /= denom ;
            *dp++ = (unc) RANGE(ival,0,255);
        }
    }
    free(tmp);
    ENDCLOCK("ConvoSepGRAY");  /* #ifdef TIMING, finish what BEGCLOCK started. */
    return(dst);
}              


#define  NCHAN  3  /*______________Three-Channel versions________________*/

puc ConvoSepRGBI (dst,      src,            xmsk, ymsk,     swide,stall,mwide,mtall)
             puc  dst; pcuc src; const int *xmsk,*ymsk; int swide,stall,mwide,mtall;
{
    register int const *mp;   /* Register vars declared in order of frequency. */
    register pcuc sp;
    register int mval,rval,gval,bval;
    register const int *mskE;
    register pcuc savep, savepE;
    register puc dp;
    puc tmp;
    int dwide = (swide - mwide + 1) * NCHAN; 
    int dtall =  stall - mtall + 1;
    int rjump = (swide*NCHAN - dwide), denom;

    BEGCLOCK("ConvoSepRGBI");  /* #ifdef TIMING, then clock this function (aa.h).*/
    if (! (tmp = (puc) malloc((size_t)dwide*stall)) || dst == src ||
        ! (dst = (puc)realloc(dst,(size_t)dwide*dtall))) {
        fprintf(stderr,"ConvoSepRGBI: bad malloc or src/dst pointers\n");
        return(NULL);  /* Clean up or exit is left to the caller. */
    }
    
    /*_________ X-convolution first: from src to tmp:________ */
    denom = 0, dp = tmp, savep = src, mp = xmsk, mskE = xmsk + mwide;
    do { denom += *mp++; } while (mp != mskE);
    for (; stall; stall--, savep += rjump) {      /* OR: savep = sp+mwide) --dude! */
        for (savepE = savep+dwide; savep != savepE; savep += NCHAN)
        {
            rval = gval = bval = 0, sp = savep, mp = xmsk; 
            do {
                mval  = *mp++ ;
                rval +=  mval * *sp++;
                gval +=  mval * *sp++;
                bval +=  mval * *sp++;
            } while (mp != mskE);
            rval /= denom;
            *dp++ = (unc) RANGE(rval,0,255);
            gval /= denom;
            *dp++ = (unc) RANGE(gval,0,255);
            bval /= denom;
            *dp++ = (unc) RANGE(bval,0,255);
        }
    }
    
    /*_________ Y-convolution second: from tmp to dst:________ */
    rjump = dwide - NCHAN + 1;     /* General formula: rjump = (wide - 1)*nChan + 1;  */
    denom = 0, dp = dst, savep = tmp, mp = ymsk, mskE = ymsk + mtall;
    do { denom += *mp++; } while (mp != mskE);
    for (; dtall; dtall--)  {
        for (savepE = savep+dwide; savep != savepE; savep += NCHAN)
        {
            rval = gval = bval = 0, sp = savep, mp = ymsk; 
            do {
                mval  = *mp++ ;
                rval +=  mval * *sp++;
                gval +=  mval * *sp++;
                bval +=  mval * *sp  ;
                sp += rjump;
            } while (mp != mskE);
            rval /= denom;
            *dp++ = (unc) RANGE(rval,0,255);
            gval /= denom;
            *dp++ = (unc) RANGE(gval,0,255);
            bval /= denom;
            *dp++ = (unc) RANGE(bval,0,255);
        }
    }
    free(tmp);
    ENDCLOCK("ConvoSepRGBI");
    return(dst);
}              

/*________________RGB in BANKS version____________________________________________*/

puc ConvoSepRGBP (dst,      src,            xmsk, ymsk,     swide,stall,mwide,mtall)
             puc  dst; pcuc src; const int *xmsk,*ymsk; int swide,stall,mwide,mtall;
{
    register int mval;
    register int const *mp;   /* Register vars declared in order of frequency. */
    register const int *mskE;
    register pcuc savep, savepE;
    pcuc rsp,gsp,bsp;
    puc  rdp,gdp,bdp,tmp;
    int denom,rval,gval,bval;
    int dwide = swide - mwide + 1; 
    int dtall = stall - mtall + 1;
    size_t tsize = (size_t)dwide*stall, sdsize = (size_t)swide*stall;  /* temps */
    
    BEGCLOCK("ConvoSepRGBP");  /* #ifdef TIMING, then clock this function (aa.h).*/
    if (! (tmp = (puc) malloc(tsize*NCHAN)) || dst == src ||
        ! (dst = (puc)realloc(dst,(size_t)dwide*dtall*NCHAN))) {
        fprintf(stderr,"ConvoSepRGBP: bad malloc or src/dst pointers\n");
        return(NULL);  /* Clean up or exit is left to the caller. */
    }  
    
    /*_________ X-convolution first: from src to tmp:________ */
    savep = src, rdp = tmp, gdp = rdp + tsize, bdp = gdp + tsize;
    mp = xmsk; mskE = xmsk + mwide;  denom = 0;
    do { denom += *mp++; } while (mp != mskE);
    for (; stall ; stall--, savep = rsp)  {                      /* dude! */
        for (savepE = savep+dwide; savep != savepE; savep++) {
            rval = gval = bval = 0;
            rsp = savep, gsp = rsp + sdsize, bsp = gsp + sdsize;  mp = xmsk;
            do { mval  = *mp++;
                 rval += mval * *rsp++;
                 gval += mval * *gsp++;
                 bval += mval * *bsp++;
            } while (mp != mskE);
            rval /= denom ;
            gval /= denom ;
            bval /= denom ;
            *rdp++ = (unc) RANGE(rval,0,255);   /* RANGE  for debugging */
            *gdp++ = (unc) RANGE(gval,0,255);
            *bdp++ = (unc) RANGE(bval,0,255);
        }
    }
    /*_________ Y-convolution second: from tmp to dst:________ */
    sdsize = dwide*dtall;
    savep = tmp, rdp = dst, gdp = rdp + sdsize, bdp = gdp + sdsize;
    mp = ymsk; mskE = mp + mtall;  denom = 0;
    do { denom += *mp++; } while (mp != mskE);
    for (; dtall; dtall--)  {
        for (savepE = savep+dwide; savep != savepE; savep++)
        {
            rval = gval = bval = 0;
            rsp = savep, gsp = rsp + tsize, bsp = gsp + tsize;  mp = ymsk;
            do { mval  = *mp++;
                 rval += mval * *rsp;  rsp += dwide;
                 gval += mval * *gsp;  gsp += dwide;
                 bval += mval * *bsp;  bsp += dwide;
            } while (mp != mskE);
            rval /= denom ;
            gval /= denom ;
            bval /= denom ;
            *rdp++ = (unc) RANGE(rval,0,255);   /* RANGE  for debugging */
            *gdp++ = (unc) RANGE(gval,0,255);
            *bdp++ = (unc) RANGE(bval,0,255);
        }
    }
    free(tmp);
    ENDCLOCK("ConvoSepRGBP");
    return(dst);
}              


#undef   NCHAN
#define  NCHAN  4  /*______________Four-Channel versions________________*/

puc ConvoSepRGBTI (dst,      src,            xmsk, ymsk,     swide,stall,mwide,mtall)
              puc  dst; pcuc src; const int *xmsk,*ymsk; int swide,stall,mwide,mtall;
{   /* 4 Channels Interleaved: R,G,B,T = Transparency. */
    register int const *mp;   /* Register vars declared in order of frequency. */
    register pcuc sp;
    register int mval,rval,gval,bval,tval;
    register const int *mskE;
    register pcuc savep, savepE;
    register puc dp;
    puc tmp;
    int dwide = (swide - mwide + 1) * NCHAN; 
    int dtall =  stall - mtall + 1;
    int denom, rjump = (swide*NCHAN - dwide);   /* jump from end of a row to next pixel...  */
    
    BEGCLOCK("ConvoSepRGBTI");
    if (! (tmp = (puc) malloc((size_t)dwide*stall)) || dst == src ||
        ! (dst = (puc)realloc(dst,(size_t)dwide*dtall))) {
        fprintf(stderr,"ConvoSepRGBI: bad malloc or src/dst pointers\n");
        return(NULL);  /* Clean up or exit is left to the caller. */
    }
    
    /*_________ X-convolution first: from src to tmp:________ */
    denom = 0, dp = tmp, savep = src, mp = xmsk, mskE = xmsk + mwide;
    do { denom += *mp++; } while (mp != mskE);
    for (; stall; stall--, savep += rjump) {      /* OR: savep = sp+mwide) --dude! */
        /** if (stall>dtall) fprintf(stderr,"savep-src, sp+mwide-src, sp+mwide-savep: %ld  %ld  %ld\n"
        ,savep-src,sp+mwide-src,sp+mwide-savep); **/
        for (savepE = savep+dwide; savep != savepE; savep += NCHAN)
        {
            rval = gval = bval = tval = 0, sp = savep, mp = xmsk; 
            do {
                mval  = *mp++ ;
                rval +=  mval * *sp++;
                gval +=  mval * *sp++;
                bval +=  mval * *sp++;
                tval +=  mval * *sp++;
            } while (mp != mskE);
            rval /= denom;
            *dp++ = (unc) RANGE(rval,0,255);
            gval /= denom;
            *dp++ = (unc) RANGE(gval,0,255);
            bval /= denom;
            *dp++ = (unc) RANGE(bval,0,255);
            tval /= denom;
            *dp++ = (unc) RANGE(tval,0,255);
        }
    }
    
    /*_________ Y-convolution second: from tmp to dst:________ */
    rjump = dwide - NCHAN + 1;     /* General formula: rjump = (wide - 1)*nChan + 1;  */
    denom = 0, dp = dst, savep = tmp, mp = ymsk, mskE = ymsk + mtall;
    do { denom += *mp++; } while (mp != mskE);
    for (; dtall; dtall--)  {
        for (savepE = savep+dwide; savep != savepE; savep += NCHAN)
        {
            rval = gval = bval = tval = 0, sp = savep, mp = ymsk; 
            do {
                mval  = *mp++ ;
                rval +=  mval * *sp++;
                gval +=  mval * *sp++;
                bval +=  mval * *sp++;
                tval +=  mval * *sp  ;
                sp += rjump;
            } while (mp != mskE);
            rval /= denom;
            *dp++ = (unc) RANGE(rval,0,255);
            gval /= denom;
            *dp++ = (unc) RANGE(gval,0,255);
            bval /= denom;
            *dp++ = (unc) RANGE(bval,0,255);
            tval /= denom;
            *dp++ = (unc) RANGE(tval,0,255);
        }
    }
    free(tmp);
    ENDCLOCK("ConvoSepRGBTI");
    return(dst);
}              

/*__________RGBTCI: FOR CONSTANT TRANSPARENCY/ALPHA CHANNEL ONLY!!!!____________________*/

puc ConvoSepRGBTCI (dst,      src,            xmsk, ymsk,     swide,stall,mwide,mtall)
               puc  dst; pcuc src; const int *xmsk,*ymsk; int swide,stall,mwide,mtall;
{   /* 4 Channels Interleaved: R,G,B,T = Constant Transparency. */
    register int const *mp;   /* Register vars declared in order of frequency. */
    register pcuc sp;
    register int mval,rval,gval,bval;
    register const int *mskE;
    register pcuc savep, savepE;
    register puc dp;
    puc tmp;
    int dwide = (swide - mwide + 1);
    int twide = dwide * (NCHAN - 1); 
    int dtall =  stall - mtall + 1;
    int denom, rjump = (swide - dwide)*NCHAN;   /* jump from end of a row to next pixel...  */
    unc tval = *(src + (swide*stall/2)*NCHAN - 1);
    dwide *= NCHAN;

    BEGCLOCK("ConvoSepRGBTCI");
    
    printf("tval == %d\n", (int)tval);   
    if (! (tmp = (puc) malloc((size_t)twide*stall)) || dst == src ||
        ! (dst = (puc)realloc(dst,(size_t)dwide*dtall))) {
        fprintf(stderr,"ConvoSepRGBI: bad malloc or src/dst pointers\n");
        return(NULL);  /* Clean up or exit is left to the caller. */
    }
    
    /*_________ X-convolution first: from 4-channel src to 3-channel tmp:________ */
    denom = 0, dp = tmp, savep = src, mp = xmsk, mskE = xmsk + mwide;
    do { denom += *mp++; } while (mp != mskE);
    for (; stall; stall--, savep += rjump) {      /* OR: savep = sp+mwide) --dude! */
        for (savepE = savep+dwide; savep != savepE; savep += NCHAN)
        {
            rval = gval = bval = 0, sp = savep, mp = xmsk; 
            do {
                mval  = *mp++ ;
                rval +=  mval * *sp++;
                gval +=  mval * *sp++;
                bval +=  mval * *sp++;
                sp++;                                 /* skip constant T/alpha channel */
            } while (mp != mskE);
            rval /= denom;
            *dp++ = (unc) RANGE(rval,0,255);
            gval /= denom;
            *dp++ = (unc) RANGE(gval,0,255);
            bval /= denom;
            *dp++ = (unc) RANGE(bval,0,255);
        }
    }
    
    /*_________ Y-convolution second: from 3-channel tmp to 4-channel dst:________ */
    rjump = twide - NCHAN + 2;     /* Exception to general formula: rjump = (wide - 1)*nChan + 1;  */
    denom = 0, dp = dst, savep = tmp, mp = ymsk, mskE = ymsk + mtall;
    do { denom += *mp++; } while (mp != mskE);
    for (; dtall; dtall--)  {
        for (savepE = savep+twide; savep != savepE; savep += (NCHAN-1))   /* compiler inserts constant? */
        {
            rval = gval = bval = 0, sp = savep, mp = ymsk; 
            do {
                mval  = *mp++ ;
                rval +=  mval * *sp++;
                gval +=  mval * *sp++;
                bval +=  mval * *sp  ;
                sp += rjump;
            } while (mp != mskE);
            rval /= denom;
            *dp++ = (unc) RANGE(rval,0,255);
            gval /= denom;
            *dp++ = (unc) RANGE(gval,0,255);
            bval /= denom;
            *dp++ = (unc) RANGE(bval,0,255);
            *dp++ = tval;                       /* Reinsert constant T/alpha channel value. */
        }
    }
    free(tmp);
    ENDCLOCK("ConvoSepRGBTCI");
    return(dst);
}              

#undef NCHAN


