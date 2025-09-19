/*
 * FileBrowser.h - MenuScreen for b    String _getParentDirectory(const String& path); // Get parent directory path
    void _parseFilePath(const String& filePath, String& directory, String& filename); // Parse file path into directory and filename components
    bool _isFilePath(const String& path);           // Check if path contains a filename (has extension)
    bool _isValidFile(const String& filename);     // Check if file should be shownsing SD card directories and files
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef FILEBROWSER_H
#define FILEBROWSER_H

#include <Arduino.h>
#include <SD.h>
#include "MenuScreen.h"
#include "M1Shield.h"
#include "TextFileViewer.h"
#include "BinaryFileViewer.h"

struct FileEntry
{
    String name;
    bool isDirectory;
    uint32_t size;
};

class FileBrowser : public MenuScreen
{
private:
    String _currentDirectory;       // Current directory path
    String _rootDirectory;          // Root directory (can't go above this)
    String _targetFilename;         // File to pre-select and scroll to
    FileEntry *_files;              // Dynamic array of current directory contents
    uint8_t _fileCount;             // Number of files in _files array
    uint8_t _fileCapacity;          // Current capacity of _files array
    String *_textExtensions;        // Dynamic array of extensions to open with TextFileViewer
    uint8_t _textExtensionCount;    // Number of text extensions
    uint8_t _textExtensionCapacity; // Current capacity of _textExtensions array
    bool _hasRootRestriction;       // Whether root directory restriction is active

    // Dynamic array management
    void _ensureFileCapacity(uint8_t minCapacity);          // Ensure _files array has minimum capacity
    void _ensureTextExtensionCapacity(uint8_t minCapacity); // Ensure _textExtensions array has minimum capacity
    void _cleanupArrays();                                  // Free allocated arrays

    // Directory operations
    bool _loadDirectoryContents();                                                    // Load files and directories into _files array
    bool _navigateToDirectory(const String &dir);                                     // Navigate to specified directory
    bool _navigateUp();                                                               // Navigate to parent directory
    String _normalizePath(const String &path);                                        // Normalize directory path
    String _getParentDirectory(const String &path);                                   // Get parent directory path
    void _parseFilePath(const String &filePath, String &directory, String &filename); // Parse file path into directory and filename components
    bool _isValidFile(const String &filename);                                        // Check if file should be shown
    bool _isTextFile(const String &filename);                                         // Check if file should open with TextFileViewer
    void _findAndSelectFile(const String &filename);                                  // Find file in current directory and select it

    // Menu generation
    void _updateMenuItems();                  // Update menu items from _files vector
    String _getFileSizeString(uint32_t size); // Convert file size to readable string

public:
    // Constructor - supports all usage patterns with default parameters
    FileBrowser(const String &directoryOrPath = "/", const String &targetFile = "", bool restrictToRoot = false);

    ~FileBrowser(); // Destructor

    // Configuration
    void addTextExtension(const String &extension);                  // Add file extension to open with TextFileViewer
    void clearTextExtensions();                                      // Clear all text extensions (will use defaults)
    void setTextExtensions(const String *extensions, uint8_t count); // Set text extensions array

    // Directory navigation
    bool navigateToDirectory(const String &directory); // Public method to navigate to directory
    String getCurrentDirectory() const;                // Get current directory path
    void refresh();                                    // Refresh current directory contents

protected:
    bool open() override;                                        // Initialize and load directory
    Screen *_getSelectedMenuItemScreen(int index) override;      // Handle file/directory selection
    const char *_getMenuItemConfigValue(uint8_t index) override; // Show file sizes
};

#endif /* FILEBROWSER_H */
