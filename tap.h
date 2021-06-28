#ifndef TAP_H
#define TAP_H value

#include "syshead.h"

void tun_alloc();

uint8_t* tun_read();
int tun_write(char* buf, int length);

void free_tun();

#endif

