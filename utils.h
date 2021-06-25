#ifndef UTILS_H
#define UTILS_H

#include "syshead.h"

void printBits(unsigned char num, int start, int bits);

uint16_t checksum(void *addr, int count, int start_sum);

#endif