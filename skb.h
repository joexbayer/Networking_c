#ifndef SKB_H
#define SKB_H

#include "syshead.h"
#include "ipv4.h"
#include "icmp.h"
#include "netdevice.h"

struct sk_buff {
    uint16_t protocol;

    struct ip_hdr* hdr;
    struct icmp* icmp;
    struct eth_hdr* e_hdr;

    struct net_device* netdev;

    uint32_t total_len;
    uint32_t len;
    uint32_t dlen;

    uint8_t *end;
    uint8_t *head;
    uint8_t *data;
    uint8_t *payload;
};

struct sk_buff* alloc_skb(struct net_device* netdev);
void free_skb(struct sk_buff* skb);

#endif