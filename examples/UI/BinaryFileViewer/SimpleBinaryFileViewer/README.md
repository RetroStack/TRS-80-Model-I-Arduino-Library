# Simple Binary File Viewer Example

This example demonstrates basic usage of the BinaryFileViewer class to display binary files from an SD card in hex dump format.

## What It Does

- Displays the contents of "firmware.bin" from the SD card in hex dump format
- Shows address, hexadecimal bytes, and ASCII representation
- Provides page navigation using the directional buttons
- Demonstrates automatic layout calculation for optimal display

## What You'll Learn

- How to create and configure a BinaryFileViewer
- Basic hex dump navigation and viewing
- Understanding the hex dump format (address + hex + ASCII)
- Memory-efficient binary file display techniques

## Hardware Requirements

- Arduino Mega 2560
- M1Shield with compatible display
- SD card with "firmware.bin" file (or any binary file)

## Usage Instructions

1. **Prepare SD Card**: Place a binary file named "firmware.bin" on your SD card
2. **Upload Code**: Upload this example to your Arduino Mega 2560
3. **Navigate**: Use the directional buttons to navigate:
   - UP: Previous page
   - DOWN: Next page
   - LEFT: Jump to first page
   - RIGHT: Jump to last page
   - MENU: Exit viewer

## File Suggestions

You can use any binary file for testing:

- **Firmware Files**: `.bin`, `.hex` files
- **ROM Dumps**: Game ROM files, system ROM dumps
- **Data Files**: Any binary data file
- **Executables**: `.exe`, compiled programs (will show as binary data)
- **Image Files**: Small image files for testing

For testing, you can create a simple binary file with known content or use any existing binary file.

## Display Format

The viewer shows data in standard hex dump format:

```
00000000: 48 65 6C 6C 6F 20 57 6F 72 6C 64 21 0A -- -- --  Hello World!....
00000010: 54 68 69 73 20 69 73 20 61 20 62 69 6E 61 72 79  This is a binary
```

- **Yellow Column**: File offset in hexadecimal
- **Cyan Columns**: Byte values in hexadecimal
- **White Column**: ASCII representation (printable characters only)

## Code Explanation

The example creates a BinaryFileViewer for "firmware.bin" with:

- Automatic hex dump display formatting
- Default button items set by constructor
- Dynamic layout calculation based on display size
- Color-coded display for easy reading

The viewer automatically handles:

- File opening and error checking
- Page calculation based on display size
- Memory-efficient buffering
- Input processing for navigation

## Automatic Features

### Layout Calculation

- Determines optimal bytes per line based on screen width
- Calculates lines per page based on screen height
- Ensures minimum readability standards

### Memory Management

- Only loads current page data into memory
- Automatically frees resources when done
- Handles large files efficiently

### Error Handling

- Displays error message if file cannot be opened
- Safe handling of file access issues
- Graceful degradation for missing files

## Next Steps

- Try different binary files to see various data patterns
- Experiment with the ComprehensiveBinaryFileViewer example for advanced features
- Use the viewer to analyze ROM dumps or firmware files
- Compare with TextFileViewer for text vs binary file viewing
