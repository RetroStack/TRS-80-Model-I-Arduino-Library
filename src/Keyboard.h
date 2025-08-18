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
  // Constructor
  Keyboard();

  // Set logger for debugging output
  void setLogger(ILogger &logger);

  // Check if any key is currently pressed
  bool isKeyPressed() const;

  // Update keyboard state by reading current values
  void update();

  // Get iterator for keyboard state changes since last update
  KeyboardChangeIterator changes();

  // Get first key that was just pressed (0 if none)
  uint8_t getFirstJustPressedKey();
};

#endif // KEYBOARD_H
