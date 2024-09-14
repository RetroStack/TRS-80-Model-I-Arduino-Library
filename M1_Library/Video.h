// Video.h

#ifndef VIDEO_H
#define VIDEO_H

#include <Arduino.h>
#include "Common.h"
#include "CRC32.h"
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
public:
  Video(ILogger *logger, Model1 *model);

  void init();
  bool checkVRAMFillChecksum(uint8_t fillValues[], int fillValuesCount);
  bool compareVRAMChecksum(uint32_t checksum);
  bool lowercaseModExists();
  void cls();
  void displayCharacterSet();
  void fillVRAM(uint8_t fillValue = 0x20, uint16_t start = VIDEO_MEM_START, uint16_t end = VIDEO_MEM_END);
  void fillVRAMwithPattern(const char *pattern = "5150", uint16_t start = VIDEO_MEM_START, uint16_t end = VIDEO_MEM_END);
  void printToScreen(const char *str);
  void printToScreen(const char *str, uint16_t startAddress);
  void printToScreen(const char *str, unsigned int x, unsigned int y, bool updateCursorPosition = false);
  void printVideoUtilitiesMenu();
  uint32_t readVRAM(bool showInHex = true, bool dumpVRAM = false);
  uint8_t readByteVRAM(uint16_t memAddress);
  void writeByteVRAM(uint16_t memAddress, uint8_t data);

private:
  Model1 *model1;
  ILogger *_logger;
  uint8_t videoData[VIDEO_MEM_SIZE];
  uint8_t memPattern[10];
  unsigned int cursorPosition;
  CRC32 crc;
  uint32_t vramChecksumTable[6];

  void memmoveVRAM(unsigned int dest, unsigned int src, unsigned int n);
  void scrollScreenUp();
};

#endif // VIDEO_H
