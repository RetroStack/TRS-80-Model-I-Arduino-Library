/*
 * LoggerScreen.h - Console screen that implements ILogger interface
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef LOGGER_SCREEN_H
#define LOGGER_SCREEN_H

#include "ConsoleScreen.h"
#include "ILogger.h"

/**
 * @brief A console screen that implements the ILogger interface for on-screen logging
 * 
 * LoggerScreen extends ConsoleScreen to provide a visual logging destination that can
 * be used anywhere an ILogger is expected. It displays log messages with formatted
 * prefixes and automatic scrolling, making it perfect for real-time debugging,
 * system monitoring, and diagnostics on the M1Shield display.
 * 
 * Features:
 * - Full ILogger interface implementation (info, warn, err)
 * - Formatted log prefixes with timestamps and levels
 * - Color-coded log levels for easy visual distinction
 * - Automatic scrolling when screen fills
 * - Standard ConsoleScreen functionality (Print interface, text control)
 * - Compatible with CompositeLogger for multi-destination logging
 * - Real-time visual feedback for debugging and monitoring
 * 
 * Usage Examples:
 * @code
 * // Basic logger screen usage
 * LoggerScreen* logScreen = new LoggerScreen("System Log");
 * M1Shield.setScreen(logScreen);
 * 
 * // Use as a logger
 * logScreen->info("System initialized");
 * logScreen->warn("Low memory: %d bytes", freeMemory);
 * logScreen->err("Sensor failure on pin %d", sensorPin);
 * 
 * // Use with CompositeLogger
 * CompositeLogger multiLogger;
 * SerialLogger serialLogger;
 * multiLogger.addLogger(&serialLogger);
 * multiLogger.addLogger(logScreen);
 * 
 * // Now logs go to both Serial and screen
 * multiLogger.info("Logged to both destinations");
 * @endcode
 */
class LoggerScreen : public ConsoleScreen, public ILogger
{
private:
    bool _showTimestamps;           ///< Whether to include timestamps in log messages
    bool _useColorCoding;           ///< Whether to use color coding for different log levels
    unsigned long _startTime;       ///< Start time for relative timestamps
    
    // Color definitions for log levels
    static const uint16_t COLOR_INFO  = 0xFFFF;  ///< White for info messages
    static const uint16_t COLOR_WARN  = 0xFFE0;  ///< Yellow for warnings
    static const uint16_t COLOR_ERROR = 0xF800;  ///< Red for errors
    static const uint16_t COLOR_TIMESTAMP = 0x7BEF;  ///< Light gray for timestamps

    /**
     * @brief Internal helper to format and display log messages
     * @param level Log level string (e.g., "INFO", "WARN", "ERR ")
     * @param color Color to use for the log level
     * @param fmt Printf-style format string
     * @param args Variable argument list
     */
    void _logMessage(const char* level, uint16_t color, const char* fmt, va_list args);

    /**
     * @brief Get current timestamp string for logging
     * @param buffer Buffer to store timestamp string
     * @param bufferSize Size of the buffer
     */
    void _getTimestamp(char* buffer, size_t bufferSize);

public:
    /**
     * @brief Constructor - create logger screen with optional title
     * @param title Screen title (optional, defaults to "Logger")
     */
    LoggerScreen(const char* title = "Logger");

    /**
     * @brief Destructor
     */
    virtual ~LoggerScreen();

    /**
     * @brief Enable or disable timestamp display in log messages
     * @param enabled true to show timestamps, false to hide them
     */
    void setTimestampEnabled(bool enabled);

    /**
     * @brief Check if timestamps are enabled
     * @return true if timestamps are enabled, false otherwise
     */
    bool isTimestampEnabled() const;

    /**
     * @brief Enable or disable color coding for different log levels
     * @param enabled true to use color coding, false for monochrome
     */
    void setColorCodingEnabled(bool enabled);

    /**
     * @brief Check if color coding is enabled
     * @return true if color coding is enabled, false otherwise
     */
    bool isColorCodingEnabled() const;

    /**
     * @brief Reset the timestamp reference point to current time
     * 
     * This resets the relative timestamp counter, useful when starting
     * a new logging session or after a system reset.
     */
    void resetTimestamp();

    // ILogger interface implementation
    void info(const char *fmt, ...) override;
    void warn(const char *fmt, ...) override;
    void err(const char *fmt, ...) override;

    // Print interface is inherited from ConsoleScreen
    // size_t write(uint8_t ch) override;  // Already implemented in ConsoleScreen
    // size_t write(const uint8_t *buffer, size_t size) override;  // Already implemented

    using Print::print;
    using Print::println;
};

#endif /* LOGGER_SCREEN_H */
