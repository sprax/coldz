/*****************************************************************************
 ggExtern.h     S.R.Lines    94.8.29
 For quick parameter hacking using command line arguments
 *****************************************************************************/
#define MAXARG   16

extern	char    *ArgStr[MAXARG];
extern	int      ArgInt[MAXARG];
extern	dbl      ArgDbl[MAXARG];

extern  char    *ProgName;
extern  const 	int     NumSrc,NumInt,NumDbl;
extern  unl      Options;
extern  unl      Devopts;
extern  int      Image_border;   /* SHOULD ALWAYS = 4 */
#define ImB      Image_border
extern  int      Ntweens;
extern  int      PyrTop;
extern  int      PyrBot;
extern  int      OutIFF;
extern  flt        Zoom;
extern  flt      MvZoom;
extern  dbl      TrsMed;
extern  dbl      TrsEnd;
extern  dbl      TrsBeg;
extern  dbl      InFill;
extern  dbl      Amplif;
