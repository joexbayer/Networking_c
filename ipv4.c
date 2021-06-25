#include "ipv4.h"


// print out a ipv4 packet in a readable format.
void print_ip_packet(struct ip_hdr* hdr){
   printf("IPv4 (ihl: %hhu version: %hhu tos: %hhu "   \
                    "len %hu id: %hu frag_offset: %hu ttl: %hhu " \
                    "proto: %hhu csum: %hx)\n", \
                    hdr->ihl, hdr->version, hdr->tos, hdr->len, hdr->id, hdr->frag_offset, hdr->ttl, hdr->proto, hdr->csum);
   printf("Source: ");
   print_ip(ntohl(hdr->saddr));
   printf("Destination: ");
   print_ip(ntohl(hdr->daddr));

}


//print out ip, needs to be ntohl
void print_ip(unsigned int ip)
{
    unsigned char bytes[4];
    bytes[0] = (ip >> 24) & 0xFF;
    bytes[1] = (ip >> 16) & 0xFF;
    bytes[2] = (ip >> 8) & 0xFF;
    bytes[3] = ip & 0xFF;   
    printf("%d.%d.%d.%d\n", bytes[3], bytes[2], bytes[1], bytes[0]);        
}

void ip_ntohl(struct ip_hdr *hdr)
{
    hdr->saddr = ntohl(hdr->saddr);
    hdr->daddr = ntohl(hdr->daddr);
    hdr->len = ntohs(hdr->len);
    hdr->id = ntohs(hdr->id);
}

char* ip_handle(struct sk_buff* skb){

    skb->data = skb->payload+(skb->hdr->ihl*4);


    if(skb->hdr->proto == UDP){
        printf("Protocol UDP implemented yet.\n");
        return NULL;
    }

    if(skb->hdr->proto == ICMPV4){
            // define icmp_len
        skb->len = skb->hdr->len - (skb->hdr->ihl * 4);
        
        // pass data to icmp, icmp_parse returns own packet data
        char* icmp_data = icmp_parse(skb);
        if(icmp_data == NULL){
            return NULL;
        }

        //do better!!
        struct ip_hdr* ip_reponse = ip_send(skb->hdr);

        // ip header length
        int ip_header_length = skb->hdr->ihl * 4;

        // create reponse
        char* reponse = malloc(skb->hdr->len + ip_header_length);
        memcpy(reponse, ip_reponse, ip_header_length);
        memcpy(reponse+ip_header_length, icmp_data, skb->hdr->len);

        free(ip_reponse);

        return reponse;
    }

    printf("Protocol %d implemented yet.\n", skb->hdr->proto); 
    return NULL;
}



char* ip_parse(struct sk_buff* skb){

    skb->protocol = IPV4;

    struct ip_hdr* hdr = (struct ip_hdr* ) skb->payload;
    skb->hdr = hdr;

    // calculate checksum, should be 0.
    uint16_t csum = checksum(hdr, hdr->ihl * 4, 0);
    if( 0 != csum){
        printf("Checksum failed (IPv4), returning NULL\n");
        return NULL;
    }

    // convert ip from network byte order to host byter order.
    ip_ntohl(hdr);

    printf("-------- Incomming -------\n");
    print_ip_packet(hdr);

    return ip_handle(skb);
}

struct ip_hdr* ip_send(struct ip_hdr* ihdr_in){
      struct ip_hdr* ihdr = malloc(sizeof(struct ip_hdr));

      ihdr->version = IPV4;
      ihdr->ihl = 0x05;
      ihdr->tos = 0;
      // fix len
      ihdr->len = ihdr_in->len;
      ihdr->frag_offset = 0x4000;
      ihdr->ttl = 64;
      ihdr->proto = ihdr_in->proto;
      ihdr->saddr = ihdr_in->daddr;
      ihdr->daddr = ihdr_in->saddr;
      ihdr->csum = 0;

        
      ihdr->len = htons(ihdr->len);
      ihdr->id = htons(ihdr_in->id);
      ihdr->daddr = htonl(ihdr->daddr);
      ihdr->saddr = htonl(ihdr->saddr);
      ihdr->csum = htons(ihdr->csum);
      ihdr->frag_offset = htons(ihdr->frag_offset);

      ihdr->csum = checksum(ihdr, ihdr->ihl * 4, 0);

      return ihdr;
}