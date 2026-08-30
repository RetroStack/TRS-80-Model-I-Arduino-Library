# SDCardLogger Class

The `SDCardLogger` class implements the `ILogger` interface and appends formatted log messages to a file on the SD card. It is intended for logging that must survive a reset or power cycle, where serial output is not available.

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Constructor](#constructor)
- [Initialization](#initialization)
- [Method Signatures](#method-signatures)
  - [Core Logging Methods](#core-logging-methods)
  - [Inherited Convenience Methods](#inherited-convenience-methods)
  - [Control Methods](#control-methods)
  - [Print Interface](#print-interface)
- [Usage Examples](#usage-examples)
  - [Basic Logging](#basic-logging)
  - [Checking Initialization](#checking-initialization)
  - [Mute/Unmute Control](#muteunmute-control)
- [Output Format](#output-format)
- [Implementation Details](#implementation-details)
- [Performance Considerations](#performance-considerations)
- [See Also](#see-also)

## Overview

`SDCardLogger` writes each log line to a file on the SD card, opening the file in append mode for every message. The SD chip select pin is taken from `M1Shield.getSDCardSelectPin()`, so the shield must be initialized before the logger.

## Features

- **Persistent Output**: Log survives resets and power cycles
- **Standard Log Levels**: Info, warning, error and debug with prefixes
- **Printf-style Formatting**: Full support for format strings and variable arguments
- **String & F() Macro Support**: Inherits convenience methods from `ILogger`
- **Mute/Unmute**: Ability to temporarily disable logging output
- **Configurable Filename**: Defaults to `log.txt`

## Constructor

```cpp
SDCardLogger(const char *filename = "log.txt")
```

Creates a logger that appends to `filename` on the SD card. The pointer is stored, not copied - pass a string literal or a buffer that outlives the logger.

## Initialization

```cpp
bool begin();
```

Initializes the SD card via `SD.begin(M1Shield.getSDCardSelectPin())`. Returns `false` if the card cannot be initialized.

> **Check the return value.** If `begin()` fails, the logging methods still accept calls but every message is silently discarded, so a missing or unreadable card produces a system that appears to run normally and logs nothing.

Call `M1Shield.begin(...)` before `SDCardLogger::begin()`, since the chip select pin comes from the shield.

## Method Signatures

### Core Logging Methods

```cpp
void info(const char *fmt, ...);     // Info messages with [INFO] prefix
void warn(const char *fmt, ...);     // Warning messages with [WARN] prefix
void err(const char *fmt, ...);      // Error messages with [ERR ] prefix
void debug(const char *fmt, ...);    // Debug messages with [DBUG] prefix
```

### Inherited Convenience Methods

```cpp
// String object support
void info(const String &msg);
void warn(const String &msg);
void err(const String &msg);
void debug(const String &msg);

// F() macro support (flash strings)
void info(const __FlashStringHelper *msg);
void warn(const __FlashStringHelper *msg);
void err(const __FlashStringHelper *msg);
void debug(const __FlashStringHelper *msg);
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
#include <Display_ST7789_320x240.h>
#include <M1Shield.h>
#include <SDCardLogger.h>

Display_ST7789_320x240 displayProvider;
SDCardLogger logger("session.log");

void setup() {
    M1Shield.begin(displayProvider);
    logger.begin();

    logger.info("System initialized");
    logger.warn("Low memory: %d bytes free", freeMemory());
    logger.err(F("Failed to read ROM"));
}
```

### Checking Initialization

```cpp
SDCardLogger logger;

void setup() {
    M1Shield.begin(displayProvider);

    if (!logger.begin()) {
        // No card, or the card could not be initialized. Fall back to
        // serial so the failure is visible.
        Serial.begin(115200);
        Serial.println(F("SD logging unavailable"));
    }
}
```

### Mute/Unmute Control

```cpp
logger.info("Starting bulk transfer...");
logger.mute();          // Avoid one SD write per byte
transferRomImage();
logger.unmute();
logger.info("Transfer complete");
```

## Output Format

Messages are appended one per line, with the same prefixes the other loggers use:

```
[INFO] System initialized
[WARN] Low memory: 512 bytes free
[ERR ] Connection timeout after 5000ms
[DBUG] Variable x=42, state=READY
```

## Implementation Details

- Formatting uses `vsnprintf()` into a 255-byte stack buffer; longer messages are truncated.
- The file is opened in append mode and closed for each message, so the log is consistent on disk even if the board resets mid-run.
- Each call currently performs **two** open/close cycles - one for the level prefix and one for the message body.
- When muted, the logging methods return before touching the card.

## Performance Considerations

- **Cost per message**: Each `close()` flushes the FAT and the directory entry, so a single log line costs several tens of milliseconds and rewrites the same sectors repeatedly. Do not log from a tight loop or from timing-sensitive bus code.
- **Flash wear**: The repeated directory-sector rewrites wear the card faster than the log size alone suggests.
- **Alternative**: For high-rate logging, use [CompositeLogger](CompositeLogger.md) with [SerialLogger](SerialLogger.md) during development and enable the SD destination only when persistence is required.
- **Memory usage**: One 255-byte stack buffer during formatting; no dynamic allocation.

## See Also

- [ILogger](ILogger.md) - Base interface documentation
- [SerialLogger](SerialLogger.md) - Serial port logging
- [CompositeLogger](CompositeLogger.md) - Multi-destination logging
- [LoggerScreen](LoggerScreen.md) - On-screen log display
- [M1Shield](M1Shield.md) - `getSDCardSelectPin()` and `isSDCardInserted()`
