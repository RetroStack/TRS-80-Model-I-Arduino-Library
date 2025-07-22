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
  ILogger *_logger;
  uint8_t _previousState[8];

public:
  Keyboard();

  void setLogger(ILogger &logger);

  bool isKeyPressed() const;
  void update();

  KeyboardChangeIterator changes();
  uint8_t getFirstJustPressedKey();
};

#endif // KEYBOARD_H
