#include "ipv4.h"


void ip_ntohl(struct ip_hdr *hdr)
{
    hdr->saddr = ntohl(hdr->saddr);
    hdr->daddr = ntohl(hdr->daddr);
    hdr->len = ntohs(hdr->len);
    hdr->id = ntohs(hdr->id);
}

void ip_htonl(struct ip_hdr* ihdr){
    ihdr->len = htons(ihdr->len);
    ihdr->id = htons(ihdr->id);
    ihdr->daddr = htonl(ihdr->daddr);
    ihdr->saddr = ihdr->saddr;
    ihdr->csum = htons(ihdr->csum);
    ihdr->frag_offset = htons(ihdr->frag_offset);
}


void ip_handle(struct sk_buff* skb){

    skb->data = skb->payload+(skb->hdr->ihl*4);

    if(skb->hdr->proto == UDP){
        //printf("Protocol UDP not implemented yet. Dropped.\n");
        free(skb);
        free(skb->head);
        return;
    }

    if(ntohl(skb->hdr->daddr) != skb->netdev->ipaddr){
        return;
    }

    if(skb->hdr->proto == ICMPV4){
            // define icmp_len
        skb->len = skb->hdr->len - (skb->hdr->ihl * 4);
        skb->total_len = ETHER_HDR_LENGTH + skb->len + (skb->hdr->ihl * 4);
        
        // pass data to icmp, icmp_parse returns own packet data
        icmp_parse(skb);
        return;
    }

    printf("Protocol %d implemented yet.\n", skb->hdr->proto);
}



void ip_parse(struct sk_buff* skb){

    skb->protocol = IPV4;

    struct ip_hdr* hdr = (struct ip_hdr* ) skb->payload;
    skb->hdr = hdr;

    // calculate checksum, should be 0.
    uint16_t csum = checksum(hdr, hdr->ihl * 4, 0);
    if( 0 != csum){
        printf("Checksum failed (IPv4), returning NULL\n");
        free(skb);
        return;
    }

    // convert ip from network byte order to host byter order.
    ip_ntohl(hdr);

    ip_handle(skb);
    return;

    printf("IPv4: Route to ");
    //print_ip(skb->hdr->daddr);
    printf("%s (NOT IMPLEMENTED)\n");



}

void ip_send(struct sk_buff* skb){

    struct ip_hdr ihdr;
    // do I need to do this?
    ihdr.version = IPV4;
    ihdr.ihl = 0x05;
    ihdr.tos = 0;
    ihdr.len = skb->hdr->len;
    ihdr.frag_offset = 0x4000;
    ihdr.ttl = 64;
    ihdr.proto = skb->protocol;

    // set soure address to netdevice
    ihdr.saddr = skb->netdev->ipaddr;

    //doesnt work like this in real!
    ihdr.daddr = skb->hdr->saddr;
    skb->dip = skb->hdr->saddr;

    // from host to network
    ip_htonl(&ihdr);

    ihdr.csum = 0;
    ihdr.csum = checksum(&ihdr, ihdr.ihl * 4, 0);

    // ip header length
    int ip_header_length = skb->hdr->ihl * 4;
    // create reponse
    char* reponse = malloc(skb->hdr->len + ip_header_length);
    memcpy(reponse, &ihdr, ip_header_length);
    memcpy(reponse+ip_header_length, skb->data, skb->hdr->len);

    free(skb->data);
    skb->data = (uint8_t *)reponse;
    skb->len = skb->len + ihdr.ihl * 4;

    skb->protocol = IP;

    ether_send(skb);
}

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
    printf("%d.%d.%d.%d", bytes[3], bytes[2], bytes[1], bytes[0]);        
}