/*****************************************************************************
 unString.c -- common unix str routines    S.R.Lines (spraxlo@ai)    93.5.13
 Usage: cc -c -D$(MFGR) where $MFGR==DEC,HP,SUN, etc; ar ru libun.a unString.o
 Then cc -lun will supply common math functions missing from your Unix/C env.
 NB: Ideally, all compilation herein would depend on which libraries are
 actually available in the environment in which this module is compiled.
 In practice, it depends entirely on macro definitions likely to coincide with
 which libraries are used -- e.g. _strdup is not commonly available on any DEC
 machines (VAX or MIPS), though some sites may have it installed in GNU glibc.
 *****************************************************************************/
#include <stdlib.h>	/* malloc, ... */
#include <string.h>	/* strlen, ... */
#include <ctype.h>
#include "unLibPrivate.h"
#include "unString.h"

/* ____________________ STRDUP _____________________________________________*/
#ifdef UN_STRDUP
char *_strdup(const char *src)
{
  size_t size = (size_t)(1 + strlen(src));
  char  *dst = (char *)malloc(size);
  memcpy(dst,src,size);
  return(dst);
}
#endif


/******************************************************************************
aaString.c    S.R.Lines (spraxlo)   92.8.27-93.6.4
******************************************************************************/


/*  strncpylower converts to lowercase and returns the number of caps found. */
int strcpylower(char *dst, const char *src)       /* OK for non-ASCII? */
{
    int foundUpper = 0;
    register char cc;
    do{ cc = *src++;
        if (isupper(cc))
            foundUpper++, cc ^= 0x20;
        *dst++ = cc;
    } while (cc);
    return(foundUpper);
}

int strncpylower(char *dst, const char *src, int maxlen)       /* OK for non-ASCII? */
{
    int foundUpper = 0;
    register char cc;
    while ((maxlen-- > 0) && (cc = *src++)) {
        if (isupper(cc))
            foundUpper++, cc ^= 0x20;
        *dst++ = cc;
    }
    *dst = '\0';
    return(foundUpper);
}


/*	Convert alphabetic chars in a string to upper or lower case.  */

char *string_to_upper (char *str)
{
	register char c, *p = str;
	register char *end = p + strlen(p);
	while (p != end) {
		c = *p;
		*p++ = (char)(islower(c) ? (c ^ 0x20) : c);
	}
	return(str);
}

char *string_to_lower (char *str)
{
	register char c, *p = str;
	register char *end = p + strlen(p);
	while (p != end) {
		c = *p;
		*p++ = (char)(isupper(c) ? (c ^ 0x20) : c);
	}
	return(str);
}


/*	string_into_(signed)_int:
	Read a positive (or signed) int from the beginning of a string, writing the
	int into the argument (int *) np and returning the address of first char in the
	string after the int.  If no int is found, the string is returned unchanged,
	and *np = 0.  Leading whitespace is not skipped.
*/    
char *string_into_int (register char *str, int *np)
{
	register char  c = *str;
	register int   n = 0;
	while (isdigit(c)) {
		n = 10*n + (c - '0');
		c = *++str;
	}
	*np = n;
	return(str);
}

char *string_into_signed_int (register char *str, int *np)
{	int positive = 1;
	register char  c = *str;
	register int   n = 0;
	if (c == '-' || c == '+')  {
		positive = (c == '+');
		c = *++str;
		}
	while (isdigit(c)) {
		n = 10*n + (c - '0');
		c = *++str;
	}
	*np = (positive ? n : -n);
	return(str);
}

