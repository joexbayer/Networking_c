#include "syshead.h"
#include "utils.h"

static int tun_fd;



/*
 * Open a FD from /dev/tun12
 * ifconfig it to use 10.0.0.2
 * Recieves pings.
 * ping 10.0.0.2
 */
static int tun_alloc()
{
    int fd;

    if( (fd = open("/dev/tun12", O_RDWR)) < 0 ) {
        perror("Cannot open TUN/TAP dev\n");
        exit(1);
    }

    system("ifconfig tun12 inet 10.0.0.1 10.0.0.2 up");

    return fd;
}

void free_tun()
{
    close(tun_fd);
}

int main()
{
    tun_fd = tun_alloc();
    while(1){
            // set FD sets
        fd_set readSockSet;
        FD_ZERO(&readSockSet);
        FD_SET(tun_fd, &readSockSet);

        // select for http_client
        int retval = select(FD_SETSIZE, &readSockSet, NULL, NULL, NULL);
        if(retval > 0){
            char buf[4000];
            read(tun_fd, buf, 4000);
            struct ip_hdr *hdr = (struct ip_hdr *) buf;
            print_ip_packet(hdr);   
            printf("\n");
        }
    }

    return 0;
}