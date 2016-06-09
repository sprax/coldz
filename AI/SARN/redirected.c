/* Utilities to determine whether or not input/output is redirected */

#include <sys/types.h>
#include <sys/stat.h>

is_input_redirected()
{
   return(!char_special(0));
}

is_output_redirected()
{
  return(!char_special(1));
}

char_special(fd)
int fd;
{
   struct stat buf;

   fstat(fd,&buf);
   return(buf.st_mode&S_IFCHR);
}


