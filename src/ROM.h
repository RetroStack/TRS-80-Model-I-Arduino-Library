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
  Model1 *_model1;
  ILogger *_logger;

  bool _checkROMNumber(uint8_t rom) const;

public:
  ROM(Model1 *model1, ILogger *logger = nullptr);

  uint16_t getROMStartAddress(uint8_t rom);
  uint16_t getROMLength(uint8_t rom);

  uint32_t getChecksum(uint8_t rom);
  const __FlashStringHelper *identifyROM();

  void printROMContents(uint8_t rom);
};

#endif // ROM_H
