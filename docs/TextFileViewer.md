# TextFileViewer

TextFileViewer is a ContentScreen extension that provides efficient viewing of text files from SD card with paging, horizontal scrolling, and auto-update capabilities. It's optimized for Arduino memory constraints by loading only the current page content.

## Key Features

- **Memory Efficient**: Loads only the current page, not the entire file
- **Paging Support**: Navigate through files with next/previous page controls
- **Horizontal Scrolling**: View wide text lines using left/right scrolling
- **Auto-Paging**: Automatically advance to new content for log files
- **Real-time Updates**: Refreshes file content for growing files
- **Configurable Display**: Customizable text color, background, and size

## Constructor

```cpp
TextFileViewer(const char* filename)
```

Creates a new TextFileViewer for the specified file. The file must exist on the SD card.

**Default Configuration:**

- Sets title to "Text File Viewer"
- Sets button items
- White text on black background
- Text size 1
- Auto-paging disabled

**Button Customization:**

```cpp
// To customize button labels, clear and set new ones:
viewer->clearButtonItems();
const char* customButtons[] = {"Back", "Toggle", "Info"};
viewer->setButtonItems(customButtons, 3);
```

## Configuration Methods

### Display Appearance

```cpp
void setTextColor(uint16_t color)
void setBackgroundColor(uint16_t color)
void setTextSize(uint8_t size)
```

Customize the visual appearance. Text size affects both line height and character width for scrolling calculations.

### Auto-Paging

```cpp
void setAutoPaging(bool enabled)
bool isAutoPagingEnabled() const
```

Enable auto-paging for log files that grow over time. When enabled, the viewer automatically advances to show new content.

## Navigation Methods

### Page Navigation

```cpp
bool nextPage()
bool previousPage()
bool goToPage(uint32_t page)
bool goToLastPage()
```

Navigate between pages. All methods return `true` if successful, `false` if the operation couldn't be performed.

### Horizontal Scrolling

```cpp
void scrollLeft()
void scrollRight()
void resetHorizontalScroll()
```

Scroll horizontally to view long lines. Character width is calculated based on text size.

## Information Methods

```cpp
uint32_t getCurrentPage() const      // 1-based for display
uint32_t getTotalPages() const
bool isFileLoaded() const
bool refreshFile()                   // Refresh file information
```

Get current status and refresh file content for real-time viewing.

## Input Handling

TextFileViewer responds to standard ContentScreen inputs:

- **UP/DOWN**: Previous/Next page
- **LEFT/RIGHT**: Horizontal scrolling
- **SELECT Button**: Toggle auto-paging
- **MENU Button**: Available for custom implementations

## Memory Management

The class is designed for Arduino efficiency:

- Only current page content is stored in memory
- File position tracking minimizes SD card seeks
- Dynamic page calculation based on display size
- Automatic cleanup of resources

## Error Handling

- Returns `false` for invalid operations (e.g., next page when on last page)
- Graceful handling of missing or corrupted files
- Safe operation with zero-length files

## Example Usage

```cpp
// Create viewer for a log file
TextFileViewer* logViewer = new TextFileViewer("debug.log");

// Configure for log monitoring
logViewer->setAutoPaging(true);
logViewer->setTextColor(M1Shield.convertColor(0x07E0)); // Green
logViewer->setTitleF(F("Debug Log"));

// Display the screen
M1Shield.setScreen(logViewer);
```

## Implementation Notes

- Extends ContentScreen for consistent UI behavior
- Uses M1Shield.getSD() for SD card access
- Calculates pages dynamically based on display dimensions
- Optimized for real-time log file monitoring
- Supports files of any size within SD card limits

## Related Classes

- **ContentScreen**: Base class providing layout framework
- **M1Shield**: Hardware abstraction for SD card access
- **Screen**: Root UI class for input/output management
