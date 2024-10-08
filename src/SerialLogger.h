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

    void mute();
    void unmute();
};

#endif /* SERIAL_LOGGER_H */