
#include "icmp.h"
#include "utils.h"

struct icmp* icmp_read(struct ip_hdr* iphdr) 
{
    struct icmp* icmp = (struct icmp *) iphdr->data;

    if(icmp->type == ICMP_V4_ECHO){
        printf("ICMP\n");
        return icmp_write(iphdr);
    }
    return NULL;
}

struct icmp* icmp_write(struct ip_hdr* iphdr)
{
    struct icmp *icmp = malloc(sizeof(struct icmp));
    
    uint16_t icmp_len = iphdr->len - (iphdr->ihl * 4);
    
    icmp = (struct icmp *)iphdr->data;

    printf("%hhu\n", icmp->type);
        
    icmp->type = ICMP_REPLY;
    icmp->csum = 0;
    icmp->csum = checksum(icmp, icmp_len, 0);

    printf("%x\n", icmp->type);

    return icmp;
}