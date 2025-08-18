/*
 * KeyboardChangeIterator.cpp - Iterator for keyboard state c// Check if there are more changes to iterate
bool KeyboardChangeIterator::hasNext() const
{
    return _found;
}

// Get the key information for the current change
KeyChange KeyboardChangeIterator::next() * Authors: Ven Reddy, Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#include "KeyboardChangeIterator.h"

#define KEY_ENTER 0xB0
#define KEY_CLEAR 0xB2
#define KEY_BREAK 0xB1
#define KEY_UP 0xDA    // Up arrow
#define KEY_DOWN 0xD9  // Down arrow
#define KEY_LEFT 0xD8  // Left arrow
#define KEY_RIGHT 0xD7 // Right arrow
#define KEY_SPACE 0x20 // Space character
#define KEY_SHIFT 0x81 // Shift key

// Unused keys, but make them distinguishable in lookup table
#define KEY_UNUSED_1 0xC2
#define KEY_UNUSED_2 0xC3
#define KEY_UNUSED_3 0xC4
#define KEY_UNUSED_4 0xC5
#define KEY_UNUSED_5 0xC6
#define KEY_UNUSED_6 0xC7
#define KEY_UNUSED_7 0xC8
#define KEY_UNUSED_8 0xC9
#define KEY_UNUSED_9 0xCA
#define KEY_UNUSED_10 0xCB
#define KEY_UNUSED_11 0xCC
#define KEY_UNUSED_12 0xCD

// Look-up table for uppercase characters
const uint8_t lookupTable[8][8] PROGMEM = {
    {'@', 'a', 'b', 'c', 'd', 'e', 'f', 'g'},                                                                         // 3801
    {'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o'},                                                                         // 3802
    {'p', 'q', 'r', 's', 't', 'u', 'v', 'w'},                                                                         // 3804
    {'x', 'y', 'z', KEY_UNUSED_1, KEY_UNUSED_2, KEY_UNUSED_3, KEY_UNUSED_4, KEY_UNUSED_5},                            // 3808
    {'0', '1', '2', '3', '4', '5', '6', '7'},                                                                         // 3810
    {'8', '9', ':', ';', ',', '-', '.', '/'},                                                                         // 3820
    {KEY_ENTER, KEY_CLEAR, KEY_BREAK, KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_SPACE},                              // 3840
    {KEY_SHIFT, KEY_UNUSED_6, KEY_UNUSED_7, KEY_UNUSED_8, KEY_UNUSED_9, KEY_UNUSED_10, KEY_UNUSED_11, KEY_UNUSED_12}, // 3860
};
const uint8_t lookupTableShift[8][8] PROGMEM = {
    {'@', 'A', 'B', 'C', 'D', 'E', 'F', 'G'},                                                                         // 3801
    {'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O'},                                                                         // 3802
    {'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W'},                                                                         // 3804
    {'X', 'Y', 'Z', KEY_UNUSED_1, KEY_UNUSED_2, KEY_UNUSED_3, KEY_UNUSED_4, KEY_UNUSED_5},                            // 3808
    {'0', '!', '"', '#', '$', '%', '&', '\''},                                                                        // 3810
    {'(', ')', '*', '+', '<', '=', '>', '?'},                                                                         // 3820
    {KEY_ENTER, KEY_CLEAR, KEY_BREAK, KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_SPACE},                              // 3840
    {KEY_SHIFT, KEY_UNUSED_6, KEY_UNUSED_7, KEY_UNUSED_8, KEY_UNUSED_9, KEY_UNUSED_10, KEY_UNUSED_11, KEY_UNUSED_12}, // 3860
};

// Constructor with previous and current keyboard states
KeyboardChangeIterator::KeyboardChangeIterator(const uint8_t *previous, const uint8_t *current)
    : _byteIndex(0), _bitMask(1), _found(false)
{
    memcpy(_previous, previous, 8);
    memcpy(_current, current, 8);

    _advanceToNextChange();
}

// Check if there are more changes to iterate
bool KeyboardChangeIterator::hasNext() const
{
    return _found;
}

// Advance to next change
void KeyboardChangeIterator::next()
{
    _bitMask <<= 1;

    if (_bitMask == 0)
    {
        _bitMask = 0x01;
        _byteIndex++;
    }

    _advanceToNextChange();
}

// Get linear key index (0-63)
uint8_t KeyboardChangeIterator::keyIndex() const
{
    return _byteIndex * 8 + _bitIndex();
}

// Get keyboard row (0-7)
uint8_t KeyboardChangeIterator::row() const
{
    return _byteIndex;
}

// Get keyboard column (0-7)
uint8_t KeyboardChangeIterator::column() const
{
    return _bitIndex();
}

// Check if key was pressed in previous state
bool KeyboardChangeIterator::wasPressed() const
{
    return (_previous[_byteIndex] & _bitMask) != 0;
}

// Check if key is pressed in current state
bool KeyboardChangeIterator::isPressed() const
{
    return (_current[_byteIndex] & _bitMask) != 0;
}

// Check if key was just pressed (transition from released to pressed)
bool KeyboardChangeIterator::wasJustPressed() const
{
    return !wasPressed() && isPressed();
}

// Check if key was just released (transition from pressed to released)
bool KeyboardChangeIterator::wasReleased() const
{
    return wasPressed() && !isPressed();
}

// Get TRS-80 key value/scan code
uint8_t KeyboardChangeIterator::keyValue() const
{
    uint8_t r = row();
    uint8_t c = column();
    if (isShiftPressed())
    {
        return pgm_read_byte(&(lookupTableShift[r][c]));
    }
    else
    {
        return pgm_read_byte(&(lookupTable[r][c]));
    }
}

// Get human-readable key name
String KeyboardChangeIterator::keyName() const
{
    uint8_t keyVal = keyValue();

    // Handle special keys
    switch (keyVal)
    {
    case KEY_ENTER:
        return "ENTER";
    case KEY_CLEAR:
        return "CLEAR";
    case KEY_BREAK:
        return "BREAK";
    case KEY_UP:
        return "UP";
    case KEY_DOWN:
        return "DOWN";
    case KEY_LEFT:
        return "LEFT";
    case KEY_RIGHT:
        return "RIGHT";
    case KEY_SPACE:
        return "SPACE";
    case KEY_SHIFT:
        return "SHIFT";

    default:
        // For regular ASCII characters
        if (keyVal >= 32 && keyVal <= 126)
        {
            return String((char)keyVal);
        }
        else
        {
            // For unknown/special keys, show hex value
            String hex = "0x";
            if (keyVal < 16)
                hex += "0";
            hex += String(keyVal, HEX);
            return hex;
        }
    }
}

// Check if shift key is currently pressed
bool KeyboardChangeIterator::isShiftPressed() const
{
    return (_current[7] & 0x01) > 0;
}

// Find and position at the next keyboard state change
void KeyboardChangeIterator::_advanceToNextChange()
{
    _found = false;
    while (_byteIndex < 8)
    {
        uint8_t prev = _previous[_byteIndex];
        uint8_t curr = _current[_byteIndex];

        uint8_t changedBits = prev ^ curr;

        while (_bitMask != 0)
        {
            if (changedBits & _bitMask)
            {
                _found = true;
                return;
            }
            _bitMask <<= 1;
        }
        _byteIndex++;
        _bitMask = 0x01;
    }
}

// Get the bit position within the current byte
uint8_t KeyboardChangeIterator::_bitIndex() const
{
    uint8_t bit = 0;
    uint8_t mask = 0x01;
    while (mask != 0)
    {
        if (_bitMask == mask)
            return bit;
        bit++;
        mask <<= 1;
    }
    return 0;
}
