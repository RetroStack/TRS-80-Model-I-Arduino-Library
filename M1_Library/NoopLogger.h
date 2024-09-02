#ifndef NOOP_LOGGER_H
#define NOOP_LOGGER_H

#include <Arduino.h>
#include "./ILogger.h"

class NoopLogger : public ILogger
{
public:
    void log(const char *fmt, ...);
    void warn(const char *fmt, ...);
    void err(const char *fmt, ...);
};

#endif /* NOOP_LOGGER_H */