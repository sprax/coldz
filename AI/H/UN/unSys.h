/*****************************************************************************
 unSys.h -- prototypes from unSys.c.    S.R.Lines (spraxlo@ai)    93.6.27-8.25
 *****************************************************************************/
#ifndef unSys_h__
#define unSys_h__

#define MAXPATHLEN 256

#ifdef WIN32
#include <winsock2.h>
#include <direct.h>
#define mkdir( name, mode )  _mkdir( name )
#endif

#ifdef    THINK_C
#include <unix.h>
#else
#include <sys/types.h>
#endif
#if 	defined(__sgi) || defined(SGI)
#include <sys/stat.h>
#endif
#ifdef NeXT
typedef	int mode_t;
extern char *strdup(const char *);
#endif

#ifdef SGI	/* For INDY / IRIX 5.1.x */
#include	<unistd.h>	/* extern char *getcwd(char *, size_t); */
#else
char *getcwd (char *buf, int size);
#endif

char *getwd (char *pathname);

void	reMkdir (const char *path, unsigned short mode, int flags);
	/* NB: mode should be of type mode_t, which is *usually* ushort */


#ifdef MAC	/* __sysv */
int	mkdir(const char *name, mode_t mode);
#endif

#endif	/* unSys_h__ */


