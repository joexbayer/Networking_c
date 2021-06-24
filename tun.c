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

            // calculate checksum, should be 0.
            uint16_t csum = checksum(hdr, hdr->ihl * 4, 0);

            if( 0 != csum){
                printf("Checksum failed (IPv4), dropping packet");
                continue;
            }

            // convert ip from network byte order to host byter order.
            ip_ntohl(hdr);

            printf("-------- Incomming -------\n");
            print_ip_packet(hdr);
            printf("-------- Incomming -------\n");

            char* ip_data = ip_get_data(buf, hdr);

            struct icmp* icmp = (struct icmp *) ip_data;

            short icmp_len = hdr->len - (hdr->ihl * 4);

            uint16_t csum_icmp = checksum(icmp, icmp_len, 0);
            if( 0 != csum_icmp){
                printf("Checksum failed (ICMP), dropping packet");
                continue;
            }

            char* icmp_data = icmp_get_data(ip_data, icmp_len);

            icmp_write(icmp, icmp_len);

            int icmp_data_length = icmp_len-sizeof(struct icmp);

            char icmp_respond_data[hdr->len];

            memcpy(icmp_respond_data, icmp, sizeof(struct icmp));
            memcpy(icmp_respond_data+sizeof(struct icmp), icmp_data, icmp_data_length);

            struct ip_hdr* ip_reponse = ip_send(hdr);


            int ip_header_length = hdr->ihl * 4;

            char reponse[hdr->len + ip_header_length];
            memcpy(reponse, ip_reponse, ip_header_length);
            memcpy(reponse+ip_header_length, icmp_respond_data, hdr->len);

            write(tun_fd, reponse, hdr->len + ip_header_length);

            struct ip_hdr *hdr_out = (struct ip_hdr *) reponse;
            ip_ntohl(hdr_out);

            printf("-------- Outgoing -------\n");
            print_ip_packet(hdr_out);
            printf("-------- Outgoing -------\n");


            free(icmp_data);
            free(ip_reponse);
        }
    }

    return 0;
}