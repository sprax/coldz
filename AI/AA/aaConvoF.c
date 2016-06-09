/******* aaPxConvoF.c
         S.R. Lines
         21.7.92  (Orig. 21 July 92)
         Range clipping not needed for positive masks cuz floats truncate down to int.
*******/

#define  DEBUG  0
#define  RANGE(x,lo,hi)  (x)    /** Instead of: (x=MAX(x,lo),x=MIN(x,hi)) **/
#include <aa/aa.h>
#include <aa/aaConvo.h>

/*___________________________________________________________________________*
    ConvoSepfGRAY/RGBI/RGBTI: use for 2-dimensional convolution when the filter
    is separable, i.e., the kernel (or "mask") possesses symmetries allowing
    separate convolutions in x & y.  For example, gaussian blurring.
    The dimensions of the destination are always less than those of the source
    by (filter_dimension - 1).  Hence, dwide = swide - mwide + 1, etc.
    So the source must already be padded (with zeros (black), or some average color,
    or by a wrap-around, etc.) before hand. 
    Called by PxConvoSepf.
 *___________________________________________________________________________*/

puc ConvoSepfGRAY (dst,      src,              xmsk, ymsk,     swide,stall,mwide,mtall)
              puc  dst; pcuc src; const float *xmsk,*ymsk; int swide,stall,mwide,mtall;
{
    register float const *mp;   /* Register vars declared in order of frequency. */
    register pcuc sp;
    register float rval;
    register const float *mskE;
    register pcuc savep, savepE;
    register puc dp;
    int j, ival;
    puc tmp;
    int dwide = swide - mwide + 1; 
    int dtall = stall - mtall + 1; 
    

    BEGCLOCK("ConvoSepfGRAY");  /* #ifdef TIMING, then clock this function (aa.h).*/
    if (! (tmp = (puc) malloc((size_t)dwide*stall)) || dst == src ||
        ! (dst = (puc)realloc(dst,(size_t)dwide*dtall))) {
        fprintf(stderr,"ConvoSepfRGBI: bad malloc or src/dst pointers\n");
        return(NULL);  /* Clean up or exit is left to the caller. */
    }
    
    /*_________ X-convolution first: from src to tmp:________ */
    dp = tmp, savep = src, mskE = xmsk + mwide;
    for (j = stall; j > 0; j--, savep = sp)  {                      /* dude! */
        for (savepE = savep+dwide; savep != savepE; savep++)
        {
            rval = 0.0, sp = savep, mp = xmsk; 
            do { rval += *mp++ * *sp++; } while (mp != mskE);
            ival  = (int) rval ;
            *dp++ = (unc) RANGE(ival,0,255);   /* RANGE  for debugging */
        }
    }

    /*_________ Y-convolution second: from tmp to dst:________ */
    dp = dst, savep = tmp, mskE = ymsk + mtall;
    for (j = dtall; j > 0; j--)  {
        for (savepE = savep+dwide; savep != savepE; savep++)
        {
            rval = 0.0, sp = savep, mp = ymsk; 
            do { rval += *mp++ * *sp; sp += dwide; } while (mp != mskE);
            ival  = (int) rval ;
            *dp++ = (unc) RANGE(ival,0,255);   /* RANGE  for debugging */
        }
    }
    free(tmp);
    ENDCLOCK("ConvoSepfGRAY");  /* #ifdef TIMING, finish what BEGCLOCK started. */
    return(dst);
}              

#undef   NCHAN
#define  NCHAN  3  /*______________Three-Channel versions________________*/

