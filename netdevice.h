#ifndef NETDEVICE_H
#define NETDEVICE_H value

#include "syshead.h"
#include "ipv4.h"

struct net_device
{
	char* name; // optional
	uint32_t ipaddr;
	uint8_t  hmac[6];
};

struct net_device* netdev_init(char* ip, char* mac);


#endif