#include "utils.h"

void print_ip(unsigned int ip)
{
    unsigned char bytes[4];
    bytes[0] = ip & 0xFF;
    bytes[1] = (ip >> 8) & 0xFF;
    bytes[2] = (ip >> 16) & 0xFF;
    bytes[3] = (ip >> 24) & 0xFF;   
    printf("%d.%d.%d.%d\n", bytes[3], bytes[2], bytes[1], bytes[0]);        
}

void printBits(unsigned char num, int start, int bits)
{
   for(unsigned char bit=start;bit<(sizeof(unsigned char) * bits); bit++)
   {
      printf("%i ", (num >> bit) & 0x01);
   }
   printf("\n");
}

void print_ip_packet(struct ip_hdr* hdr){
   printf("IPv4 (ihl: %hhu version: %hhu tos: %hhu "   \
                    "len %hu id: %hu frag_offset: %hu ttl: %hhu " \
                    "proto: %hhu csum: %hx)\n", \
                    hdr->ihl, hdr->version, hdr->tos, hdr->len, hdr->id, hdr->frag_offset, hdr->ttl, hdr->proto, hdr->csum);
   printf("Source: ");
   print_ip(ntohl(hdr->saddr));
   printf("Destination: ");
   print_ip(ntohl(hdr->daddr));

}