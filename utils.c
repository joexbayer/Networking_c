#include "utils.h"

void printBits(unsigned char num, int start, int bits)
{
   for(unsigned char bit=start;bit<(sizeof(unsigned char) * bits); bit++)
   {
      printf("%i ", (num >> bit) & 0x01);
   }
   printf("\n");
}


// call with checksum(hdr, hdr->ihl * 4, 0);
uint16_t checksum(void *addr, int count, int start_sum){
       /* Compute Internet Checksum for "count" bytes
        *         beginning at location "addr".
        * Taken from https://datatracker.ietf.org/doc/html/rfc1071#section-4.1
        */
   register uint32_t sum = start_sum;

   uint16_t * ptr = addr;

    while( count > 1 )  {
       /*  This is the inner loop */
           sum += * ptr++;
           count -= 2;
   }

       /*  Add left-over byte, if any */
   if( count > 0 )
           sum += *(uint8_t *) ptr;

       /*  Fold 32-bit sum to 16 bits */
   while (sum>>16)
       sum = (sum & 0xffff) + (sum >> 16);

   return ~sum;
}

uint16_t transport_checksum(uint32_t saddr, uint32_t daddr, uint8_t proto, uint8_t *data, uint16_t len)
{
    uint32_t sum = 0;

    sum += htonl(saddr);
    sum += htonl(daddr);
    sum += htons(proto);
    sum += len;
    
    return checksum(data, ntohs(len), sum);
}

