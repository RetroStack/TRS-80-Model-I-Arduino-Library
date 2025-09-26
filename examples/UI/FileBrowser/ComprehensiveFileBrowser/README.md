# Comprehensive FileBrowser Example

## What This Example Does

This comprehensive FileBrowser example demonstrates:

1. **Multiple Browser Configurations**: Different setups for various use cases
2. **Directory Restrictions**: Limiting browsing to specific directories
3. **File Pre-selection**: Automatically scrolling to and highlighting specific files
4. **Custom File Types**: Configuring which extensions open with TextFileViewer
5. **Menu Integration**: Using MenuScreen to offer multiple browser options

## What You'll Learn

- How to implement directory restrictions for security/organization
- Understanding file pre-selection and auto-scrolling functionality
- Configuring custom file type extensions for smart viewer selection
- Integrating FileBrowser with MenuScreen for complex applications
- Advanced FileBrowser constructor options and configuration methods

## Example Configurations

### 1. Basic Root Browsing

- **Purpose**: Full SD card access from root directory
- **Features**: No restrictions, all file types, complete navigation
- **Use Case**: General file management and exploration

### 2. Restricted Log Browsing

- **Purpose**: Browse only the /logs directory (cannot go above it)
- **Features**: Root restriction prevents navigation outside /logs
- **Use Case**: Log viewing applications where users should only see logs

### 3. Pre-selection Browsing

- **Purpose**: Open /data directory and automatically highlight config.txt
- **Features**: Automatic file finding and selection for quick access
- **Use Case**: Applications that need to quickly access specific configuration files

### 4. Custom File Types

- **Purpose**: Define custom extensions that open with TextFileViewer
- **Features**: .cfg, .ini, .conf files treated as text in addition to defaults
- **Use Case**: Configuration management applications with various text formats

### 5. Document-Only Browsing

- **Purpose**: Restricted to /documents with document-specific file types
- **Features**: Combined directory restriction and file type filtering
- **Use Case**: Document management systems focused on text documents

## Directory Structure Setup

To fully test this example, create this structure on your SD card:

```
/
├── logs/
│   ├── system.log
│   ├── error.log
│   └── debug.log
├── data/
│   ├── config.txt
│   ├── settings.ini
│   └── data.bin
├── documents/
│   ├── readme.txt
│   ├── manual.md
│   └── notes.doc
└── misc/
    ├── image.jpg
    ├── program.exe
    └── archive.zip
```

## Advanced Features Demonstrated

### Directory Restrictions

```cpp
// Restrict browsing to /logs directory only
FileBrowser* browser = new FileBrowser("/logs", "", true);
```

### File Pre-selection

```cpp
// Open /data directory and highlight config.txt
FileBrowser* browser = new FileBrowser("/data", "config.txt");
```

### Custom File Extensions

```cpp
FileBrowser* browser = new FileBrowser("/");
browser->clearTextExtensions();
browser->addTextExtension("cfg");
browser->addTextExtension("ini");
browser->addTextExtension("conf");
```

## Controls

- **UP/DOWN**: Navigate through menu options or files
- **RIGHT/MENU**: Select menu option or file/directory
- **LEFT**: Return to previous menu level or parent directory

## Implementation Patterns

This example shows how to:

1. **Create Multiple Browser Instances**: Different configurations for different needs
2. **Integrate with MenuScreen**: Offering choices to users
3. **Configure Extensions**: Customizing file type handling
4. **Implement Restrictions**: Security and organization through directory limits
5. **Handle Pre-selection**: User experience improvements through auto-navigation

## Hardware Requirements

- Arduino Mega 2560 (required for sufficient memory and GPIO pins)
- M1Shield with compatible display
- SD card with test directory structure

## Next Steps

After exploring this comprehensive example:

1. Create your own directory structures for testing
2. Experiment with different restriction and pre-selection combinations
3. Try integrating FileBrowser into larger applications
4. Explore the API documentation for additional configuration options
