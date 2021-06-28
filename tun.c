#include "tun.h"


static int tun_fd;


/*
 * Open a FD from /dev/tun12
 * ifconfig it to use 10.0.0.2
 * Recieves pings.
 * ping 10.0.0.2
 */
void tun_alloc()
{
    int fd;

    if( (fd = open("/dev/tap12", O_RDWR)) < 0 ) {
        perror("Cannot open TUN/TAP dev\n");
        exit(1);
    }

    printf("Tun has been opened: /dev/tun12 (\n");

    system("ifconfig tap12 inet 10.0.0.1 10.0.0.255 up");

    printf("Tun has been configuered: inet 10.0.0.255 \n\n");

    printf("Use: 'nc -u  10.0.0.2 8080' to communicate with UDP (NOT WORKING)\n");

    printf("Use: 'ping 10.0.0.2' to communicate with ICMP\n\n");


    tun_fd = fd;
}

uint8_t* tun_read(){
    // set FD sets
    fd_set readSockSet;
    FD_ZERO(&readSockSet);
    FD_SET(tun_fd, &readSockSet);

    // select for http_client
    int retval = select(FD_SETSIZE, &readSockSet, NULL, NULL, NULL);
    if(retval > 0){
        uint8_t* buf = malloc(4000);
        read(tun_fd, buf, 4000);


        return buf;
    }
    return NULL;
}

int tun_write(char* buf, int length){
    return write(tun_fd, buf, length);
}

void free_tun()
{
    close(tun_fd);
}