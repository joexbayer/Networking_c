#include "utils.h"
#include "syshead.h"
#include "tap.h"
#include "ether.h"
#include "netdevice.h"


struct sk_buff* skb_store;
char* reponse_store;

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

void intHandler() {
    printf("\n\nShutting down:\n");
    printf("Closing Tap: ");
    free_tap();
    printf("DONE\n");

    printf("Closing Net device: ");
    free_netdev(skb_store->netdev);
    printf("DONE\n");

    printf("Closing Socket Buffer: ");
    free_skb(skb_store);
    printf("DONE\n");
    free(reponse_store);
    printf("Shutdown Successful. Goodbye!\n");
    exit(0);
}


int main()
{

    signal(SIGINT, intHandler);


    tap_alloc();
    struct net_device* netdev = netdev_init("10.0.0.3", "jo:e0:ba:ye:r0:25");

    while(1){

        uint8_t* buf = tap_read();
        if (buf == NULL){
            //printf("Error reading packet. Dropped.\n");
            continue;
        }

        struct sk_buff* skb = alloc_skb(netdev);
        skb->payload = buf;
        skb->head = buf;

        skb_store = skb;

        char* reponse = ether_parse(skb);
        if(reponse == NULL){
            continue;
        }
        reponse_store = reponse;

        int wc = tap_write(reponse, skb->total_len);
        if(wc == 0){
            printf("Error sending packet.\n");
            free(reponse);
        } else {
            free(reponse);
        }
        free_skb(skb);

    }

    return 0;
}