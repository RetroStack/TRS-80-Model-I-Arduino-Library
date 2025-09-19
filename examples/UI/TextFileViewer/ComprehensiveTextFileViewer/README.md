# Comprehensive Text File Viewer Example

This example demonstrates advanced usage of the TextFileViewer class with custom enhancements, menu integration, and real-time monitoring capabilities.

## What It Does

- Provides a menu-driven interface for viewing multiple files
- Demonstrates different viewer configurations for different file types
- Shows auto-paging functionality for log files
- Includes custom status display and automatic refresh
- Demonstrates TextFileViewer inheritance for enhanced features

## What You'll Learn

- Advanced TextFileViewer configuration and customization
- Integration with MenuScreen for file selection
- Auto-paging setup for monitoring log files
- Custom viewer enhancements through inheritance
- Real-time file monitoring and refresh techniques
- Different color schemes for different file types

## Hardware Requirements

- Arduino Mega 2560
- M1Shield with compatible display
- SD card with sample text files

## Required Files on SD Card

Create these sample files on your SD card for the full demo:

### readme.txt

```
TextFileViewer Demo
==================

Welcome to the comprehensive TextFileViewer demonstration!

This file shows basic text viewing capabilities with:
- Multi-page navigation
- Horizontal scrolling for long lines
- Memory-efficient display

Use the directional buttons to navigate:
- UP/DOWN: Page navigation
- LEFT/RIGHT: Horizontal scrolling
- A/B: Quick page navigation

This is page 1 of the readme file. Add more content
to see how the paging system works automatically!
```

### config.txt

```
# System Configuration File
# =========================

display.brightness=80
display.timeout=300
system.debug=true
network.enabled=false
storage.compression=true

# User preferences
user.name=RetroStack
user.theme=dark
user.startup_screen=main_menu

# Hardware settings
hardware.model=trs80_model1
hardware.memory=48k
hardware.expansion=disk_system
```

### debug.log

```
[2024-01-15 10:00:00] System startup initiated
[2024-01-15 10:00:01] Display initialized: 320x240
[2024-01-15 10:00:02] SD card mounted successfully
[2024-01-15 10:00:03] TextFileViewer loaded
[2024-01-15 10:00:04] Auto-paging enabled for log monitoring
[2024-01-15 10:00:05] System ready for operation
```

### system.ini

```
[Display]
Width=320
Height=240
ColorDepth=16
Rotation=0

[Memory]
HeapSize=8192
StackSize=2048
BufferSize=512

[Features]
TextViewer=enabled
MenuSystem=enabled
AutoPaging=enabled
Logging=debug
```

## Features Demonstrated

### Menu Integration

- File selection through MenuScreen
- Dynamic TextFileViewer creation
- Proper memory management

### Auto-Paging

- Automatic monitoring of log files
- Real-time content updates
- Status indicators for active monitoring

### Custom Enhancement

- Enhanced TextFileViewer with status display
- Automatic refresh timers
- Custom drawing overlays

### Configuration Options

- Different colors for different file types
- Adaptive text sizing
- Status information display

## Usage Instructions

1. **Setup Files**: Create the sample files on your SD card
2. **Upload Code**: Upload this example to your Arduino Mega 2560
3. **Navigate Menu**: Use UP/DOWN to select files, A to open
4. **View Files**: Navigate within files using directional controls
5. **Monitor Logs**: Select debug.log to see auto-paging in action

## Navigation Controls

### Menu Navigation

- UP/DOWN: Select file
- A Button: Open selected file
- B Button: (Menu handles)

### File Viewing

- UP/DOWN: Previous/Next page
- LEFT/RIGHT: Horizontal scrolling
- A Button: Next page
- B Button: Return to menu

## Code Architecture

### FileViewerMenu Class

- Extends MenuScreen for file selection
- Creates configured TextFileViewer instances
- Handles different file type settings

### EnhancedTextFileViewer Class

- Extends TextFileViewer with custom features
- Adds status information display
- Implements automatic refresh for logs

## Advanced Features

### Status Display

- Shows current page and total pages
- Indicates auto-paging mode
- Custom drawing with background

### Auto-Refresh

- Monitors file changes every 2 seconds
- Updates display when content grows
- Maintains current position when possible

### Memory Management

- Dynamic object creation and cleanup
- Efficient page-based loading
- Proper resource handling

## Next Steps

- Experiment with different file sizes and types
- Modify refresh intervals and display settings
- Add custom file type detection
- Implement file search and filtering features
