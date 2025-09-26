# Text File Viewer Examples

This directory contains examples demonstrating the TextFileViewer class for displaying text files from SD card with navigation and auto-update capabilities.

## Available Examples

### SimpleTextFileViewer

Basic example showing how to view text files with paging, horizontal scrolling, and auto-paging features.

## Key Features

- **File Navigation**: Page-based navigation for large text files
- **Horizontal Scrolling**: Support for long lines without text wrapping
- **Auto-Paging**: Real-time monitoring of file updates (perfect for log files)
- **Memory Efficient**: Optimized for Arduino memory constraints
- **Fast Rendering**: Smooth navigation and display updates

## Common Use Cases

- **Log File Monitoring**: View and monitor log files with auto-update
- **Configuration Files**: Display system configuration and settings files
- **Data Files**: View CSV, JSON, or other structured text data
- **Documentation**: Display README files and documentation
- **Debug Output**: Monitor debug logs and system output

## TextFileViewer Overview

The TextFileViewer class extends ContentScreen and provides:

```cpp
TextFileViewer viewer("filename.txt");
viewer.setAutoPaging(true);         // Enable auto-paging for log files
viewer.setTextSize(1);              // Set text size
viewer.setTextColor(0xFFFF);        // Set text color
```

## Navigation Controls

- **UP/DOWN**: Navigate between pages
- **LEFT/RIGHT**: Horizontal scrolling for long lines
- **MENU**: Toggle auto-paging mode

## File Requirements

- Files must be stored on SD card
- Text files with various line endings supported
- No automatic text wrapping (preserves original formatting)
- Real-time file update detection for auto-paging mode

All examples use the M1Shield hardware abstraction for consistent SD card access and display management.
