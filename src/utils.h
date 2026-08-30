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

char *uint8ToBinary(uint8_t value, char *buffer);   // Convert 8-bit value to binary string representation
char *uint16ToBinary(uint16_t value, char *buffer); // Convert 16-bit value to binary string representation

char pinStatus(bool value);    // Get pin status character ('o' for output, 'i' for input)
char busStatus(uint8_t value); // Get bus status character ('o' for output, 'i' for input, '?' for unknown)

void asmWait(uint16_t wait);                                    // Precise nanosecond delay using inline assembly (16MHz ATMega)
void asmWait(uint16_t outerLoopCount, uint16_t innerLoopCount); // Nested loop delay for longer durations using inline assembly

uint16_t chunkLength(uint32_t offset, uint16_t total, uint16_t chunkSize); // Length of the chunk at an offset, 0 when the range is exhausted

bool normalizePath(const char *path, char *out, size_t outSize); // Collapse "." and ".." segments; returns false if the result does not fit
bool pathIsWithin(const char *path, const char *root);           // Component-wise containment test for two normalized paths

// Copies a flash string into RAM for the duration of a scope, then frees it.
// The copy-out-of-flash-and-delegate idiom was hand-written at a dozen sites,
// which had already drifted three ways: some used a heap buffer, one used a
// runtime-sized stack array, and only some logged an allocation failure.
class FlashBuffer
{
private:
    char *_buffer;

    // Copying would double-free; the buffer is owned by exactly one scope.
    FlashBuffer(const FlashBuffer &);
    FlashBuffer &operator=(const FlashBuffer &);

public:
    explicit FlashBuffer(const __FlashStringHelper *text);
    ~FlashBuffer();

    const char *c_str() const { return _buffer; } // nullptr if the copy failed
    bool valid() const { return _buffer != nullptr; }
};

#endif // UTILS_H
