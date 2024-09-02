#include "./SerialLogger.h"

void SerialLogger::_log(const char *fmt, ...)
{
    va_list arguments;

    const int LEN = 255;
    char buffer[LEN];
    snprintf(buffer, LEN, fmt, arguments);

    Serial.println(buffer);
}

void SerialLogger::info(const char *fmt, ...)
{
    if (_silent)
        return;

    va_list arguments;

    Serial.print("[INFO] ");
    _log(fmt, arguments);
}

void SerialLogger::warn(const char *fmt, ...)
{
    if (_silent)
        return;

    va_list arguments;

    Serial.print("[WARN] ");
    _log(fmt, arguments);
}

void SerialLogger::err(const char *fmt, ...)
{
    if (_silent)
        return;

    va_list arguments;

    Serial.print("[ERR ] ");
    _log(fmt, arguments);
}

void SerialLogger::mute()
{
    _silent = true;
}
void SerialLogger::unmute()
{
    _silent = false;
}
