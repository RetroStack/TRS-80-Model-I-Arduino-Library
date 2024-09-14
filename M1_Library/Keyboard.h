// Keyboard.h

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <Arduino.h>
#include "Common.h"
#include "Utils.h"
#include "./ILogger.h"

#define KEYBOARD_MEM_ADDRESS 0x3800
#define KEYBOARD_SHIFT_KEY 0x3880

class Model1; // Forward declaration

class Keyboard
{
public:
  Keyboard(ILogger *logger, Model1 *model);

  uint8_t scanKeyboard();

private:
  Model1 *model1;
  ILogger *_logger;
};

#endif // KEYBOARD_H
