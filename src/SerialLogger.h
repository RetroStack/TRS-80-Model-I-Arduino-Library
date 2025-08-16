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
    bool _silent = false;

    void _log(const char *fmt, va_list arguments);

public:
    void info(const char *fmt, ...);
    void warn(const char *fmt, ...);
    void err(const char *fmt, ...);
    void debug(const char *fmt, ...);

    using ILogger::err;
    using ILogger::info;
    using ILogger::warn;
    using ILogger::debug;

    size_t write(uint8_t ch) override;
    size_t write(const uint8_t *buffer, size_t size) override;

    void mute();
    void unmute();

    using Print::print;
    using Print::println;
};

#endif /* SERIAL_LOGGER_H */