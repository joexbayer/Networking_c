
#include "icmp.h"
#include "utils.h"

char* icmp_get_data(char* buf, int total_size){


    char* data = malloc(total_size-sizeof(struct icmp));

    memcpy(data, buf+sizeof(struct icmp), total_size-sizeof(struct icmp));

    return data;
}

char* icmp_handle(struct icmp* icmp, short length, char* buf){
    char* icmp_data = icmp_get_data(buf, length);

    icmp_write(icmp, length);

    int icmp_data_length = length-sizeof(struct icmp);

    char* icmp_respond_data = malloc(length);

    memcpy(icmp_respond_data, icmp, sizeof(struct icmp));
    memcpy(icmp_respond_data+sizeof(struct icmp), icmp_data, icmp_data_length);

    return icmp_respond_data;

}  

char* icmp_parse(char* buf, short length){

    struct icmp* icmp_hdr = (struct icmp * ) buf;

    memcpy(icmp_hdr, buf, length);
    // calculate checksum, should be 0.

    uint16_t csum_icmp = checksum(icmp_hdr, length, 0);
    if( 0 != csum_icmp){
        printf("Checksum failed (ICMP), returning NULL");
        return NULL;
    }

    return icmp_handle(icmp_hdr, length, buf);
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