puc ConvoSepfRGBI (dst,      src,              xmsk, ymsk,     swide,stall,mwide,mtall)
             puc   dst; pcuc src; const float *xmsk,*ymsk; int swide,stall,mwide,mtall;
{
    register float const *mp;   /* Register vars declared in order of frequency. */
    register pcuc sp;
    register float mval,rval,gval,bval;
    register const float *mskE;
    register pcuc savep, savepE;
    register puc dp;
    int j, ival;
    puc tmp;
    int dwide = (swide - mwide + 1) * NCHAN; 
    int dtall =  stall - mtall + 1;
    int rjump = (swide*NCHAN - dwide);   /* general formulas: ... */
    
    BEGCLOCK("ConvoSepfRGBI");  /* #ifdef TIMING, then clock this function (aa.h).*/
    if (! (tmp = (puc) malloc((size_t)dwide*stall)) || dst == src ||
        ! (dst = (puc)realloc(dst,(size_t)dwide*dtall))) {
        fprintf(stderr,"ConvoSepfRGBI: bad malloc or src/dst pointers\n");
        return(NULL);  /* Clean up or exit is left to the caller. */
    }
    
    /*_________ X-convolution first: from src to tmp:________ */
    dp = tmp, savep = src, mskE = xmsk + mwide;
    for (j = stall; j > 0; j--, savep += rjump) {      /* OR: savep = sp+mwide) --dude! */
        for (savepE = savep+dwide; savep != savepE; savep += NCHAN)
        {
            rval = gval = bval = 0.0, sp = savep, mp = xmsk; 
            do {
                mval  = *mp++ ;
                rval +=  mval * *sp++;
                gval +=  mval * *sp++;
                bval +=  mval * *sp++;
            } while (mp != mskE);
            ival  = (int) rval ;
            *dp++ = (unc) RANGE(ival,0,255);
            ival  = (int) gval ;
            *dp++ = (unc) RANGE(ival,0,255);
            ival  = (int) bval ;
            *dp++ = (unc) RANGE(ival,0,255);
        }
    }
    
    /*_________ Y-convolution second: from tmp to dst:________ */
    rjump = dwide - NCHAN + 1;     /* General formula: rjump = (wide - 1)*nChan + 1;  */
    dp = dst, savep = tmp, mskE = ymsk + mtall;
    for (j = dtall; j > 0; j--)  {
        for (savepE = savep+dwide; savep != savepE; savep += NCHAN)
        {
            rval = gval = bval = 0.0, sp = savep, mp = ymsk; 
            do {
                mval  = *mp++ ;
                rval +=  mval * *sp++;
                gval +=  mval * *sp++;
                bval +=  mval * *sp  ;
                sp += rjump;
            } while (mp != mskE);
            ival  = (int) rval ;
            *dp++ = (unc) RANGE(ival,0,255);
            ival  = (int) gval ;
            *dp++ = (unc) RANGE(ival,0,255);
            ival  = (int) bval ;
            *dp++ = (unc) RANGE(ival,0,255);
        }
    }
    free(tmp);
    ENDCLOCK("ConvoSepfRGBI");  /* #ifdef TIMING, finish what BEGCLOCK started. */
    return(dst);
}              


/*________________RGB in BANKS version____________________________________________*/

puc ConvoSepfRGBP (dst,      src,              xmsk, ymsk,     swide,stall,mwide,mtall)
              puc  dst; pcuc src; const float *xmsk,*ymsk; int swide,stall,mwide,mtall;
{   /* Register vars declared in order of frequency. */
    register float mval;
    register int   ival;
    register float const *mp;
    register const float *mskE;
    register pcuc savep, savepE;
    pcuc rsp,gsp,bsp;
    puc  rdp,gdp,bdp,tmp;
    float rval,gval,bval;
    int dwide = swide - mwide + 1; 
    int dtall = stall - mtall + 1;
    size_t tsize = (size_t)dwide*stall, sdsize = (size_t)swide*stall;  /* temps */
    
    BEGCLOCK("ConvoSepfRGBP");  /* #ifdef TIMING, then clock this function (aa.h).*/
    if (! (tmp = (puc) malloc(tsize*NCHAN)) || dst == src ||
        ! (dst = (puc)realloc(dst,(size_t)dwide*dtall*NCHAN))) {
        fprintf(stderr,"ConvoSepfRGBP: bad malloc or src/dst pointers\n");
        return(NULL);  /* Clean up or exit is left to the caller. */
    }  
    
    /*_________ X-convolution first: from src to tmp:________ */
    savep = src, rdp = tmp, gdp = rdp + tsize, bdp = gdp + tsize;
    mskE = xmsk + mwide;
    for (; stall ; stall--, savep = rsp)  {                      /* dude! */
        for (savepE = savep+dwide; savep != savepE; savep++) {
            rval = gval = bval = 0;
            rsp = savep, gsp = rsp + sdsize, bsp = gsp + sdsize;  mp = xmsk;
            do { mval  = *mp++;
                 rval += mval * *rsp++;
                 gval += mval * *gsp++;
                 bval += mval * *bsp++;
            } while (mp != mskE);
            ival   = (int) rval;
            *rdp++ = (unc) RANGE(ival,0,255);
            ival   = (int) gval;
            *gdp++ = (unc) RANGE(ival,0,255);
            ival   = (int) bval;
            *bdp++ = (unc) RANGE(ival,0,255);
        }
    }
    /*_________ Y-convolution second: from tmp to dst:________ */
    sdsize = dwide*dtall;
    savep = tmp, rdp = dst, gdp = rdp + sdsize, bdp = gdp + sdsize;
    mskE = mp + mtall;    for (; dtall; dtall--)  {
        for (savepE = savep+dwide; savep != savepE; savep++)
        {
            rval = gval = bval = 0;
            rsp = savep, gsp = rsp + tsize, bsp = gsp + tsize;  mp = ymsk;
            do { mval  = *mp++;
                 rval += mval * *rsp;  rsp += dwide;
                 gval += mval * *gsp;  gsp += dwide;
                 bval += mval * *bsp;  bsp += dwide;
            } while (mp != mskE);
            ival   = (int) rval;
            *rdp++ = (unc) RANGE(ival,0,255);
            ival   = (int) gval;
            *gdp++ = (unc) RANGE(ival,0,255);
            ival   = (int) bval;
            *bdp++ = (unc) RANGE(ival,0,255);
        }
    }
    free(tmp);
    ENDCLOCK("ConvoSepfRGBP");
    return(dst);
}              



