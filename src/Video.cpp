#include "Video.h"
#include "Model1.h"

const uint8_t VIDEO_COLS = 64;
const uint8_t VIDEO_ROWS = 16;
const uint16_t VIDEO_MEM_START = 0x3C00;

const uint8_t SPACE_CHARACTER = 0x20;

const uint8_t CASSETTE_PORT = 0xff;

/**
 * Initializes the TRS-80 Model 1 Video Subsystem
 */
Video::Video(Model1 *model1, ILogger *logger)
{
  _model1 = model1;
  _logger = logger;

  _cursorPositionX = 0;
  _cursorPositionY = 0;

  _autoScroll = true;
  _hasLowerCaseMod = false;

  _viewPort.x = 0;
  _viewPort.y = 0;
  _viewPort.width = VIDEO_COLS;
  _viewPort.height = VIDEO_ROWS;
}

/**
 * Initializes the TRS-80 Model 1 Video Subsystem with custom viewport
 */
Video::Video(Model1 *model1, ViewPort viewPort, ILogger *logger)
{
  _model1 = model1;
  _logger = logger;

  _cursorPositionX = 0;
  _cursorPositionY = 0;

  _autoScroll = true;
  _hasLowerCaseMod = false;

  // Validate and auto-correct viewport
  if (viewPort.x >= VIDEO_COLS)
  {
    viewPort.x = VIDEO_COLS - 1;
    if (logger)
      logger->warn("X coordinate of viewport is larger than there is space. Reset to %d.", viewPort.x);
  }
  if (viewPort.y >= VIDEO_ROWS)
  {
    viewPort.y = VIDEO_ROWS - 1;
    if (logger)
      logger->warn("Y coordinate of viewport is larger than there is space. Reset to %d.", viewPort.y);
  }
  if (viewPort.x + viewPort.width > VIDEO_COLS)
  {
    viewPort.width = VIDEO_COLS - viewPort.x;
    if (logger)
      logger->warn("Width of viewport is larger than there is space. Reset to %d.", viewPort.width);
  }
  if (viewPort.y + viewPort.height > VIDEO_ROWS)
  {
    viewPort.height = VIDEO_ROWS - viewPort.y;
    if (logger)
      logger->warn("Height of viewport is larger than there is space. Reset to %d.", viewPort.height);
  }

  _viewPort = viewPort;
}

/**
 * Calculates the physical address of the row
 *
 * NOTE: No overflow is checked.
 */
uint16_t Video::_getRowAddress(uint8_t y)
{
  return VIDEO_MEM_START + ((_viewPort.y + y) * VIDEO_COLS);
}

/**
 * Calculates the physical address of the column
 *
 * NOTE: No overflow is checked.
 */
uint16_t Video::_getColumnAddress(uint16_t rowAddress, uint8_t x)
{
  return rowAddress + _viewPort.x + x;
}

/**
 * Calculates the physical address for x and y coordinate
 *
 * NOTE: No overflow is checked.
 */
uint16_t Video::_getAddress(uint8_t x, uint8_t y)
{
  uint16_t rowAddress = _getRowAddress(y);
  return _getColumnAddress(rowAddress, x);
}

/**
 * Gets the current x coordinate within the viewport
 */
uint8_t Video::getX()
{
  return _cursorPositionX;
}

/**
 * Sets the cursor to a specified x coordinate
 */
void Video::setX(uint8_t x)
{
  if (x > _viewPort.width)
  {
    _cursorPositionX = _viewPort.width - 1;
  }
  else
  {
    _cursorPositionX = x;
  }
}

/**
 * Gets the current y coordinate within the viewport
 */
uint8_t Video::getY()
{
  return _cursorPositionY;
}

/**
 * Sets the cursor to a specified y coordinate
 */
void Video::setY(uint8_t y)
{
  if (y > _viewPort.height)
  {
    _cursorPositionY = _viewPort.height - 1;
  }
  else
  {
    _cursorPositionY = y;
  }
}

