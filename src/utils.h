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

// Convert 8-bit value to binary string representation
char *uint8ToBinary(uint8_t value, char *buffer);

// Convert 16-bit value to binary string representation
char *uint16ToBinary(uint16_t value, char *buffer);

// Get pin status character ('o' for output, 'i' for input)
char pinStatus(bool value);

// Get bus status character ('o' for output, 'i' for input, '?' for unknown)
char busStatus(uint8_t value);

// Precise nanosecond delay using inline assembly (16MHz ATMega)
void asmWait(uint16_t wait);

// Nested loop delay for longer durations using inline assembly
void asmWait(uint16_t outerLoopCount, uint16_t innerLoopCount);

#endif // UTILS_H
