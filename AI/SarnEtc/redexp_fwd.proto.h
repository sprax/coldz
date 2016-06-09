
/* redexp_fwd.c */
int reduce_fwd(FPYR pyr, int botlev, int toplev, FILTER rf);
int expand_fwd(FPYR pyr, int botlev, int toplev, FILTER ef, int mode);
int _reduce_odd_fwd(FIMAGE in, FIMAGE out, FILTER rf);
int _expand_odd_fwd(FIMAGE in, FIMAGE out, FILTER ef, int mode);

