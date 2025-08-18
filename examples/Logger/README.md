# Logger Examples

This folder contains examples demonstrating the logging system capabilities for debugging, monitoring, and system analysis.

## Overview

The Arduino Library provides a comprehensive logging framework with multiple output destinations and advanced features. These examples show how to use different logger types for effective debugging and system monitoring.

## Logging System Components

The logging framework includes:

- **SerialLogger**: Basic logging to Serial Monitor
- **LoggerScreen**: Visual logging on M1Shield display
- **CompositeLogger**: Multi-destination logging system
- **Log Levels**: Info, Warning, and Error message types
- **Advanced Features**: Timestamps, color coding, muting, and formatting

## Examples

### [SerialLogger](SerialLogger/README.md)

**Basic serial output logging**

Learn fundamental logging concepts including:

- Basic serial output logging setup
- Different log levels (info, warn, err)
- Logger integration with Model1 class
- Muting and unmuting functionality
- Basic debugging techniques

### [CompositeLogger](CompositeLogger/README.md)

**Multi-destination logging system**

Explore advanced logging capabilities including:

- Simultaneous logging to multiple destinations
- SerialLogger and LoggerScreen integration
- Dynamic logger registration and management
- Real-time visual feedback with color coding
- Complex system monitoring and status reporting

## Key Concepts

### Log Levels

The logging system supports three message types:

- **Info**: General information and status messages
- **Warn**: Warning conditions that need attention
- **Error**: Error conditions requiring immediate action

### Output Destinations

Multiple logging destinations available:

- **Serial Monitor**: Traditional text-based debugging output
- **Display Screen**: Visual logging with color coding and timestamps
- **Composite**: Combined output to multiple destinations simultaneously

### Advanced Features

Professional logging capabilities:

- **Timestamps**: Automatic time stamping of messages
- **Color Coding**: Visual differentiation of log levels
- **Formatted Output**: Printf-style message formatting
- **Dynamic Control**: Runtime muting and logger management

## Important Considerations

### Serial Logging

- **Baud Rate**: Choose appropriate baud rate for your needs (9600-115200)
- **Buffer Management**: Be aware of serial buffer limitations
- **Performance**: High-frequency logging can impact system performance
- **Debugging**: Essential for troubleshooting and development

### Display Logging

- **Screen Space**: Limited display area for log messages
- **Visual Impact**: Color coding improves readability
- **Timestamps**: Help track message timing and sequence
- **Real-time Feedback**: Immediate visual confirmation of system status

### Composite Logging

- **Resource Usage**: Multiple destinations increase memory and processing overhead
- **Synchronization**: All registered loggers receive identical messages
- **Flexibility**: Can add/remove loggers dynamically
- **Redundancy**: Multiple outputs provide backup logging capability

## Common Use Cases

- **Development Debugging**: Track program execution and identify issues
- **System Monitoring**: Monitor hardware status and performance
- **Error Reporting**: Capture and report system errors and warnings
- **Status Updates**: Provide real-time system status information
- **Performance Analysis**: Track timing and performance metrics
- **User Feedback**: Provide visual feedback for system operations

## Getting Started

1. **Start with SerialLogger**: Learn basic logging concepts and setup
2. **Understand log levels**: Practice using info, warn, and error messages
3. **Explore integration**: Learn how to integrate with Model1 and other components
4. **Advanced features**: Move to CompositeLogger for multi-destination logging

## Integration with Other Examples

Logger examples integrate well with:

- **M1Shield Examples**: Use LoggerScreen for visual debugging
- **Model1 Examples**: Integrate logging for hardware debugging
- **UI Examples**: Add logging to user interface applications
- **System Projects**: Essential for complex system development

The logging framework provides essential debugging and monitoring capabilities for all Arduino Library projects.
