/*******  aaConvoClip.c
          S.R. Lines
          9.9.92  (Orig. 21 July 92)
          NOTES: The convolution masks these functions take as (pointer)
          arguments may include positive and negative integers; there is
          no one way of normalizing or even rectifying the results that is
          germane to of all the operations this makes possible.  Despite
          this, these functions do divide each resultant pixel value by
          the sum of only the positive mask entries, and then rectify and
          clip that "normalized" value to be within the range 0 - 255.
          So these convolutions with clipping may be used to implement a
          limited class of non-linear operators.
********/
#define   TIMING  0
#define   URCLIP(x)  if (x <   0)  x =  -x;\
                     if (x > 255)  x = 255;
#include <aa/aa.h>
#include <aa/aaConvo.h>
#define   DEBUG 4
#include <un/unBug.h>


/*___________________________________________________________________________*
    ConvoClipSepGRAY/RGBI/RGBTI: use for 2-dimensional convolution when the filter
    is separable, i.e., the kernel (or "mask") possesses symmetries allowing
    separate convolutions in x & y.  For example, gaussian blurring.
    The dimensions of the destination are always less than those of the source
    by (filter_dimension - 1).  Hence, dwide = swide - mwide + 1, etc.
    So the source must already be padded (with zeros (black), or some average color,
    or by a wrap-around, etc.) before hand.
    NB: These functions are only for positive masks, since they normalize by dividing
    by the sum of all weights each mask.  For signed masks, modify these functions to
    take in the x- and y-denominators as arguments (NOT just the sum of abs. values!)
    Called by PxConvoClipSep.
 *___________________________________________________________________________*/

puc ConvoClipSepGRAY (dst,      src,            xmsk, ymsk,     swide,stall,mwide,mtall)
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

    BEGCLOCK("ConvoClipSepGRAY");  /* #ifdef TIMING, then clock this function (aa.h).*/
    
    if (! (tmp = (puc) malloc((size_t)dwide*stall)) || dst == src ||
        ! (dst = (puc)realloc(dst,(size_t)dwide*dtall))) {
        fprintf(stderr,"ConvoClipSepRGBI: bad malloc or src/dst pointers\n");
        return(NULL);  /* Clean up or exit is left to the caller. */
    }
    
    /*_________ X-convolution first: from src to tmp:________ */
    for (denom = 0, mp = xmsk, mskE = mp + mwide; mp != mskE; )
        { ival = *mp++;  if (ival > 0)  denom += ival; }
    for (dp = tmp, savep = src; stall ; stall--, savep = sp)  {                      /* dude! */
        for (savepE = savep+dwide; savep != savepE; savep++)
        {
            ival = 0, sp = savep, mp = xmsk; 
            do { ival += *mp++ * *sp++; } while (mp != mskE);
            ival /= denom ;
            URCLIP(ival);  *dp++ = (unc)ival;
        }
    }

    /*_________ Y-convolution second: from tmp to dst:________ */
    for (denom = 0, mp = ymsk, mskE = mp + mtall; mp != mskE; )
        { ival = *mp++;  if (ival > 0)  denom += ival; }
    for (dp = dst, savep = tmp ; dtall; dtall--)  {
        for (savepE = savep+dwide; savep != savepE; savep++)
        {
            ival = 0, sp = savep, mp = ymsk; 
            do { ival += *mp++ * *sp; sp += dwide; } while (mp != mskE);
            ival /= denom ;
            URCLIP(ival);  *dp++ = (unc) ival;
        }
    }
    free(tmp);
    ENDCLOCK("ConvoClipSepGRAY");  /* #ifdef TIMING, finish what BEGCLOCK started. */
    return(dst);
}              


#define  NCHAN  3  /*______________Three-Channel versions________________*/

