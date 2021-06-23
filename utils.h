#ifndef UTILS_H
#define UTILS_H

#include "syshead.h"

void printBits(unsigned char num, int start, int bits);



// call with checksum(hdr, hdr->ihl * 4, 0);
uint16_t checksum(void *addr, int count, int start_sum){
       /* Compute Internet Checksum for "count" bytes
        *         beginning at location "addr".
        * Taken from https://datatracker.ietf.org/doc/html/rfc1071#section-4.1
        */
   register long sum = 0;

    while( count > 1 )  {
       /*  This is the inner loop */
           sum += * (unsigned short) addr++;
           count -= 2;
   }

       /*  Add left-over byte, if any */
   if( count > 0 )
           sum += * (unsigned char *) addr;

       /*  Fold 32-bit sum to 16 bits */
   while (sum>>16)
       sum = (sum & 0xffff) + (sum >> 16);

   return ~sum;
}

#endif