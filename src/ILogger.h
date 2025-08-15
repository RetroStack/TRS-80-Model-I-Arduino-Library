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
    void info(const String &msg) { info("%s", msg.c_str()); }
    void warn(const String &msg) { warn("%s", msg.c_str()); }
    void err(const String &msg) { err("%s", msg.c_str()); }

    // F() macro versions that work directly with flash strings (more memory efficient)
    void info(const __FlashStringHelper *msg)
    {
        size_t len = strlen_P((const char *)msg);
        if (len > 0)
        {
            char buffer[len + 1]; // +1 for null terminator
            strcpy_P(buffer, (const char *)msg);
            buffer[len] = '\0'; // Ensure null termination
            info("%s", buffer);
        }
    }
    void warn(const __FlashStringHelper *msg)
    {
        size_t len = strlen_P((const char *)msg);
        if (len > 0)
        {
            char buffer[len + 1]; // +1 for null terminator
            strcpy_P(buffer, (const char *)msg);
            buffer[len] = '\0'; // Ensure null termination
            warn("%s", buffer);
        }
    }
    void err(const __FlashStringHelper *msg)
    {
        size_t len = strlen_P((const char *)msg);
        if (len > 0)
        {
            char buffer[len + 1]; // +1 for null terminator
            strcpy_P(buffer, (const char *)msg);
            buffer[len] = '\0'; // Ensure null termination
            err("%s", buffer);
        }
    }

    virtual size_t write(uint8_t ch) = 0;
    virtual size_t write(const uint8_t *buffer, size_t size) = 0;
};

#endif /* ILOGGER_H */