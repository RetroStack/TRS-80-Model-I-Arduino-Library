/*
 * LoggerScreen.h - Console screen with logging functionality
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef LOGGER_SCREEN_H
#define LOGGER_SCREEN_H

#include "ConsoleScreen.h"
#include "ILogger.h"

// A console screen with logging functionality compatible with ILogger
class LoggerScreen : public ConsoleScreen
{
private:
    bool _showTimestamps;     // Whether to include timestamps in log messages
    bool _useColorCoding;     // Whether to use color coding for different log levels
    unsigned long _startTime; // Start time for relative timestamps

    // Color definitions for log levels
    static const uint16_t COLOR_INFO = 0xFFFF;      // White for info messages
    static const uint16_t COLOR_WARN = 0xFFE0;      // Yellow for warnings
    static const uint16_t COLOR_ERROR = 0xF800;     // Red for errors
    static const uint16_t COLOR_DEBUG = 0x07FF;     // Cyan for debug messages
    static const uint16_t COLOR_TIMESTAMP = 0x7BEF; // Light gray for timestamps

    void _logMessage(const char *level, uint16_t color, const char *fmt, va_list args); // Internal helper to format and display log messages

    void _getTimestamp(char *buffer, size_t bufferSize); // Get current timestamp string for logging

public:
    LoggerScreen(const char *title = "Logger"); // Constructor - create logger screen with optional title
    virtual ~LoggerScreen();                    // Destructor

    void setTimestampEnabled(bool enabled); // Enable or disable timestamp display in log messages
    bool isTimestampEnabled() const;        // Check if timestamps are enabled

    void setColorCodingEnabled(bool enabled); // Enable or disable color coding for different log levels
    bool isColorCodingEnabled() const;        // Check if color coding is enabled

    void resetTimestamp(); // Reset the timestamp reference point to current time

    // Logging interface (compatible with ILogger but not inheriting)
    void info(const char *fmt, ...);
    void warn(const char *fmt, ...);
    void err(const char *fmt, ...);
    void debug(const char *fmt, ...);

    ILogger *asLogger(); // Get an ILogger adapter for this LoggerScreen

private:
    // Inner class that implements ILogger and forwards to the parent LoggerScreen
    class LoggerAdapter;
    LoggerAdapter *_loggerAdapter;
};

#endif /* LOGGER_SCREEN_H */
