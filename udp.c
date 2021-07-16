#include "udp.h"

void udp_ntohs(struct udp_hdr* hdr){

	hdr->srcport = ntohs(hdr->srcport);
	hdr->destport = ntohs(hdr->destport);
	hdr->udp_length = ntohs(hdr->udp_length);
	hdr->checksum = ntohs(hdr->checksum);

}

void udp_handle(struct sk_buff* skb){

	if(skb->udp_hdr->destport == 8080){

		int payloadsize = skb->udp_hdr->udp_length-sizeof(struct udp_hdr);
		char* payload = malloc(payloadsize);
		memcpy(payload, skb->data, payloadsize);

		// add null byte.
		payload[payloadsize] = '\0';

		printf("UDP Port: %d -> %s", skb->udp_hdr->destport, payload);

	}

}

void upd_parse(struct sk_buff* skb){

	struct udp_hdr* hdr = (struct udp_hdr* ) skb->data;

	skb->udp_hdr = hdr;

	skb->data = skb->data + sizeof(struct udp_hdr);

	udp_ntohs(hdr);

	udp_handle(skb);

}