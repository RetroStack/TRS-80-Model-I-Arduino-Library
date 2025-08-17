/*
 * utils.h - File to manage utility functions used throughout the library
 * Authors: Ven Reddy, Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>

/**
 * Wait for exactly 1 CPU cycles (1x nop), total delay:
 *   - 16 MHz CPU: 62.5 ns
 *   - Each nop = 62.5 ns
 */
#define asmShortNoop() __asm__ __volatile__("nop")

/**
 * Wait for exactly 2 CPU cycles (2x nop), total delay:
 *   - 16 MHz CPU: 125 ns
 *   - Each nop = 62.5 ns
 */
#define asmNoop() __asm__ __volatile__("nop\nnop")

char *uint8ToBinary(uint8_t value, char *buffer);
char *uint16ToBinary(uint16_t value, char *buffer);
char pinStatus(bool value);
char busStatus(uint8_t value);

void asmWait(uint16_t wait);
void asmWait(uint16_t outerLoopCount, uint16_t innerLoopCount);

#endif // UTILS_H
