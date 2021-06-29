#include "arp.h"

struct arp_entry* arp_cache[ARP_MAX_ENTRY];
int arp_cache_entry_count = 0;

void arp_ntohs(struct arp_hdr* a_hdr){

	a_hdr->hwtype = ntohs(a_hdr->hwtype);
	a_hdr->opcode = ntohs(a_hdr->opcode);
	a_hdr->protype = ntohs(a_hdr->protype);
}

void arp_content_ntohs(struct arp_content* content){

	content->sip = ntohl(content->sip);
	content->dip = ntohl(content->dip);
}

void arp_content_htons(struct arp_content* content){

	content->sip = htonl(content->sip);
	content->dip = htonl(content->dip);
}

void arp_htons(struct arp_hdr* a_hdr){

	a_hdr->hwtype = htons(a_hdr->hwtype);
	a_hdr->opcode = htons(a_hdr->opcode);
	a_hdr->protype = htons(a_hdr->protype);
}

int arp_add_entry(struct arp_content* arp){

	// TODO: Improved ARP cache system. and support for HWtype.
	if(arp_cache_entry_count == ARP_MAX_ENTRY){
		printf("ARP cache is full!\n");
		return -1;
	}

	// check if arp entry already exists for givn IP
	for (int i = 0; i < arp_cache_entry_count; ++i)
	{
		if(arp_cache[i]->sip == arp->sip){
			return 0;
		}
	}

	// add new entry;
	struct arp_entry* arp_entry = malloc(sizeof(struct arp_entry));

	memcpy(arp_entry->smac, arp->smac, 6);
	arp_entry->sip = arp->sip;

	arp_cache[arp_cache_entry_count] = arp_entry;
	arp_cache_entry_count++;

	printf("ARP entry: ");
		printf("MAC: %.2hhx:%.2hhx:%.2hhx:%.2hhx:%.2hhx:%.2hhx IP: ", arp_entry->smac[0],arp_entry->smac[1],arp_entry->smac[2],arp_entry->smac[3],arp_entry->smac[4], arp_entry->smac[5]);
		print_ip(htonl(arp_entry->sip));
		printf("\n");
	return 0;
}

void arp_send(struct sk_buff* skb){

	struct arp_hdr a_hdr;
	struct arp_content a_content;

	memcpy(&a_hdr, skb->a_hdr, sizeof(struct arp_hdr));
	a_hdr.opcode = ARP_REPLY;

	// MAC
	memcpy(a_content.smac, skb->netdev->hmac, 6);

	struct arp_content* sarp_content = (struct arp_content*) skb->data;

	memcpy(a_content.dmac, sarp_content->smac, 6);
	// IP
	a_content.sip = ntohl(skb->netdev->ipaddr);
	a_content.dip = sarp_content->sip;

	skb->dip = sarp_content->sip;


	int arp_content_length = sizeof(struct arp_hdr) + sizeof(struct arp_content);
	char* arp_reponse_content = malloc(arp_content_length);

	arp_htons(&a_hdr);
	arp_content_htons(&a_content);

	memcpy(arp_reponse_content, &a_hdr, sizeof(struct arp_hdr));
	memcpy(arp_reponse_content+sizeof(struct arp_hdr), &a_content, sizeof(struct arp_content));

	free(skb->head);	

	skb->data = (uint8_t*) arp_reponse_content;

	skb->len = arp_content_length;
	skb->protocol = ARP;

	printf("ARP request from ");
		print_ip(htonl(skb->dip));
		printf(" -> [RESPONDED].\n");

	ether_send(skb);

}

int arp_search(uint32_t ip, uint8_t* inputmac){

	for (int i = 0; i < arp_cache_entry_count; ++i)
	{
		if(arp_cache[i]->sip == ip){
			memcpy(inputmac, arp_cache[i]->smac, 6);
			return 0;
		}
	}
	printf("ARP entry was not found.\n");
	return -1;

}

void arp_handle(struct sk_buff* skb){
	struct arp_content* arp_content = (struct arp_content*) skb->data;

	arp_content_ntohs(arp_content);

	arp_add_entry(arp_content);

	arp_send(skb);
}

void arp_parse(struct sk_buff* skb){

	skb->data = skb->payload;

	struct arp_hdr* a_hdr = (struct apr_hdr*) skb->data;
	skb->a_hdr = a_hdr;
	skb->data = skb->data + sizeof(struct arp_hdr);


	arp_ntohs(a_hdr);

	if(a_hdr->opcode == ARP_REQUEST && a_hdr->hwtype == ARP_ETHERNET && a_hdr->protype == ARP_IPV4){

		arp_handle(skb);
		return;
	}

}


void arp_free(){
	for (int i = 0; i < arp_cache_entry_count; ++i)
	{
		free(arp_cache[i]);
	}
}