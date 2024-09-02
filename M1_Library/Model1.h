#ifndef MODEL1_H
#define MODEL1_H

#include <Arduino.h>
#include "./Common.h"
#include "./Utils.h"
#include "./Keyboard.h"
#include "./ROM.h"
#include "./Video.h"
#include "./ILogger.h"

class Model1
{
private:
  ILogger *_logger;

  Keyboard *_keyboard;
  ROM *_rom;
  Video *_video;

  void initControlPins();
  void setAllPinsPortsToInput();

public:
  Model1(ILogger *logger);

  Keyboard *getKeyboard();
  ROM *getROM();
  Video *getVideo();

  void displayCtrlPinStatus();
  void setAddressLinesToInput();
  void setAddressLinesToOutput(uint16_t memAddress = 0x3C00);
  void setDataLinesToInput();
  void setDataLinesToOutput();
  void setTESTPin(int state);
  void turnOffReadWriteRASLines();
};

#endif // MODEL1_H