/**
 * Sets the cursor to a specified x & y coordinate
 */
void Video::setXY(uint8_t x, uint8_t y)
{
  setX(x);
  setY(y);
}

/**
 * Gets the absolute horizontal start position of viewport
 */
uint8_t Video::getStartX()
{
  return _viewPort.x;
}

/**
 * Gets the absolute horizontal end position of viewport
 */
uint8_t Video::getEndX()
{
  return _viewPort.x + _viewPort.width;
}

/**
 * Gets the absolute vertical start position of viewport
 */
uint8_t Video::getStartY()
{
  return _viewPort.y;
}

/**
 * Gets the absolute vertical end position of viewport
 */
uint8_t Video::getEndY()
{
  return _viewPort.y + _viewPort.height;
}

/**
 * Gets the width of the viewport
 */
uint8_t Video::getWidth()
{
  return _viewPort.width;
}

/**
 * Gets the height of the viewport
 */
uint8_t Video::getHeight()
{
  return _viewPort.height;
}

/**
 * Gets the total size of the viewport
 */
uint16_t Video::getSize()
{
  return _viewPort.width * _viewPort.height;
}

/**
 * Gets the absolute X coordinate when given the viewport x coordinate
 */
uint8_t Video::getAbsoluteX(uint8_t x)
{
  int xCoord = _viewPort.x + x;
  return xCoord > VIDEO_COLS ? VIDEO_COLS : xCoord;
}

/**
 * Gets the absolute Y coordinate when given the viewport y coordinate
 */
uint8_t Video::getAbsoluteY(uint8_t y)
{
  int yCoord = _viewPort.y + y;
  return yCoord > VIDEO_ROWS ? VIDEO_ROWS : yCoord;
}

/**
 * Clears the screen, filling it with space characters
 */
void Video::cls()
{
  cls(SPACE_CHARACTER);
}

/**
 * Clears the screen, filling it with a specific character
 */
void Video::cls(char character)
{
  cls(&character, 1);
}

/**
 * Clears the screen, filling it with a specific string
 */
void Video::cls(char *characters)
{
  uint16_t length = strlen(characters);
  cls(characters, length);
}

/**
 * Clears the screen, filling it with a specific string
 */
void Video::cls(char *characters, uint16_t length)
{
  int i = 0;
  for (uint16_t y = 0; y < _viewPort.height; y++)
  {
    int rowAddress = _getRowAddress(y);
    for (uint16_t x = 0; x < _viewPort.width; x++)
    {
      _model1->writeMemory(_getColumnAddress(rowAddress, x), convertLocalCharacterToModel1(characters[i % length]));
      i++;
    }
  }
  _cursorPositionX = 0;
  _cursorPositionY = 0;
}

/**
 * Scrolls the screen by 1 row, including the cursor
 */
void Video::scroll()
{
  scroll(1);
}

/**
 * Scrolls the screen by x rows, including the cursor
 */
