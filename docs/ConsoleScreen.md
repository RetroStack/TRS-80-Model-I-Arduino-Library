# ConsoleScreen Class

## Overview

The `ConsoleScreen` class is a M1TerminalScreen-based implementation that provides a system console for capturing, storing, and displaying log messages. It implements the `ILogger` interface to serve as a centralized logging destination while providing a terminal-style interface for viewing recent logs.

## Key Features

- **Memory Efficient**: Fixed 2KB circular buffer for log storage
- **ILogger Interface**: Direct integration with existing logging infrastructure
- **Print Stream Compatible**: Works with Arduino Serial/Stream operations
- **Real-time Display**: Live log updates with automatic scrolling
- **Filtering**: Show all logs, warnings+errors, or errors only
- **Navigation**: Scroll through log history with up/down controls
- **Statistics**: Track error/warning counts and buffer usage

## Architecture

```
Screen (abstract base)
    ↓
ContentScreen (structured layout)
    ↓
M1TerminalScreen (terminal rendering)
    ↓
ConsoleScreen (log capture & display) ← ILogger interface
    ↓
YourConsoleScreen (specific logging behavior)
```

## Memory Usage

### Log Buffer Structure

```cpp
struct LogEntry {
    uint32_t timestamp;    // 4 bytes - millis() when logged
    uint8_t level;         // 1 byte - 0=info, 1=warn, 2=error
    char message[59];      // 59 bytes - message text
    // Total: 64 bytes per entry
};
```

### Total Memory Footprint

- **Log Buffer**: 32 entries × 64 bytes = 2,048 bytes
- **Terminal Video Memory**: 1,024 bytes (inherited from M1TerminalScreen)
- **Working Buffers**: ~128 bytes for formatting and print buffering
- **Total**: ~3.2KB RAM usage

## Implementation Guide

### 1. Basic Console Usage

```cpp
class SystemConsole : public ConsoleScreen
{
public:
    SystemConsole() : ConsoleScreen() {
        _setTitle("System Debug Console");
        setFilterLevel(1); // Show warnings and errors only
    }

    bool initialize() override {
        if (!ConsoleScreen::initialize()) {
            return false;
        }

        info("System console initialized");
        return true;
    }
};
```

### 2. Global Logger Setup

```cpp
// Global console instance
SystemConsole g_console;

// Make it available as ILogger interface
ILogger* getSystemLogger() {
    return &g_console;
}

void setup() {
    // Initialize console
    g_console.initialize();
    g_console.open();

    // Now any component can log
    getSystemLogger()->info("System started");
}
```

### 3. Integration with Existing Code

```cpp
void someSystemFunction() {
    ILogger* logger = getSystemLogger();

    if (initializeHardware()) {
        logger->info("Hardware init successful");
    } else {
        logger->err("Hardware init failed!");
    }
}
```

## Display Layout

The ConsoleScreen uses the M1TerminalScreen layout structure:

```
┌─────────────────────────────┐
│ Header: System Console      │
├─────────────────────────────┤
│ 12:34 [INFO] System ready   │
│ 12:34 [WARN] Temp high: 75C │
│ 12:35 [ERR!] SPI timeout    │
│ 12:35 [INFO] Recovery OK    │
│ 12:36 [WARN] Memory low     │
│ ...more log entries...      │
│ (scroll with UP/DOWN)       │
├─────────────────────────────┤
│ [M] Menu [F] Filter [C] Clr │
├─────────────────────────────┤
│ Progress: ████░░░░ 50%      │
└─────────────────────────────┘
```

## Log Entry Format

Each log line follows this format:

```
HH:MM [LEVEL] message text (up to 45 chars)
```

- **HH:MM**: Hours and minutes from millis() timestamp
- **[LEVEL]**: `[INFO]`, `[WARN]`, or `[ERR!]`
- **message**: Log message text, truncated to fit terminal width

## Input Controls

### Console-Specific Controls

| Input         | Action                                         |
| ------------- | ---------------------------------------------- |
| RIGHT Button  | Cycle filter level (ALL → WARN+ERR → ERR ONLY) |
| UP Button     | Clear all logs                                 |
| DOWN Button   | Toggle auto-scroll mode                        |
| Joystick UP   | Scroll up through log history                  |
| Joystick DOWN | Scroll down through log history                |

