// Model1.h

#ifndef MODEL1_H
#define MODEL1_H

#include <Arduino.h>
#include "Common.h"
#include "Keyboard.h"
#include "ROM.h"
#include "Utils.h"
#include "Video.h"
#include "./ILogger.h"

class Model1
{
private:
  ILogger *_logger;

public:
  Model1(ILogger *logger);
  void init();

  void displayCtrlPinStatus();
  void initControlPins();
  void setAddressLinesToInput();
  void setAddressLinesToOutput(uint16_t memAddress = 0x3C00);
  void setAllPinsPortsToInput();
  void setDataLinesToInput();
  void setDataLinesToOutput();
  void setTESTPin(int state);
  void turnOffReadWriteRASLines();

  Video video;
  Keyboard keyboard;
  ROM rom;

private:
protected:
};

#endif // MODEL1_H
