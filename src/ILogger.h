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
    // The format attribute makes the compiler check these call sites. Index 2 is
    // the format and 3 the first variadic argument, because `this` is index 1.
    virtual void info(const char *fmt, ...) __attribute__((format(printf, 2, 3))) = 0;  // Log informational message with format string
    virtual void warn(const char *fmt, ...) __attribute__((format(printf, 2, 3))) = 0;  // Log warning message with format string
    virtual void err(const char *fmt, ...) __attribute__((format(printf, 2, 3))) = 0;   // Log error message with format string
    virtual void debug(const char *fmt, ...) __attribute__((format(printf, 2, 3))) = 0; // Log debug message with format string

    // String versions. These take the message itself, never a format string: a
    // String is usually built at runtime, and a runtime format string turns any
    // stray '%' in it -- an SD filename, a line read from a file -- into a
    // directive that consumes arguments that were never passed.

    // Log informational message from Arduino String object
    void info(const String &message)
    {
        info("%s", message.c_str());
    }

    // Log warning message from Arduino String object
    void warn(const String &message)
    {
        warn("%s", message.c_str());
    }

    // Log error message from Arduino String object
    void err(const String &message)
    {
        err("%s", message.c_str());
    }

    // Log debug message from Arduino String object
    void debug(const String &message)
    {
        debug("%s", message.c_str());
    }

    // F() macro versions with format string support (more memory efficient)

    // Log informational message from FlashString (F() macro)
    void infoF(const __FlashStringHelper *fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        _formatFlash(INFO, fmt, args);
        va_end(args);
    }

    // Log warning message from FlashString (F() macro)
    void warnF(const __FlashStringHelper *fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        _formatFlash(WARN, fmt, args);
        va_end(args);
    }

    // Log error message from FlashString (F() macro)
    void errF(const __FlashStringHelper *fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        _formatFlash(ERR, fmt, args);
        va_end(args);
    }

    // Log debug message from FlashString (F() macro)
    void debugF(const __FlashStringHelper *fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        _formatFlash(DEBUG, fmt, args);
        va_end(args);
    }

protected:
    enum Level
    {
        INFO,
        WARN,
        ERR,
        DEBUG
    };

    void _dispatch(Level level, const char *message)
    {
        switch (level)
        {
        case INFO:
            info("%s", message);
            break;
        case WARN:
            warn("%s", message);
            break;
        case ERR:
            err("%s", message);
            break;
        case DEBUG:
            debug("%s", message);
            break;
        }
    }

    // Formats a flash-held format string. The four F() methods used to carry a
    // copy of this each: a runtime-sized stack array for the format -- directly
    // under a comment claiming it avoided one -- plus a 256-byte buffer for the
    // result, on a part with 8KB of SRAM. vsnprintf_P reads the format straight
    // out of flash, so the copy is gone and only one buffer remains.
    void _formatFlash(Level level, const __FlashStringHelper *fmt, va_list args)
    {
        if (fmt == nullptr)
        {
            return;
        }

        char formatted[128];
        vsnprintf_P(formatted, sizeof(formatted), (const char *)fmt, args);
        formatted[sizeof(formatted) - 1] = '\0';
        _dispatch(level, formatted);
    }

public:
    virtual size_t write(uint8_t ch) = 0;                         // Write single character (Print interface)
    virtual size_t write(const uint8_t *buffer, size_t size) = 0; // Write buffer of characters (Print interface)
};

#endif /* ILOGGER_H */