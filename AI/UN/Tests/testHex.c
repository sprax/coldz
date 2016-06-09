
#include <stdio.h>
#include <limits.h>

void main(int argc, char *argv, char **envp) { 

printf(" INT_MAX is %lu\n",  INT_MAX);
printf("UINT_MAX is %lu\n", UINT_MAX);
printf("0x00000000 is %lu\n", 0x00000000);
printf("0x0000000F is %lu\n", 0x0000000F);
printf("0x000000F0 is %lu\n", 0x000000F0);
printf("0x000000FF is %lu\n", 0x000000FF);
printf("0x0F000000 is %lu\n", 0x0F000000);
printf("0xF0000000 is %lu\n", 0xF0000000);

}


