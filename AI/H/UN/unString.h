/*****************************************************************************
 unString.h    S.R.Lines (spraxlo@ai)    93.5.13-7.4
 Usage: cc -c -D$(MFGR) where $MFGR==DEC,HP,SUN, etc; ar ru libun.a unString.o
 Then cc -lun will supply common math functions missing from your Unix/C env.
 NB: Ideally, all compilation herein would depend on which libraries are
 actually available in the environment in which this module is compiled.
 In practice, it depends entirely on macro definitions likely to coincide with
 which libraries are used -- e.g. strdup is not commonly available on any DEC
 machines (VAX or MIPS), though some sites may have it installed in GNU glibc.
 *****************************************************************************/
#ifndef unString_h__
#define unString_h__

#if	defined(HP) || defined(SUN)
extern char	*strdup(char *src);
#else
extern char	*strdup(const char *src);
#endif

#ifndef _NO_PROTO

int 	strncpylower(char *dst, const char *src, int maxlen);
int 	strcpylower(char *dst, const char *src);   /* non-ASCII OK? */
char	*string_into_int (register char *str, int *np);
char	*string_into_signed_int (register char *str, int *np);
char	*string_to_lower ( char *str );
char	*string_to_upper ( char *str );

#else   /* ifndef _NO_PROTO */

int 	strncpylower();
int 	strcpylower();
char	*string_into_int ();
char	*string_into_signed_int ();
char	*string_to_lower ();
char	*string_to_upper ();

#endif  /* _NO_PROTO */


#endif	/* unString_h__ */


