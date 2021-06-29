#include "ether.h"


void print_ether(struct eth_hdr* hdr){
	printf("eth ("                                       \
                    "dmac: %.2hhx:%.2hhx:%.2hhx:%.2hhx:%.2hhx:%.2hhx, " \
                    "smac: %.2hhx:%.2hhx:%.2hhx:%.2hhx:%.2hhx:%.2hhx, " \
                    "ethertype: %.4hx)\n",                               \
                    hdr->dmac[0], hdr->dmac[1], hdr->dmac[2], hdr->dmac[3], \
                    hdr->dmac[4], hdr->dmac[5], hdr->smac[0], hdr->smac[1], \
                    hdr->smac[2], hdr->smac[3], hdr->smac[4], hdr->smac[5], hdr->ethertype);
}

void ether_send(struct sk_buff* skb){

    skb->len = skb->len + ETHER_HDR_LENGTH;

    char* reponse_ether = malloc(skb->len);

    struct eth_hdr e_hdr;
    e_hdr.ethertype = IP;

    // replace with arp
    memcpy(e_hdr.dmac, skb->e_hdr->smac, 6);

    memcpy(e_hdr.smac, skb->netdev->hmac, 6);
    e_hdr.ethertype = htons(e_hdr.ethertype);

	memcpy(reponse_ether, &e_hdr, ETHER_HDR_LENGTH);
    memcpy(reponse_ether+ETHER_HDR_LENGTH, skb->data, skb->len - ETHER_HDR_LENGTH);

    free(skb->data);
    skb->data = (uint8_t *)reponse_ether;

    // check return
    int status = netdev_add_transmit_queue(skb);
    if(status < 0){
    	printf("Netdevice transmission buffer is full! Dropped.\n");
    }
}

void ether_parse(struct sk_buff* skb){

	// parse ethernet header and setup skb
	struct eth_hdr* hdr = (struct eth_hdr * ) skb->payload;
	skb->e_hdr = hdr;
	skb->payload = skb->payload + ETHER_HDR_LENGTH;
	skb->e_hdr->ethertype = ntohs(skb->e_hdr->ethertype);

	switch(skb->e_hdr->ethertype){
		case IP:
			ip_parse(skb);
			break;

		case ARP:
			printf("Protocol ARP not implemented yet. Dropped.\n");
			free(skb);
			free(skb->head);
			return;

		default:
			printf("Unknown layer 3 protocol. Dropped.\n");
			free(skb);
			free(skb->head);
			return;
	}

	return;
}
