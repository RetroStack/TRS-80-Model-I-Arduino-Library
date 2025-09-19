# Simple Text File Viewer Example

## What It Does

This example demonstrates the TextFileViewer class, which displays text files from an SD card with full navigation, scrolling, and auto-update capabilities. The viewer provides a file reader interface that can handle large text files with efficient paging.

## What You'll Learn

- How to create and configure a TextFileViewer screen
- File navigation with paging (UP/DOWN)
- Horizontal scrolling for long lines (LEFT/RIGHT)
- Auto-paging for log files that are being updated
- Text appearance customization (color, size)
- Memory-efficient file reading and display

## Hardware Requirements

- Arduino Mega 2560
- M1Shield with display
- SD card with text files

## Key Features

### Navigation

- **Page Navigation**: UP/DOWN buttons or joystick to navigate through pages
- **Horizontal Scrolling**: LEFT/RIGHT buttons or joystick to scroll long lines
- **Auto-Paging Toggle**: SELECT button to enable/disable automatic file monitoring
- **Auto-Paging**: Automatic detection of file updates and jump to last page

### Display Features

- **No Text Wrapping**: Lines maintain their original structure
- **Fast Rendering**: Efficient content drawing for smooth navigation
- **Page Information**: Current page and total pages displayed in footer
- **Scroll Indicator**: Shows horizontal scroll position when scrolling

### File Handling

- **Large File Support**: Memory-efficient loading of text files
- **Real-time Updates**: Auto-paging mode checks for file changes every second
- **Multiple Formats**: Handles various text file formats and line endings

## Usage

1. Place a text file named "readme.txt" on your SD card
2. Upload the example to your Arduino
3. Use the controls to navigate the file

## Controls

- **UP/DOWN Buttons/Joystick**: Navigate between pages
- **LEFT/RIGHT Buttons/Joystick**: Scroll horizontally for long lines
- **MENU Button**: Toggle auto-paging mode (useful for log files)

## Configuration Options

```cpp
// Text appearance
viewer->setTextColor(color);         // Set text color
viewer->setBackgroundColor(color);   // Set background color
viewer->setTextSize(size);           // Set text size (1-3)

// Auto-paging for log files
viewer->setAutoPaging(true);         // Enable auto-paging

// Manual navigation
viewer->goToPage(pageNumber);        // Jump to specific page
viewer->goToLastPage();              // Jump to last page
viewer->resetHorizontalScroll();     // Reset horizontal scroll
```

## File Format Support

The TextFileViewer supports:

- Plain text files (.txt)
- Log files with continuous updates
- Files with various line endings (Windows, Unix, Mac)
- Files with long lines (horizontal scrolling)
- Large files (memory-efficient paging)

## Auto-Paging Mode

When enabled, auto-paging mode:

- Checks file for updates every second
- Automatically jumps to the last page when new content is detected
- Perfect for monitoring log files that are being written to
- Can be toggled on/off with the SELECT button

## Performance Notes

- Files are loaded into memory for fast access
- Only the visible page is rendered for optimal performance
- Horizontal scrolling moves in 5-character increments
- Large files are handled efficiently with minimal Arduino RAM usage

## Example Files

Try creating these example files on your SD card:

**readme.txt** - A simple text file:

```
Welcome to the TextFileViewer!

This is a demonstration of the text file viewing capabilities.

Features:
- Page navigation with UP/DOWN
- Horizontal scrolling with LEFT/RIGHT
- Auto-paging for log files
- Fast rendering and navigation

You can scroll left and right to see this very long line that extends beyond the screen width and demonstrates the horizontal scrolling feature.

End of file.
```

**log.txt** - A log file for testing auto-paging:

```
2024-01-01 10:00:00 - System started
2024-01-01 10:01:00 - User logged in
2024-01-01 10:02:00 - File accessed
```

With auto-paging enabled, the viewer will automatically jump to show new log entries as they're added to the file.