#undef   NCHAN
#define  NCHAN  4  /*______________Four-Channel versions________________*/

puc ConvoSepfRGBTI (dst,      src,              xmsk, ymsk,     swide,stall,mwide,mtall)
               puc  dst; pcuc src; const float *xmsk,*ymsk; int swide,stall,mwide,mtall;
{   /* 4 Channels Interleaved: R,G,B,T = Transparency. */
    register float const *mp;   /* Register vars declared in order of frequency. */
    register pcuc sp;
    register float mval,rval,gval,bval,tval;
    register const float *mskE;
    register pcuc savep, savepE;
    register puc dp;
    int j, ival;
    puc tmp;
    int dwide = (swide - mwide + 1) * NCHAN; 
    int dtall =  stall - mtall + 1;
    int rjump = (swide*NCHAN - dwide);   /* jump from end of a row to next pixel...  */
    
    BEGCLOCK("ConvoSepfRGBTI");  /* #ifdef TIMING, then clock this function (aa.h).*/
    if (! (tmp = (puc) malloc((size_t)dwide*stall)) || dst == src ||
        ! (dst = (puc)realloc(dst,(size_t)dwide*dtall))) {
        fprintf(stderr,"ConvoSepfRGBI: bad malloc or src/dst pointers\n");
        return(NULL);  /* Clean up or exit is left to the caller. */
    }
    
    /*_________ X-convolution first: from src to tmp:________ */
    dp = tmp, savep = src, mskE = xmsk + mwide;
    for (j = stall; j > 0; j--, savep += rjump) {      /* OR: savep = sp+mwide) --dude! */
        for (savepE = savep+dwide; savep != savepE; savep += NCHAN)
        {
            rval = gval = bval = tval = 0.0, sp = savep, mp = xmsk; 
            do {
                mval  = *mp++ ;
                rval +=  mval * *sp++;
                gval +=  mval * *sp++;
                bval +=  mval * *sp++;
                tval +=  mval * *sp++;         /* don't delete the ++ from sp++ !! */
            } while (mp != mskE);
            ival  = (int) rval ;
            *dp++ = (unc) RANGE(ival,0,255);
            ival  = (int) gval ;
            *dp++ = (unc) RANGE(ival,0,255);
            ival  = (int) bval ;
            *dp++ = (unc) RANGE(ival,0,255);
            ival  = (int) tval ;
            *dp++ = (unc) RANGE(ival,0,255);
        }
    }
    
    /*_________ Y-convolution second: from tmp to dst:________ */
    rjump = dwide - NCHAN + 1;     /* General formula: rjump = (wide - 1)*nChan + 1;  */
    dp = dst, savep = tmp, mskE = ymsk + mtall;
    for (j = dtall; j > 0; j--)  {
        for (savepE = savep+dwide; savep != savepE; savep += NCHAN)
        {
            rval = gval = bval = tval = 0.0, sp = savep, mp = ymsk; 
            do {
                mval  = *mp++ ;
                rval +=  mval * *sp++;
                gval +=  mval * *sp++;
                bval +=  mval * *sp++;
                tval +=  mval * *sp  ;
                sp += rjump;
            } while (mp != mskE);
            ival  = (int) rval ;
            *dp++ = (unc) RANGE(ival,0,255);
            ival  = (int) gval ;
            *dp++ = (unc) RANGE(ival,0,255);
            ival  = (int) bval ;
            *dp++ = (unc) RANGE(ival,0,255);
            ival  = (int) tval ;
            *dp++ = (unc) RANGE(ival,0,255);
        }
    }
    free(tmp);
    ENDCLOCK("ConvoSepfRGBTI");  /* #ifdef TIMING, finish what BEGCLOCK started. */
    return(dst);
}              

