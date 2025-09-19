/*
 * FileBrowser.cpp - MenuScreen for browsing SD card directories and files
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#include "FileBrowser.h"
#include "M1Shield.h"

// Constructor - handles all usage patterns intelligently
FileBrowser::FileBrowser(const String &directoryOrPath, const String &targetFile, bool restrictToRoot) : MenuScreen()
{
    String directory = directoryOrPath;
    String filename = targetFile;

    // Initialize dynamic arrays
    _files = nullptr;
    _fileCount = 0;
    _fileCapacity = 0;
    _textExtensions = nullptr;
    _textExtensionCount = 0;
    _textExtensionCapacity = 0;

    // Smart path parsing logic
    if (filename.length() == 0 && _isFilePath(directoryOrPath))
    {
        // First parameter contains a file path - parse it
        _parseFilePath(directoryOrPath, directory, filename);
    }

    // Set up directory paths
    _currentDirectory = _normalizePath(directory);
    _targetFilename = filename;

    // Handle root restriction
    if (restrictToRoot)
    {
        _rootDirectory = _currentDirectory;
        _hasRootRestriction = true;
    }
    else
    {
        _rootDirectory = "/";
        _hasRootRestriction = false;
    }

    // Set default text file extensions
    _ensureTextExtensionCapacity(2);
    _textExtensions[0] = "log";
    _textExtensions[1] = "txt";
    _textExtensionCount = 2;

    // Set title and button items
    setTitleF(F("File Browser"));
    const char *buttons[] = {"[M/<] Back", "[>] Select"};
    setButtonItems(buttons, 2);
}

// Destructor
FileBrowser::~FileBrowser()
{
    _cleanupArrays();
}

// Clean up allocated arrays
void FileBrowser::_cleanupArrays()
{
    if (_files)
    {
        delete[] _files;
        _files = nullptr;
    }
    _fileCount = 0;
    _fileCapacity = 0;

    if (_textExtensions)
    {
        delete[] _textExtensions;
        _textExtensions = nullptr;
    }
    _textExtensionCount = 0;
    _textExtensionCapacity = 0;
}

// Ensure _files array has minimum capacity
void FileBrowser::_ensureFileCapacity(uint8_t minCapacity)
{
    if (_fileCapacity >= minCapacity)
        return;

    // Calculate new capacity (grow by 50% or minimum needed)
    uint8_t newCapacity = _fileCapacity + (_fileCapacity >> 1);
    if (newCapacity < minCapacity)
        newCapacity = minCapacity;

    FileEntry *newFiles = new FileEntry[newCapacity];

    // Copy existing data
    for (uint8_t i = 0; i < _fileCount; i++)
    {
        newFiles[i] = _files[i];
    }

    // Clean up old array
    if (_files)
    {
        delete[] _files;
    }

    _files = newFiles;
    _fileCapacity = newCapacity;
}

// Ensure _textExtensions array has minimum capacity
void FileBrowser::_ensureTextExtensionCapacity(uint8_t minCapacity)
{
    if (_textExtensionCapacity >= minCapacity)
        return;

    // Calculate new capacity (grow by 50% or minimum needed)
    uint8_t newCapacity = _textExtensionCapacity + (_textExtensionCapacity >> 1);
    if (newCapacity < minCapacity)
        newCapacity = minCapacity;

    String *newExtensions = new String[newCapacity];

    // Copy existing data
    for (uint8_t i = 0; i < _textExtensionCount; i++)
    {
        newExtensions[i] = _textExtensions[i];
    }

    // Clean up old array
    if (_textExtensions)
    {
        delete[] _textExtensions;
    }

    _textExtensions = newExtensions;
    _textExtensionCapacity = newCapacity;
}

// Initialize and load directory
bool FileBrowser::open()
{
    // Call parent implementation first
    if (!MenuScreen::open())
    {
        return false;
    }

    // Initialize SD card if not already done
    if (!SD.begin(M1Shield.getSDCardSelectPin()))
    {
        notifyF(F("Error: Failed to initialize SD card"));
        return false;
    }

    // Load directory contents
    if (!_loadDirectoryContents())
    {
        notifyF(F("Error: Could not read directory"));
        return false;
    }

    // Update menu items from loaded files
    _updateMenuItems();

    // Pre-select target file if specified
    if (_targetFilename.length() > 0)
    {
        _findAndSelectFile(_targetFilename);
    }

    return true;
}

// Load files and directories into _files array
bool FileBrowser::_loadDirectoryContents()
{
    // Clear existing files
    _fileCount = 0;

    File dir = SD.open(_currentDirectory.c_str());
    if (!dir)
    {
        return false;
    }

    if (!dir.isDirectory())
    {
        dir.close();
        return false;
    }

    // First pass: count entries to determine required capacity
    uint8_t entryCount = 0;

    // Count ".." entry if applicable
    if (_currentDirectory != "/" && (!_hasRootRestriction || _currentDirectory != _rootDirectory))
    {
        entryCount++;
    }

    // Count directory entries
    File entry = dir.openNextFile();
    while (entry)
    {
        String entryName = entry.name();

        // Skip hidden files and current directory
        if (entryName.startsWith(".") && entryName != "..")
        {
            entry.close();
            entry = dir.openNextFile();
            continue;
        }

        // Only count files that pass our filter
        bool isDirectory = entry.isDirectory();
        if (isDirectory || _isValidFile(entryName))
        {
            entryCount++;
        }

        entry.close();
        entry = dir.openNextFile();
    }

    // Ensure we have enough capacity
    _ensureFileCapacity(entryCount);

    // Reset directory position for second pass
    dir.close();
    dir = SD.open(_currentDirectory.c_str());

    // Second pass: add entries to array
    // Add ".." entry if not at root or if root restriction allows it
    if (_currentDirectory != "/" && (!_hasRootRestriction || _currentDirectory != _rootDirectory))
    {
        _files[_fileCount].name = "..";
        _files[_fileCount].isDirectory = true;
        _files[_fileCount].size = 0;
        _fileCount++;
    }

    // Read directory entries
    entry = dir.openNextFile();
    while (entry)
    {
        String entryName = entry.name();

        // Skip hidden files and current directory
        if (entryName.startsWith(".") && entryName != "..")
        {
            entry.close();
            entry = dir.openNextFile();
            continue;
        }

        // Only add files that pass our filter
        bool isDirectory = entry.isDirectory();
        if (isDirectory || _isValidFile(entryName))
        {
            _files[_fileCount].name = entryName;
            _files[_fileCount].isDirectory = isDirectory;
            _files[_fileCount].size = isDirectory ? 0 : entry.size();
            _fileCount++;
        }

        entry.close();
        entry = dir.openNextFile();
    }

    dir.close();

    // Sort entries: directories first, then files, both alphabetically (simple bubble sort)
    for (uint8_t i = 0; i < _fileCount - 1; i++)
    {
        for (uint8_t j = 0; j < _fileCount - i - 1; j++)
        {
            bool swap = false;

            // ".." always comes first
            if (_files[j + 1].name == "..")
            {
                swap = true;
            }
            else if (_files[j].name == "..")
            {
                swap = false;
            }
            else if (_files[j].isDirectory != _files[j + 1].isDirectory)
            {
                // Directories before files
                swap = _files[j + 1].isDirectory;
            }
            else
            {
                // Alphabetical within same type
                swap = _files[j].name.compareToIgnoreCase(_files[j + 1].name) > 0;
            }

            if (swap)
            {
                FileEntry temp = _files[j];
                _files[j] = _files[j + 1];
                _files[j + 1] = temp;
            }
        }
    }

    return true;
}

// Navigate to specified directory
bool FileBrowser::_navigateToDirectory(const String &dir)
{
    String newPath;

    if (dir == "..")
    {
        newPath = _getParentDirectory(_currentDirectory);
    }
    else if (dir.startsWith("/"))
    {
        newPath = _normalizePath(dir);
    }
    else
    {
        // Relative path
        newPath = _currentDirectory;
        if (!newPath.endsWith("/"))
            newPath += "/";
        newPath += dir;
        newPath = _normalizePath(newPath);
    }

    // Check root restriction
    if (_hasRootRestriction && !newPath.startsWith(_rootDirectory))
    {
        notifyF(F("Access restricted to root directory"));
        return false;
    }

    // Verify directory exists
    File testDir = SD.open(newPath.c_str());
    if (!testDir || !testDir.isDirectory())
    {
        if (testDir)
            testDir.close();
        notifyF(F("Directory not found"));
        return false;
    }
    testDir.close();

    _currentDirectory = newPath;
    return true;
}

// Navigate to parent directory
bool FileBrowser::_navigateUp()
{
    return _navigateToDirectory("..");
}

// Normalize directory path
String FileBrowser::_normalizePath(const String &path)
{
    String normalized = path;

    // Ensure starts with /
    if (!normalized.startsWith("/"))
    {
        normalized = "/" + normalized;
    }

    // Remove trailing slash unless it's the root
    if (normalized.length() > 1 && normalized.endsWith("/"))
    {
        normalized = normalized.substring(0, normalized.length() - 1);
    }

    return normalized;
}

// Get parent directory path
String FileBrowser::_getParentDirectory(const String &path)
{
    if (path == "/")
        return "/";

    int lastSlash = path.lastIndexOf("/");
    if (lastSlash <= 0)
        return "/";

    return path.substring(0, lastSlash);
}

// Check if file should be shown
bool FileBrowser::_isValidFile(const String &filename)
{
    // For now, show all files
    // Could be extended to filter by extension or size
    return true;
}

// Check if file should open with TextFileViewer
bool FileBrowser::_isTextFile(const String &filename)
{
    int dotIndex = filename.lastIndexOf(".");
    if (dotIndex == -1)
        return false;

    String extension = filename.substring(dotIndex + 1);
    extension.toLowerCase();

    for (uint8_t i = 0; i < _textExtensionCount; i++)
    {
        if (extension == _textExtensions[i])
            return true;
    }

    return false;
}

// Find file in current directory and select it
void FileBrowser::_findAndSelectFile(const String &filename)
{
    for (uint8_t i = 0; i < _fileCount; i++)
    {
        if (_files[i].name.equalsIgnoreCase(filename))
        {
            _setSelectedMenuItemIndex(i);
            return;
        }
    }
}

// Update menu items from _files array
void FileBrowser::_updateMenuItems()
{
    if (_fileCount == 0)
    {
        // Show empty directory message
        const char *emptyItems[] = {"<Empty Directory>"};
        setMenuItems(emptyItems, 1);
        return;
    }

    // Create menu item array
    const char **menuItems = new const char *[_fileCount];
    for (uint8_t i = 0; i < _fileCount; i++)
    {
        menuItems[i] = _files[i].name.c_str();
    }

    setMenuItems(menuItems, _fileCount);
    delete[] menuItems;
}

// Convert file size to readable string
String FileBrowser::_getFileSizeString(uint32_t size)
{
    if (size < 1024)
    {
        return String(size) + "B";
    }
    else if (size < 1024 * 1024)
    {
        return String(size / 1024) + "K";
    }
    else
    {
        return String(size / (1024 * 1024)) + "M";
    }
}

// Add file extension to open with TextFileViewer
void FileBrowser::addTextExtension(const String &extension)
{
    String ext = extension;
    ext.toLowerCase();

    // Check if already exists
    for (uint8_t i = 0; i < _textExtensionCount; i++)
    {
        if (_textExtensions[i] == ext)
            return;
    }

    // Ensure we have capacity for one more extension
    _ensureTextExtensionCapacity(_textExtensionCount + 1);

    _textExtensions[_textExtensionCount] = ext;
    _textExtensionCount++;
}

// Clear all text extensions
void FileBrowser::clearTextExtensions()
{
    _textExtensionCount = 0;
}

// Set text extensions array
void FileBrowser::setTextExtensions(const String *extensions, uint8_t count)
{
    // Ensure we have enough capacity
    _ensureTextExtensionCapacity(count);

    _textExtensionCount = count;

    for (uint8_t i = 0; i < _textExtensionCount; i++)
    {
        _textExtensions[i] = extensions[i];
        _textExtensions[i].toLowerCase();
    }
}

// Public method to navigate to directory
bool FileBrowser::navigateToDirectory(const String &directory)
{
    if (_navigateToDirectory(directory))
    {
        if (_loadDirectoryContents())
        {
            _updateMenuItems();
            if (isActive())
            {
                refreshMenu();
            }
            return true;
        }
    }
    return false;
}

// Get current directory path
String FileBrowser::getCurrentDirectory() const
{
    return _currentDirectory;
}

// Refresh current directory contents
void FileBrowser::refresh()
{
    if (_loadDirectoryContents())
    {
        _updateMenuItems();
        if (isActive())
        {
            refreshMenu();
        }
    }
}

// Handle file/directory selection
Screen *FileBrowser::_getSelectedMenuItemScreen(int index)
{
    if (index < 0 || index >= (int)_fileCount)
    {
        return nullptr;
    }

    if (_fileCount == 0)
    {
        notifyF(F("Directory is empty"));
        return nullptr;
    }

    const FileEntry &selected = _files[index];

    if (selected.isDirectory)
    {
        // Navigate to directory
        if (_navigateToDirectory(selected.name))
        {
            if (_loadDirectoryContents())
            {
                _updateMenuItems();
                refreshMenu();
                notifyF(F("Directory changed"));
            }
            else
            {
                notifyF(F("Error reading directory"));
            }
        }
        return nullptr; // Stay on this screen
    }
    else
    {
        // Open file with appropriate viewer
        String fullPath = _currentDirectory;
        if (!fullPath.endsWith("/"))
            fullPath += "/";
        fullPath += selected.name;

        if (_isTextFile(selected.name))
        {
            // Open with TextFileViewer
            return new TextFileViewer(fullPath.c_str());
        }
        else
        {
            // Open with BinaryFileViewer
            return new BinaryFileViewer(fullPath.c_str());
        }
    }
}

// Show file sizes
const char *FileBrowser::_getMenuItemConfigValue(uint8_t index)
{
    if (index >= _fileCount)
        return nullptr;

    const FileEntry &entry = _files[index];

    if (entry.isDirectory)
    {
        if (entry.name == "..")
            return "UP";
        else
            return "DIR";
    }
    else
    {
        // Return file size as a static string
        static String sizeStr;
        sizeStr = _getFileSizeString(entry.size);
        return sizeStr.c_str();
    }
}

// Check if a path contains a filename (has an extension)
bool FileBrowser::_isFilePath(const String &path)
{
    int lastSlash = path.lastIndexOf('/');
    int lastDot = path.lastIndexOf('.');

    // If there's a dot after the last slash (or no slash), it's likely a filename
    return (lastDot > lastSlash);
}

// Parse a file path into directory and filename components
void FileBrowser::_parseFilePath(const String &filePath, String &directory, String &filename)
{
    int lastSlash = filePath.lastIndexOf('/');

    if (lastSlash == -1)
    {
        // No slash found - treat as filename in root
        directory = "/";
        filename = filePath;
    }
    else
    {
        // Split at the last slash
        directory = filePath.substring(0, lastSlash);
        if (directory.length() == 0)
        {
            directory = "/"; // Root directory
        }
        filename = filePath.substring(lastSlash + 1);
    }
}
