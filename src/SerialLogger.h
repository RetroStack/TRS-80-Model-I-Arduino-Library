/*
 * SerialLogger.h - Class for logging to the serial interface
 * Authors: Ven Reddy, Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef SERIAL_LOGGER_H
#define SERIAL_LOGGER_H

#include <Arduino.h>
#include "ILogger.h"

class SerialLogger : public ILogger
{
private:
    bool _silent = false; // Flag to suppress output when true

    void _log(const char *level, const char *fmt, va_list arguments); // Internal logging implementation with variable arguments

public:
    void info(const char *fmt, ...) override __attribute__((format(printf, 2, 3)));  // Log informational message to serial output
    void warn(const char *fmt, ...) override __attribute__((format(printf, 2, 3)));  // Log warning message to serial output
    void err(const char *fmt, ...) override __attribute__((format(printf, 2, 3)));   // Log error message to serial output
    void debug(const char *fmt, ...) override __attribute__((format(printf, 2, 3))); // Log debug message to serial output

    using ILogger::debug;
    using ILogger::err;
    using ILogger::info;
    using ILogger::warn;

    size_t write(uint8_t ch) override;                         // Write single character to serial output
    size_t write(const uint8_t *buffer, size_t size) override; // Write buffer of characters to serial output

    void mute();   // Disable all logging output (silent mode)
    void unmute(); // Re-enable logging output

    using Print::print;
    using Print::println;
};

#endif /* SERIAL_LOGGER_H */