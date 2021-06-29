#ifndef NETDEVICE_H
#define NETDEVICE_H

#include "syshead.h"
#include "ipv4.h"
#include "tap.h"
#include "skb.h"

#define MAX_TRANSMIT 10

struct net_device
{
	char* name; // optional
	uint32_t ipaddr;
	uint8_t  hmac[6];
};

struct net_device* netdev_init(char* ip, char* mac);
void free_netdev(struct net_device* netdev);

void netdev_loop(struct net_device* netdev);
void netdev_user_loop(struct net_device* netdev);

int netdev_add_transmit_queue(struct sk_buff* skb);

#endif