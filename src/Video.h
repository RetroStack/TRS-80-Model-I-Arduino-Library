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
  ILogger *_logger;   // Logger instance for debugging output
  ViewPort _viewPort; // Viewport boundaries for video operations

  uint8_t _cursorPositionX; // Current cursor X position (0-63)
  uint8_t _cursorPositionY; // Current cursor Y position (0-15)
  bool _autoScroll;         // Enable automatic scrolling when cursor reaches bottom
  bool _hasLowerCaseMod;    // True if lowercase modification is available

  void _print(const char character, bool raw); // Internal character printing with raw mode option

public:
  Video(); // Constructor

  void setLogger(ILogger &logger);     // Set logger for debugging output
  void setViewPort(ViewPort viewPort); // Set viewport boundaries for video operations

  uint16_t getRowAddress(uint8_t y);                         // Get memory address for specified row
  uint16_t getColumnAddress(uint16_t rowAddress, uint8_t x); // Get memory address for column within a row
  uint16_t getAddress(uint8_t x, uint8_t y);                 // Get memory address for coordinates

  uint8_t getX();       // Get current cursor X position
  void setX(uint8_t x); // Set cursor X position

  uint8_t getY();       // Get current cursor Y position
  void setY(uint8_t y); // Set cursor Y position

  void setXY(uint8_t x, uint8_t y); // Set cursor position

  uint8_t getStartX(); // Get viewport start X coordinate
  uint8_t getEndX();   // Get viewport end X coordinate

  uint8_t getStartY(); // Get viewport start Y coordinate
  uint8_t getEndY();   // Get viewport end Y coordinate

  uint8_t getWidth();  // Get viewport width
  uint8_t getHeight(); // Get viewport height
  uint16_t getSize();  // Get total viewport size in characters

  uint8_t getAbsoluteX(uint8_t x); // Convert relative X to absolute screen coordinate
  uint8_t getAbsoluteY(uint8_t y); // Convert relative Y to absolute screen coordinate

  void cls();                                  // Clear screen with spaces
  void cls(char character);                    // Clear screen with specified character
  void cls(char *characters);                  // Clear screen with character array
  void cls(char *characters, uint16_t length); // Clear screen with character array of specified length

  void scroll();             // Scroll screen up by one row
  void scroll(uint8_t rows); // Scroll screen up by specified number of rows

  char *read(uint8_t x, uint8_t y, uint16_t length, bool raw); // Read characters from screen at specified position and length

  size_t write(uint8_t ch) override;                         // Write single character (Print interface)
  size_t write(const uint8_t *buffer, size_t size) override; // Write buffer of characters (Print interface)

  void print(const char character, bool raw);                         // Print character with raw option
  void print(uint8_t x, uint8_t y, const char *str);                  // Print string at specified position
  void print(uint8_t x, uint8_t y, const char *str, uint16_t length); // Print string at specified position with length limit

  void setAutoScroll(bool autoScroll);        // Enable or disable automatic scrolling
  void setLowerCaseMod(bool hasLowerCaseMod); // Set whether lowercase modification is available

  bool captureToSD(const char *filename, bool useLocalCharacterSet = true); // Capture current viewport to SD card file

  char convertModel1CharacterToLocal(char character); // Convert TRS-80 character to local character
  char convertLocalCharacterToModel1(char character); // Convert local character to TRS-80 character

  using Print::print;
  using Print::println;
};

#endif // VIDEO_H
