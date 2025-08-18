/*
 * Video.h - Class for accessing the TRS-80 Model 1 Video sub-system
 * Authors: Ven Reddy, Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef VIDEO_H
#define VIDEO_H

#include <Arduino.h>
#include "ILogger.h"
#include "Model1.h"
#include <Print.h>

/**
 * Structure for the viewport information
 */
struct ViewPort
{
  uint8_t x;
  uint8_t y;
  uint8_t width;
  uint8_t height;
};

class Video : public Print
{
private:
  ILogger *_logger;
  ViewPort _viewPort;

  uint8_t _cursorPositionX;
  uint8_t _cursorPositionY;
  bool _autoScroll;
  bool _hasLowerCaseMod;

  void _print(const char character, bool raw);

public:
  // Constructor
  Video();

  // Set logger for debugging output
  void setLogger(ILogger &logger);

  // Set viewport boundaries for video operations
  void setViewPort(ViewPort viewPort);

  // Get memory address for specified row
  uint16_t getRowAddress(uint8_t y);

  // Get memory address for column within a row
  uint16_t getColumnAddress(uint16_t rowAddress, uint8_t x);

  // Get memory address for coordinates
  uint16_t getAddress(uint8_t x, uint8_t y);

  // Get current cursor X position
  uint8_t getX();

  // Set cursor X position
  void setX(uint8_t x);

  // Get current cursor Y position
  uint8_t getY();

  // Set cursor Y position
  void setY(uint8_t y);

  // Set cursor position
  void setXY(uint8_t x, uint8_t y);

  // Get viewport start X coordinate
  uint8_t getStartX();

  // Get viewport end X coordinate
  uint8_t getEndX();

  // Get viewport start Y coordinate
  uint8_t getStartY();

  // Get viewport end Y coordinate
  uint8_t getEndY();

  // Get viewport width
  uint8_t getWidth();

  // Get viewport height
  uint8_t getHeight();

  // Get total viewport size in characters
  uint16_t getSize();

  // Convert relative X to absolute screen coordinate
  uint8_t getAbsoluteX(uint8_t x);

  // Convert relative Y to absolute screen coordinate
  uint8_t getAbsoluteY(uint8_t y);

  // Clear screen with spaces
  void cls();

  // Clear screen with specified character
  void cls(char character);

  // Clear screen with character array
  void cls(char *characters);

  // Clear screen with character array of specified length
  void cls(char *characters, uint16_t length);

  // Scroll screen up by one row
  void scroll();

  // Scroll screen up by specified number of rows
  void scroll(uint8_t rows);

  // Read characters from screen at specified position and length
  char *read(uint8_t x, uint8_t y, uint16_t length, bool raw);

  // Write single character (Print interface)
  size_t write(uint8_t ch) override;

  // Write buffer of characters (Print interface)
  size_t write(const uint8_t *buffer, size_t size) override;

  // Print character with raw option
  void print(const char character, bool raw);

  // Print string at specified position
  void print(uint8_t x, uint8_t y, const char *str);

  // Print string at specified position with length limit
  void print(uint8_t x, uint8_t y, const char *str, uint16_t length);

  // Enable or disable automatic scrolling
  void setAutoScroll(bool autoScroll);

  // Set whether lowercase modification is available
  void setLowerCaseMod(bool hasLowerCaseMod);

  // Convert TRS-80 character to local character
  char convertModel1CharacterToLocal(char character);

  // Convert local character to TRS-80 character
  char convertLocalCharacterToModel1(char character);

  using Print::print;
  using Print::println;
};

#endif // VIDEO_H
