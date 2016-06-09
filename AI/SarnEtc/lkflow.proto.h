
/* lkflow.c */
void coarse_fine(FPYR pyr1, FPYR pyr2, FPYR dx, FPYR dy, FPYR conf, int botlev, int finlev, int toplev);
void coarse_fine_mask(FPYR pyr1, FPYR pyr2, FPYR mask, FPYR dx, FPYR dy, FPYR conf, int botlev, int finlev, int toplev);
void lkflow(FIMAGE img1, FIMAGE img2, FIMAGE *dx_out, FIMAGE *dy_out, int botlev, int finlev, int toplev);

