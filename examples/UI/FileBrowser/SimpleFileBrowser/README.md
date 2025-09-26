# Simple FileBrowser Example

## What This Example Does

This simple FileBrowser example demonstrates:

1. **SD Card Navigation**: Browse directories and files on the SD card
2. **File Type Detection**: Automatically identifies text files (.txt, .log) vs binary files
3. **Automatic Viewer Selection**: Opens TextFileViewer for text files, BinaryFileViewer for others
4. **Directory Navigation**: Support for entering directories and using ".." to go up
5. **File Information**: Shows file sizes and directory indicators
6. **File Path Support**: Constructor examples showing various ways to specify target files

## What You'll Learn

- How to use the FileBrowser class for SD card navigation
- Understanding file type detection and viewer selection
- Directory navigation patterns and user interface
- Integration with TextFileViewer and BinaryFileViewer
- File size display and directory indicators
- Different constructor patterns including file path parsing

## Constructor Examples

The example includes commented examples showing different ways to create a FileBrowser using the unified constructor:

```cpp
// Basic browsing from root
M1Shield.setScreen(new FileBrowser());

// Pre-select a file in root directory
M1Shield.setScreen(new FileBrowser("/", "config.txt"));

// Browse specific directory
M1Shield.setScreen(new FileBrowser("/logs"));

// Browse specific directory with file selection
M1Shield.setScreen(new FileBrowser("/logs", "error.log"));

// Automatic file path parsing
M1Shield.setScreen(new FileBrowser("/logs/system.log"));

// Restricted browsing
M1Shield.setScreen(new FileBrowser("/data", "config.txt", true));
```

The unified constructor intelligently handles all usage patterns with default parameters and automatic file path parsing.

## What is FileBrowser?

FileBrowser is a MenuScreen-based component that provides:

- **SD Card Integration**: Direct access to SD card files and directories
- **Automatic Navigation**: Up/Down/Select navigation with directory support
- **Smart File Handling**: Automatic selection of appropriate file viewers
- **Directory Management**: Support for ".." navigation and directory restrictions
- **File Filtering**: Configurable file type filtering and extension management

## Controls

- **UP/DOWN**: Navigate through files and directories
- **RIGHT/MENU**: Select file or enter directory
- **LEFT**: Go back to parent directory (when available)

## File Type Handling

- **Text Files** (.txt, .log): Opens with TextFileViewer for easy reading
- **Binary Files** (all others): Opens with BinaryFileViewer for hex dump viewing
- **Directories**: Navigate into them to browse contents
- **Parent Directory** (..): Go up one level in the directory structure

## Getting Started

1. **Prepare SD Card**: Insert an SD card with some files and directories
2. **Update Display**: Uncomment the correct display provider for your hardware
3. **Upload and Run**: Upload the sketch and navigate through your files

## Hardware Requirements

- Arduino Mega 2560 (required for sufficient memory and GPIO pins)
- M1Shield with compatible display
- SD card with files to browse

## Display Recommendations

- **TFT Displays**: Recommended for best experience (ILI9341, ST7735, HX8357)
- **OLED Displays**: Work but have limited space for file lists (SSD1306, SH1106)

## Next Steps

After mastering this simple file browser:

1. Try creating files with different extensions to see type detection
2. Create nested directories to test navigation
3. Experiment with the ComprehensiveFileBrowser example for advanced features
4. Learn about directory restrictions and pre-file selection
