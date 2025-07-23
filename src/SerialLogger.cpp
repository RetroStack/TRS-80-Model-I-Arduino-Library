#include "SerialLogger.h"

/**
 * Logs a formatted string to the serial interface
 */
void SerialLogger::_log(const char *fmt, va_list arguments)
{
    const int LEN = 255;
    char buffer[LEN];
    vsnprintf(buffer, LEN, fmt, arguments);

    Serial.println(buffer);
}

/**
 * Write a byte to the serial logger and return byte written.
 */
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

/**
 * Write a list of bytes to the serial logger and returns the number of bytes written.
 */
size_t SerialLogger::write(const uint8_t *buffer, size_t size)
{
    size_t result = 0;
    for (uint16_t i = 0; i < size; i++)
    {
        result += write(buffer[i]);
    }
    return result;
}

/**
 * Logs a formatted info string
 */
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

/**
 * Logs a formatted warning string
 */
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

/**
 * Logs a formatted error string
 */
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

void SerialLogger::mute()
{
    _silent = true;
}
void SerialLogger::unmute()
{
    _silent = false;
}
