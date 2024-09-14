#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>

char *uint8ToBinary(uint8_t value, char *buffer);
char pinStatus(bool value);
char busStatus(uint8_t value);

void asmNoop();

void asmWait(uint16_t wait);
void asmWait(uint16_t outerLoopCount, uint16_t innerLoopCount);

#endif // UTILS_H