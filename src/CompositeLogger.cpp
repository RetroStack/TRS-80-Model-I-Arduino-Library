/*
 * CompositeLogger.cpp - Composite logger implementation
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#include "CompositeLogger.h"

// Constructor initializes empty logger array
CompositeLogger::CompositeLogger() : _loggerCount(0)
{
    // Initialize logger array to null pointers
    for (uint8_t i = 0; i < MAX_LOGGERS; i++)
    {
        _loggers[i] = nullptr;
    }
}

// Add a logger to the composite logger
bool CompositeLogger::addLogger(ILogger *logger)
{
    // Check for null pointer
    if (logger == nullptr)
    {
        return false;
    }

    // Check if we've reached maximum capacity
    if (_loggerCount >= MAX_LOGGERS)
    {
        return false;
    }

    // Check if logger is already registered (prevent duplicates)
    if (hasLogger(logger))
    {
        return false;
    }

    // Add logger to the first available slot
    _loggers[_loggerCount] = logger;
    _loggerCount++;

    return true;
}

// Remove a logger from the composite logger
bool CompositeLogger::removeLogger(ILogger *logger)
{
    // Check for null pointer
    if (logger == nullptr)
    {
        return false;
    }

    // Find the logger in our array
    for (uint8_t i = 0; i < _loggerCount; i++)
    {
        if (_loggers[i] == logger)
        {
            // Shift remaining loggers down to fill the gap
            for (uint8_t j = i; j < _loggerCount - 1; j++)
            {
                _loggers[j] = _loggers[j + 1];
            }

            // Clear the last slot and decrement count
            _loggers[_loggerCount - 1] = nullptr;
            _loggerCount--;

            return true;
        }
    }

    return false; // Logger not found
}

// Clear all loggers from the composite logger
void CompositeLogger::clearLoggers()
{
    for (uint8_t i = 0; i < MAX_LOGGERS; i++)
    {
        _loggers[i] = nullptr;
    }
    _loggerCount = 0;
}

// Get the number of registered loggers
uint8_t CompositeLogger::getLoggerCount() const
{
    return _loggerCount;
}

// Check if a specific logger is registered
bool CompositeLogger::hasLogger(ILogger *logger) const
{
    if (logger == nullptr)
    {
        return false;
    }

    for (uint8_t i = 0; i < _loggerCount; i++)
    {
        if (_loggers[i] == logger)
        {
            return true;
        }
    }

    return false;
}

// Log informational messages
void CompositeLogger::info(const char *fmt, ...)
{
    // Create formatted string once
    const int LEN = 255;
    char buffer[LEN];

    va_list arguments;
    va_start(arguments, fmt);
    vsnprintf(buffer, LEN, fmt, arguments);
    va_end(arguments);

    // Forward formatted string to each registered logger
    for (uint8_t i = 0; i < _loggerCount; i++)
    {
        if (_loggers[i] != nullptr)
        {
            _loggers[i]->info("%s", buffer);
        }
    }
}

// Log warning messages
void CompositeLogger::warn(const char *fmt, ...)
{
    // Create formatted string once
    const int LEN = 255;
    char buffer[LEN];

    va_list arguments;
    va_start(arguments, fmt);
    vsnprintf(buffer, LEN, fmt, arguments);
    va_end(arguments);

    // Forward formatted string to each registered logger
    for (uint8_t i = 0; i < _loggerCount; i++)
    {
        if (_loggers[i] != nullptr)
        {
            _loggers[i]->warn("%s", buffer);
        }
    }
}

// Log error messages
void CompositeLogger::err(const char *fmt, ...)
{
    // Create formatted string once
    const int LEN = 255;
    char buffer[LEN];

    va_list arguments;
    va_start(arguments, fmt);
    vsnprintf(buffer, LEN, fmt, arguments);
    va_end(arguments);

    // Forward formatted string to each registered logger
    for (uint8_t i = 0; i < _loggerCount; i++)
    {
        if (_loggers[i] != nullptr)
        {
            _loggers[i]->err("%s", buffer);
        }
    }
}

// Log debug messages
void CompositeLogger::debug(const char *fmt, ...)
{
    // Create formatted string once
    const int LEN = 255;
    char buffer[LEN];

    va_list arguments;
    va_start(arguments, fmt);
    vsnprintf(buffer, LEN, fmt, arguments);
    va_end(arguments);

    // Forward formatted string to each registered logger
    for (uint8_t i = 0; i < _loggerCount; i++)
    {
        if (_loggers[i] != nullptr)
        {
            _loggers[i]->debug("%s", buffer);
        }
    }
}

// Log raw byte data
size_t CompositeLogger::write(uint8_t ch)
{
    size_t totalWritten = 0;

    // Forward to each registered logger
    for (uint8_t i = 0; i < _loggerCount; i++)
    {
        if (_loggers[i] != nullptr)
        {
            totalWritten += _loggers[i]->write(ch);
        }
    }

    return totalWritten;
}

// Log raw byte data from buffer
size_t CompositeLogger::write(const uint8_t *buffer, size_t size)
{
    if (!buffer)
    {
        return 0; // Can't log an error in a logger about null buffer, just return 0
    }

    size_t totalWritten = 0;

    // Forward to each registered logger
    for (uint8_t i = 0; i < _loggerCount; i++)
    {
        if (_loggers[i] != nullptr)
        {
            totalWritten += _loggers[i]->write(buffer, size);
        }
    }

    return totalWritten;
}
