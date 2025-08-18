/*
 * SerialLogger.cpp - Class for logging to the serial interface
 * Authors: Ven Reddy, Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#include "SerialLogger.h"

// Internal logging method with format string and arguments
void SerialLogger::_log(const char *fmt, va_list arguments)
{
    const int LEN = 255;
    char buffer[LEN];
    vsnprintf(buffer, LEN, fmt, arguments);

    Serial.println(buffer);
}

// Write single character to serial output
size_t SerialLogger::write(uint8_t ch)
{
    if (ch == '\n')
    {
        return Serial.println();
    }
    else
    {
        return Serial.print((char)ch);
    }
}

// Write buffer to serial output
size_t SerialLogger::write(const uint8_t *buffer, size_t size)
{
    size_t result = 0;
    for (uint16_t i = 0; i < size; i++)
    {
        result += write(buffer[i]);
    }
    return result;
}

// Log info message with format string
void SerialLogger::info(const char *fmt, ...)
{
    if (_silent)
        return;

    va_list arguments;
    va_start(arguments, fmt);

    Serial.print("[INFO] ");
    _log(fmt, arguments);

    va_end(arguments);
}

// Log warning message with format string
void SerialLogger::warn(const char *fmt, ...)
{
    if (_silent)
        return;

    va_list arguments;
    va_start(arguments, fmt);

    Serial.print("[WARN] ");
    _log(fmt, arguments);

    va_end(arguments);
}

// Log error message with format string
void SerialLogger::err(const char *fmt, ...)
{
    if (_silent)
        return;

    va_list arguments;
    va_start(arguments, fmt);

    Serial.print("[ERR ] ");
    _log(fmt, arguments);

    va_end(arguments);
}

// Log debug message with format string
void SerialLogger::debug(const char *fmt, ...)
{
    if (_silent)
        return;

    va_list arguments;
    va_start(arguments, fmt);

    Serial.print("[DBUG] ");
    _log(fmt, arguments);

    va_end(arguments);
}

// Disable logging output
void SerialLogger::mute()
{
    _silent = true;
}

// Enable logging output
void SerialLogger::unmute()
{
    _silent = false;
}
