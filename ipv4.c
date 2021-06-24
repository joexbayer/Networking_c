#include "ipv4.h"


// print out a ipv4 packet in a readable format.
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


//print out ip, needs to be ntohl
void print_ip(unsigned int ip)
{
    unsigned char bytes[4];
    bytes[0] = (ip >> 24) & 0xFF;
    bytes[1] = (ip >> 16) & 0xFF;
    bytes[2] = (ip >> 8) & 0xFF;
    bytes[3] = ip & 0xFF;   
    printf("%d.%d.%d.%d\n", bytes[3], bytes[2], bytes[1], bytes[0]);        
}

void ip_ntohl(struct ip_hdr *hdr)
{
    hdr->saddr = ntohl(hdr->saddr);
    hdr->daddr = ntohl(hdr->daddr);
    hdr->len = ntohs(hdr->len);
    hdr->id = ntohs(hdr->id);
}


// call with checksum(hdr, hdr->ihl * 4, 0);
uint16_t checksum(void *addr, int count, int start_sum){
       /* Compute Internet Checksum for "count" bytes
        *         beginning at location "addr".
        * Taken from https://datatracker.ietf.org/doc/html/rfc1071#section-4.1
        */
   register long sum = 0;

    while( count > 1 )  {
       /*  This is the inner loop */
           sum += * (unsigned short*) addr++;
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