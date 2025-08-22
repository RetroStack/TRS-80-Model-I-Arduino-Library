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

    // Rotational logging buffer
    struct LogEntry
    {
        char *message;           // Dynamically allocated log message
        uint16_t color;          // Color for the message
        unsigned long timestamp; // Timestamp when logged
    };

    LogEntry *_logBuffer;  // Circular buffer for log entries
    uint16_t _bufferSize;  // Size of the log buffer (0 = disabled)
    uint16_t _bufferHead;  // Head position in circular buffer
    uint16_t _bufferCount; // Number of entries currently in buffer

    // Color definitions for log levels
    static const uint16_t COLOR_INFO = 0xFFFF;      // White for info messages
    static const uint16_t COLOR_WARN = 0xFFE0;      // Yellow for warnings
    static const uint16_t COLOR_ERROR = 0xF800;     // Red for errors
    static const uint16_t COLOR_DEBUG = 0x07FF;     // Cyan for debug messages
    static const uint16_t COLOR_TIMESTAMP = 0x7BEF; // Light gray for timestamps

    void _logMessage(const char *level, uint16_t color, const char *fmt, va_list args); // Internal helper to format and display log messages

    void _getTimestamp(char *buffer, size_t bufferSize); // Get current timestamp string for logging

    void _addToBuffer(const char *logLine, uint16_t color); // Add entry to circular buffer
    void _replayBuffer();                                   // Replay all buffered entries to console

public:
    LoggerScreen(const char *title = "Logger"); // Constructor - create logger screen with optional title
    virtual ~LoggerScreen();                    // Destructor

    bool open() override; // Override to replay buffered entries when opening

    void setTimestampEnabled(bool enabled); // Enable or disable timestamp display in log messages
    bool isTimestampEnabled() const;        // Check if timestamps are enabled

    void setColorCodingEnabled(bool enabled); // Enable or disable color coding for different log levels
    bool isColorCodingEnabled() const;        // Check if color coding is enabled

    void resetTimestamp(); // Reset the timestamp reference point to current time

    // Rotational buffer management
    void setLogBufferSize(uint16_t size); // Set the size of the rotational log buffer (0 = disabled)
    uint16_t getLogBufferSize() const;    // Get current buffer size
    void clearLogBuffer();                // Clear all entries from the log buffer
    uint16_t getLogBufferCount() const;   // Get number of entries currently in buffer

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
