/*
 * SDCardLogger.cpp - Class for logging to SD card files
 * Authors: Ven Reddy, Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#include "SDCardLogger.h"

// Constructor with optional filename parameter
SDCardLogger::SDCardLogger(const char *filename)
{
    _filename = filename;
    _silent = false;
}

// Initialize SD card and prepare log file
bool SDCardLogger::begin()
{
    // Initialize SD card
    if (!SD.begin(M1Shield.getSDCardSelectPin()))
    {
        return false;
    }

    return true;
}

// Internal logging method with format string and arguments
void SDCardLogger::_log(const char *fmt, va_list arguments)
{
    const int LEN = 255;
    char buffer[LEN];
    vsnprintf(buffer, LEN, fmt, arguments);

    // Open file in append mode
    File logFile = SD.open(_filename, FILE_WRITE);
    if (logFile)
    {
        logFile.println(buffer);
        logFile.close();
    }
}

// Write single character to SD card log file
size_t SDCardLogger::write(uint8_t ch)
{
    File logFile = SD.open(_filename, FILE_WRITE);
    if (!logFile)
    {
        return 0;
    }

    size_t result;
    if (ch == '\n')
    {
        result = logFile.println();
    }
    else
    {
        result = logFile.print((char)ch);
    }

    logFile.close();
    return result;
}

// Write buffer to SD card log file
size_t SDCardLogger::write(const uint8_t *buffer, size_t size)
{
    File logFile = SD.open(_filename, FILE_WRITE);
    if (!logFile)
    {
        return 0;
    }

    size_t result = 0;
    for (uint16_t i = 0; i < size; i++)
    {
        if (buffer[i] == '\n')
        {
            result += logFile.println();
        }
        else
        {
            result += logFile.print((char)buffer[i]);
        }
    }

    logFile.close();
    return result;
}

// Log info message with format string
void SDCardLogger::info(const char *fmt, ...)
{
    if (_silent)
        return;

    va_list arguments;
    va_start(arguments, fmt);

    File logFile = SD.open(_filename, FILE_WRITE);
    if (logFile)
    {
        logFile.print("[INFO] ");
        logFile.close();
    }

    _log(fmt, arguments);

    va_end(arguments);
}

// Log warning message with format string
void SDCardLogger::warn(const char *fmt, ...)
{
    if (_silent)
        return;

    va_list arguments;
    va_start(arguments, fmt);

    File logFile = SD.open(_filename, FILE_WRITE);
    if (logFile)
    {
        logFile.print("[WARN] ");
        logFile.close();
    }

    _log(fmt, arguments);

    va_end(arguments);
}

// Log error message with format string
void SDCardLogger::err(const char *fmt, ...)
{
    if (_silent)
        return;

    va_list arguments;
    va_start(arguments, fmt);

    File logFile = SD.open(_filename, FILE_WRITE);
    if (logFile)
    {
        logFile.print("[ERR ] ");
        logFile.close();
    }

    _log(fmt, arguments);

    va_end(arguments);
}

// Log debug message with format string
void SDCardLogger::debug(const char *fmt, ...)
{
    if (_silent)
        return;

    va_list arguments;
    va_start(arguments, fmt);

    File logFile = SD.open(_filename, FILE_WRITE);
    if (logFile)
    {
        logFile.print("[DBUG] ");
        logFile.close();
    }

    _log(fmt, arguments);

    va_end(arguments);
}

// Disable logging output
void SDCardLogger::mute()
{
    _silent = true;
}

// Enable logging output
void SDCardLogger::unmute()
{
    _silent = false;
}
