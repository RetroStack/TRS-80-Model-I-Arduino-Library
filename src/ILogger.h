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

    // String versions that call the const char* versions
    virtual void info(const String &msg) { info("%s", msg.c_str()); }
    virtual void warn(const String &msg) { warn("%s", msg.c_str()); }
    virtual void err(const String &msg) { err("%s", msg.c_str()); }

    // F() macro versions that work directly with flash strings (more memory efficient)
    virtual void info(const __FlashStringHelper *msg) { info(reinterpret_cast<const char *>(msg)); }
    virtual void warn(const __FlashStringHelper *msg) { warn(reinterpret_cast<const char *>(msg)); }
    virtual void err(const __FlashStringHelper *msg) { err(reinterpret_cast<const char *>(msg)); }

    virtual size_t write(uint8_t ch) = 0;
    virtual size_t write(const uint8_t *buffer, size_t size) = 0;
};

#endif /* ILOGGER_H */