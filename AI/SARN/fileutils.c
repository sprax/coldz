/* Return filename extension (suffix) */

#include <stdio.h>
#include <string.h>
#include "imghdr.h"

/* Maps image type code to a string */
char *typestr(type_code)
short type_code;
{
  char *type;
  switch(type_code) {
    case DUNNO:  type = "dunno"; break;
    case BYTE:   type = "byte"; break;
    case UBYTE:  type = "unsigned byte"; break;
    case SHORT:  type = "short"; break;
    case USHORT: type = "unsigned short"; break;
    case LONG:   type = "long"; break;
    case FLOAT:  type = "float"; break; 
    default: type = NULL; break;
  }
  return type;
}


/* Determines filetype from filename extension */
int filetype(filename)
char *filename;
{
  char *ext,*file_ext();

  if ((ext = file_ext(filename)) == NULL)
    return(DUNNO);
  if (strcmp(ext,"pix") == 0)
    return(UBYTE);
  else if (strcmp(ext,"float") == 0)
    return(FLOAT);
  else if (strcmp(ext,"short") == 0)
    return(SHORT);
  else if (strcmp(ext,"long") == 0)
    return(LONG);
  else
    return(DUNNO);
}

/* Return filename extension (suffix) */

char *file_ext(filename)
char *filename;
{
  char *s;
  static char extension[20];

  if ((s = strrchr(filename,'.')) == NULL)
    return(NULL);
  return(strcpy(extension,s+1));
}

