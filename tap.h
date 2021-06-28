#ifndef TAP_H
#define TAP_H value

#include "syshead.h"

void tap_alloc();

uint8_t* tap_read();
int tap_write(char* buf, int length);

void free_tap();

#endif