puc ConvoClipSepRGBI (dst,      src,            xmsk, ymsk,     swide,stall,mwide,mtall)
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

    BEGCLOCK("ConvoClipSepRGBI");  /* #ifdef TIMING, then clock this function (aa.h).*/
    if (! (tmp = (puc) malloc((size_t)dwide*stall)) || dst == src ||
        ! (dst = (puc)realloc(dst,(size_t)dwide*dtall))) {
        fprintf(stderr,"ConvoClipSepRGBI: bad malloc or src/dst pointers\n");
        return(NULL);  /* Clean up or exit is left to the caller. */
    }
    
    /*_________ X-convolution first: from src to tmp:________ */
    for (denom = 0, mp = xmsk, mskE = mp + mwide; mp != mskE; )
        { mval = *mp++;  if (mval > 0)  denom += mval; }
    for (dp = tmp, savep = src; stall; stall--, savep += rjump) {      /* OR: savep = sp+mwide) --dude! */
        for (savepE = savep+dwide; savep != savepE; savep += NCHAN)
        {
            rval = gval = bval = 0, sp = savep, mp = xmsk; 
            do {
                mval  = *mp++ ;
                rval +=  mval * *sp++;
                gval +=  mval * *sp++;
                bval +=  mval * *sp++;
            } while (mp != mskE);
            mval = rval/denom;  URCLIP(mval);  *dp++ = (unc) mval;
            mval = gval/denom;  URCLIP(mval);  *dp++ = (unc) mval;
            mval = bval/denom;  URCLIP(mval);  *dp++ = (unc) mval;
        }
    }
    
    /*_________ Y-convolution second: from tmp to dst:________ */
    rjump = dwide - NCHAN + 1;     /* General formula: rjump = (wide - 1)*nChan + 1;  */
    for (denom = 0, mp = ymsk, mskE = mp + mtall; mp != mskE; )
        { mval = *mp++;  if (mval > 0)  denom += mval; }
    for (dp = dst, savep = tmp; dtall; dtall--)  {
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
            mval = rval/denom;  URCLIP(mval);  *dp++ = (unc) mval;
            mval = gval/denom;  URCLIP(mval);  *dp++ = (unc) mval;
            mval = bval/denom;  URCLIP(mval);  *dp++ = (unc) mval;
        }
    }
    free(tmp);
    ENDCLOCK("ConvoClipSepRGBI");
    return(dst);
}              

/*________________RGB in BANKS version____________________________________________*/

puc ConvoClipSepRGBP (dst,      src,            xmsk, ymsk,     swide,stall,mwide,mtall)
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
    
    BEGCLOCK("ConvoClipSepRGBP");  /* #ifdef TIMING, then clock this function (aa.h).*/
    if (! (tmp = (puc) malloc(tsize*NCHAN)) || dst == src ||
        ! (dst = (puc)realloc(dst,(size_t)dwide*dtall*NCHAN))) {
        fprintf(stderr,"ConvoClipSepRGBP: bad malloc or src/dst pointers\n");
        return(NULL);  /* Clean up or exit is left to the caller. */
    }  
    
    /*_________ X-convolution first: from src to tmp:________ */
    rdp = tmp, gdp = rdp + tsize, bdp = gdp + tsize;
    for (denom = 0, mp = xmsk, mskE = mp + mwide; mp != mskE; )
        { mval = *mp++;  if (mval > 0)  denom += mval; }
    for (savep = src; stall ; stall--, savep = rsp)  {                      /* dude! */
        for (savepE = savep+dwide; savep != savepE; savep++) {
            rval = gval = bval = 0;
            rsp = savep, gsp = rsp + sdsize, bsp = gsp + sdsize;  mp = xmsk;
            do { mval  = *mp++;
                 rval += mval * *rsp++;
                 gval += mval * *gsp++;
                 bval += mval * *bsp++;
            } while (mp != mskE);
            mval = rval/denom;  URCLIP(mval);  *rdp++ = (unc) mval;
            mval = gval/denom;  URCLIP(mval);  *gdp++ = (unc) mval;
            mval = bval/denom;  URCLIP(mval);  *bdp++ = (unc) mval;
        }
    }
    /*_________ Y-convolution second: from tmp to dst:________ */
    sdsize = dwide*dtall;
    rdp = dst, gdp = rdp + sdsize, bdp = gdp + sdsize;
    for (denom = 0, mp = ymsk, mskE = mp + mtall; mp != mskE; )
        { mval = *mp++;  if (mval > 0)  denom += mval; }
    for (savep = tmp; dtall; dtall--)  {
        for (savepE = savep+dwide; savep != savepE; savep++)
        {
            rval = gval = bval = 0;
            rsp = savep, gsp = rsp + tsize, bsp = gsp + tsize;  mp = ymsk;
            do { mval  = *mp++;
                 rval += mval * *rsp;  rsp += dwide;
                 gval += mval * *gsp;  gsp += dwide;
                 bval += mval * *bsp;  bsp += dwide;
            } while (mp != mskE);
            mval = rval/denom;  URCLIP(mval);  *rdp++ = (unc) mval;
            mval = gval/denom;  URCLIP(mval);  *gdp++ = (unc) mval;
            mval = bval/denom;  URCLIP(mval);  *bdp++ = (unc) mval;
        }
    }
    free(tmp);
    ENDCLOCK("ConvoClipSepRGBP");
    return(dst);
}              


