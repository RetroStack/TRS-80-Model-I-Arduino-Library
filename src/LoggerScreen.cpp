/*
 * LoggerScreen.cpp - Screen implementation for logger display
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#include "LoggerScreen.h"
#include "M1Shield.h"
#include <Arduino.h>

/**
 * @brief Internal adapter class that implements ILogger and forwards to LoggerScreen
 */
class LoggerScreen::LoggerAdapter : public ILogger
{
private:
    LoggerScreen *_parent;

public:
    LoggerAdapter(LoggerScreen *parent) : _parent(parent) {}
    virtual ~LoggerAdapter() = default;

    void info(const char *fmt, ...) override
    {
        va_list args;
        va_start(args, fmt);
        _parent->_logMessage("INFO", _parent->COLOR_INFO, fmt, args);
        va_end(args);
    }

    void warn(const char *fmt, ...) override
    {
        va_list args;
        va_start(args, fmt);
        _parent->_logMessage("WARN", _parent->COLOR_WARN, fmt, args);
        va_end(args);
    }

    void err(const char *fmt, ...) override
    {
        va_list args;
        va_start(args, fmt);
        _parent->_logMessage("ERR ", _parent->COLOR_ERROR, fmt, args);
        va_end(args);
    }

    void debug(const char *fmt, ...) override
    {
        va_list args;
        va_start(args, fmt);
        _parent->_logMessage("DEBUG", _parent->COLOR_DEBUG, fmt, args);
        va_end(args);
    }

    using ILogger::debug;
    using ILogger::err;
    using ILogger::info;
    using ILogger::warn;

    size_t write(uint8_t ch) override
    {
        return _parent->write(ch);
    }

    size_t write(const uint8_t *buffer, size_t size) override
    {
        return _parent->write(buffer, size);
    }
};

/**
 * @brief Constructor - create logger screen with optional title
 */
LoggerScreen::LoggerScreen(const char *title) : ConsoleScreen(), _loggerAdapter(nullptr)
{
    // Set the screen title
    setTitle(title);

    // Initialize logger settings
    _showTimestamps = isSmallDisplay() ? false : true; // Default to no timestamps on small displays
    _useColorCoding = true;
    _startTime = millis();

    // Set up console appearance for logging
    setTextColor(COLOR_INFO);
    setConsoleBackground(0x0000); // Black background
    setTextSize(1);               // Small text for more lines

    // Update button labels for logger screen
    const char *buttonItems[1] = {"[M] Close Log"};
    setButtonItems(buttonItems, 1);

    // Create the logger adapter
    _loggerAdapter = new LoggerAdapter(this);
}

/**
 * @brief Destructor
 */
LoggerScreen::~LoggerScreen()
{
    delete _loggerAdapter;
    // Base class handles cleanup
}

/**
 * @brief Get an ILogger adapter for this LoggerScreen
 */
ILogger *LoggerScreen::asLogger()
{
    return _loggerAdapter;
}

/**
 * @brief Enable or disable timestamp display in log messages
 */
void LoggerScreen::setTimestampEnabled(bool enabled)
{
    _showTimestamps = enabled;
}

/**
 * @brief Check if timestamps are enabled
 */
bool LoggerScreen::isTimestampEnabled() const
{
    return _showTimestamps;
}

/**
 * @brief Enable or disable color coding for different log levels
 */
void LoggerScreen::setColorCodingEnabled(bool enabled)
{
    _useColorCoding = enabled;
}

/**
 * @brief Check if color coding is enabled
 */
bool LoggerScreen::isColorCodingEnabled() const
{
    return _useColorCoding;
}

/**
 * @brief Reset the timestamp reference point to current time
 */
void LoggerScreen::resetTimestamp()
{
    _startTime = millis();
}

/**
 * @brief Log an informational message
 */
void LoggerScreen::info(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    _logMessage(isSmallDisplay() ? "I" : "INFO", COLOR_INFO, fmt, args);
    va_end(args);
}

/**
 * @brief Log a warning message
 */
void LoggerScreen::warn(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    _logMessage(isSmallDisplay() ? "W" : "WARN", COLOR_WARN, fmt, args);
    va_end(args);
}

/**
 * @brief Log an error message
 */
void LoggerScreen::err(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    _logMessage(isSmallDisplay() ? "E" : "ERR ", COLOR_ERROR, fmt, args);
    va_end(args);
}

/**
 * @brief Log a debug message
 */
void LoggerScreen::debug(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    _logMessage(isSmallDisplay() ? "D" : "DBUG", COLOR_DEBUG, fmt, args);
    va_end(args);
}

/**
 * @brief Internal helper to format and display log messages
 */
void LoggerScreen::_logMessage(const char *level, uint16_t color, const char *fmt, va_list args)
{
    if (!isActive())
        return;

    // Format the user message
    const int MSG_LEN = 200;
    char messageBuffer[MSG_LEN];
    vsnprintf(messageBuffer, MSG_LEN, fmt, args);

    // Build complete log line
    const int LINE_LEN = 256;
    char logLine[LINE_LEN];

    if (_showTimestamps)
    {
        char timestamp[16];
        _getTimestamp(timestamp, sizeof(timestamp));

        if (_useColorCoding)
        {
            // We'll handle color in the display code
            snprintf(logLine, LINE_LEN, "[%s] [%s] %s", timestamp, level, messageBuffer);
        }
        else
        {
            snprintf(logLine, LINE_LEN, "[%s] [%s] %s", timestamp, level, messageBuffer);
        }
    }
    else
    {
        snprintf(logLine, LINE_LEN, "[%s] %s", level, messageBuffer);
    }

    // Display the log line with appropriate color
    if (_useColorCoding)
    {
        // Set color for this log level
        setTextColor(color);

        // Print the log line using ConsoleScreen's println
        ConsoleScreen::println(logLine);
    }
    else
    {
        // Simple monochrome output
        ConsoleScreen::println(logLine);
    }
}

/**
 * @brief Get current timestamp string for logging
 */
void LoggerScreen::_getTimestamp(char *buffer, size_t bufferSize)
{
    unsigned long elapsed = millis() - _startTime;
    unsigned long seconds = elapsed / 1000;
    unsigned long minutes = seconds / 60;
    unsigned long hours = minutes / 60;

    seconds %= 60;
    minutes %= 60;
    hours %= 24; // Roll over after 24 hours

    if (hours > 0)
    {
        snprintf(buffer, bufferSize, "%02lu:%02lu:%02lu", hours, minutes, seconds);
    }
    else
    {
        snprintf(buffer, bufferSize, "%02lu:%02lu", minutes, seconds);
    }
}
