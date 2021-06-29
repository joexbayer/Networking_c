#ifndef ARP_H
#define ARP_H

#include "syshead.h"
#include "ether.h"

#define ARP_ETHERNET    0x0001
#define ARP_IPV4        0x0800
#define ARP_REQUEST     0x0001
#define ARP_REPLY       0x0002

#define ARP_MAX_ENTRY 20



struct arp_hdr
{
    uint16_t hwtype;
    uint16_t protype;
    uint8_t hwsize;
    uint8_t prosize;
    uint16_t opcode;
} __attribute__((packed));

struct arp_content
{
    uint8_t smac[6];
    uint32_t sip;
    uint8_t dmac[6];
    uint32_t dip;
} __attribute__((packed));

struct arp_entry
{
	uint8_t smac[6];
	uint32_t sip;
}__attribute__((packed));

void arp_parse(struct sk_buff* skb);
int arp_search(uint32_t ip, uint8_t* inputmac);
void arp_free();


#endif