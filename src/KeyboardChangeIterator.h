/*
 * KeyboardChangeIterator.h - Iterator for keyboard state changes
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
    // Constructor with previous and current keyboard states
    KeyboardChangeIterator(const uint8_t *previous, const uint8_t *current);

    // Check if there are more changes to iterate
    bool hasNext() const;

    // Advance to next change
    void next();

    // Get linear key index (0-63)
    uint8_t keyIndex() const;

    // Get keyboard row (0-7)
    uint8_t row() const;

    // Get keyboard column (0-7)
    uint8_t column() const;

    // Check if key was pressed in previous state
    bool wasPressed() const;

    // Check if key is pressed in current state
    bool isPressed() const;

    // Check if key was just pressed (transition from released to pressed)
    bool wasJustPressed() const;

    // Check if key was just released (transition from pressed to released)
    bool wasReleased() const;

    // Get TRS-80 key value/scan code
    uint8_t keyValue() const;

    // Get human-readable key name
    String keyName() const;

    // Check if shift key is currently pressed
    bool isShiftPressed() const;

private:
    uint8_t _previous[8]; // Previous keyboard state for comparison
    uint8_t _current[8];  // Current keyboard state for comparison
    uint8_t _byteIndex;   // Current byte being examined (0-7)
    uint8_t _bitMask;     // Current bit mask for change detection
    bool _found;          // True if a change has been found at current position

    void _advanceToNextChange(); // Move iterator to next keyboard state change
    uint8_t _bitIndex() const;   // Get bit index within current byte
};

#endif /* KEYBOARDCHANGEITERATOR_H */
