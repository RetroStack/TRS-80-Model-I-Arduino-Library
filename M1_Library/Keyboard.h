#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <Arduino.h>
#include "./ILogger.h"
#include "./Model1.h"

class Keyboard
{
private:
  Model1 *_model1;
  ILogger *_logger;

public:
  Keyboard(Model1 *model1, ILogger *logger = nullptr);

  bool isKeyPressed();
  uint8_t scan();
};

#endif // KEYBOARD_H