### Inherited Terminal Controls

| Input       | Action                   |
| ----------- | ------------------------ |
| LEFT Button | Toggle view (left/right) |
| MENU Button | Exit console             |

## ILogger Interface Implementation

### Formatted Logging Methods

```cpp
// Log with formatted strings
console.info("Temperature: %d°C", temperature);
console.warn("Battery low: %d%%", batteryLevel);
console.err("Error code: 0x%04X", errorCode);
```

### Print Stream Compatibility

```cpp
// Works with Serial-style operations
console.print("Value: ");
console.println(42);

// Redirect other streams
Serial.setLogger(&console);  // If supported
```

### Log Levels

```cpp
// Three log levels available
LOG_LEVEL_INFO  = 0  // General information
LOG_LEVEL_WARN  = 1  // Warnings and alerts
LOG_LEVEL_ERROR = 2  // Errors and critical issues
```

## Filtering and Navigation

### Filter Levels

```cpp
// Set filter programmatically
console.setFilterLevel(0);  // Show all logs
console.setFilterLevel(1);  // Show warnings and errors only
console.setFilterLevel(2);  // Show errors only

// Check current filter
uint8_t level = console.getFilterLevel();
```

### Auto-scroll Control

```cpp
// Enable/disable auto-scroll to newest entries
console.setAutoScroll(true);   // Default: scroll to new logs
console.setAutoScroll(false);  // Manual navigation only

bool autoScroll = console.getAutoScroll();
```

### Manual Navigation

```cpp
// Programmatic scrolling (if needed)
// Note: These are internal methods, use joystick for user control
_scrollUp();    // Scroll toward older entries
_scrollDown();  // Scroll toward newer entries
_scrollToNewest(); // Jump to newest entries
```

## Statistics and Monitoring

### Buffer Usage

```cpp
// Check log buffer status
uint8_t totalLogs = console.getLogCount();      // Total logs stored
uint8_t maxLogs = 32;                           // Maximum capacity
uint8_t usage = (totalLogs * 100) / maxLogs;   // Percentage full
```

### Error Tracking

```cpp
// Count specific log types
uint8_t errorCount = console.getErrorCount();
uint8_t warningCount = console.getWarningCount();

// Monitor system health
if (errorCount > 10) {
    // Take corrective action
}
```

## Advanced Usage

### Custom Log Processing

```cpp
class DiagnosticConsole : public ConsoleScreen
{
private:
    uint8_t _criticalErrors;

public:
    void err(const char* fmt, ...) override {
        // Call parent implementation
        ConsoleScreen::err(fmt, ...);

        // Custom error handling
        _criticalErrors++;
        if (_criticalErrors >= 5) {
            // Trigger system reset or safe mode
            triggerSafeMode();
        }
    }
};
```

### Integration with System Monitoring

```cpp
class SystemMonitor {
private:
    ILogger* _logger;

public:
    SystemMonitor(ILogger* logger) : _logger(logger) {}

    void checkSystemHealth() {
        float temperature = readTemperature();
        if (temperature > 80.0) {
            _logger->warn("High temperature: %.1f°C", temperature);
        }

        int freeMemory = getFreeMemory();
        if (freeMemory < 500) {
            _logger->err("Low memory: %d bytes", freeMemory);
        }
    }
};
```

### Persistent Logging

```cpp
class PersistentConsole : public ConsoleScreen
{
private:
    File _logFile;

public:
    void info(const char* fmt, ...) override {
        // Log to console display
        ConsoleScreen::info(fmt, ...);

        // Also save to SD card or EEPROM
        if (_logFile) {
            va_list args;
            va_start(args, fmt);
            _logFile.vprintf(fmt, args);
            _logFile.println();
            va_end(args);
        }
    }
};
```

## Performance Considerations

### Memory Efficiency

- **Circular Buffer**: Oldest logs automatically overwritten
- **Fixed Size**: No dynamic allocation or memory fragmentation
- **Compact Format**: 64 bytes per log entry including timestamp
- **Efficient Rendering**: Only visible logs are drawn to terminal

### Real-time Performance

```cpp
// Minimal impact on system performance
void criticalSystemFunction() {
    // Logging is fast - just buffer copy and pointer increment
    logger->info("Critical operation started");

    performCriticalWork();

    logger->info("Critical operation completed");
}
```

