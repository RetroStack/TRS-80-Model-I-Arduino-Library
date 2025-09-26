/*
 * ROM.h - Class for accessing the TRS-80 Model 1 ROM
 * Authors: Ven Reddy, Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef ROM_H
#define ROM_H

#include <Arduino.h>
#include "ILogger.h"
#include "Model1.h"

typedef void (*ContentPrinter)(const uint8_t *data, uint16_t length, uint16_t offset);

class ROM
{
private:
  ILogger *_logger; // Logger instance for debugging output

  bool _checkROMNumber(uint8_t rom) const; // Validate ROM number is within valid range (0-2)

public:
  ROM(); // Constructor

  void setLogger(ILogger &logger); // Set logger for debugging output

  uint16_t getROMStartAddress(uint8_t rom); // Get starting memory address for specified ROM number
  uint16_t getROMLength(uint8_t rom);       // Get byte length of specified ROM

  uint32_t getChecksum(uint8_t rom);        // Calculate checksum for specified ROM
  const __FlashStringHelper *identifyROM(); // Identify ROM contents and return description string

  bool dumpROMToSD(uint8_t rom, const char *filename); // Dump single ROM contents as binary to SD card file
  bool dumpAllROMsToSD(const char *filename);          // Dump all ROMs combined as binary to SD card file

  // Print ROM contents with specified formatting options
  void printROMContents(uint8_t rom, PRINT_STYLE style = BOTH, bool relative = true, uint16_t bytesPerLine = 32);
};

#endif // ROM_H
