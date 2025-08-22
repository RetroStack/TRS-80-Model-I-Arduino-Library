/*
 * LoggerScreen.cpp - Screen implementation for logger display
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#include "LoggerScreen.h"
#include "M1Shield.h"
#include <Arduino.h>

class LoggerScreen::LoggerAdapter : public ILogger
{
private:
    LoggerScreen *_parent;

public:
    LoggerAdapter(LoggerScreen *parent) : _parent(parent) {}
    virtual ~LoggerAdapter() = default;

    // Log informational messages
    void info(const char *fmt, ...) override
    {
        va_list args;
        va_start(args, fmt);
        _parent->_logMessage("INFO", _parent->COLOR_INFO, fmt, args);
        va_end(args);
    }

    // Log warning messages
    void warn(const char *fmt, ...) override
    {
        va_list args;
        va_start(args, fmt);
        _parent->_logMessage("WARN", _parent->COLOR_WARN, fmt, args);
        va_end(args);
    }

    // Log error messages
    void err(const char *fmt, ...) override
    {
        va_list args;
        va_start(args, fmt);
        _parent->_logMessage("ERR ", _parent->COLOR_ERROR, fmt, args);
        va_end(args);
    }

    // Log debug messages
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

    // Log raw byte output
    // This will be called for each byte written to the logger
    size_t write(uint8_t ch) override
    {
        return _parent->write(ch);
    }

    // Log raw byte output
    // This will be called for each byte written to the logger
    size_t write(const uint8_t *buffer, size_t size) override
    {
        return _parent->write(buffer, size);
    }
};

// Constructor with title
LoggerScreen::LoggerScreen(const char *title) : ConsoleScreen(), _loggerAdapter(nullptr)
{
    // Set the screen title
    setTitle(title);

    // Initialize logger settings
    _showTimestamps = isSmallDisplay() ? false : true; // Default to no timestamps on small displays
    _useColorCoding = true;
    _startTime = millis();

    // Initialize rotational buffer (disabled by default)
    _logBuffer = nullptr;
    _bufferSize = 0;
    _bufferHead = 0;
    _bufferCount = 0;

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

// Destructor
LoggerScreen::~LoggerScreen()
{
    delete _loggerAdapter;

    // Clean up dynamically allocated messages
    if (_logBuffer)
    {
        for (uint16_t i = 0; i < _bufferSize; i++)
        {
            delete[] _logBuffer[i].message;
        }
        delete[] _logBuffer;
    }

    // Base class handles cleanup
}

// Get the logger adapter interface
ILogger *LoggerScreen::asLogger()
{
    return _loggerAdapter;
}

// Override open to replay buffered entries
bool LoggerScreen::open()
{
    // Call parent implementation first
    bool result = ConsoleScreen::open();

    // Replay buffered entries if buffer exists and has content
    if (_logBuffer && _bufferCount > 0)
    {
        _replayBuffer();
    }

    return result;
}

// Set whether to show timestamps in log messages
void LoggerScreen::setTimestampEnabled(bool enabled)
{
    _showTimestamps = enabled;
}

// Check whether timestamps are enabled
bool LoggerScreen::isTimestampEnabled() const
{
    return _showTimestamps;
}

// Set whether to show color coding in log messages
void LoggerScreen::setColorCodingEnabled(bool enabled)
{
    _useColorCoding = enabled;
}

// Check whether color coding is enabled
bool LoggerScreen::isColorCodingEnabled() const
{
    return _useColorCoding;
}

// Reset the timestamp
void LoggerScreen::resetTimestamp()
{
    _startTime = millis();
}

// Set the size of the rotational log buffer
void LoggerScreen::setLogBufferSize(uint16_t size)
{
    // Clean up existing buffer and all message strings
    if (_logBuffer)
    {
        for (uint16_t i = 0; i < _bufferSize; i++)
        {
            delete[] _logBuffer[i].message;
        }
        delete[] _logBuffer;
    }

    _logBuffer = nullptr;
    _bufferSize = 0;
    _bufferHead = 0;
    _bufferCount = 0;

    // Create new buffer if size > 0
    if (size > 0)
    {
        _logBuffer = new LogEntry[size];
        if (_logBuffer) // Check allocation success
        {
            _bufferSize = size;
            // Initialize all message pointers to nullptr
            for (uint16_t i = 0; i < size; i++)
            {
                _logBuffer[i].message = nullptr;
                _logBuffer[i].color = COLOR_INFO;
                _logBuffer[i].timestamp = 0;
            }
        }
    }
}

// Get current buffer size
uint16_t LoggerScreen::getLogBufferSize() const
{
    return _bufferSize;
}

// Clear all entries from the log buffer
void LoggerScreen::clearLogBuffer()
{
    if (_logBuffer)
    {
        // Free all dynamically allocated message strings
        for (uint16_t i = 0; i < _bufferSize; i++)
        {
            delete[] _logBuffer[i].message;
            _logBuffer[i].message = nullptr;
        }
    }

    _bufferHead = 0;
    _bufferCount = 0;
}

// Get number of entries currently in buffer
uint16_t LoggerScreen::getLogBufferCount() const
{
    return _bufferCount;
}

// Log informational messages
void LoggerScreen::info(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    _logMessage(isSmallDisplay() ? "I" : "INFO", COLOR_INFO, fmt, args);
    va_end(args);
}

// Log warning messages
void LoggerScreen::warn(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    _logMessage(isSmallDisplay() ? "W" : "WARN", COLOR_WARN, fmt, args);
    va_end(args);
}

// Log error messages
void LoggerScreen::err(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    _logMessage(isSmallDisplay() ? "E" : "ERR ", COLOR_ERROR, fmt, args);
    va_end(args);
}

// Log debug messages
void LoggerScreen::debug(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    _logMessage(isSmallDisplay() ? "D" : "DBUG", COLOR_DEBUG, fmt, args);
    va_end(args);
}

// Log messages
void LoggerScreen::_logMessage(const char *level, uint16_t color, const char *fmt, va_list args)
{
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

    // Add to buffer regardless of whether screen is active
    _addToBuffer(logLine, color);

    // Display the log line if screen is active
    if (isActive())
    {
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
}

// Get the current timestamp
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

// Add entry to circular buffer
void LoggerScreen::_addToBuffer(const char *logLine, uint16_t color)
{
    // Skip if buffer is not allocated
    if (!_logBuffer || _bufferSize == 0)
        return;

    // Free existing message at head position if buffer is full (about to overwrite)
    if (_bufferCount == _bufferSize && _logBuffer[_bufferHead].message)
    {
        delete[] _logBuffer[_bufferHead].message;
        _logBuffer[_bufferHead].message = nullptr;
    }

    // Allocate memory for the new message
    size_t msgLen = strlen(logLine) + 1;
    _logBuffer[_bufferHead].message = new char[msgLen];
    if (_logBuffer[_bufferHead].message)
    {
        strcpy(_logBuffer[_bufferHead].message, logLine);
        _logBuffer[_bufferHead].color = color;
        _logBuffer[_bufferHead].timestamp = millis();

        // Move head to next position (circular)
        _bufferHead = (_bufferHead + 1) % _bufferSize;

        // Update count (don't exceed buffer size)
        if (_bufferCount < _bufferSize)
        {
            _bufferCount++;
        }
    }
}

// Replay all buffered entries to console
void LoggerScreen::_replayBuffer()
{
    if (!_logBuffer || _bufferCount == 0)
        return;

    // Calculate starting position for replay
    uint16_t startPos;
    if (_bufferCount < _bufferSize)
    {
        // Buffer not full yet, start from beginning
        startPos = 0;
    }
    else
    {
        // Buffer is full, start from head (oldest entry)
        startPos = _bufferHead;
    }

    // Replay entries in chronological order
    for (uint16_t i = 0; i < _bufferCount; i++)
    {
        uint16_t pos = (startPos + i) % _bufferSize;

        // Skip entries with null message pointers
        if (!_logBuffer[pos].message)
            continue;

        if (_useColorCoding)
        {
            setTextColor(_logBuffer[pos].color);
        }

        ConsoleScreen::println(_logBuffer[pos].message);
    }
}
