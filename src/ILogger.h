/*
 * ILogger.h - Interface for a logger
 * Authors: Ven Reddy, Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef ILOGGER_H
#define ILOGGER_H

#include <Print.h>

class ILogger : public Print
{
public:
    virtual void info(const char *fmt, ...) = 0;
    virtual void warn(const char *fmt, ...) = 0;
    virtual void err(const char *fmt, ...) = 0;

    virtual size_t write(uint8_t ch) = 0;
    virtual size_t write(const uint8_t *buffer, size_t size) = 0;
};

#endif /* ILOGGER_H */