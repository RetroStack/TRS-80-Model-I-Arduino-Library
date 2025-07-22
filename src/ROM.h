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
  ILogger *_logger;

  bool _checkROMNumber(uint8_t rom) const;

public:
  ROM();

  void setLogger(ILogger &logger);

  uint16_t getROMStartAddress(uint8_t rom);
  uint16_t getROMLength(uint8_t rom);

  uint32_t getChecksum(uint8_t rom);
  const __FlashStringHelper *identifyROM();

  void printROMContents(uint8_t rom, PRINT_STYLE style = BOTH, bool relative = true, uint16_t bytesPerLine = 32);
};

#endif // ROM_H
