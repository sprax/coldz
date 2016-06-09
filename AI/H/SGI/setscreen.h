#ifndef SETSCREENDEF
#define SETSCREENDEF

typedef struct halftone {
    int totsize, dx, dy, w2;
    int octant, base, mint, maxt;
    unsigned char *bm;
    int *nexti;
    float angle;
} halftone;


/* setscreen.c */
float sampleproc(int dx, int dy, int x, int y, float (*proc )(), int rot90);
halftone *setscreen(float lines, float angle, float (*proc )());
int screenrow(halftone *ht, short *buf, int n, int x, int y);
int bmscreenrow(halftone *ht, short *buf, unsigned char *bits, int n, int x, int y);
int bmscreenlevel1(halftone *ht, unsigned char level, unsigned char *bits, int x, int y);
int bmscreenlevel(halftone *ht, unsigned char level, unsigned char *bits, int n, int x, int y);
int printht(halftone *ht);

#endif



