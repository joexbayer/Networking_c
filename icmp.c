
#include "icmp.h"
#include "utils.h"

char* icmp_get_data(char* buf, int total_size){


    char* data = malloc(total_size-sizeof(struct icmp));

    memcpy(data, buf+sizeof(struct icmp), total_size-sizeof(struct icmp));

    return data;
}

void icmp_write(struct icmp* icmp, short length)
{

    if(icmp->type != ICMP_V4_ECHO){
        printf("Wrong ICMP type!\n");
        return;
    }

    short icmp_len = length;
        
    icmp->type = ICMP_REPLY;
    icmp->csum = 0;
    icmp->csum = checksum(icmp, icmp_len, 0);
}

