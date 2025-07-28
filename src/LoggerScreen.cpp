#include "LoggerScreen.h"
#include "M1Shield.h"
#include <Arduino.h>

/**
 * @brief Constructor - create logger screen with optional title
 */
LoggerScreen::LoggerScreen(const char* title) : ConsoleScreen()
{
    // Set the screen title
    _setTitle(title);
    
    // Initialize logger settings
    _showTimestamps = true;
    _useColorCoding = true;
    _startTime = millis();
    
    // Set up console appearance for logging
    setTextColor(COLOR_INFO);
    setConsoleBackground(0x0000); // Black background
    setTextSize(1); // Small text for more lines
    
    // Update button labels for logger screen
    const char *buttonItems[1] = {"[M] Close Log"};
    _setButtonItems(buttonItems, 1);
}

/**
 * @brief Destructor
 */
LoggerScreen::~LoggerScreen()
{
    // Base class handles cleanup
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
    _logMessage("INFO", COLOR_INFO, fmt, args);
    va_end(args);
}

/**
 * @brief Log a warning message
 */
void LoggerScreen::warn(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    _logMessage("WARN", COLOR_WARN, fmt, args);
    va_end(args);
}

/**
 * @brief Log an error message
 */
void LoggerScreen::err(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    _logMessage("ERR ", COLOR_ERROR, fmt, args);
    va_end(args);
}

/**
 * @brief Internal helper to format and display log messages
 */
void LoggerScreen::_logMessage(const char* level, uint16_t color, const char* fmt, va_list args)
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
        // Save current color settings
        uint16_t originalColor = _textColor;
        
        // Set color for this log level
        setTextColor(color);
        
        // Print the log line
        println(logLine);
        
        // Restore original color
        setTextColor(originalColor);
    }
    else
    {
        // Simple monochrome output
        println(logLine);
    }
}

/**
 * @brief Get current timestamp string for logging
 */
void LoggerScreen::_getTimestamp(char* buffer, size_t bufferSize)
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
