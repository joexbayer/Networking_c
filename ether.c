#include "ether.h"

#include "tap.h"


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

    char* response_ether = malloc(skb->len);

    struct eth_hdr e_hdr;
    e_hdr.ethertype = skb->protocol;

    // search arp for entry.
    uint8_t* arp_search_result = malloc(6);
    int arp_cache_hit = arp_search(skb->dip, arp_search_result);
    if(arp_cache_hit < 0){
    	free(skb->data);
    	free(skb);
    	return;
    }

    memcpy(e_hdr.dmac, arp_search_result, 6);
    free(arp_search_result);

    memcpy(e_hdr.smac, skb->netdev->hmac, 6);
    e_hdr.ethertype = htons(e_hdr.ethertype);

	memcpy(response_ether, &e_hdr, ETHER_HDR_LENGTH);
    memcpy(response_ether+ETHER_HDR_LENGTH, skb->data, skb->len - ETHER_HDR_LENGTH);

    free(skb->data);

    skb->data = (uint8_t *)response_ether;

    skb->total_len = skb->len;

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

	uint8_t broadcastmac = {255, 255, 255, 255, 255, 255};
	if(memcmp(skb->e_hdr->dmac, skb->netdev->hmac, 6) || memcmp(skb->e_hdr->dmac, &broadcastmac, 6)){

		switch(skb->e_hdr->ethertype){
			case IP:
				ip_parse(skb);
				break;

			case ARP:
				arp_parse(skb);
				return;

			default:
				printf("Unknown layer 3 protocol. Dropped.\n");
				free(skb);
				free(skb->head);
				return;
		}

	}

	return;
}
