/*****************************************************************************
rowBlnd.h    S.R.Lines (spraxlo@ai.mit.edu)    93.7.14
Assumptions:
	[See rowClip.c]
    comparison ((unc)'c' < 'C') promotes both sides to int (not unc,short,etc)
    comparison ((flt)'c' < 'C') promotes both sides to float (not double)
    comparison ((dbl)'c' < 'C') promotes both sides to double.
*****************************************************************************/


void rowBlendUnc2(ppu dst, ppu srcA, ppu srcB, int wide,int tall, double ctrs);

void rowBlend2FltToUnc(puc* dst,pfl *srcA,pfl *srcB,int wide,int tall,double ctrs);


