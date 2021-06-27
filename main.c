#include "utils.h"
#include "icmp.h"
#include "syshead.h"
#include "tun.h"
#include "skb.h"
#include "ether.h"


int main()
{
    tun_alloc();

    while(1){

        uint8_t* buf = tun_read();
        if (buf == NULL){
            //printf("Error reading packet. Dropped.\n");
            continue;
        }

        struct sk_buff* skb = alloc_skb();
        skb->payload = buf;
        skb->head = buf;

        char* reponse = ether_parse(skb);
        if(reponse == NULL){
            printf("Error reading packet. Dropped.\n");
            continue;
        }

        int wc = tun_write(reponse, skb->total_len);
        if(wc == 0){
            printf("Error sending packet.\n");
        }
        free(reponse);
    }

    return 0;
}