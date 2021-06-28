#include "utils.h"
#include "icmp.h"
#include "syshead.h"
#include "tap.h"
#include "skb.h"
#include "ether.h"


/* 

struct sockaddr_in sa;
char str[INET_ADDRSTRLEN];

// store this IP address in sa:
inet_pton(AF_INET, "192.0.2.33", &(sa.sin_addr));

// now get it back and print it
inet_ntop(AF_INET, &(sa.sin_addr), str, INET_ADDRSTRLEN);

printf("%s\n", str); // prints "192.0.2.33"


inet_addr() 
*/


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
            continue;
        }

        int wc = tun_write(reponse, skb->total_len);
        if(wc == 0){
            printf("Error sending packet.\n");
            free(reponse);
        } else {
            free(reponse);
        }

    }

    return 0;
}