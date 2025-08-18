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
  // Constructor
  ROM();

  // Set logger for debugging output
  void setLogger(ILogger &logger);

  // Get starting memory address for specified ROM number
  uint16_t getROMStartAddress(uint8_t rom);

  // Get byte length of specified ROM
  uint16_t getROMLength(uint8_t rom);

  // Calculate checksum for specified ROM
  uint32_t getChecksum(uint8_t rom);

  // Identify ROM contents and return description string
  const __FlashStringHelper *identifyROM();

  // Print ROM contents with specified formatting options
  void printROMContents(uint8_t rom, PRINT_STYLE style = BOTH, bool relative = true, uint16_t bytesPerLine = 32);
};

#endif // ROM_H
