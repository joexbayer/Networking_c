#ifndef ICMP_H
#define ICMP_H

#include "syshead.h"
#include "ipv4.h"

#define ICMP_REPLY           0x00
#define ICMP_V4_ECHO            0x08

struct icmp {
    uint8_t type;
    uint8_t code;
    uint16_t csum;
    uint8_t data[];
} __attribute__((packed));

struct icmp_echo {
    uint16_t id;
    uint16_t seq;
    uint8_t data[];
} __attribute__((packed));


struct icmp* icmp_read(struct ip_hdr* iphdr);
struct icmp* icmp_write(struct ip_hdr* iphdr);

#endif