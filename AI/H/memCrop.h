/******************************************************************************
 memCrop.h    S.R.Lines (spraxlo)   92.8.27-93.7.22
 ******************************************************************************/
#ifndef memResize_h__
#define memResize_h__

/* #include	"unTypes.h" */

puc PadCropCtr(pcuc src, int dwide, int dtall, int swide, int stall, int nChan, int padval);
puc PadCropCtrP(pcuc src, int dwide, int dtall, int swide, int stall, int nChan, int padval);
puc Reframe(puc dst, pcuc src, int dwide, int dtall, int swide, int stall, int nChan, int lef, int top, int padval);
puc resizeCtr(puc dst, pcuc src, int dwide, int dtall, int swide, int stall, int nChan, int padval);

#endif  /* memResize_h__ */


