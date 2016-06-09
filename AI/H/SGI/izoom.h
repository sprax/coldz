/* izoom.h */

#ifndef IZOOMDEF
#define IZOOMDEF

#define IMPULSE		1
#define BOX		2
#define TRIANGLE	3
#define QUADRATIC	4
#define MITCHELL	5
#define GAUSSIAN	6

typedef struct FILTER {
    int n, totw, halftotw;
    short *dat;
    short *w;
} FILTER;

typedef struct zoom {
    int (*getfunc)();
    short *abuf;
    short *bbuf;
    int anx, any;
    int bnx, bny;
    short **xmap;
    int type;
    int curay;
    int y;
    FILTER *xfilt, *yfilt;	/* stuff for fitered zoom */
    short *tbuf;
    int nrows, clamp, ay;
    short **filtrows;
    int *accrow;
} zoom;


#endif


/* prototypes from izoom.c */
float filterinteg(float bmin, float bmax, float blurf);
int setfiltertype(int filttype);
int copyimage(int (*getfunc )(), int (*putfunc )(), int nx, int ny);
zoom *newzoom(int (*getfunc )(), int anx, int any, int bnx, int bny, int filttype, float blur);
int getzoomrow(zoom *z, short *buf, int y);
int freezoom(zoom *z);
int filterzoom(int (*getfunc )(), int (*putfunc )(), int anx, int any, int bnx, int bny, int filttype, float blur);
int zoomxfilt(int (*filtfunc )());
float filt_box(float x);
float filt_triangle(float x);
float filt_quadratic(float x);
float filt_mitchell(float x);
float filt_gaussian(float x);


