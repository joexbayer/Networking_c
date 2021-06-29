#include "netdevice.h"

struct sk_buff* netdev_transmit_list[MAX_TRANSMIT];
uint8_t netdev_transmit_list_counter = 0;

// abstraction for tap_write()
int netdev_transmit(struct sk_buff* skb){

	int wc = tap_write( (char*) skb->data, skb->total_len);
    if(wc == 0){
    	printf("Error sending packet.\n");
    }
    return wc;
}

int netdev_add_transmit_queue(struct sk_buff* skb){

	if (netdev_transmit_list_counter == MAX_TRANSMIT){
		return -1;
	}

	netdev_transmit_list[netdev_transmit_list_counter] = skb;
	netdev_transmit_list_counter++;

	return 0;
}

uint8_t* netdev_recv(){

	uint8_t* buf = tap_read();
    if (buf == NULL){
        //printf("Error reading packet. Dropped.\n");
        return NULL;
    }

    return buf;
}


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


void netdev_loop(struct net_device* netdev){

	while(1){

        uint8_t* buf = netdev_recv();

        struct sk_buff* skb = alloc_skb(netdev);
        skb->payload = buf;
        skb->head = buf;

        ether_parse(skb);
        
        for (int i = 0; i < netdev_transmit_list_counter; ++i)
        {
        	int wc = netdev_transmit(netdev_transmit_list[i]);
        	if(wc == 0){
        		printf("Packet could not be sent!\n");
        	}
        	free(netdev_transmit_list[i]);
        	free(netdev_transmit_list[i]->data);
        }
        netdev_transmit_list_counter = 0;
    }
}

// thread?
void netdev_user_loop(){

}

void free_netdev(struct net_device* netdev){
	free(netdev);
} 