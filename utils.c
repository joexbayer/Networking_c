#include "utils.h"

void printBits(unsigned char num, int start, int bits)
{
   for(unsigned char bit=start;bit<(sizeof(unsigned char) * bits); bit++)
   {
      printf("%i ", (num >> bit) & 0x01);
   }
   printf("\n");
}