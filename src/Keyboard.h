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
  ILogger *_logger;          // Logger instance for debugging output
  uint8_t _previousState[8]; // Previous keyboard state for change detection

public:
  Keyboard(); // Constructor

  void setLogger(ILogger &logger); // Set logger for debugging output

  void update(); // Update keyboard state by reading current values

  bool isKeyPressed() const;        // Check if any key is currently pressed
  KeyboardChangeIterator changes(); // Get iterator for keyboard state changes since last update
  uint8_t getFirstJustPressedKey(); // Get first key that was just pressed (0 if none)
};

#endif // KEYBOARD_H
