/*
 * Model1.h - Class for accessing the TRS-80 Model 1 low-level signals
 * Authors: Ven Reddy, Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef KEYBOARDCHANGEITERATOR_H
#define KEYBOARDCHANGEITERATOR_H

#include <Arduino.h>
#include <string.h>

class KeyboardChangeIterator
{
public:
    KeyboardChangeIterator(const uint8_t *previous, const uint8_t *current);

    bool hasNext() const;
    void next();

    uint8_t keyIndex() const;
    uint8_t row() const;
    uint8_t column() const;

    bool wasPressed() const;
    bool isPressed() const;

    bool wasJustPressed() const;
    bool wasReleased() const;

    uint8_t keyValue() const;
    String keyName() const;
    bool isShiftPressed() const;

private:
    uint8_t _previous[8];
    uint8_t _current[8];
    uint8_t _byteIndex;
    uint8_t _bitMask;
    bool _found;

    void _advanceToNextChange();
    uint8_t _bitIndex() const;
};

#endif /* KEYBOARDCHANGEITERATOR_H */
