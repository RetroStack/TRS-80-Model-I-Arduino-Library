# FileBrowser Examples

This directory contains examples demonstrating the FileBrowser class, which provides SD card file and directory browsing capabilities using the MenuScreen framework.

## Examples Overview

### [SimpleFileBrowser](SimpleFileBrowser/)

**Purpose**: Basic SD card file browsing  
**Difficulty**: Beginner  
**Features**:

- Root directory browsing
- Automatic file type detection
- TextFileViewer for .txt and .log files
- BinaryFileViewer for all other files
- Directory navigation with ".." support

### [ComprehensiveFileBrowser](ComprehensiveFileBrowser/)

**Purpose**: Advanced FileBrowser features and configurations  
**Difficulty**: Advanced  
**Features**:

- Multiple browser configurations
- Directory restrictions and root limiting
- File pre-selection and auto-scrolling
- Custom file type extensions
- MenuScreen integration

## What is FileBrowser?

FileBrowser is a MenuScreen-based component that provides comprehensive SD card file and directory browsing capabilities:

### Core Features

- **Directory Navigation**: Browse folders with ".." support for going up
- **File Type Detection**: Automatically detects text vs binary files
- **Smart Viewer Selection**: Opens appropriate viewer based on file type
- **Directory Restrictions**: Optional root directory limiting for security
- **File Pre-selection**: Automatically scroll to and highlight specific files

### File Type Handling

- **Text Files** (.txt, .log by default): Opens with TextFileViewer
- **Binary Files** (all others): Opens with BinaryFileViewer for hex dump
- **Custom Extensions**: Configurable file type associations
- **Directory Support**: Navigate into directories and use ".." to go up

### Advanced Capabilities

- **Root Restrictions**: Prevent navigation above a specified directory
- **Target File Selection**: Pre-select and scroll to specific files
- **Custom File Extensions**: Configure which files open with which viewers
- **Integration Ready**: Works seamlessly with MenuScreen applications

## Learning Path

1. **Start with SimpleFileBrowser** - Learn basic file browsing concepts
2. **Explore ComprehensiveFileBrowser** - See advanced features and configurations
3. **Study the API** - Read the full documentation for integration patterns

## Class Hierarchy

```
Screen (base class)
+-- ContentScreen (adds header/footer/progress)
    +-- MenuScreen (adds menu navigation)
        +-- FileBrowser (adds file system browsing)
```

FileBrowser inherits ALL MenuScreen and ContentScreen features:

- User feedback systems (alert, confirm, notify)
- Automatic menu navigation and pagination
- Header, footer, and progress bar support
- Adaptive layouts for different display sizes

## Hardware Requirements

- **Arduino Mega 2560**: Required for sufficient memory and GPIO pins
- **M1Shield**: With compatible display
- **SD Card**: With files and directories to browse

## Display Recommendations

- **TFT Displays**: Recommended for best file browsing experience

  - ILI9341 (320x240) - Excellent for file lists
  - ST7735 (128x160) - Good for basic browsing
  - HX8357 (480x320) - Best for large file lists

- **OLED Displays**: Work but limited space for file lists
  - SSD1306 (128x64) - Minimal but functional
  - SH1106 (128x64) - Similar to SSD1306

## Common Use Cases

### Application File Management

```cpp
// Browse application data with restriction
FileBrowser* browser = new FileBrowser("/appdata", "", true);
```

### Log File Viewer

```cpp
// Browse logs and pre-select latest log
FileBrowser* browser = new FileBrowser("/logs", "latest.log");
```

### Configuration Editor

```cpp
// Custom text file types for config files
FileBrowser* browser = new FileBrowser("/config");
browser->addTextExtension("cfg");
browser->addTextExtension("ini");
```

### Document Browser

```cpp
// Document-focused browsing
FileBrowser* browser = new FileBrowser("/docs");
browser->clearTextExtensions();
browser->addTextExtension("txt");
browser->addTextExtension("md");
```

## Integration Patterns

FileBrowser integrates easily with other M1Shield components:

- **With MenuScreen**: Offer multiple browser configurations
- **With TextFileViewer**: Automatic text file viewing
- **With BinaryFileViewer**: Automatic binary file hex dump viewing
- **With Applications**: File selection and management features

## Next Steps

After mastering these examples:

1. **Create Custom Applications**: Integrate FileBrowser into your projects
2. **Explore File Viewers**: Learn TextFileViewer and BinaryFileViewer
3. **Build File Managers**: Create complete file management applications
4. **Study Integration**: Learn how to embed FileBrowser in larger systems
