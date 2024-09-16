/*
 * Keyboard.h - Class for accessing the TRS-80 Model 1 Keyboard
 * Authors: Ven Reddy, Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <Arduino.h>
#include "ILogger.h"
#include "Model1.h"

class Keyboard
{
private:
  Model1 *_model1;
  ILogger *_logger;

public:
  Keyboard(Model1 *model1, ILogger *logger = nullptr);

  bool isKeyPressed();
  uint8_t scanKey();
};

#endif // KEYBOARD_H
