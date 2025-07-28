# LoggerScreen

The `LoggerScreen` is a visual logging destination that combines the functionality of `ConsoleScreen` with the `ILogger` interface. It provides real-time, on-screen logging with formatted output, color coding, and timestamps - perfect for debugging and monitoring applications on the M1Shield display.

## Features

- **Visual Logging**: Display log messages directly on the M1Shield screen
- **ILogger Interface**: Full compatibility with the logging system and CompositeLogger
- **Color-Coded Levels**: Different colors for INFO (white), WARN (yellow), and ERR (red)
- **Timestamps**: Optional relative timestamps showing elapsed time
- **Auto-Scrolling**: Automatic screen clearing when full for continuous logging
- **Print Interface**: Inherited from ConsoleScreen for direct text output
- **Real-Time Feedback**: Immediate visual feedback for debugging and monitoring

## Usage

### Basic Logger Screen

```cpp
#include <LoggerScreen.h>
#include <M1Shield.h>

LoggerScreen* logScreen = new LoggerScreen("System Monitor");

void setup() {
    M1Shield.begin(displayProvider);
    M1Shield.setScreen(logScreen);
    
    // Log some messages
    logScreen->info("System starting up...");
    logScreen->warn("Configuration using defaults");
    logScreen->err("Sensor not detected");
}

void loop() {
    // Your application code here
    logScreen->info("Loop iteration %d", loopCount++);
    delay(1000);
}
```

### Integration with CompositeLogger

```cpp
#include <CompositeLogger.h>
#include <SerialLogger.h>
#include <LoggerScreen.h>

SerialLogger serialLogger;
LoggerScreen* screenLogger = new LoggerScreen("Debug Log");
CompositeLogger multiLogger;

void setup() {
    Serial.begin(115200);
    M1Shield.begin(displayProvider);
    
    // Set up multi-destination logging
    multiLogger.addLogger(&serialLogger);    // Log to Serial
    multiLogger.addLogger(screenLogger);     // Log to screen
    
    M1Shield.setScreen(screenLogger);
    
    // Now logs appear in both Serial Monitor and on screen
    multiLogger.info("Multi-destination logging active");
    multiLogger.warn("System memory: %d bytes free", getFreeMemory());
}
```

### Configuration Options

```cpp
LoggerScreen* logger = new LoggerScreen("Custom Logger");

// Configure display options
logger->setTimestampEnabled(false);      // Disable timestamps
logger->setColorCodingEnabled(false);    // Use monochrome output
logger->setTextSize(2);                  // Larger text
logger->setConsoleBackground(0x0010);    // Dark blue background

// Reset timestamp counter
logger->resetTimestamp();
```

## API Reference

### Constructor

```cpp
LoggerScreen(const char* title = "Logger")
```
Creates a new logger screen with the specified title.

### Configuration Methods

```cpp
void setTimestampEnabled(bool enabled)
bool isTimestampEnabled() const
```
Control whether timestamps are displayed in log messages.

```cpp
void setColorCodingEnabled(bool enabled)
bool isColorCodingEnabled() const
```
Control whether different log levels use different colors.

```cpp
void resetTimestamp()
```
Reset the timestamp reference point to the current time.

### ILogger Interface

```cpp
void info(const char *fmt, ...)
void warn(const char *fmt, ...)
void err(const char *fmt, ...)
```
Standard logging methods with printf-style formatting. Messages are displayed with appropriate formatting and colors.

### Inherited from ConsoleScreen

The LoggerScreen inherits all ConsoleScreen functionality:

```cpp
// Print interface methods
size_t write(uint8_t ch)
size_t print(...)
size_t println(...)

// Console control
void cls()
void refresh()
void setTextColor(uint16_t foreground, uint16_t background = 0)
void setConsoleBackground(uint16_t color)
void setTextSize(uint8_t size)
void setTabSize(uint8_t size)
```

## Visual Output Format

### With Timestamps (Default)
```
[12:34] [INFO] System initialized
[12:35] [WARN] Low memory: 512 bytes
[12:36] [ERR ] Sensor timeout on pin 7
```

### Without Timestamps
```
[INFO] System initialized
[WARN] Low memory: 512 bytes
[ERR ] Sensor timeout on pin 7
```

### Color Coding
- **INFO**: White text - normal system information
- **WARN**: Yellow text - warnings and non-critical issues
- **ERR**: Red text - errors and critical problems
- **Timestamps**: Light gray text for easy distinction

## Use Cases

### System Diagnostics

```cpp
LoggerScreen* diagLogger = new LoggerScreen("Diagnostics");

void runDiagnostics() {
    diagLogger->info("Starting system diagnostics...");
    
    if (testMemory()) {
        diagLogger->info("Memory test: PASSED");
    } else {
        diagLogger->err("Memory test: FAILED");
    }
    
    if (testSensors()) {
        diagLogger->info("Sensor test: PASSED");
    } else {
        diagLogger->warn("Sensor test: Some sensors offline");
    }
    
    diagLogger->info("Diagnostics complete");
}
```

### Real-Time Monitoring

```cpp
LoggerScreen* monitor = new LoggerScreen("System Monitor");

void loop() {
    int memFree = getFreeMemory();
    int tempC = readTemperature();
    
    if (memFree < 512) {
        monitor->warn("Low memory: %d bytes free", memFree);
    }
    
    if (tempC > 75) {
        monitor->err("High temperature: %d°C", tempC);
    } else {
        monitor->info("Status OK - Temp: %d°C, Mem: %d", tempC, memFree);
    }
    
    delay(5000);
}
```

### Debug Output

```cpp
LoggerScreen* debugLog = new LoggerScreen("Debug Output");

void debugFunction() {
    debugLog->info("Entering function: %s", __FUNCTION__);
    
    for (int i = 0; i < sensorCount; i++) {
        int value = readSensor(i);
        if (value < 0) {
            debugLog->err("Sensor %d read error: %d", i, value);
        } else {
            debugLog->info("Sensor %d: %d", i, value);
        }
    }
    
    debugLog->info("Function complete");
}
```

### Development vs Production Logging

```cpp
CompositeLogger logger;
SerialLogger serialLogger;

void setupLogging() {
    // Always log to serial for development
    logger.addLogger(&serialLogger);
    
    #ifdef DEBUG_BUILD
    // In debug builds, also show on screen
    LoggerScreen* screenLogger = new LoggerScreen("Debug Log");
    logger.addLogger(screenLogger);
    M1Shield.setScreen(screenLogger);
    #endif
    
    logger.info("Logging system configured");
}
```

## Performance Considerations

- **Memory Usage**: Uses string buffers for formatting (256 bytes for log lines)
- **Display Updates**: Each log message triggers a screen update
- **Color Changes**: Color coding requires additional text color changes
- **Scrolling**: Automatic screen clearing when full provides consistent performance

## Integration with M1Shield

The LoggerScreen works seamlessly with the M1Shield framework:

- **Navigation**: Standard M1Shield navigation (Menu button to exit)
- **Display Providers**: Compatible with all DisplayProvider types
- **Screen Management**: Full lifecycle management by M1Shield
- **Input Handling**: Inherits standard screen input handling

## Thread Safety

LoggerScreen is not inherently thread-safe. In multi-threaded environments, external synchronization is required when multiple threads log to the same LoggerScreen instance.
