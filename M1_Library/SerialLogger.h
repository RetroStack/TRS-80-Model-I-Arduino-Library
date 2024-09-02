#ifndef SERIAL_LOGGER_H
#define SERIAL_LOGGER_H

#include <Arduino.h>

#include "./ILogger.h"

class SerialLogger : public ILogger
{
private:
    bool _silent = false;

    void _log(const char *fmt, ...);

public:
    void info(const char *fmt, ...);
    void warn(const char *fmt, ...);
    void err(const char *fmt, ...);

    void mute();
    void unmute();
};

#endif /* SERIAL_LOGGER_H */