#include "netdevice.h"


struct net_device* netdev_init(char* ip, char* mac){

	struct net_device* netdev = malloc(sizeof(struct net_device));

	struct sockaddr_in sa;
	inet_pton(AF_INET, ip, &(sa.sin_addr));

	// set netdevice ip address.
	netdev->ipaddr = sa.sin_addr.s_addr;

	// set netdevice mac address.
	sscanf(mac, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &netdev->hmac[0],&netdev->hmac[1],&netdev->hmac[2],
                                                 &netdev->hmac[3],&netdev->hmac[4],&netdev->hmac[5]);

	printf("Device Configuration:\nIP: %s\nMAC: %s\n\n", ip, mac);
	printf("Use: 'nc -u  %s 8080' to communicate with UDP (NOT WORKING)\n", ip);
    printf("Use: 'ping %s' to communicate with ICMP\n\n", ip);

	return netdev;
}

void free_netdev(struct net_device* netdev){
	free(netdev);
}