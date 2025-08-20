/*
 * CompositeLogger.h - Logger that forwards events to multiple registered loggers
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef COMPOSITE_LOGGER_H
#define COMPOSITE_LOGGER_H

#include <Arduino.h>
#include "ILogger.h"

// A composite logger that forwards log events to multiple registered ILogger instances
class CompositeLogger : public ILogger
{
private:
    static const uint8_t MAX_LOGGERS = 8; // Maximum number of loggers that can be registered
    ILogger *_loggers[MAX_LOGGERS];       // Array of registered logger pointers
    uint8_t _loggerCount;                 // Current number of registered loggers

public:
    CompositeLogger(); // Constructor - initializes empty composite logger

    bool addLogger(ILogger *logger);    // Add a logger to the composite logger
    bool removeLogger(ILogger *logger); // Remove a specific logger from the composite logger
    void clearLoggers();                // Remove all registered loggers
    uint8_t getLoggerCount() const;     // Get the number of currently registered loggers

    bool hasLogger(ILogger *logger) const; // Check if a specific logger is registered

    // ILogger interface implementation
    void info(const char *fmt, ...) override;  // Log informational message to all registered loggers
    void warn(const char *fmt, ...) override;  // Log warning message to all registered loggers
    void err(const char *fmt, ...) override;   // Log error message to all registered loggers
    void debug(const char *fmt, ...) override; // Log debug message to all registered loggers

    using ILogger::debug;
    using ILogger::err;
    using ILogger::info;
    using ILogger::warn;

    size_t write(uint8_t ch) override;                         // Write single character to all registered loggers
    size_t write(const uint8_t *buffer, size_t size) override; // Write buffer to all registered loggers

    using Print::print;
    using Print::println;
};

#endif /* COMPOSITE_LOGGER_H */
