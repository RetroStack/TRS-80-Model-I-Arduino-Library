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

class Video
{
private:
  Model1 *_model1;
  ILogger *_logger;
  ViewPort _viewPort;

  uint8_t _cursorPositionX;
  uint8_t _cursorPositionY;
  bool _autoScroll;
  bool _hasLowerCaseMod;

  uint16_t _getRowAddress(uint8_t y);
  uint16_t _getColumnAddress(uint16_t rowAddress, uint8_t x);
  uint16_t _getAddress(uint8_t x, uint8_t y);

  void _print(const char character, bool raw);

public:
  Video(Model1 *model1, ILogger *logger = nullptr);
  Video(Model1 *model1, ViewPort viewPort, ILogger *logger = nullptr);

  uint8_t getX();
  void setX(uint8_t x);
  uint8_t getY();
  void setY(uint8_t y);
  void setXY(uint8_t x, uint8_t y);

  uint8_t getStartX();
  uint8_t getEndX();
  uint8_t getStartY();
  uint8_t getEndY();
  uint8_t getWidth();
  uint8_t getHeight();
  uint16_t getSize();

  uint8_t getAbsoluteX(uint8_t x);
  uint8_t getAbsoluteY(uint8_t y);

  void cls();
  void cls(char character);
  void cls(char *characters);
  void cls(char *characters, uint16_t length);

  void scroll();
  void scroll(uint8_t rows);

  char *read(uint8_t x, uint8_t y, uint16_t length, bool raw);

  void print(const char character);
  void print(const char character, bool raw);

  void print(const char *str);
  void print(const char *str, uint16_t length);

  void printLn();
  void printLn(const char *str);
  void printLn(const char *str, uint16_t length);

  void print(const char *str, uint8_t x, uint8_t y);
  void print(const char *str, uint16_t length, uint8_t x, uint8_t y);

  void setAutoScroll(bool autoScroll);
  void setLowerCaseMod(bool hasLowerCaseMod);

  char convertModel1CharacterToLocal(char character);
  char convertLocalCharacterToModel1(char character);
};

#endif // VIDEO_H
