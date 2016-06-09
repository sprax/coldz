/*****************************************************************************
 unLib.h -- for libun.a clients.    S.R.Lines (spraxlo@ai.mit.edu)    93.6.28
 Usage: #include <unLib.h> in sources linked with libun.a
   Eventually, this file may split into unMath.h, unString.h, etc., but I'm
 only adding functions as I need them.
   Ideally, all declarations herein would depend on which libraries are
 actually available in the environment in which this module is included.
 In practice, it depends entirely on macro definitions likely to coincide with
 which libraries are used -- e.g. strdup is not commonly available on any DEC
 machines (VAX or MIPS), though some sites may have it installed in GNU glibc.
 In cases I know about, compiler predefines are used instead of user-defined
 macros, e.g. __STDC__,__GNUC__,SGI_CC, etc.
   ANSI C is recommended; this code supports cc's which don't implement const,
 but I draw the line at no prototypes.  (If you really must use an old ucb cc,
 add in the #ifdef _NO_PROTO convention.)
 *****************************************************************************/
#ifndef unLib_h__
#define unLib_h__

#include	"unMath.h"
#include	"unMem.h"
#include	"unString.h"
#include	"unSys.h"

#include	"unFlags.h"
#include	"unParse.h"

#if 0		/* The library itself doesn't use these:  */
#include	"unTypes.h"
#include	"unArg.h"
#endif

#endif  /* unLib_h__ */


