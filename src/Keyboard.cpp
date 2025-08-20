/*
 * Keyboard.cpp - Class for accessing the keyboard matrix
 * Authors: Ven Reddy, Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#include "Keyboard.h"
#include "Model1.h"
#include "utils.h"

#define KEYBOARD_ALL_ADDRESS 0x38FF
#define KEYBOARD_MEM_ADDRESS 0x3800

// Constructor - initialize keyboard interface
Keyboard::Keyboard()
{
  _logger = nullptr;

  memset(_previousState, 0, sizeof(_previousState));
}

// Set logger for debugging output
void Keyboard::setLogger(ILogger &logger)
{
  _logger = &logger;
}

// Check if any key is currently pressed
bool Keyboard::isKeyPressed() const
{
  return Model1.readMemory(KEYBOARD_ALL_ADDRESS) > 0;
}

// Update keyboard state by reading current values
void Keyboard::update()
{
  for (int i = 0; i < 8; i++)
  {
    uint16_t keyMemAddress = KEYBOARD_MEM_ADDRESS + (1 << i);
    _previousState[i] = Model1.readMemory(keyMemAddress);
  }
}

// Get iterator for keyboard state changes since last update
KeyboardChangeIterator Keyboard::changes()
{
  uint8_t keyboardState[8];
  for (int i = 0; i < 8; i++)
  {
    uint16_t keyMemAddress = KEYBOARD_MEM_ADDRESS + (1 << i);
    keyboardState[i] = Model1.readMemory(keyMemAddress);
  }

  KeyboardChangeIterator it(_previousState, keyboardState);

  memcpy(_previousState, keyboardState, sizeof(keyboardState));

  return it;
}

// Get the first key that was just pressed
uint8_t Keyboard::getFirstJustPressedKey()
{
  KeyboardChangeIterator it = changes();
  while (it.hasNext())
  {
    if (it.wasJustPressed())
    {
      return it.keyValue();
    }
    it.next();
  }

  return 0;
}
