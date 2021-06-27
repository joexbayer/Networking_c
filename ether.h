#ifndef ETHER_H
#define ETHER_H value


#include "syshead.h"
#include "ipv4.h"
#include "skb.h"

#define ETHER_HDR_LENGTH 14

struct eth_hdr 
{
    uint8_t  dmac[6];
    uint8_t  smac[6];
    uint16_t ethertype;
} __attribute__((packed));

char* ether_parse(struct sk_buff* skb);


#endif