#undef   NCHAN
#define  NCHAN  4  /*______________Four-Channel versions________________*/

puc ConvoClipSepRGBTI (dst,      src,            xmsk, ymsk,     swide,stall,mwide,mtall)
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
    
    BEGCLOCK("ConvoClipSepRGBTI");
    if (! (tmp = (puc) malloc((size_t)dwide*stall)) || dst == src ||
        ! (dst = (puc)realloc(dst,(size_t)dwide*dtall))) {
        fprintf(stderr,"ConvoClipSepRGBI: bad malloc or src/dst pointers\n");
        return(NULL);  /* Clean up or exit is left to the caller. */
    }
    
    /*_________ X-convolution first: from src to tmp:________ */
    for (denom = 0, mp = xmsk, mskE = mp + mwide; mp != mskE; )
        { mval = *mp++;  if (mval > 0)  denom += mval; }
    for (dp = tmp, savep = src; stall; stall--, savep += rjump) { /* OR: savep = sp+mwide) --dude! */
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
            mval = rval/denom;  URCLIP(mval);  *dp++ = (unc) mval;
            mval = gval/denom;  URCLIP(mval);  *dp++ = (unc) mval;
            mval = bval/denom;  URCLIP(mval);  *dp++ = (unc) mval;
            mval = tval/denom;  URCLIP(mval);  *dp++ = (unc) mval;
        }
    }
    
    /*_________ Y-convolution second: from tmp to dst:________ */
    rjump = dwide - NCHAN + 1;     /* General formula: rjump = (wide - 1)*nChan + 1;  */
    for (denom = 0, mp = ymsk, mskE = ymsk + mtall; mp != mskE; )
        { mval = *mp++;  if (mval > 0)  denom += mval; }
    for (dp = dst, savep = tmp; dtall; dtall--)  {
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
            mval = rval/denom;  URCLIP(mval);  *dp++ = (unc) mval;
            mval = gval/denom;  URCLIP(mval);  *dp++ = (unc) mval;
            mval = bval/denom;  URCLIP(mval);  *dp++ = (unc) mval;
            mval = tval/denom;  URCLIP(mval);  *dp++ = (unc) mval;
        }
    }
    free(tmp);
    ENDCLOCK("ConvoClipSepRGBTI");
    return(dst);
}              

/*__________RGBTCI: FOR CONSTANT TRANSPARENCY/ALPHA CHANNEL ONLY!!!!____________________*/

puc ConvoClipSepRGBTCI (dst,      src,            xmsk, ymsk,     swide,stall,mwide,mtall)
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

    BEGCLOCK("ConvoClipSepRGBTCI");
    
    printf("tval == %d\n", (int)tval);   
    if (! (tmp = (puc) malloc((size_t)twide*stall)) || dst == src ||
        ! (dst = (puc)realloc(dst,(size_t)dwide*dtall))) {
        fprintf(stderr,"ConvoClipSepRGBI: bad malloc or src/dst pointers\n");
        return(NULL);  /* Clean up or exit is left to the caller. */
    }
    
    /*_________ X-convolution first: from 4-channel src to 3-channel tmp:________ */
    for (denom = 0, mp = xmsk, mskE = mp + mwide; mp != mskE; )
        { mval = *mp++;  if (mval > 0)  denom += mval; }
    for (dp = tmp, savep = src; stall; stall--, savep += rjump) {      /* OR: savep = sp+mwide) --dude! */
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
            mval = rval/denom;  URCLIP(mval);  *dp++ = (unc) mval;
            mval = gval/denom;  URCLIP(mval);  *dp++ = (unc) mval;
            mval = bval/denom;  URCLIP(mval);  *dp++ = (unc) mval;
        }
    }
    
    /*_________ Y-convolution second: from 3-channel tmp to 4-channel dst:________ */
    rjump = twide - NCHAN + 2;     /* Exception to general formula: rjump = (wide - 1)*nChan + 1;  */
    for (denom = 0, mp = ymsk, mskE = mp + mtall; mp != mskE; )
        { mval = *mp++;  if (mval > 0)  denom += mval; }
    for (dp = dst, savep = tmp; dtall; dtall--)  {
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
            mval = rval/denom;  URCLIP(mval);  *dp++ = (unc) mval;
            mval = gval/denom;  URCLIP(mval);  *dp++ = (unc) mval;
            mval = bval/denom;  URCLIP(mval);  *dp++ = (unc) mval;
            *dp++= tval;                       /* Reinsert constant T/alpha channel value. */
        }
    }
    free(tmp);
    ENDCLOCK("ConvoClipSepRGBTCI");
    return(dst);
}              

#undef NCHAN


