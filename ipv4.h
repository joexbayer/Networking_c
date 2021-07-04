#ifndef IPV4_H
#define IPV4_H

#include "syshead.h"
#include "icmp.h"
#include "skb.h"
#include "ether.h"
#include "udp.h"

// IPv4 header
struct ip_hdr {
    uint8_t ihl : 4;
    uint8_t version : 4;
    uint8_t tos;
    uint16_t len;
    uint16_t id;
    uint16_t frag_offset;
    uint8_t ttl;
    uint8_t proto;
    uint16_t csum;
    uint32_t saddr;
    uint32_t daddr;
} __attribute__((packed));

#define IPV4 0x04
#define ICMPV4 0x01
#define UDP 0x11

void print_ip(unsigned int ip);
void print_ip_packet(struct ip_hdr* hdr);

void ip_parse(struct sk_buff * skb);

void ip_ntohl(struct ip_hdr *hdr);

void ip_send(struct sk_buff* skb);


#endif