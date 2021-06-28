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

char* ether_parse(struct sk_buff* skb){

	// parse ethernet header and setup skb
	struct eth_hdr* hdr = (struct eth_hdr * ) skb->payload;
	skb->e_hdr = hdr;
	skb->payload = skb->payload + ETHER_HDR_LENGTH;
	skb->e_hdr->ethertype = ntohs(skb->e_hdr->ethertype);

	char* reponse_3 = NULL;
	switch(skb->e_hdr->ethertype){
		case IP:
			reponse_3 = ip_parse(skb);
		    if(reponse_3 == NULL){
		        return NULL;
		    }

		    //--> THIS SHOULD BE DONE THROUGH ARP IN IP
		    struct eth_hdr* hdr_response = malloc(ETHER_HDR_LENGTH);

		    char* reponse_ether = calloc(0, skb->total_len);

		    memcpy(hdr_response->dmac, hdr->smac, 6);
		    memcpy(hdr_response->smac, hdr->smac, 6);

		    hdr_response->ethertype = hdr->ethertype;
		    hdr_response->ethertype = htons(hdr_response->ethertype);

		    // <----

			memcpy(reponse_ether, hdr_response, ETHER_HDR_LENGTH);
		    memcpy(reponse_ether+ETHER_HDR_LENGTH, reponse_3, skb->total_len - ETHER_HDR_LENGTH);

		    free(reponse_3);
		    free(hdr_response);
		    return reponse_ether;

		case ARP:
			printf("Protocol ARP not implemented yet. Dropped.\n");
			return NULL;

		default:
			printf("Unknown layer 3 protocol. Dropped.\n");
			return NULL;
	}

	return NULL;
}
