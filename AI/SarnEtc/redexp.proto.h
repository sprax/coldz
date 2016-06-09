
/* redexp.proto.h */

int
reduce(FPYR pyr, int botlev, int toplev, FILTER rf);
int
expand(FPYR pyr, int botlev, int toplev, FILTER ef, int mode);
int
_reduce_odd(FIMAGE in, FIMAGE out, FILTER rf);
int
_expand_odd(FIMAGE in, FIMAGE out, FILTER ef, int mode);


