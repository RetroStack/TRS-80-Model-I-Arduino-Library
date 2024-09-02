#ifndef ROM_H
#define ROM_H

#include <Arduino.h>
#include "./ILogger.h"

class Model1; // forward decleration

class ROM
{
private:
  Model1 *_model1;
  ILogger *_logger;

public:
  ROM(ILogger *logger, Model1 *model);

  uint32_t getChecksum();
};

#endif // ROM_H