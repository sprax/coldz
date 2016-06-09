#ifndef TEXTUREDEF
#define TEXTUREDEF

#include <SGI/image.h>
#include <SGI/vect.h>

typedef struct texture {
	int xsize;
	int ysize;
	int zsize;
	int bpp;
	unsigned char **data;
} TEXTURE;

/* texture.c */
float onewrap(float v);
float ease(float x);
int vcubewrap(vect *v, vect *vw);
int vforceunit(vect *v);
int addnormals(vect *a, vect *b, vect *c);
TEXTURE *tmalloc(int xsize, int ysize, int zsize, int bpp);
TEXTURE *tmopen(char *name);
int copytotm(short *rowbuf, TEXTURE *tm, int y, int z);
int tmfree(TEXTURE *tm);
int tmclose(TEXTURE *tm);
int tmsample(TEXTURE *tm, vect *v, vect *c);
int imgsample(TEXTURE *tm, int xpos, int ypos, vect *c);
int envsample(TEXTURE *tm, vect *v, vect *c);
int vecttoenv(vect *v, vect *e);
int envtovect(vect *e, vect *v);
int spheretovect(vect *s, vect *v);
float wave(vect *pos);
float noisefunc(vect *v);
float ocean(vect *pos);
float turbulence(vect *v, int n);
int texchecks(vect *v, vect *c);
int texrampx(vect *v, vect *c);
int noise3(int x, int y, int z);
float fnoise3(float x, float y, float z);
int tmavg(TEXTURE *tm, vect *avg);

#endif