void Video::scroll(uint8_t rows)
{
  if (rows == 0)
    return;

  // If there are more rows than available, just cap it at the maximum number of rows
  if (rows > _viewPort.height)
  {
    rows = _viewPort.height;
  }

  // Only copy if not the whole memory gets replaced with spaces anyways
  if (rows < _viewPort.height)
  {
    for (uint16_t y = rows; y < _viewPort.height; y++)
    {
      uint16_t src = _getColumnAddress(_getRowAddress(y), 0);
      uint16_t dst = _getColumnAddress(_getRowAddress(y - rows), 0);
      _model1->copyMemory(src, dst, _viewPort.width);
    }
  }

  // Fill the copied area with spaces
  for (uint16_t y = _viewPort.height - rows; y < _viewPort.height; y++)
  {
    _model1->fillMemory(SPACE_CHARACTER, _getColumnAddress(_getRowAddress(y), 0), _viewPort.width);
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

/**
 * Reads a string from the video memory at a specific location and length
 */
char *Video::read(uint8_t x, uint8_t y, uint16_t length, bool raw)
{
  uint8_t *buffer = new uint8_t[length + 1];

  // Make sure this is filled with zeros in case the area is shorter than length
  for (int i = 0; i < length + 1; i++)
  {
    buffer[i] = 0;
  }

  // Read in string
  for (int i = 0; i < length; i++)
  {
    uint16_t addr = _getColumnAddress(_getRowAddress(y), x);
    uint8_t character = _model1->readMemory(addr);

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

/**
 * Prints one character to the current cursor position
 */
void Video::print(const char character)
{
  _print(character, false);
}

/**
 * Prints one character to the current cursor position
 */
void Video::print(const char character, bool raw)
{
  _print(character, raw);
}

/**
 * Prints one character to the current cursor position
 */
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
    uint16_t address = _getAddress(_cursorPositionX, _cursorPositionY);
    uint8_t data = character;
    if (!raw)
    {
      data = convertLocalCharacterToModel1(character);
    }
    _model1->writeMemory(address, data);
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

/**
 * Prints a string to the current cursor position
 */
void Video::print(const char *str)
{
  uint16_t length = strlen(str);
  print(str, length);
}

/**
 * Prints a string to the current cursor position
 */
void Video::print(const char *str, uint16_t length)
{
  for (uint16_t i = 0; i < length; i++)
  {
    if (str[i] == '\0')
      return;
    print(str[i]);
  }
}

/**
 * Prints a new line to the current cursor position
 */
void Video::printLn()
{
  print('\n');
}

/**
 * Prints a string with a new line to the current cursor position
 */
void Video::printLn(const char *str)
{
  uint16_t length = strlen(str);
  print(str, length);
  print('\n');
}

/**
 * Prints a string with a new line to the current cursor position
 */
void Video::printLn(const char *str, uint16_t length)
{
  print(str, length);
  print('\n');
}

/**
 * Prints a string to the given x and y coordinate within the viewport
 */
void Video::print(const char *str, uint8_t x, uint8_t y)
{
  uint16_t length = strlen(str);
  setXY(x, y);
  print(str, length);
}

/**
 * Prints a string to the given x and y coordinate within the viewport
 */
void Video::print(const char *str, uint16_t length, uint8_t x, uint8_t y)
{
  setXY(x, y);
  print(str, length);
}

/**
 * Checks whether video mode is in 64 characters
 */
bool Video::is64Mode()
{
  return (_model1->readIO(CASSETTE_PORT) & 0b01000000) > 0;
}

/**
 * Changes the video mode to 32 characters
 */
void Video::set32Mode()
{
  _model1->writeIO(CASSETTE_PORT, 0b00001000);
}

/**
 * Changes the video mode to 64 characters
 */
void Video::set64Mode()
{
  _model1->writeIO(CASSETTE_PORT, 0b00000000);
}

/**
 * Sets whether the video should automatically scroll when the cursor reaches the end of the viewport
 */
void Video::setAutoScroll(bool autoScroll)
{
  _autoScroll = autoScroll;
}

/**
 * Sets whether the video has a lower-case mod or not
 */
void Video::setLowerCaseMod(bool hasLowerCaseMod)
{
  _hasLowerCaseMod = hasLowerCaseMod;
}

/**
 * Converts a string coming from the Model 1 to the local ASCII format
 */
char Video::convertModel1CharacterToLocal(char character)
{
  character &= 0x7F; // Clear the high bit - No graphics support

  if (character >= 0 && character < 32)
  {
    character += 64; // Shift to real upper-case
  }

  return character;
}

/**
 * Converts an ASCII string to the Model 1 format
 */
char Video::convertLocalCharacterToModel1(char character)
{
  character &= 0x7F; // Clear the high bit - No graphics support

  if (!_hasLowerCaseMod && (character >= 96 && character < 128))
  {
    character -= 32; // Shift to upper-case
  }

  return character;
}