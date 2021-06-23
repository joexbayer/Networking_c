#ifndef UTILS_H
#define UTILS_H

#include "utils.h"
#include "syshead.h"

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

void print_ip(unsigned int ip);
void printBits(unsigned char num, int start, int bits);
void print_ip_packet(struct ip_hdr* hdr);

#endif