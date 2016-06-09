/*******  aaGaussianBlurF.c
          S.R. Lines
          10.10.92  (Orig. 10.92) **************/
#include <aa/aa.h>
#include <aa/aaConvo.h>
#include <aa/aaResize.h>
#include <aa/aaGaussians.h>
#define   DEBUG  0

/*___________________________________________________________________________*
    RadialBlur/f, GaussianBlur/f: (see both aaGaussianBlur.c and aaGaussianBlurF.c)
    Give them the blur radius or x and y blur lengths and they convolve using
    the proper mask, with the output size determined by flags.
    The float versions (suffixed by an "f") are about 3 times slower, but in
    some ways are more accurately gaussian.  It doesn't make a lot of sense
    to use both.  For real improvement, it would pay
    better to taylor the values in aaGaussiansN.c (q.v.) to one's own blurring needs.
    
    NB: If only interleaved pixmaps are used, and no distinction is made between
    constant and variable 4th channels, then the information in the kind argument
    is completely determined by just the number of channels, nChan.  Also, flags
    determines only the output size and how to pad borders.  Hence, CraigBlur.
 *___________________________________________________________________________*/

puc GaussianBlurf (dst,      src,      sdwide, sdtall,xlen,ylen,nChan,kind,flags)
              puc  dst; pcuc src; int *sdwide,*sdtall,xlen,ylen,nChan,kind,flags;
{   
  if (0 < xlen && xlen < MAX_BLUR_LENGTH &&
      0 < ylen && ylen < MAX_BLUR_LENGTH) {
      return( ConvoSepf(dst,src,GaussiansF[xlen],GaussiansF[ylen]
                      ,sdwide,sdtall,xlen,ylen,nChan,kind,flags));
      }
  fprintf(stderr,"GaussianBlurf: bad x or y blur length\n");
  return(NULL);
}



puc RadialBlurf (dst,      src,      sdwide, sdtall,radius,nChan,kind,flags)
            puc  dst; pcuc src; int *sdwide,*sdtall,radius,nChan,kind,flags;
{ 
    int q = 2*radius + 1;
    if (0 < q && q < MAX_BLUR_LENGTH)
        return( ConvoSepf(dst,src,GaussiansF[q],GaussiansF[q]
                        ,sdwide,sdtall,q,q,nChan,kind,flags));
    fprintf(stderr,"RadialBlurf: bad blur radius\n");
    return(NULL);
}


puc CraigBlurf (dst,      src,      sdwide, sdtall,radius,nChan)
           puc  dst; pcuc src; int *sdwide,*sdtall,radius,nChan;
{ 
    int kind, q = 2*radius + 1;
    switch (nChan) {
        case 1:  kind = GRAY;  break;
        case 3:  kind = RGBI;  break;
        case 4:
        default: kind = RGBTI; break;
    }   
    if (0 < q && q < MAX_BLUR_LENGTH)
        return(ConvoSepf(dst,src,GaussiansF[q],GaussiansF[q]
                       ,sdwide,sdtall,q,q,nChan,kind,EXPAND|ZEROPAD));
    fprintf(stderr,"CraigBlurf: bad blur radius\n");
    return(NULL);
}


/*___________________________________________________________________________*
    ConvoSepf: Functionally the same as PxConvoSepf (in aaPxConvoF.c), but without
    the Px structs.  The source is (optionally) padded before being convolved
    with separate x and y filters, which are stored as floats instead of ints.
    The biggest difference between Convosepf and ConvoSep, other than a factor
    of roughly 3 in speed, is the float version may blur more at the same radius.
    But this is only because, as of 10.10.92, the representations of gaussians 
    in ints (aaGaussiansN.c) and those in floats don't quite agree.  Basically,
    the float versions were truncated at about 2*sigma, where sigma is the
    variance of the gaussian, which has the effect (after renormalization) of
    flattening the curve while still emphasizing its shape in the middle, not
    the ends.            
    ((The ends can be expected just to introduce a little noise anyway, and don't
    make a lot of sense in big masks over arbitrary images.  If the images are
    really of flat objects, the exact shape of the blurring mask is less important.
    If not, then the periphery will just be noise anyway.))
 *___________________________________________________________________________*/

