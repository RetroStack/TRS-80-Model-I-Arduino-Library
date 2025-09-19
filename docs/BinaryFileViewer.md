# BinaryFileViewer

BinaryFileViewer is a ConsoleScreen extension that provides hex dump viewing of binary files from SD card. It displays files in the traditional hex editor format with address, hexadecimal bytes, and ASCII representation columns, similar to the ROM content viewer design.

## Key Features

- **Hex Dump Format**: Address, hex bytes, and ASCII columns
- **Memory Efficient**: Loads only the current page, not the entire file
- **Dynamic Layout**: Adapts to display size for optimal viewing
- **Color-coded Display**: Yellow addresses, cyan hex bytes, white ASCII
- **Large File Support**: Handles files of any size within SD card limits
- **Page Navigation**: Navigate through files with next/previous page controls

## Constructor

```cpp
BinaryFileViewer(const char* filename)
```

Creates a new BinaryFileViewer for the specified binary file. The file must exist on the SD card.

**Default Configuration:**

- Sets title to "Binary File Viewer"
- Black console background with white text
- Button items: ["Menu", "Up:Prev", "Dn:Next"]
- Automatic display layout calculation

## Display Format

The hex dump format follows standard conventions:

```
00000000: 48 65 6C 6C 6F 20 57 6F 72 6C 64 21 0A -- -- --  Hello World!....
00000010: 54 68 69 73 20 69 73 20 61 20 62 69 6E 61 72 79  This is a binary
00000020: 20 66 69 6C 65 20 65 78 61 6D 70 6C 65 2E -- --   file example...
```

- **Address Column (Yellow)**: 8-digit hexadecimal file offset
- **Hex Bytes (Cyan)**: Hexadecimal representation of file bytes
- **ASCII Column (White)**: Printable ASCII characters (0x20-0x7F), others as "."

## Navigation Methods

### Page Navigation

```cpp
bool nextPage()
bool previousPage()
bool goToPage(uint32_t page)
bool goToLastPage()
bool goToOffset(uint32_t offset)
```

Navigate through the file by pages or jump to specific locations. All methods return `true` if successful.

## Information Methods

```cpp
uint32_t getCurrentOffset() const    // Current file position
uint32_t getFileSize() const         // Total file size in bytes
bool isFileOpen() const              // File open status
uint32_t getCurrentPage() const      // Current page (1-based)
uint32_t getTotalPages() const       // Total pages in file
```

Get current status and file information.

## Input Handling

BinaryFileViewer responds to ConsoleScreen inputs:

- **UP**: Previous page
- **DOWN**: Next page
- **LEFT**: Go to first page (offset 0)
- **RIGHT**: Go to last page
- **MENU**: Exit viewer
- **SELECT**: Reserved for future features

## Automatic Layout Calculation

The viewer dynamically calculates the optimal display layout based on screen size:

### Lines Per Page

- Uses available content height minus header space
- Minimum of 5 lines guaranteed
- Adapts to different display sizes

### Bytes Per Line

- Calculates based on available content width
- Format: `XXXXXXXX: XX XX XX ... ASCII`
- Prefers multiples of 8 for alignment
- Range: 8-32 bytes per line

## Memory Management

Designed for Arduino efficiency:

- **Page Buffering**: Only current page data stored in memory
- **Dynamic Allocation**: Buffer size calculated based on display
- **Automatic Cleanup**: Resources freed on destruction
- **Large File Support**: No file size limitations

## Error Handling

- Graceful handling of missing files
- File access error reporting
- Safe operation with corrupted files
- Memory allocation failure protection

## Example Usage

```cpp
// Create viewer for a binary file
BinaryFileViewer* binViewer = new BinaryFileViewer("firmware.bin");

// The viewer automatically configures itself
M1Shield.setScreen(binViewer);

// Navigation happens through standard input handling
// UP/DOWN for pages, LEFT/RIGHT for first/last page
```

## Implementation Notes

- Extends ConsoleScreen for text-based display
- Uses SD library directly for SD card access
- Follows ROM content viewer design patterns
- Color scheme optimized for readability
- Efficient file streaming for large files

## Use Cases

- **Firmware Analysis**: Examine binary firmware files
- **Data File Inspection**: View binary data files in hex format
- **ROM Dumps**: Analyze ROM dump files
- **Debugging**: Inspect binary output from programs
- **File Verification**: Check file contents and structure

## Related Classes

- **ConsoleScreen**: Base class providing text console functionality
- **TextFileViewer**: Sister class for viewing text files
- **SD Library**: Direct SD card access for file operations
- **Screen**: Root UI class for input/output management
