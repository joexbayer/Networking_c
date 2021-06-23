#ifndef IPV4_H
#define IPV4_H

#include "syshead.h"
#include "icmp.h"

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
    uint8_t data[];
} __attribute__((packed));

#define IPV4 0x04
#define ICMPV4 0x01

void print_ip(unsigned int ip);
void print_ip_packet(struct ip_hdr* hdr);

void ip_ntohl(struct ip_hdr *hdr);
uint16_t checksum(void *addr, int count, int start_sum);

struct ip_hdr* ip_send(struct ip_hdr* ihdr_in, struct icmp* icmp);

#endif