/*__________RGBTCI: FOR CONSTANT TRANSPARENCY/ALPHA CHANNEL ONLY!!!!____________________*/

puc ConvoSepfRGBTCI (dst,      src,              xmsk, ymsk,     swide,stall,mwide,mtall)
                puc  dst; pcuc src; const float *xmsk,*ymsk; int swide,stall,mwide,mtall;
{   /* 4 Channels Interleaved: R,G,B,T = Constant Transparency. */
    register float const *mp;   /* Register vars declared in order of frequency. */
    register pcuc sp;
    register float mval,rval,gval,bval;
    register const float *mskE;
    register pcuc savep, savepE;
    register puc dp;
    int j, ival;
    puc tmp;
    int dwide = (swide - mwide + 1);
    int twide = dwide * (NCHAN - 1); 
    int dtall =  stall - mtall + 1;
    int rjump = (swide - dwide)*NCHAN;   /* jump from end of a row to next pixel...  */
    unc tval = *(src + (swide*stall/2)*NCHAN - 1);
    dwide *= NCHAN;
    
    BEGCLOCK("ConvoSepfRGBTCI");  /* #ifdef TIMING, then clock this function (aa.h).*/
    if (! (tmp = (puc) malloc((size_t)twide*stall)) || dst == src ||
        ! (dst = (puc)realloc(dst,(size_t)dwide*dtall))) {
        fprintf(stderr,"ConvoSepfRGBI: bad malloc or src/dst pointers\n");
        return(NULL);  /* Clean up or exit is left to the caller. */
    }
    
    /*_________ X-convolution first: from 4-channel src to 3-channel tmp:________ */
    dp = tmp, savep = src, mskE = xmsk + mwide;
    for (j = stall; j > 0; j--, savep += rjump) {      /* OR: savep = sp+mwide) --dude! */
        for (savepE = savep+dwide; savep != savepE; savep += NCHAN)
        {
            rval = gval = bval = 0.0, sp = savep, mp = xmsk; 
            do {
                mval  = *mp++ ;
                rval +=  mval * *sp++;
                gval +=  mval * *sp++;
                bval +=  mval * *sp++;
                sp++;                                 /* skip constant T/alpha channel */
            } while (mp != mskE);
            ival  = (int) rval ;
            *dp++ = (unc) RANGE(ival,0,255);
            ival  = (int) gval ;
            *dp++ = (unc) RANGE(ival,0,255);
            ival  = (int) bval ;
            *dp++ = (unc) RANGE(ival,0,255);
        }
    }
    
    /*_________ Y-convolution second: from 3-channel tmp to 4-channel dst:________ */
    rjump = twide - NCHAN + 2;     /* Exception to general formula: rjump = (wide - 1)*nChan + 1;  */
    dp = dst, savep = tmp, mskE = ymsk + mtall;
    for (j = dtall; j > 0; j--)  {
        for (savepE = savep+twide; savep != savepE; savep += (NCHAN-1))   /* compiler inserts constant? */
        {
            rval = gval = bval = 0.0, sp = savep, mp = ymsk; 
            do {
                mval  = *mp++ ;
                rval +=  mval * *sp++;
                gval +=  mval * *sp++;
                bval +=  mval * *sp  ;
                sp += rjump;
            } while (mp != mskE);
            ival  = (int) rval ;
            *dp++ = (unc) RANGE(ival,0,255);
            ival  = (int) gval ;
            *dp++ = (unc) RANGE(ival,0,255);
            ival  = (int) bval ;
            *dp++ = (unc) RANGE(ival,0,255);
            *dp++ = tval;                       /* Reinsert constant T/alpha channel value. */
        }
    }
    free(tmp);
    ENDCLOCK("ConvoSepfRGBTCI");
    return(dst);
}              

#undef NCHAN



        /** if (j > stall - 6) fprintf(stderr,"savep-src, sp+mwide-src, sp+mwide-savep: %ld  %ld  %ld\n"
                                             , savep-src, sp+mwide-src, sp+mwide-savep); **/


