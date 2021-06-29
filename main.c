#include "utils.h"
#include "syshead.h"
#include "tap.h"
#include "ether.h"
#include "netdevice.h"
#include "arp.h"

struct net_device* netdev;

void intHandler() {
    printf("\n\nShutting down:\n");
    printf("Closing Tap: ");
    free_tap();
    printf("DONE\n");

    printf("Freeing ARP cache: ");
    arp_free();
    printf("DONE\n");

    printf("Closing Netdev: ");
    free_netdev(netdev);
    printf("DONE\n");



    printf("Shutdown Successful. Goodbye!\n");

    exit(0);
}


int main()
{

    printf("%d\n", getpid());

    // setup  --->
    signal(SIGINT, intHandler);

    tap_alloc();

    netdev = netdev_init("10.0.0.3", "01:00:01:00:01:25");

    netdev_loop(netdev);

    // <---



    return 0;
}