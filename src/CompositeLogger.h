/*
 * CompositeLogger.h - Logger that forwards events to multiple registered loggers
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef COMPOSITE_LOGGER_H
#define COMPOSITE_LOGGER_H

#include <Arduino.h>
#include "ILogger.h"

/**
 * @brief A composite logger that forwards log events to multiple registered ILogger instances
 *
 * CompositeLogger implements the ILogger interface and allows you to register multiple
 * logger instances. When log methods are called, the composite logger forwards the
 * calls to all registered loggers, enabling output to multiple destinations
 * simultaneously (e.g., Serial, SD card, network, etc.).
 *
 * Features:
 * - Register multiple ILogger instances
 * - Remove specific loggers
 * - Clear all registered loggers
 * - Automatic forwarding of all ILogger method calls
 * - Memory efficient storage of logger references
 *
 * Example usage:
 * @code
 * SerialLogger serialLogger;
 * // Assume we have a FileLogger fileLogger;
 *
 * CompositeLogger multiLogger;
 * multiLogger.addLogger(&serialLogger);
 * multiLogger.addLogger(&fileLogger);
 *
 * // Now logs go to both Serial and file
 * multiLogger.info("System initialized");
 * multiLogger.warn("Low memory warning");
 * @endcode
 */
class CompositeLogger : public ILogger
{
private:
    static const uint8_t MAX_LOGGERS = 8; // Maximum number of loggers that can be registered
    ILogger *_loggers[MAX_LOGGERS];       // Array of registered logger pointers
    uint8_t _loggerCount;                 // Current number of registered loggers

public:
    /**
     * @brief Constructor - initializes empty composite logger
     */
    CompositeLogger();

    /**
     * @brief Add a logger to the composite logger
     * @param logger Pointer to ILogger instance to add
     * @return true if logger was added successfully, false if maximum capacity reached
     */
    bool addLogger(ILogger *logger);

    /**
     * @brief Remove a specific logger from the composite logger
     * @param logger Pointer to ILogger instance to remove
     * @return true if logger was found and removed, false if not found
     */
    bool removeLogger(ILogger *logger);

    /**
     * @brief Remove all registered loggers
     */
    void clearLoggers();

    /**
     * @brief Get the number of currently registered loggers
     * @return Number of registered loggers
     */
    uint8_t getLoggerCount() const;

    /**
     * @brief Check if a specific logger is registered
     * @param logger Pointer to ILogger instance to check
     * @return true if logger is registered, false otherwise
     */
    bool hasLogger(ILogger *logger) const;

    // ILogger interface implementation
    void info(const char *fmt, ...) override;
    void warn(const char *fmt, ...) override;
    void err(const char *fmt, ...) override;

    size_t write(uint8_t ch) override;
    size_t write(const uint8_t *buffer, size_t size) override;

    using Print::print;
    using Print::println;
};

#endif /* COMPOSITE_LOGGER_H */
