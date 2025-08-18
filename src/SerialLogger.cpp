/*
 * SerialLogger.cpp - Class for logging to the serial interface
 * Authors: Ven Reddy, Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#include "SerialLogger.h"

void SerialLogger::_log(const char *fmt, va_list arguments)
{
    const int LEN = 255;
    char buffer[LEN];
    vsnprintf(buffer, LEN, fmt, arguments);

    Serial.println(buffer);
}

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

size_t SerialLogger::write(const uint8_t *buffer, size_t size)
{
    size_t result = 0;
    for (uint16_t i = 0; i < size; i++)
    {
        result += write(buffer[i]);
    }
    return result;
}

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

void SerialLogger::mute()
{
    _silent = true;
}
void SerialLogger::unmute()
{
    _silent = false;
}
