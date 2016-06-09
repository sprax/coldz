/*****************************************************************************
 ggParse.h    S.R.Lines  (spraxlo@{ai|media}, srl@athena)    93.10.20
 Just some convenience functions for parsing arglists like (argc,argv).
 *****************************************************************************/
#ifndef ggParse_h__
#define ggParse_h__

#ifndef NaN
#define NaN (-LONG_MIN)		/* Shouldn't compiler protest? */
#endif
			/*	@@ STILL EXPERIMENTAL: */
int
ggParse ( unl *options, unl *devopt
        , char **argStr, int *argInt, dbl *argDbl
        , int argc, char **argv, char **envp
        , int numSrc, int maxStr, int maxInt, int maxDbl);
void
ggParsePrn (unl options, char **argStr, int *argInt, dbl *argDbl);
void
ggInitArgs ( unl *options, char **argStr, int *argInt, dbl *argDbl
           , int argc, char **argv, char **envp
           , int numSrc, int numStr, int numInt, int numDbl);

#endif	/* ggParse_h__ */


