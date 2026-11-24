#pragma once

#include <stdint.h>

extern const uint8_t rndtable[256];
extern uint32_t rndindex;
extern uint32_t prndindex;

uint8_t gameplay_random_uint8(void);
float gameplay_random_float(void);
uint8_t visual_random_uint8(void);
float visual_random_float(void);

void gameplay_random_reset(void);
void visual_random_reset(void);
void random_reset(void);
