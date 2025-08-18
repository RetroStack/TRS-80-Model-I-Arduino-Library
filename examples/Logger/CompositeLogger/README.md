# CompositeLogger Example

This example demonstrates advanced multi-destination logging with SerialLogger and LoggerScreen integration.

## What You'll Learn

- How to set up multi-destination logging with CompositeLogger
- How to register and manage multiple logger instances
- How to create visual logging with LoggerScreen on M1Shield
- How to implement real-time system monitoring with synchronized output
- Advanced logging techniques for complex applications

## What It Does

This comprehensive example provides a complete multi-destination logging system:

1. **Multi-Destination Setup**: Logs simultaneously to Serial Monitor and M1Shield display
2. **Dynamic Logger Management**: Register and manage multiple logger instances
3. **Real-Time Monitoring**: Simulates device monitoring with status updates
4. **Visual Feedback**: Color-coded messages on display with timestamps
5. **System Simulation**: Demonstrates logging in a realistic monitoring scenario

## Key Features

### Composite Logging System

- **SerialLogger Integration**: Traditional serial debugging output
- **LoggerScreen Integration**: Visual logging on M1Shield display
- **Synchronized Output**: Identical messages to all registered destinations
- **Dynamic Registration**: Add/remove loggers at runtime

### Advanced Visual Features

- **Color Coding**: Different colors for Info, Warn, and Error messages
- **Timestamps**: Automatic time stamping of all messages
- **Scrolling Display**: Automatic message scrolling on screen
- **Real-Time Updates**: Live system status and monitoring

### System Monitoring Simulation

- **Device Management**: Simulates monitoring of 5 different devices
- **Status Changes**: Dynamic online/offline state changes
- **Performance Metrics**: CPU usage, memory, and connection statistics
- **Error Simulation**: Realistic error conditions and reporting

## Technical Implementation

### Composite Logger Setup

```cpp
#include <CompositeLogger.h>
#include <SerialLogger.h>
#include <LoggerScreen.h>

SerialLogger serialLogger;
LoggerScreen *screenLogger;
CompositeLogger multiLogger;

void setup() {
    // Create screen logger
    screenLogger = new LoggerScreen("Multi-Log Demo");
    screenLogger->setTimestampEnabled(true);
    screenLogger->setColorCodingEnabled(true);

    // Register both loggers
    multiLogger.addLogger(&serialLogger);
    multiLogger.addLogger(screenLogger->asLogger());
}
```

### Multi-Destination Logging

```cpp
// This message appears in BOTH Serial Monitor AND on display
multiLogger.info("System status: %d devices online", onlineCount);
multiLogger.warn("Low signal detected on device %d", deviceId);
multiLogger.err("Connection failed: %s", errorMessage);
```

### Advanced Features

- **Printf-Style Formatting**: Full printf formatting support
- **Dynamic Logger Count**: Track number of active loggers
- **State Management**: Comprehensive system state monitoring
- **Performance Tracking**: Monitor logging system performance

## Sample Output

### Serial Monitor Output

```
=== Multi-Destination Logging Example ===
Messages will appear in both Serial Monitor and on-screen display

Setting up multi-destination logging...
Serial logger registered
Screen logger registered
Total loggers active: 2

[INFO] Multi-destination logging system started
[INFO] Active destinations: Serial + Screen
[INFO] Scanning for 5 devices...
[INFO] Device scan initiated
[INFO] Temperature Sensor: Signal 85%
[ERR ] Humidity Sensor: No response
[INFO] Motion Detector: Signal 92%
[WARN] Weak signal on Light Sensor: 58%
[INFO] Scan complete: 3 online, 2 offline
[WARN] 2 devices need attention
```

### Screen Display Output

```
Multi-Log Demo
===============
[00:01] [INFO] Multi-destination logging system started
[00:01] [INFO] Active destinations: Serial + Screen
[00:01] [INFO] Scanning for 5 devices...
[00:01] [INFO] Device scan initiated
[00:01] [INFO] Temperature Sensor: Signal 85%
[00:01] [ERR ] Humidity Sensor: No response
[00:01] [INFO] Motion Detector: Signal 92%
[00:01] [WARN] Weak signal on Light Sensor: 58%
[00:01] [INFO] Scan complete: 3 online, 2 offline
[00:01] [WARN] 2 devices need attention
```

## Hardware Requirements

- Arduino Mega 2560 (required for sufficient GPIO pins and memory)
- M1Shield with compatible display for LoggerScreen
- Serial connection for SerialLogger output
- Sufficient memory for dual logging operations

## DisplayProvider Configuration

Select your display provider for LoggerScreen:

```cpp
// For ST7789 displays (most common)
#include <Display_ST7789_320x240.h>
Display_ST7789_320x240 displayProvider;

// For other display types, see M1Shield examples
```

## Monitoring Features

### Device Simulation

- **5 Different Devices**: Temperature, Humidity, Motion, Light, Door sensors
- **Dynamic States**: Devices go online/offline randomly
- **Signal Strength**: Variable signal strength with warnings
- **Realistic Behavior**: Simulates real-world device monitoring

### System Metrics

- **Connection Statistics**: Track connection attempts and success rate
- **Performance Monitoring**: CPU usage and memory tracking
- **Uptime Tracking**: System operational time
- **Error Simulation**: Random error conditions for testing

### Advanced Logging Patterns

- **Periodic Scanning**: Device scans every 8 seconds
- **Status Reports**: System status every 15 seconds
- **Real-Time Updates**: Immediate response to state changes
- **Comprehensive Coverage**: All system aspects monitored

## Benefits of Multi-Destination Logging

### Serial Monitor Benefits

- **Detailed Logging**: Complete message history
- **Copy/Paste**: Easy to copy messages for analysis
- **Permanent Record**: Logging survives system resets
- **Development Focus**: Traditional debugging environment

### Display Screen Benefits

- **Real-Time Visual**: Immediate visual feedback
- **Color Coding**: Quick visual identification of message types
- **Standalone Operation**: Works without computer connection
- **User Interface**: Integrated with application display

### Combined Benefits

- **Redundancy**: Message backup across multiple destinations
- **Flexibility**: Choose appropriate output for situation
- **Debugging**: Serial for development, display for operation
- **Documentation**: Visual and textual logging simultaneously

## Usage Notes

- Requires more memory and processing than single-destination logging
- Display updates may affect system performance with high-frequency logging
- Color coding improves message categorization and readability
- Essential for applications requiring both development and operational logging

## Integration with Other Examples

This CompositeLogger example pairs well with:

- **SerialLogger**: Start here after mastering basic logging
- **M1Shield Examples**: Foundation for visual debugging
- **Complex Projects**: Essential for multi-component systems

CompositeLogger provides professional-grade logging capabilities for serious Arduino Library applications requiring comprehensive monitoring and debugging.
