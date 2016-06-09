/*****************************************************************************
 vxyInvt.h     S.R.Lines     93.7.15-26
 Verbs: send, spot, spat, splat, splash, rain, snow, throw, fall, land, hit
 Pairs: snd & rcv, src & dst, fwd & inv
 *****************************************************************************/

void vxyInvtBil ( flt **accW, flt **invX, flt **invY, flt **fwdX, flt **fwdY
                , int wide, int tall, int LTbord, int BRbord
                , double xmean, double ymean, double infimum);

void vxySpatBil ( flt **accW, flt **invX, flt **invY, flt **fwdX, flt **fwdY
                , int wide, int tall, int TLbord, int BRbord );

void vxySpatBilP( flt **accW, flt **invX, flt **invY, flt **fwdX, flt **fwdY
                , int wide, int tall, int TLbord, int BRbord );

void vxyZeroFlt ( flt **accW, flt **invX, flt **invY
                , int wide, int tall, int TLbord, int BRbord);



