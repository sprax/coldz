/*****************************************************************************
 unParse.h    S.R.Lines  (spraxlo@{ai|media}, srl@athena)    93.7.14
 Just some convenience functions for parsing arglists like (argc,argv).
 *****************************************************************************/
#ifndef unParse_h__
#define unParse_h__

#ifndef unTypes_h__
#include	<un/unTypes.h>
#endif

#ifndef NaN
#define NaN (-LONG_MIN)		/* Shouldn't compiler protest? */
#endif
			/*	@@ STILL EXPERIMENTAL: */
typedef	union	ctypes {
/* long	double	ldbval;	** ever used? **/
	double	dblval;
	float	fltval;
	size_t	sztval;
	long	lngval;
	int	intval;
	chr*	chrptr;
	chr	chrval;
	unc*	uncptr;
	unc	uncval;
	void*	vp;
} ctypes;

#define	tstUnParse(A,B,C,D) unParsePrn(A,B,C,D)

int 	argUnion ( ctypes *arg, char *cp, int argno);
int 	argType (  double  *dp, char *cp, int argno);

int unParse ( unsigned long *options, char **argStr, int *argInt, dbl *argDbl // see ggParse for more...
            , int argc, char **argv, char **envp);

void	unParsePrn (unsigned options, char **argStr, int *argInt, dbl *argDbl);

#endif	/* unParse_h__ */


