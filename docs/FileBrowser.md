# FileBrowser Class

The FileBrowser class provides comprehensive SD card file and directory browsing capabilities through a MenuScreen-based interface. It automatically detects file types and opens appropriate viewers, supports directory navigation with ".." entries, and offers advanced features like directory restrictions and file pre-selection.

**Memory Efficient Design**: Uses dynamic arrays that grow as needed, providing optimal memory usage without the overhead of std::vector.

## Memory Management

- **Dynamic File Array**: Automatically sized based on directory contents
- **Dynamic Text Extensions**: Grows as extensions are added
- **No Fixed Limits**: Arrays expand as needed (limited only by available memory)
- **Automatic Cleanup**: Memory is properly freed when the FileBrowser is destroyed

## Table of Contents

- [Constructor](#constructor)
- [Directory Navigation](#directory-navigation)
- [File Type Management](#file-type-management)
- [Configuration Methods](#configuration-methods)
- [Protected Methods](#protected-methods)
- [Usage Examples](#usage-examples)
- [Integration Patterns](#integration-patterns)
- [Advanced Features](#advanced-features)

## Constructor

### Unified Constructor

```cpp
FileBrowser(const String& directoryOrPath = "/", const String& targetFile = "", bool restrictToRoot = false)
```

Creates a FileBrowser with intelligent parameter handling that supports all usage patterns.

**Parameters:**

- `directoryOrPath` - Directory to browse, or full file path (automatically parsed). Default: "/" (root)
- `targetFile` - Optional filename to pre-select (ignored if first parameter contains a file path)
- `restrictToRoot` - If true, prevents navigation above the starting directory. Default: false

**Smart Parameter Logic:**

- If `targetFile` is empty and `directoryOrPath` contains a file extension, the path is automatically parsed
- The directory component becomes the starting directory
- The filename component becomes the target file to pre-select

**Usage Examples:**

```cpp
// Browse from root directory
FileBrowser* browser1 = new FileBrowser();

// Browse specific directory
FileBrowser* browser2 = new FileBrowser("/logs");

// Pre-select file in specific directory
FileBrowser* browser3 = new FileBrowser("/logs", "error.log");

// Automatic file path parsing
FileBrowser* browser4 = new FileBrowser("/logs/system.log");

// Restricted browsing with file path
FileBrowser* browser5 = new FileBrowser("/data/config.txt", "", true);

// All parameters specified explicitly
FileBrowser* browser6 = new FileBrowser("/restricted", "target.txt", true);
```

**Equivalent Old API Patterns:**

```cpp
// Old: FileBrowser()
// New: FileBrowser() or FileBrowser("/")

// Old: FileBrowser("file.txt")
// New: FileBrowser("/", "file.txt") or FileBrowser("file.txt")

// Old: FileBrowser("/dir", "file.txt")
// New: FileBrowser("/dir", "file.txt")

// Old: FileBrowser("/dir", "file.txt", true)
// New: FileBrowser("/dir", "file.txt", true)
```

## Directory Navigation

### Public Navigation Methods

#### `bool navigateToDirectory(const String& directory)`

Navigate to a specific directory programmatically.

**Returns:** `true` if navigation successful, `false` otherwise

#### `String getCurrentDirectory() const`

Get the current directory path.

**Returns:** Current directory path as String

#### `void refresh()`

Refresh the current directory contents and update the display.

## File Type Management

### Extension Configuration

#### `void addTextExtension(const String& extension)`

Add a file extension that should open with TextFileViewer.

**Parameters:**

- `extension` - File extension (without dot, e.g., "cfg")

#### `void clearTextExtensions()`

Clear all text file extensions (will revert to empty list).

#### ````cpp

void setTextExtensions(const String\* extensions, uint8_t count)

````
Sets the complete list of text file extensions from an array.

**Parameters:**
- `extensions` - Array of extension strings
- `count` - Number of extensions in the array (no limit, memory permitting)

**Example:**
```cpp
String textExts[] = {"txt", "log", "cfg", "ini", "md", "json"};
browser->setTextExtensions(textExts, 6);
````

Set the complete list of text file extensions.

**Parameters:**

- `extensions` - Vector of file extensions

### Default Text Extensions

By default, these extensions open with TextFileViewer:

- `txt` - Text files
- `log` - Log files

All other file types open with BinaryFileViewer for hex dump viewing.

## Configuration Methods

### Directory Display

The FileBrowser automatically shows:

- **Directories**: Displayed with "DIR" indicator
- **Parent Directory**: ".." entry with "UP" indicator (when available)
- **Files**: Displayed with file size (B, K, M format)

### File Filtering

Currently shows all files and directories. Future versions may include filtering options.

## Protected Methods

### Core Virtual Method Overrides

#### `bool open() override`

Initializes the SD card, loads directory contents, and sets up the menu.

#### `Screen* _getSelectedMenuItemScreen(int index) override`

Handles file and directory selection:

- **Directories**: Navigates into the directory
- **Text Files**: Opens with TextFileViewer
- **Binary Files**: Opens with BinaryFileViewer

#### `const char* _getMenuItemConfigValue(uint8_t index) override`

Provides configuration values displayed on the right side of menu items:

- **Directories**: Shows "DIR"
- **Parent Directory**: Shows "UP"
- **Files**: Shows file size in human-readable format

## Usage Examples

### Basic File Browsing

```cpp
// Simple root directory browsing
M1Shield.setScreen(new FileBrowser());
```

### Directory-Specific Browsing

```cpp
// Start in a specific directory
M1Shield.setScreen(new FileBrowser("/logs"));
```

### File Pre-selection

```cpp
// Navigate to directory and highlight specific file
M1Shield.setScreen(new FileBrowser("/config", "settings.txt"));
```

### Restricted Browsing

```cpp
// Restrict browsing to logs directory only
FileBrowser* browser = new FileBrowser("/logs", "", true);
M1Shield.setScreen(browser);
```

### Custom File Types

```cpp
// Add custom text file extensions
FileBrowser* browser = new FileBrowser("/");
browser->addTextExtension("cfg");
browser->addTextExtension("ini");
browser->addTextExtension("conf");
M1Shield.setScreen(browser);
```

## Integration Patterns

### With MenuScreen Applications

```cpp
class MainMenu : public MenuScreen {
protected:
    Screen* _getSelectedMenuItemScreen(int index) override {
        switch(index) {
            case 0: return new FileBrowser("/data");
            case 1: return new FileBrowser("/logs", "latest.log");
            case 2: return new FileBrowser("/config", "", true);
        }
        return nullptr;
    }
};
```

### Application Data Management

```cpp
// Application-specific file browser
class AppFileBrowser : public FileBrowser {
public:
    AppFileBrowser() : FileBrowser("/appdata", "", true) {
        // Only show application files
        clearTextExtensions();
        addTextExtension("cfg");
        addTextExtension("dat");
        addTextExtension("log");
    }
};
```

## Advanced Features

### Directory Restrictions

```cpp
// Prevent navigation above specified directory
FileBrowser* browser = new FileBrowser("/restricted", "", true);
```

This is useful for:

- **Security**: Prevent access to system directories
- **Organization**: Keep users in relevant folders
- **Applications**: Limit scope to application data

### File Pre-selection

```cpp
// Automatically scroll to and highlight file
FileBrowser* browser = new FileBrowser("/logs", "error.log");
```

Benefits:

- **User Experience**: Quick access to important files
- **Workflow**: Direct navigation to recently used files
- **Applications**: Context-aware file access

### Custom File Type Handling

```cpp
FileBrowser* browser = new FileBrowser("/");
browser->clearTextExtensions();
browser->addTextExtension("json");
browser->addTextExtension("xml");
browser->addTextExtension("csv");
```

Use cases:

- **Data Files**: Treat structured data as text
- **Configuration**: Application-specific config formats
- **Development**: Source code and markup files

## Navigation Controls

- **UP/DOWN**: Navigate through files and directories
- **RIGHT/MENU**: Select file or enter directory
- **LEFT**: Return to parent directory (when available)

## File Size Display

Files show size in human-readable format:

- **Bytes**: 0-1023B (e.g., "512B")
- **Kilobytes**: 1-1023K (e.g., "64K")
- **Megabytes**: 1M+ (e.g., "5M")

## Error Handling

The FileBrowser handles common error conditions:

- **SD Card Not Present**: Shows error notification
- **Directory Not Found**: Shows error notification
- **Access Denied**: Shows restriction notification
- **Empty Directory**: Shows "<Empty Directory>" message

## Memory Considerations

The FileBrowser uses std::vector for file list management:

- **Dynamic Allocation**: Memory usage scales with directory size
- **Sorting**: Files are sorted (directories first, then alphabetical)
- **Cleanup**: Automatic memory management through destructors

## Notes

- **MenuScreen Inheritance**: FileBrowser inherits all MenuScreen and ContentScreen features
- **SD Card Required**: Requires SD card initialization and presence
- **File System Support**: Works with standard Arduino SD library file systems
- **Path Handling**: Automatic path normalization and validation
- **Case Sensitivity**: File extension matching is case-insensitive
