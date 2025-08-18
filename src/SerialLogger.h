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

    void _log(const char *fmt, va_list arguments); // Internal logging implementation with variable arguments

public:
    // Log informational message to serial output
    void info(const char *fmt, ...);

    // Log warning message to serial output
    void warn(const char *fmt, ...);

    // Log error message to serial output
    void err(const char *fmt, ...);

    // Log debug message to serial output
    void debug(const char *fmt, ...);

    using ILogger::debug;
    using ILogger::err;
    using ILogger::info;
    using ILogger::warn;

    // Write single character to serial output
    size_t write(uint8_t ch) override;

    // Write buffer of characters to serial output
    size_t write(const uint8_t *buffer, size_t size) override;

    // Disable all logging output (silent mode)
    void mute();

    // Re-enable logging output
    void unmute();

    using Print::print;
    using Print::println;
};

#endif /* SERIAL_LOGGER_H */