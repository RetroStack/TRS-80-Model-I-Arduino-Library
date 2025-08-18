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

Keyboard::Keyboard()
{
  _logger = nullptr;

  memset(_previousState, 0, sizeof(_previousState));
}

void Keyboard::setLogger(ILogger &logger)
{
  _logger = &logger;
}

bool Keyboard::isKeyPressed() const
{
  return Model1.readMemory(KEYBOARD_ALL_ADDRESS) > 0;
}

void Keyboard::update()
{
  for (int i = 0; i < 8; i++)
  {
    uint16_t keyMemAddress = KEYBOARD_MEM_ADDRESS + (1 << i);
    _previousState[i] = Model1.readMemory(keyMemAddress);
  }
}

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
