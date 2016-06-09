/*****************************************************************************
 inParm.h    S.R.Lines     95.05.27
#include	<unTypes.h>
 *****************************************************************************/
#ifndef inParm_h__
#define inParm_h__

#define MAXFRAMES 2048
#define MAXPARM     24

extern	flt	InFrax[MAXPARM];	
extern	flt	InCwts[MAXPARM];	

void clrsFromXYZ (flt clrs[], dbl tX, dbl tY, dbl tZ, int numSrc);
void clrsFromXY  (flt clrs[], dbl trsX, dbl trsY, int numSrc);

int parmFromNtweens2(flt parm[][MAXPARM], int ntweens, int numSrc);
int parmFromNtweens4(flt parm[][MAXPARM], int horz, int vert, int numSrc);
int parmFromNtweens8(flt parm[][MAXPARM], int nx, int ny, int nz, int numSrc);

int parmReadXYZ (flt parm[][MAXPARM], char *path, int numSrc);



#endif  /* inParm_h__ */


