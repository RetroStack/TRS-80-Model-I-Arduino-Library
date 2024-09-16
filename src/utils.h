/*
 * Keyboard.h - File to manage utility functions used throughout the library
 * Authors: Ven Reddy, Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

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