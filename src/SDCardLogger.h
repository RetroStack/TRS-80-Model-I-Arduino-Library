/*
 * SerialLogger.h - Class for logging to the serial interface
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef SDCARD_LOGGER_H
#define SDCARD_LOGGER_H

#include <Arduino.h>
#include <SD.h>
#include "ILogger.h"
#include "M1Shield.h"

class SDCardLogger : public ILogger
{
private:
    const char *_filename;
    bool _silent = false; // Flag to suppress output when true

    void _log(const char *fmt, va_list arguments); // Internal logging implementation with variable arguments

public:
    SDCardLogger(const char *filename = "log.txt"); // Constructor with optional filename

    bool begin(); // Initialize SD card and log file

    void info(const char *fmt, ...);  // Log informational message to SD card
    void warn(const char *fmt, ...);  // Log warning message to SD card
    void err(const char *fmt, ...);   // Log error message to SD card
    void debug(const char *fmt, ...); // Log debug message to SD card

    using ILogger::debug;
    using ILogger::err;
    using ILogger::info;
    using ILogger::warn;

    size_t write(uint8_t ch) override;                         // Write single character to SD card
    size_t write(const uint8_t *buffer, size_t size) override; // Write buffer of characters to SD card

    void mute();   // Disable all logging output (silent mode)
    void unmute(); // Re-enable logging output

    using Print::print;
    using Print::println;
};

#endif /* SDCARD_LOGGER_H */