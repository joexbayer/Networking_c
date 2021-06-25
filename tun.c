#include "syshead.h"
#include "utils.h"
#include "ipv4.h"
#include "icmp.h"

static int tun_fd;



/*
 * Open a FD from /dev/tun12
 * ifconfig it to use 10.0.0.2
 * Recieves pings.
 * ping 10.0.0.2
 */
static void tun_alloc()
{
    int fd;

    if( (fd = open("/dev/tun12", O_RDWR)) < 0 ) {
        perror("Cannot open TUN/TAP dev\n");
        exit(1);
    }

    printf("Tun has been opened: /dev/tun12 (\n");

    system("ifconfig tun12 inet 10.0.0.1 10.0.0.2 up");

    printf("Tun has been configuered: inet 10.0.0.2 \n\n");

    printf("Use: 'nc -u  10.0.0.2 8080' to communicate with UDP (NOT WORKING)\n");

    printf("Use: 'ping 10.0.0.2' to communicate with ICMP\n");


    tun_fd = fd;
}

char* tun_read(){
    // set FD sets
    fd_set readSockSet;
    FD_ZERO(&readSockSet);
    FD_SET(tun_fd, &readSockSet);

    // select for http_client
    int retval = select(FD_SETSIZE, &readSockSet, NULL, NULL, NULL);
    if(retval > 0){
        char* buf = malloc(4000);
        read(tun_fd, buf, 4000);

        printf("Incomming Tun packet! \n");

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

int main()
{
    tun_alloc();

    while(1){

        char* buf = tun_read();
        if (buf == NULL){
            printf("Error reading packet. Dropped.\n");
            continue;
        }

        char* reponse = ip_parse(buf);
        if(reponse == NULL){
            printf("Error reading packet. Dropped.\n");
            continue;
        }

        // bad -> 
        struct ip_hdr* hdr = (struct ip_hdr * ) buf;
        ip_ntohl(hdr);
        // <- fix with sk_buff

        int wc = tun_write(reponse, hdr->len + (hdr->ihl * 4));
        if(wc == 0){
            printf("Error sending packet.\n");
        }
        free(reponse);
    }

    return 0;
}