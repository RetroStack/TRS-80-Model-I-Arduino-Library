/*
 * Video.cpp - Class for accessing video memory and display operations
 * Authors: Ven Reddy, Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#include "Video.h"
#include "Model1.h"

const uint8_t VIDEO_COLS = 64;
const uint8_t VIDEO_ROWS = 16;
const uint16_t VIDEO_MEM_START = 0x3C00;

const uint8_t SPACE_CHARACTER = 0x20;

// Constructor
Video::Video()
{
  _logger = nullptr;

  _cursorPositionX = 0;
  _cursorPositionY = 0;

  _autoScroll = true;
  _hasLowerCaseMod = false;

  _viewPort.x = 0;
  _viewPort.y = 0;
  _viewPort.width = VIDEO_COLS;
  _viewPort.height = VIDEO_ROWS;
}

// Set the logger for debugging output
void Video::setLogger(ILogger &logger)
{
  _logger = &logger;
}

// Set the active video viewport
void Video::setViewPort(ViewPort viewPort)
{
  // Validate and auto-correct viewport
  if (viewPort.x >= VIDEO_COLS)
  {
    viewPort.x = VIDEO_COLS - 1;
    if (_logger)
      _logger->warn(F("X coordinate of viewport is larger than there is space. Reset to %d."), viewPort.x);
  }
  if (viewPort.y >= VIDEO_ROWS)
  {
    viewPort.y = VIDEO_ROWS - 1;
    if (_logger)
      _logger->warn(F("Y coordinate of viewport is larger than there is space. Reset to %d."), viewPort.y);
  }
  if (viewPort.x + viewPort.width > VIDEO_COLS)
  {
    viewPort.width = VIDEO_COLS - viewPort.x;
    if (_logger)
      _logger->warn(F("Width of viewport is larger than there is space. Reset to %d."), viewPort.width);
  }
  if (viewPort.y + viewPort.height > VIDEO_ROWS)
  {
    viewPort.height = VIDEO_ROWS - viewPort.y;
    if (_logger)
      _logger->warn(F("Height of viewport is larger than there is space. Reset to %d."), viewPort.height);
  }

  _viewPort = viewPort;
}

// Get memory address for the start of a video row
uint16_t Video::getRowAddress(uint8_t y)
{
  return VIDEO_MEM_START + ((_viewPort.y + y) * VIDEO_COLS);
}

// Get memory address for a specific column in a row
uint16_t Video::getColumnAddress(uint16_t rowAddress, uint8_t x)
{
  return rowAddress + _viewPort.x + x;
}

// Get memory address for specific x,y coordinates
uint16_t Video::getAddress(uint8_t x, uint8_t y)
{
  uint16_t rowAddress = getRowAddress(y);
  return getColumnAddress(rowAddress, x);
}

// Get current cursor X position
uint8_t Video::getX()
{
  return _cursorPositionX;
}

// Set cursor X position with bounds checking
void Video::setX(uint8_t x)
{
  if (x > _viewPort.width)
  {
    if (_logger)
      _logger->warn(F("Video: X cursor position %d out of bounds (max %d). Reset to %d."), x, _viewPort.width, _viewPort.width - 1);
    _cursorPositionX = _viewPort.width - 1;
  }
  else
  {
    _cursorPositionX = x;
  }
}

// Get current cursor Y position
uint8_t Video::getY()
{
  return _cursorPositionY;
}

// Set cursor Y position with bounds checking
void Video::setY(uint8_t y)
{
  if (y > _viewPort.height)
  {
    if (_logger)
      _logger->warn(F("Video: Y cursor position %d out of bounds (max %d). Reset to %d."), y, _viewPort.height, _viewPort.height - 1);
    _cursorPositionY = _viewPort.height - 1;
  }
  else
  {
    _cursorPositionY = y;
  }
}

// Set cursor X and Y positions
void Video::setXY(uint8_t x, uint8_t y)
{
  setX(x);
  setY(y);
}

// Get viewport start X coordinate
uint8_t Video::getStartX()
{
  return _viewPort.x;
}

// Get viewport end X coordinate
uint8_t Video::getEndX()
{
  return _viewPort.x + _viewPort.width;
}

// Get viewport start Y coordinate
uint8_t Video::getStartY()
{
  return _viewPort.y;
}

// Get viewport end Y coordinate
uint8_t Video::getEndY()
{
  return _viewPort.y + _viewPort.height;
}

// Get viewport width
uint8_t Video::getWidth()
{
  return _viewPort.width;
}

// Get viewport height
uint8_t Video::getHeight()
{
  return _viewPort.height;
}

// Get total size of viewport (width * height)
uint16_t Video::getSize()
{
  return _viewPort.width * _viewPort.height;
}

// Convert relative X coordinate to absolute screen coordinate
uint8_t Video::getAbsoluteX(uint8_t x)
{
  int xCoord = _viewPort.x + x;
  return xCoord > VIDEO_COLS ? VIDEO_COLS : xCoord;
}

// Convert relative Y coordinate to absolute screen coordinate
uint8_t Video::getAbsoluteY(uint8_t y)
{
  int yCoord = _viewPort.y + y;
  return yCoord > VIDEO_ROWS ? VIDEO_ROWS : yCoord;
}

// Clear screen with space characters
void Video::cls()
{
  cls(SPACE_CHARACTER);
}

// Clear screen with specified character
void Video::cls(char character)
{
  cls(&character, 1);
}

// Clear screen with character array
void Video::cls(char *characters)
{
  if (!characters)
  {
    if (_logger)
      _logger->err(F("Video: cls() called with null character array"));
    return;
  }
  uint16_t length = strlen(characters);
  if (length == 0)
  {
    if (_logger)
      _logger->warn(F("Video: cls() called with empty character array"));
    return;
  }
  cls(characters, length);
}

// Clear screen with character array of specified length
void Video::cls(char *characters, uint16_t length)
{
  if (!characters)
  {
    if (_logger)
      _logger->err(F("Video: cls() called with null character array"));
    return;
  }
  if (length == 0)
  {
    if (_logger)
      _logger->warn(F("Video: cls() called with length 0"));
    return;
  }
  int i = 0;
  for (uint16_t y = 0; y < _viewPort.height; y++)
  {
    int rowAddress = getRowAddress(y);
    for (uint16_t x = 0; x < _viewPort.width; x++)
    {
      Model1.writeMemory(getColumnAddress(rowAddress, x), convertLocalCharacterToModel1(characters[i % length]));
      i++;
    }
  }
  _cursorPositionX = 0;
  _cursorPositionY = 0;
}

// Scroll screen up by one row
void Video::scroll()
{
  scroll(1);
}

// Scroll screen up by specified number of rows
void Video::scroll(uint8_t rows)
{
  if (rows == 0)
  {
    if (_logger)
      _logger->warn(F("Video: Scroll called with 0 rows - no action taken"));
    return;
  }

  // If there are more rows than available, just cap it at the maximum number of rows
  if (rows > _viewPort.height)
  {
    if (_logger)
      _logger->info(F("Video: Scroll rows %d exceeds viewport height %d. Capped to %d."), rows, _viewPort.height, _viewPort.height);
    rows = _viewPort.height;
  }

  // Only copy if not the whole memory gets replaced with spaces anyways
  if (rows < _viewPort.height)
  {
    for (uint16_t y = rows; y < _viewPort.height; y++)
    {
      uint16_t src = getColumnAddress(getRowAddress(y), 0);
      uint16_t dst = getColumnAddress(getRowAddress(y - rows), 0);
      Model1.copyMemory(src, dst, _viewPort.width);
    }
  }

  // Fill the copied area with spaces
  for (uint16_t y = _viewPort.height - rows; y < _viewPort.height; y++)
  {
    Model1.fillMemory(SPACE_CHARACTER, getColumnAddress(getRowAddress(y), 0), _viewPort.width);
  }

  // Move the current cursor position the rows up
  uint16_t cursorPositionY = _cursorPositionY - rows;
  if (cursorPositionY < 0)
  {
    _cursorPositionY = 0;
  }
  else
  {
    _cursorPositionY = cursorPositionY;
  }
}

char *Video::read(uint8_t x, uint8_t y, uint16_t length, bool raw)
{
  uint8_t *buffer = (uint8_t *)malloc((length + 1) * sizeof(uint8_t));
  if (!buffer)
  {
    if (_logger)
    {
      _logger->err(F("Video: Failed to allocate memory for read buffer"));
    }
    return nullptr;
  }

  // Make sure this is filled with zeros in case the area is shorter than length
  for (uint16_t i = 0; i < length + 1; i++)
  {
    buffer[i] = 0;
  }

  // Read in string
  for (uint16_t i = 0; i < length; i++)
  {
    uint16_t addr = getColumnAddress(getRowAddress(y), x);
    uint8_t character = Model1.readMemory(addr);

    if (raw)
    {
      buffer[i] = character;
    }
    else
    {
      buffer[i] = convertModel1CharacterToLocal(character);
    }

    x++;
    if (x >= _viewPort.width)
    {
      x = 0;
      y++;
      if (y >= _viewPort.height)
      {
        break; // Prevent reading out of bounds
      }
    }
  }

  return (char *)buffer;
}

size_t Video::write(uint8_t ch)
{
  _print((char)ch, false);
  return 1;
}

size_t Video::write(const uint8_t *buffer, size_t size)
{
  if (!buffer)
  {
    if (_logger)
      _logger->errF(F("Video: write() called with null buffer"));
    return 0;
  }
  if (size == 0)
  {
    if (_logger)
      _logger->warnF(F("Video: write() called with length 0"));
    return 0; // Not an error, just nothing to write
  }
  size_t result = 0;
  for (uint16_t i = 0; i < size; i++)
  {
    result += write(buffer[i]);
  }
  return result;
}

void Video::print(const char character, bool raw)
{
  _print(character, raw);
}

void Video::_print(const char character, bool raw)
{
  if (character == '\0')
  {
    return;
  }
  else if (character == '\n')
  {
    _cursorPositionX = 0;
    _cursorPositionY++;
    return;
  }
  else if (character == '\r')
  {
    return; // Do nothing
  }
  else if (character == '\t')
  {
    uint8_t len = _cursorPositionX % 4;
    if (len == 0)
      len = 4;
    for (int i = 0; i < len; i++)
    {
      print(' ');
    }
    return;
  }
  else
  {
    uint16_t address = getAddress(_cursorPositionX, _cursorPositionY);
    uint8_t data = character;
    if (!raw)
    {
      data = convertLocalCharacterToModel1(character);
    }
    Model1.writeMemory(address, data);
    _cursorPositionX++;
  }

  // Check if we need to wrap the cursor position
  if (_cursorPositionX >= _viewPort.width)
  {
    _cursorPositionX = 0;
    _cursorPositionY++;
  }

  // Check if we need to scroll the screen
  if (_cursorPositionY >= _viewPort.height)
  {
    if (_autoScroll)
    {
      scroll(1);
    }
    else
    {
      _cursorPositionY = 0;
    }
  }
}

void Video::print(uint8_t x, uint8_t y, const char *str)
{
  if (!str)
  {
    if (_logger)
      _logger->err(F("Video: print() called with null string"));
    return;
  }
  uint16_t length = strlen(str);
  setXY(x, y);
  write((const uint8_t *)str, length);
}

void Video::print(uint8_t x, uint8_t y, const char *str, uint16_t length)
{
  if (!str)
  {
    if (_logger)
      _logger->err(F("Video: print() called with null string"));
    return;
  }
  if (length == 0)
  {
    if (_logger)
      _logger->warn(F("Video: print() called with length 0"));
    return;
  }
  setXY(x, y);
  write((const uint8_t *)str, length);
}

void Video::setAutoScroll(bool autoScroll)
{
  _autoScroll = autoScroll;
}

void Video::setLowerCaseMod(bool hasLowerCaseMod)
{
  _hasLowerCaseMod = hasLowerCaseMod;
}

char Video::convertModel1CharacterToLocal(char character)
{
  character &= 0x7F; // Clear the high bit - No graphics support

  if (character >= 0 && character < 32)
  {
    character += 64; // Shift to real upper-case
  }

  return character;
}

char Video::convertLocalCharacterToModel1(char character)
{
  character &= 0x7F; // Clear the high bit - No graphics support

  if (!_hasLowerCaseMod && (character >= 96 && character < 128))
  {
    character -= 32; // Shift to upper-case
  }

  return character;
}
