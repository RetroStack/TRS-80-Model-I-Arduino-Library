#include "CompositeLogger.h"

/**
 * Constructor - initializes empty composite logger
 */
CompositeLogger::CompositeLogger() : _loggerCount(0)
{
    // Initialize logger array to null pointers
    for (uint8_t i = 0; i < MAX_LOGGERS; i++)
    {
        _loggers[i] = nullptr;
    }
}

/**
 * Add a logger to the composite logger
 */
bool CompositeLogger::addLogger(ILogger* logger)
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

/**
 * Remove a specific logger from the composite logger
 */
bool CompositeLogger::removeLogger(ILogger* logger)
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

/**
 * Remove all registered loggers
 */
void CompositeLogger::clearLoggers()
{
    for (uint8_t i = 0; i < MAX_LOGGERS; i++)
    {
        _loggers[i] = nullptr;
    }
    _loggerCount = 0;
}

/**
 * Get the number of currently registered loggers
 */
uint8_t CompositeLogger::getLoggerCount() const
{
    return _loggerCount;
}

/**
 * Check if a specific logger is registered
 */
bool CompositeLogger::hasLogger(ILogger* logger) const
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

/**
 * Logs a formatted info string to all registered loggers
 */
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

/**
 * Logs a formatted warning string to all registered loggers
 */
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

/**
 * Logs a formatted error string to all registered loggers
 */
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

/**
 * Write a byte to all registered loggers
 */
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

/**
 * Write a buffer to all registered loggers
 */
size_t CompositeLogger::write(const uint8_t *buffer, size_t size)
{
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
