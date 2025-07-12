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
#include "KeyboardChangeIterator.h"

class Keyboard
{
private:
  Model1 *_model1;
  ILogger *_logger;
  uint8_t _previousState[8];

public:
  Keyboard(Model1 *model1, ILogger *logger = nullptr);

  bool isKeyPressed() const;
  void readState();

  KeyboardChangeIterator changes();
  uint8_t getFirstJustPressedKey();
};

#endif // KEYBOARD_H