puc ConvoSepf (dst,      src,              xmsk, ymsk,      sdwide, sdtall,mwide,mtall,nChan,kind,flags)
          puc  dst; pcuc src; const float *xmsk,*ymsk; int *sdwide,*sdtall,mwide,mtall,nChan,kind,flags;
{
    int twide = *sdwide, ttall = *sdtall;
    puc tmp;   /* (padded) temporary */
  
    if (flags < 1)  flags = (SAMESIZE | ZEROPAD);      /* default flags */
  
    /*_______________________Pad the source, else output be smaller than input____*/
    if (flags & CONTRACT)
        tmp = (puc) src;
    else {
        int padval = 0;              /* value with which to fill the borders. */
        if (flags & MEANPAD)
            padval = 0; /* MeanColor(src,wide,tall,kind); */
        if (flags & EXPAND)  {
            twide += mwide - 1;
            ttall += mtall - 1;
        }
        /* else if (flags & SAMESIZE)   the default: don't need to do anything. */
        twide += mwide-1;        /*  width of tmp */
        ttall += mtall-1;        /* height of tmp */
        if (! (kind & PX_BANKS))
            tmp = PadCropCtr( src,twide,ttall,*sdwide,*sdtall,nChan,padval);
        else 
            tmp = PadCropCtrP(src,twide,ttall,*sdwide,*sdtall,nChan,padval);
        if (! tmp) {
            fprintf(stderr,"ConvoSep: padding failed\n");
            return(NULL);
        }
    }
    *sdwide = twide-mwide+1;
    *sdtall = ttall-mtall+1;
    if (DEBUG > 2)
        printf("ConvoSepf: %d %d --convo--> %d %d\n", twide,ttall,twide-mwide+1,ttall-mtall+1);
    /* printf("ConvoSepf: early return, wide,tall == %d %d\n", twide,ttall); return(tmp);  */
    
    /*_________________Do the convolution___________________________*/
    switch (kind)  {
        case GRAY:   dst = ConvoSepfGRAY(  dst,tmp,xmsk,ymsk,twide,ttall,mwide,mtall); break;
        case RGBI:   dst = ConvoSepfRGBI(  dst,tmp,xmsk,ymsk,twide,ttall,mwide,mtall); break;
        case RGBP:   dst = ConvoSepfRGBP(  dst,tmp,xmsk,ymsk,twide,ttall,mwide,mtall); break;
        case RGBTI:  dst = ConvoSepfRGBTI( dst,tmp,xmsk,ymsk,twide,ttall,mwide,mtall); break;
        case RGBTCI: dst = ConvoSepfRGBTCI(dst,tmp,xmsk,ymsk,twide,ttall,mwide,mtall); break;
        default: fprintf(stderr,"ConvoSepf: kind unknown: %d\n",kind);  return(NULL);
    }
    if (! (flags & CONTRACT))
        free(tmp);
    if (dst == NULL)
        fprintf(stderr,"ConvoSepf: convolution failed\n");
    return(dst);
}


/*___________________________________________________________________________*
    ConvoSep (in aaGaussianBlur.c): 
    Functionally the same as PxConvoSep (in aaPxConvo.c), but without
    the Px structs.  The source is (optionally) padded before being convolved
    with separate x and y filters.  The x and y filters may be the 1D components
    of a 2D separable filter, or whatever.  Use something like sampled gaussians
    for blurring; if the x and y filters are identical, the convolution will be
    radially symmetric.
    
    PROTOCOL: Assuming src points to a pixmap of input dimensions *sdwide,*sdtall,
    convolve it separately with xmsk of length mwide and ymsk of length mtall,
    overwriting *sdwide,*sdtall with the output dimensions (determined by flags),
    and returning a pointer to the new pixmap.  If the argument dst already points
    to sufficient space, it is reused; otherwise, the return value will be a new
    pointer (realloc) or else NULL.
    
    ((Thus, always test and use the return value, not dst.  On return of NULL, calls
    of the form dst = ConvoSep(dst,...) can render data priorly pointed to by dst
    inaccessible, so avoid it unless dst is priorly NULL, is not the sole pointer
    to that data, but just a copy (obtained, e.g., as an argument to the current
    function), or else will exit on return of NULL anyway.))
 *___________________________________________________________________________*/



