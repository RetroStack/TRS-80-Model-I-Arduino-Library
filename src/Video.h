// Video.h

#ifndef VIDEO_H
#define VIDEO_H

#include "CRC32.h"
#include "Shield.h"

#define VIDEO_COLS        64
#define VIDEO_ROWS        16
#define VIDEO_MEM_START   0x3C00
#define VIDEO_MEM_SIZE    (VIDEO_COLS * VIDEO_ROWS)
#define VIDEO_MEM_END     VIDEO_MEM_START + VIDEO_MEM_SIZE
#define VIDEO_LAST_ROW    (VIDEO_MEM_START + VIDEO_MEM_SIZE - VIDEO_COLS)

class Video {
public:
  static void init();
  static bool checkVRAMFillChecksum(uint8_t fillValues[], int fillValuesCount);
  static bool compareVRAMChecksum(uint32_t checksum);
  static bool lowercaseModExists(bool silent = true);
  static void cls();
  static void displayCharacterSet();
  static void fillVRAM(bool silent = true, uint8_t fillValue = 0x20, uint16_t start = VIDEO_MEM_START, uint16_t end = VIDEO_MEM_END);
  static void fillVRAMwithPattern(bool silent = true, const char* pattern = "5150", uint16_t start = VIDEO_MEM_START, uint16_t end = VIDEO_MEM_END);
  static void printToScreen(const char *str);
  static void printToScreen(const char *str, uint16_t startAddress);
  static void printToScreen(const char *str, unsigned int x, unsigned int y, bool updateCursorPosition = false);
  static void printVideoUtilitiesMenu();
  static uint32_t readVRAM(bool silent = false, bool showInHex = true, bool dumpVRAM = false);
  static uint8_t readByteVRAM(uint16_t memAddress);
  static void writeByteVRAM(uint16_t memAddress, uint8_t data);

private:
  static uint8_t videoData[VIDEO_MEM_SIZE];
  static uint8_t memPattern[10];
  static unsigned int cursorPosition;
  static CRC32 crc;
  static uint32_t vramChecksumTable[6];

  static void memmoveVRAM(unsigned int dest, unsigned int src, unsigned int n);
  static void scrollScreenUp();
};

#endif