### Buffer Management

```cpp
// Monitor and manage buffer usage
void manageLogBuffer() {
    if (console.getLogCount() >= 30) {
        // Near capacity - increase log filtering
        console.setFilterLevel(LOG_LEVEL_ERROR);
        console.warn("Log buffer nearly full - filtering errors only");
    }
}
```

## Integration Examples

### With MenuScreen

```cpp
Screen* MenuScreen::_getSelectedMenuItemScreen(int index) {
    switch (index) {
        case 0:  // Console option
            return &g_systemConsole;
        case 1:  // Diagnostics
            g_systemConsole.info("Diagnostics menu selected");
            return new DiagnosticsScreen();
        // ... other options
    }
}
```

### Global Error Handler

```cpp
void globalErrorHandler(const char* component, int errorCode) {
    ILogger* logger = getSystemLogger();
    logger->err("%s error: code %d", component, errorCode);

    // Take appropriate action based on error severity
    if (errorCode >= 1000) {
        logger->err("Critical error - system halt required");
        systemHalt();
    }
}
```

### Debugging Integration

```cpp
#ifdef DEBUG
    #define DEBUG_LOG(fmt, ...) getSystemLogger()->info(fmt, ##__VA_ARGS__)
    #define DEBUG_WARN(fmt, ...) getSystemLogger()->warn(fmt, ##__VA_ARGS__)
    #define DEBUG_ERR(fmt, ...) getSystemLogger()->err(fmt, ##__VA_ARGS__)
#else
    #define DEBUG_LOG(fmt, ...)
    #define DEBUG_WARN(fmt, ...)
    #define DEBUG_ERR(fmt, ...)
#endif

void debuggableFunction() {
    DEBUG_LOG("Function entry");

    if (someCondition) {
        DEBUG_WARN("Unusual condition detected");
    }

    DEBUG_LOG("Function exit");
}
```

## Error Handling

### Initialization Errors

```cpp
bool SystemConsole::initialize() {
    if (!ConsoleScreen::initialize()) {
        Serial.println("FATAL: Console initialization failed");
        return false;
    }

    // Verify buffer allocation
    if (!_logBuffer) {
        Serial.println("FATAL: Log buffer allocation failed");
        return false;
    }

    info("Console system ready - %d entries available", MAX_LOG_ENTRIES);
    return true;
}
```

### Buffer Overflow Protection

```cpp
// Buffer overflow is handled automatically by circular buffer
// Oldest entries are overwritten when buffer is full
// Progress bar shows buffer utilization percentage
```

## Complete Example

See `/examples/SimpleConsoleScreen/` for a complete working implementation that demonstrates:

- Real-time log capture and display
- Demo mode with simulated system events
- Filter cycling and manual navigation
- Integration with global logging system
- Statistics tracking and buffer management
- External logging from other system components

## Best Practices

### 1. Global Logger Pattern

```cpp
// Singleton pattern for system-wide logging
class LoggerManager {
private:
    static ILogger* _instance;

public:
    static void setLogger(ILogger* logger) { _instance = logger; }
    static ILogger* getLogger() { return _instance; }
};

// Usage throughout system
LoggerManager::getLogger()->info("System event");
```

### 2. Structured Logging

```cpp
// Use consistent format for easier parsing
logger->info("TEMP: sensor=%d value=%d status=%s", sensorId, temp, status);
logger->warn("MEM: free=%d used=%d fragmentation=%d%%", free, used, frag);
logger->err("I2C: addr=0x%02X error=%d retry=%d", addr, err, retry);
```

### 3. Performance Monitoring

```cpp
void monitorPerformance() {
    uint32_t startTime = micros();

    performCriticalTask();

    uint32_t duration = micros() - startTime;
    if (duration > 1000) {
        logger->warn("PERF: task took %d μs (threshold: 1000)", duration);
    }
}
```

### 4. Resource Management

```cpp
void ConsoleScreen::cleanup() {
    // Log shutdown sequence
    info("Console shutdown initiated");

    // Flush any pending logs
    _flushPrintBuffer();

    // Clear sensitive data
    memset(_logBuffer, 0, sizeof(_logBuffer));

    ConsoleScreen::cleanup();

    Serial.println("Console shutdown complete");
}
```
