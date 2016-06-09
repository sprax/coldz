/*****************************************************************************
 unSys.c -- (un)common system calls.  S.R.Lines (spraxlo@ai)   93.5.13-7.19
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>	   /* for strdup */
#include <errno.h>
#include "unLibPrivate.h"  /* unLib's internal switchboard */
#include "unFlags.h"	   /* Macros and prototypes for libun.a client srcs */
#include "unSys.h"         /* Macros and prototypes for libun.a client srcs */

/*_____________________ DIRECTORIES ____________________________________________*/

#ifdef	UN_GETW
int getw(FILE *fp)
{
  unsigned short uL, uH; 
  uL  = (unsigned short) fgetc(fp);	/* the low order byte 1st (!) */
  uH  = (unsigned short) fgetc(fp);	/* now the hi order byte.     */
  uL += uH*256;
  return uL;
}
#endif


#ifdef MAC

char *getcwd (char *buf, int size)
{
  strcpy(buf,"getcwd_NOIMP");
  return(buf);
}

char *getwd (char *pathname)
{
  return getcwd(pathname,MAXPATHLEN);
}

#endif

void reMkdir (const char *path, unsigned short mode, int flags)
{       /* NB: mode should be of type mode_t, which is *usually* ushort */
#ifdef MAC 
  if (flags & O_WARN)
    fprintf(stderr,"Warning -- reMkdir: MAC version does nothing.\n");
  return;
#else
  char *ss;
  int j;

  if (mkdir(path,(mode_t)0775) == -1) {

    switch (errno) {
      case EEXIST:	/* The named file already exists. */
        if (flags & O_WARN)
          fprintf(stderr,"Warning: directory %s already exists\n", path);
        break;
      case ENOENT:	/* A component of the path prefix does not exist.
      			   -OR- The path is longer than the maximum allowed.
                           --SGI's manpage lists both...
                        @@ */
			/* OK, try (recursively) making the missing entry,
                           at least if it's the last... */
                   ss = _strdup(path);
                   j  = strlen(ss);
                   while (j > 0 && ss[j] != '/') j--;
                   ss[j] = '\0'; 
                   if ( ! (j = mkdir(ss,mode)))
                     reMkdir(path,mode,flags);
                   free(ss);
                   break;
      case ENAMETOOLONG:/* The length of the path argument exceeds {PATH_MAX}, or a
                           pathname component is longer than {NAME_MAX}. */
      case ENOTDIR:	/* A component of the path prefix is not a directory. */
      case EACCES:	/* Either a component of the path prefix denies search
                           permission or write permission is denied on the parent
                           directory of the directory to be created. */
      case EROFS:	/* The path prefix resides on a read-only file system. */
      case EFAULT:	/* Path points outside the allocated address space of the 
                           process. */
      case EMLINK:	/* The maximum number of links to the parent directory
                           would exceed {LINK_MAX}. */
      case ENOSPC:	/* The directory in which the entry for the new directory
                      is being placed cannot be extended because there is no
                      space left on the file system containing the directory
                      or the new directory cannot be created because there
                      there is no space left on the file system that will
                      contain the directory or there are no free inodes on the
                      file system on which the directory is being created. */
#define EDQUOT   WSAEDQUOT 
      case EDQUOT:	/* The directory in which the entry for the new directory
                      is being placed cannot be extended either because the
                      user's quota of disk blocks on the file system
                      containing the directory has been exhausted or the new
                      directory cannot be created because the user's quota of
                      disk blocks on the file system that will contain the
                      directory has been exhausted or the user's quota of
                      inodes on the file system on which the directory is
                      being created has been exhausted. */
      case EIO: 	/* An I/O error has occurred while accessing the file system. */
      default:
        fprintf (stderr, "reMkdir(%s) failed, errno %d\n", path,errno);
        perror("mkdir");
        exit(errno);
    }
  }
#endif
}




#ifdef	UN_MKDIR	/* prototype follows SysV implementation (#ifdef __sysv) */
int	mkdir(const char *name, mode_t mode)
{
# ifdef MAC
  return(0);
# else
  return(-1);
# endif
}
#endif


