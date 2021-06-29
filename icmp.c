
#include "icmp.h"



void icmp_print(struct sk_buff* skb){
    unsigned char bytes[4];
    bytes[3] = (skb->hdr->saddr >> 24) & 0xFF;
    bytes[2] = (skb->hdr->saddr >> 16) & 0xFF;
    bytes[1] = (skb->hdr->saddr >> 8) & 0xFF;
    bytes[0] = skb->hdr->saddr & 0xFF;  

    printf("ICMP : %d bytes to %d.%d.%d.%d: icmp_seq= %d ttl=64 protocol: IPv4\n", skb->hdr->len - skb->hdr->ihl*4, bytes[3], bytes[2], bytes[1], bytes[0], skb->icmp->sequence/256);
}

void icmp_handle(struct sk_buff* skb){

    skb->data = skb->data + sizeof(struct icmp);

    // ICMP reply setup
    skb->icmp->type = ICMP_REPLY;
    skb->icmp->csum = 0;
    skb->icmp->csum = checksum(skb->icmp, skb->len, 0);

    // setup response
    int icmp_data_length = skb->len-sizeof(struct icmp);
    char* icmp_respond_data = malloc(skb->len);
    memcpy(icmp_respond_data, skb->icmp, sizeof(struct icmp));
    memcpy(icmp_respond_data+sizeof(struct icmp), skb->data, icmp_data_length);

    icmp_print(skb);

    // ---- RESPOND ----
    skb->data = (uint8_t *)icmp_respond_data;
    skb->protocol = ICMPV4;

    ip_send(skb);

}  

void icmp_parse(struct sk_buff* skb){


    struct icmp* icmp_hdr = (struct icmp * ) skb->data;
    skb->icmp = icmp_hdr;

    // calculate checksum, should be 0.
    uint16_t csum_icmp = checksum(icmp_hdr, skb->len, 0);
    if( 0 != csum_icmp){
        printf("Checksum failed (ICMP), returning NULL");
        return;
    }

    icmp_handle(skb);
}

void icmp_write(struct icmp* icmp, short length)
{

    if(icmp->type != ICMP_V4_ECHO){
        printf("Wrong ICMP type!\n");
        return;
    }

    short icmp_len = length;
        
    icmp->type = ICMP_REPLY;
    icmp->csum = 0;
    icmp->csum = checksum(icmp, icmp_len, 0);
}

