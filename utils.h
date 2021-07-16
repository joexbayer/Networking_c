#ifndef UTILS_H
#define UTILS_H

#include "syshead.h"

void printBits(unsigned char num, int start, int bits);

uint16_t checksum(void *addr, int count, int start_sum);

uint16_t transport_checksum(uint32_t saddr, uint32_t daddr, uint8_t proto, uint8_t *data, uint16_t len);

#endif