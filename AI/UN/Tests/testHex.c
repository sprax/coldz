
#include <stdio.h>
#include <limits.h>

int main(int argc, char **argv, char **envp) { 

printf(" INT_MAX is %d\n",  INT_MAX);
printf("UINT_MAX is %u\n", UINT_MAX);
printf("0x00000000 is %u\n", 0x00000000);
printf("0x0000000F is %u\n", 0x0000000F);
printf("0x000000F0 is %u\n", 0x000000F0);
printf("0x000000FF is %u\n", 0x000000FF);
printf("0x0F000000 is %u\n", 0x0F000000);
printf("0xF0000000 is %u\n", 0xF0000000);
}

