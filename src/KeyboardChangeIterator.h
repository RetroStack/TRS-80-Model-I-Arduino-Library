/*
 * KeyboardChangeIterator.h - Iterator for keyboard state changes
 * Authors: Ven Reddy, Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef KEYBOARD_CHANGE_ITERATOR_H
#define KEYBOARD_CHANGE_ITERATOR_H

#include <Arduino.h>
#include <string.h>

class KeyboardChangeIterator
{
public:
    KeyboardChangeIterator(const uint8_t *previous, const uint8_t *current); // Constructor with previous and current keyboard states

    bool hasNext() const; // Check if there are more changes to iterate
    void next();          // Advance to next change

    uint8_t keyIndex() const; // Get linear key index (0-63)
    uint8_t row() const;      // Get keyboard row (0-7)
    uint8_t column() const;   // Get keyboard column (0-7)

    bool wasPressed() const;     // Check if key was pressed in previous state
    bool isPressed() const;      // Check if key is pressed in current state
    bool wasJustPressed() const; // Check if key was just pressed (transition from released to pressed)
    bool wasReleased() const;    // Check if key was just released (transition from pressed to released)

    uint8_t keyValue() const; // Get TRS-80 key value/scan code
    String keyName() const;   // Get human-readable key name

    bool isShiftPressed() const; // Check if shift key is currently pressed

private:
    uint8_t _previous[8]; // Previous keyboard state for comparison
    uint8_t _current[8];  // Current keyboard state for comparison
    uint8_t _byteIndex;   // Current byte being examined (0-7)
    uint8_t _bitMask;     // Current bit mask for change detection
    bool _found;          // True if a change has been found at current position

    void _advanceToNextChange(); // Move iterator to next keyboard state change
    uint8_t _bitIndex() const;   // Get bit index within current byte
};

#endif /* KEYBOARD_CHANGE_ITERATOR_H */
