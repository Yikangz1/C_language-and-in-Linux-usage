#include <stdio.h>
#define SRC0_PTR (*((volatile unsigned int *) 0x20250000))
int main()
{
   /*  Write C code in this online editor and run it. */
   printf("Hello, World! \n");
    int i;
 long hex1;
 int* hex2 = (int *) (& SRC0_PTR);
 
 i = 1;
 hex1 = 0x20250000;
 printf("hex1 + i = %x\n", hex1+i);
 printf("hex2 = %x\n", hex2);
 printf("hex2 + i = %x\n", hex2+i);
 printf ("size of hex1(long): %d\n",sizeof(hex1));
 printf ("size of hex2: %d\n",sizeof(hex2));
 printf ("size of hex1: %d\n",sizeof(SRC0_PTR));

 int* ii=&i;
 printf ("size of ii: %d\n",sizeof(ii));
 printf ("size of i(int): %d\n",sizeof(i));


}
