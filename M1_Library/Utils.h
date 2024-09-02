#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>

void asmWait(uint16_t wait);
void asmWait(uint16_t outerLoopCount, uint16_t innerLoopCount);

#endif // UTILS_H