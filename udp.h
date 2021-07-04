#ifndef UDP_H
#define UDP_H

#include "syshead.h"
#include "skb.h"

struct udp_hdr
{
	uint16_t srcport;
	uint16_t destport;
	uint16_t udp_length;
	uint16_t checksum;

};

void upd_parse(struct sk_buff* skb);

#endif