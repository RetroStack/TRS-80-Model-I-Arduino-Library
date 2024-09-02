#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <Arduino.h>
#include "./ILogger.h"

class Model1; // Forward declaration

class Keyboard
{
private:
  Model1 *_model1;
  ILogger *_logger;

public:
  Keyboard(ILogger *logger, Model1 *model);

  uint8_t scan();
};

#endif // KEYBOARD_H
