/*
 * Arduino.h - Minimal stub for native (host) tests
 *
 * Just enough of the Arduino environment to compile the parts of this library
 * that do not touch hardware. Only fixed-width integer types are needed.
 *
 * Authors: Marcel Erz (RetroStack)
 * License: MIT
 */

#ifndef TEST_STUB_ARDUINO_H
#define TEST_STUB_ARDUINO_H

#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cstdlib>

// Flash strings. On AVR these live in program memory and need the _P family to
// read; on the host there is one address space, so they are ordinary pointers
// and the _P functions are their plain equivalents.
class __FlashStringHelper;
#define PROGMEM
#define F(string_literal) (reinterpret_cast<const __FlashStringHelper *>(string_literal))

using std::free;
using std::malloc;

inline size_t strlen_P(const char *s) { return std::strlen(s); }
inline char *strcpy_P(char *dest, const char *src) { return std::strcpy(dest, src); }

#endif // TEST_STUB_ARDUINO_H
