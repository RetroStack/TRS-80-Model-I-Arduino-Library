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

class ROM
{
private:
  Model1 *_model1;
  ILogger *_logger;

public:
  ROM(Model1 *model1, ILogger *logger = nullptr);

  uint8_t *getContent();
};

#endif // ROM_H
