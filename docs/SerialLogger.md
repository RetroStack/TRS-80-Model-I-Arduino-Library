# SerialLogger Class

The `SerialLogger` class implements the `ILogger` interface and sends formatted log messages to the Arduino Serial interface. It provides a simple, reliable way to output debug and status information during development and runtime.

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Constructor](#constructor)
- [Method Signatures](#method-signatures)
  - [Core Logging Methods](#core-logging-methods)
  - [Inherited Convenience Methods](#inherited-convenience-methods)
  - [Control Methods](#control-methods)
  - [Print Interface](#print-interface)
- [Usage Examples](#usage-examples)
  - [Basic Logging](#basic-logging)
  - [Printf-style Formatting](#printf-style-formatting)
  - [String Objects and F() Macro](#string-objects-and-f-macro)
  - [Mute/Unmute Control](#muteunmute-control)
- [Output Format](#output-format)
- [Implementation Details](#implementation-details)
- [Integration Examples](#integration-examples)
- [Performance Considerations](#performance-considerations)
- [See Also](#see-also)

## Overview

The `SerialLogger` class implements the `ILogger` interface and sends formatted log messages to the Arduino Serial interface. It provides a simple, reliable way to output debug and status information during development and runtime.

## Features

- **Standard Log Levels**: Info, warning, and error message types with prefixes
- **Printf-style Formatting**: Full support for format strings and variable arguments
- **String & F() Macro Support**: Inherits convenience methods from `ILogger` interface
- **Mute/Unmute**: Ability to temporarily disable logging output
- **Serial Integration**: Direct output to Arduino Serial interface

## Constructor

```cpp
SerialLogger()
```

Creates a new SerialLogger instance in unmuted state. No parameters required.

## Method Signatures

### Core Logging Methods

```cpp
void info(const char *fmt, ...);     // Info messages with [INFO] prefix
void warn(const char *fmt, ...);     // Warning messages with [WARN] prefix
void err(const char *fmt, ...);      // Error messages with [ERR ] prefix
```

### Inherited Convenience Methods

```cpp
// String object support
void info(const String &msg);
void warn(const String &msg);
void err(const String &msg);

// F() macro support (flash strings)
void info(const __FlashStringHelper *msg);
void warn(const __FlashStringHelper *msg);
void err(const __FlashStringHelper *msg);
```

### Control Methods

```cpp
void mute();      // Disable all logging output
void unmute();    // Re-enable logging output
```

### Print Interface

```cpp
size_t write(uint8_t ch) override;
size_t write(const uint8_t *buffer, size_t size) override;
```

## Usage Examples

### Basic Logging

```cpp
SerialLogger logger;

void setup() {
    Serial.begin(9600);

    logger.info("System initialized");
    logger.warn("Low memory detected");
    logger.err("Failed to initialize sensor");
}
```

### Printf-style Formatting

```cpp
int sensorValue = 42;
float voltage = 3.3;
const char* status = "OK";

logger.info("Sensor reading: %d (%.2fV) - %s", sensorValue, voltage, status);
logger.warn("Temperature: %d°C exceeds threshold", temperature);
logger.err("Error code: 0x%02X in module %s", errorCode, moduleName);
```

### String Objects and F() Macro

```cpp
String deviceName = "DHT22";
int reading = 25;

// String object
logger.info(String("Device: ") + deviceName + " ready");

// F() macro (saves RAM)
logger.info(F("Starting temperature monitoring..."));
logger.warn(F("Calibration required"));

// Mixed usage
logger.info("Device %s reports %d°C", deviceName.c_str(), reading);
```

### Mute/Unmute Control

```cpp
SerialLogger logger;

logger.info("Starting diagnostics...");
logger.mute();
logger.info("This message will not appear");
logger.warn("Neither will this warning");
logger.unmute();
logger.info("Diagnostics complete");
```

## Output Format

Messages are formatted with prefixes for easy identification:

```
[INFO] System initialized
[WARN] Low memory: 512 bytes free
[ERR ] Connection timeout after 5000ms
```

## Implementation Details

### Internal Formatting

- Uses `vsnprintf()` for safe string formatting
- Buffer size limited to prevent memory overflow
- Thread-safe on single-core Arduino systems

### Mute State

- When muted, all logging methods return immediately
- No processing overhead when logging is disabled
- Mute state persists until explicitly unmuted

### Serial Dependency

- Requires `Serial.begin()` to be called before use
- Output appears in Serial Monitor or connected terminal
- No buffering - messages sent immediately

## Integration Examples

### With Global Logger

```cpp
#include "SerialLogger.h"

SerialLogger globalLogger;

void setup() {
    Serial.begin(9600);
    globalLogger.info("Application started");
}

void someFunction() {
    globalLogger.warn("Function called with invalid parameter");
}
```

### With Component Integration

```cpp
class SensorManager {
private:
    ILogger* _logger;

public:
    SensorManager(ILogger* logger) : _logger(logger) {}

    void initialize() {
        _logger->info("Initializing sensors...");
        // Sensor init code
        _logger->info("Sensors ready");
    }
};

SerialLogger logger;
SensorManager sensors(&logger);
```

### Development vs Production

```cpp
SerialLogger logger;

void setup() {
    Serial.begin(9600);

    #ifdef DEBUG
        logger.unmute();
        logger.info("Debug mode enabled");
    #else
        logger.mute();  // Disable logging in production
    #endif
}
```

## Performance Considerations

- **Minimal Overhead**: When muted, logging calls have near-zero cost
- **Serial Speed**: Output speed limited by Serial baud rate
- **Memory Usage**: Small memory footprint, no dynamic allocation
- **Format Processing**: Printf formatting adds some CPU overhead

## See Also

- [ILogger](ILogger.md) - Base interface documentation
- [CompositeLogger](CompositeLogger.md) - Multi-destination logging
- [LoggerScreen](LoggerScreen.md) - On-screen log display
