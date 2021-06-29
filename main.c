#include "utils.h"
#include "syshead.h"
#include "tap.h"
#include "ether.h"
#include "netdevice.h"

void intHandler() {
    printf("\n\nShutting down:\n");
    printf("Closing Tap: ");
    free_tap();
    printf("DONE\n");

    printf("Shutdown Successful. Goodbye!\n");
    exit(0);
}


int main()
{

    // setup  --->
    signal(SIGINT, intHandler);

    tap_alloc();

    struct net_device* netdev = netdev_init("10.0.0.3", "jo:e0:ba:ye:r0:25");

    netdev_loop(netdev);

    // <---



    return 0;
}