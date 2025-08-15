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

    // String versions with optional format arguments
    void info(const String &fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        
        char formatted[256]; // Fixed size buffer to avoid VLA
        vsnprintf(formatted, sizeof(formatted), fmt.c_str(), args);
        formatted[sizeof(formatted) - 1] = '\0'; // Ensure null termination
        info("%s", formatted);
        
        va_end(args);
    }
    
    void warn(const String &fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        
        char formatted[256]; // Fixed size buffer to avoid VLA
        vsnprintf(formatted, sizeof(formatted), fmt.c_str(), args);
        formatted[sizeof(formatted) - 1] = '\0'; // Ensure null termination
        warn("%s", formatted);
        
        va_end(args);
    }
    
    void err(const String &fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        
        char formatted[256]; // Fixed size buffer to avoid VLA
        vsnprintf(formatted, sizeof(formatted), fmt.c_str(), args);
        formatted[sizeof(formatted) - 1] = '\0'; // Ensure null termination
        err("%s", formatted);
        
        va_end(args);
    }

    // F() macro versions with format string support (more memory efficient)
    void infoF(const __FlashStringHelper *fmt, ...)
    {
        va_list args;
        va_start(args, fmt);

        size_t len = strlen_P((const char *)fmt);
        if (len > 0)
        {
            char buffer[len + 1]; // +1 for null terminator
            strcpy_P(buffer, (const char *)fmt);
            buffer[len] = '\0'; // Ensure null termination

            // Use a temporary buffer for formatted output
            char formatted[256]; // Fixed size buffer to avoid VLA
            vsnprintf(formatted, sizeof(formatted), buffer, args);
            formatted[sizeof(formatted) - 1] = '\0'; // Ensure null termination
            info("%s", formatted);
        }

        va_end(args);
    }

    void warnF(const __FlashStringHelper *fmt, ...)
    {
        va_list args;
        va_start(args, fmt);

        size_t len = strlen_P((const char *)fmt);
        if (len > 0)
        {
            char buffer[len + 1]; // +1 for null terminator
            strcpy_P(buffer, (const char *)fmt);
            buffer[len] = '\0'; // Ensure null termination

            // Use a temporary buffer for formatted output
            char formatted[256]; // Fixed size buffer to avoid VLA
            vsnprintf(formatted, sizeof(formatted), buffer, args);
            formatted[sizeof(formatted) - 1] = '\0'; // Ensure null termination
            warn("%s", formatted);
        }

        va_end(args);
    }

    void errF(const __FlashStringHelper *fmt, ...)
    {
        va_list args;
        va_start(args, fmt);

        size_t len = strlen_P((const char *)fmt);
        if (len > 0)
        {
            char buffer[len + 1]; // +1 for null terminator
            strcpy_P(buffer, (const char *)fmt);
            buffer[len] = '\0'; // Ensure null termination

            // Use a temporary buffer for formatted output
            char formatted[256]; // Fixed size buffer to avoid VLA
            vsnprintf(formatted, sizeof(formatted), buffer, args);
            formatted[sizeof(formatted) - 1] = '\0'; // Ensure null termination
            err("%s", formatted);
        }

        va_end(args);
    }

    virtual size_t write(uint8_t ch) = 0;
    virtual size_t write(const uint8_t *buffer, size_t size) = 0;
};

#endif /* ILOGGER_H */