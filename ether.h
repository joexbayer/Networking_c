#ifndef ETHER_H
#define ETHER_H


#include "syshead.h"
#include "ipv4.h"
#include "skb.h"

#define ETHER_HDR_LENGTH 14

#define IP 0x0800
#define ARP 0x0806
#define RARP 0x8035


struct eth_hdr 
{
    uint8_t  dmac[6];
    uint8_t  smac[6];
    uint16_t ethertype;
} __attribute__((packed));

char* ether_parse(struct sk_buff* skb);


#endif