
/* libgutil/gutil_row.c */
void zerorow(short *sptr, int n);
void copyrow(short *s, short *d, int n);
void setrow(short *sptr, int val, int n);
void clamprow(short *iptr, short *optr, int n);
void accrow(short *iptr, short *sptr, int w, int n);
void divrow(short *iptr, short *optr, int tot, int n);
void rgbrowtobw(unsigned short *rbuf, unsigned short *gbuf
               , unsigned short *bbuf, unsigned short *obuf, int n);
void addsrow(short *dptr, short *sptr, int n);
void subsrow(short *dptr, short *sptr, int n);
void bitstorow(unsigned char *bits, short *sbuf, int n);
void rowtobits(short *sbuf, unsigned char *bits, int n);
void bitrevbytes(unsigned char *buf, int n);
void flipsrow(short *sptr, int n);
void bwtocpack(unsigned short *b, unsigned long *l, int n);
void rgbtocpack( unsigned short *r, unsigned short *g, unsigned short *b
               , unsigned long  *l, int n);
void rgbatocpack(unsigned short *r, unsigned short *g, unsigned short *b
                , unsigned short *a, unsigned long *l, int n);

void cpacktorgb ( unsigned long  *l, unsigned short *r
                , unsigned short *g, unsigned short *b, int n);

void cpacktorgba( unsigned long  *l, unsigned short *r, unsigned short *g
                , unsigned short *b, unsigned short *a, int n);
void normrow(IMAGE *image, short *buf);
void getbwrow(IMAGE *image, unsigned short *buf, int y);
void putfliprow(IMAGE *image, unsigned short *buf, int y, int z, int flipcode);
void getfliprow(IMAGE *image, unsigned short *buf, int y, int z, int flipcode);
void ditherrow(short *buf, int y, int n);


