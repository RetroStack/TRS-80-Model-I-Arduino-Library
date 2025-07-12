#include "Keyboard.h"
#include "Model1.h"
#include "utils.h"

#define KEYBOARD_ALL_ADDRESS 0x38FF
#define KEYBOARD_MEM_ADDRESS 0x3800

Keyboard::Keyboard(Model1 *model1, ILogger *logger)
{
  _model1 = model1;
  _logger = logger;

  memset(_previousState, 0, sizeof(_previousState));
}

/**
 * Checks whether any key is pressed at the moment
 */
bool Keyboard::isKeyPressed() const
{
  return _model1->readMemory(KEYBOARD_ALL_ADDRESS) > 0;
}

/**
 * Reads the current state of the keyboard, dismissing all changes.
 */
void Keyboard::readState()
{
  for (int i = 0; i < 8; i++)
  {
    uint16_t keyMemAddress = KEYBOARD_MEM_ADDRESS + (1 << i);
    _previousState[i] = _model1->readMemory(keyMemAddress);
  }
}

/**
 * Scans the keyboard and returns an iterator for the changed keys.
 */
KeyboardChangeIterator Keyboard::changes()
{
  uint8_t keyboardState[8];
  for (int i = 0; i < 8; i++)
  {
    uint16_t keyMemAddress = KEYBOARD_MEM_ADDRESS + (1 << i);
    keyboardState[i] = _model1->readMemory(keyMemAddress);
  }

  KeyboardChangeIterator it(_previousState, keyboardState);

  memcpy(_previousState, keyboardState, sizeof(keyboardState));

  return it;
}

/**
 * Scans the keyboard and returns the value of the first pressed key.
 * If no key is pressed, it returns 0.
 */
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
