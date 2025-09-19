# Binary File Viewer Examples

The BinaryFileViewer class provides hex dump viewing capabilities for binary files stored on SD card. This directory contains examples demonstrating different aspects of binary file viewing.

## Examples

### SimpleBinaryFileViewer

Basic usage demonstration showing:

- Loading and viewing a binary file in hex dump format
- Standard navigation controls (UP/DOWN for pages, LEFT/RIGHT for first/last)
- Automatic layout calculation for optimal display
- Color-coded hex dump format (address, hex bytes, ASCII)

### ComprehensiveBinaryFileViewer (Future)

Advanced features including:

- Menu-driven file selection
- Multiple file format support
- Search and goto functionality
- Custom display options

## Binary File Viewer Features

### Display Format

- **Address Column**: 8-digit hex file offset (yellow)
- **Hex Bytes**: Hexadecimal byte values (cyan)
- **ASCII Column**: Printable characters and dots (white)

### Navigation

- Page-based viewing for memory efficiency
- Jump to specific offsets or pages
- First/last page quick navigation

### Memory Efficiency

- Only current page loaded into memory
- Dynamic page size calculation
- Support for files of any size

### File Support

- Any binary file format
- ROM dumps and firmware files
- Data files and executables
- Automatic error handling for missing files

## Getting Started

1. Place binary files on your SD card
2. Choose the appropriate example
3. Upload to Arduino Mega 2560
4. Use directional controls to navigate

The BinaryFileViewer is perfect for:

- Analyzing firmware and ROM files
- Debugging binary data
- Learning about file formats
- Examining binary file structure
