#ifndef VIDEO_H
#define VIDEO_H

#include <Arduino.h>
#include "./ILogger.h"

#define VIDEO_COLS 64
#define VIDEO_ROWS 16
#define VIDEO_MEM_START 0x3C00
#define VIDEO_MEM_SIZE (VIDEO_COLS * VIDEO_ROWS)
#define VIDEO_MEM_END VIDEO_MEM_START + VIDEO_MEM_SIZE
#define VIDEO_LAST_ROW (VIDEO_MEM_START + VIDEO_MEM_SIZE - VIDEO_COLS)

class Model1; // forward decleration

class Video
{
private:
  Model1 *model1;
  ILogger *_logger;

  uint16_t _cursorPosition;

public:
  Video(ILogger *logger, Model1 *model);

  void cls();

  void printToScreen(const char *str);
  void printToScreen(const char *str, uint16_t startAddress);
  void printToScreen(const char *str, uint8_t x, uint8_t y, bool updateCursorPosition = false);

  void fillVRAM(uint8_t fillValue = 0x20, uint16_t start = VIDEO_MEM_START, uint16_t end = VIDEO_MEM_END);
  void fillVRAMwithPattern(const char *pattern = "5150", uint16_t start = VIDEO_MEM_START, uint16_t end = VIDEO_MEM_END);

  uint8_t *readVRAM();
  uint8_t readByteVRAM(uint16_t memAddress);
  void writeByteVRAM(uint16_t memAddress, uint8_t data);

  void memmoveVRAM(unsigned int dest, unsigned int src, unsigned int n);
  void scrollScreenUp();
};

#endif // VIDEO_H
