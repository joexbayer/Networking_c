#include "tap.h"


static int tap_fd;


/*
 * Open a FD from /dev/tap12
 * Tap virtual network interface configuration.
 * From: http://tuntaposx.sourceforge.net/
 */
void tap_alloc()
{
    int fd;

    if( (fd = open("/dev/tap12", O_RDWR)) < 0 ) {
        perror("Cannot open tap/TAP dev\n");
        exit(1);
    }

    system("ifconfig tap12 inet 10.0.0.1 10.0.0.255 up");

    printf("Tap has been opened: /dev/tap12\n\n");
    printf("Interface Configuration:\nDefault Gateway: 10.0.0.1\nNetmask: 255.255.255.0\nBroadcast 10.0.0.255\n\n");

    tap_fd = fd;
}

uint8_t* tap_read(){
    // set FD sets
    fd_set readSockSet;
    FD_ZERO(&readSockSet);
    FD_SET(tap_fd, &readSockSet);

    // select for http_client
    int retval = select(FD_SETSIZE, &readSockSet, NULL, NULL, NULL);
    if(retval > 0){
        uint8_t* buf = malloc(4000);
        read(tap_fd, buf, 4000);

        return buf;
    }
    return NULL;
}

int tap_write(char* buf, int length){
    return write(tap_fd, buf, length);
}

void free_tap()
{
    close(tap_fd);
}