# SerialLogger Example

This example demonstrates basic serial output logging for debugging and system monitoring.

## What You'll Learn

- How to set up and use SerialLogger for debugging output
- Understanding different log levels (info, warn, err)
- How to integrate logging with the Model1 class
- How to control logging output with muting/unmuting
- Basic debugging techniques and best practices

## What It Does

This simple example provides a foundation for serial-based debugging:

1. **Logger Setup**: Initializes SerialLogger with Model1 integration
2. **Log Level Demonstration**: Shows info, warn, and error message types
3. **Model1 Integration**: Demonstrates how Model1 uses the logger
4. **Muting Control**: Shows how to temporarily disable logging output
5. **State Logging**: Uses Model1's built-in state logging capability

## Key Features

### Basic Logging Operations

- **SerialLogger Creation**: Simple logger instantiation and setup
- **Model1 Integration**: Logger registration with Model1 class
- **Direct Usage**: Using logger independently from Model1
- **Log Levels**: Demonstration of info, warn, and err methods

### Output Control

- **Muting**: Temporarily disable all logging output
- **Unmuting**: Re-enable logging output
- **Selective Logging**: Control what gets logged and when

### System Integration

- **Model1 State Logging**: Built-in hardware state debugging
- **Error Reporting**: Automatic error logging from Model1 operations
- **Development Support**: Essential debugging during development

## Technical Implementation

### Logger Setup

```cpp
#include <SerialLogger.h>

// Create logger instance
SerialLogger logger;

void setup() {
    Serial.begin(9600);
    Model1.begin();
    Model1.setLogger(logger);  // Register with Model1
}
```

### Log Level Usage

- **Info Messages**: `logger.info("System is setup")`
- **Warning Messages**: `logger.warn("System is activated!")`
- **Error Messages**: `logger.err("Operation failed")`

### Muting Control

```cpp
logger.mute();      // Disable output
logger.info("This won't appear");
logger.unmute();    // Re-enable output
logger.info("This will appear");
```

## Sample Output

```
[INFO] System is setup
[WARN] System is activated!
[ERR ] Bus operation failed: TEST* signal not active
[INFO] I am back!
[INFO] Model1 State: Bus=INACTIVE, TEST*=LOW, Clock=STOPPED
[ERR ] Memory write failed: Address 0x3400, Value 128
```

## Hardware Requirements

- Arduino Mega 2560 (required for sufficient GPIO pins)
- Serial connection to computer for monitoring
- Optional: TRS-80 Model I for full Model1 functionality

## Understanding the Output

### Log Format

Each log message includes:

- **Level Indicator**: [INFO], [WARN], or [ERR ]
- **Message Content**: Your custom message text
- **System Context**: Relevant system information

### Log Levels Explained

- **INFO**: General information about system operation
- **WARN**: Conditions that may need attention but aren't critical
- **ERR**: Error conditions that prevent normal operation

### Model1 Integration

When logger is registered with Model1:

- Hardware operations automatically generate log messages
- Error conditions are automatically reported
- State information can be logged on demand

## Usage Notes

- Set appropriate baud rate for your debugging needs
- Higher baud rates allow more frequent logging
- Be mindful of serial buffer limitations
- Use muting to reduce output during normal operation
- Essential tool for development and troubleshooting

## Common Debugging Patterns

### Startup Sequence

```cpp
logger.info("System initializing...");
Model1.begin();
logger.info("Hardware initialized");
logger.info("System ready");
```

### Error Handling

```cpp
if (!Model1.writeMemory(address, value)) {
    logger.err("Memory write failed: Address 0x%04X", address);
}
```

### Status Monitoring

```cpp
logger.info("Checking system status...");
Model1.logState();  // Logs current hardware state
```

## Integration with Other Examples

This SerialLogger example pairs well with:

- **Model1 Examples**: Add debugging to hardware operations
- **CompositeLogger**: Upgrade to multi-destination logging
- **Development Projects**: Essential debugging tool

SerialLogger provides the foundation for effective debugging and system monitoring in all Arduino Library projects.
