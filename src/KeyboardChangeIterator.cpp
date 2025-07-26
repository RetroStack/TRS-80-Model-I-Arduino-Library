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

/**
 * Initializes the iterator with the previous and current keyboard states.
 */
KeyboardChangeIterator::KeyboardChangeIterator(const uint8_t *previous, const uint8_t *current)
    : _byteIndex(0), _bitMask(1), _found(false)
{
    memcpy(_previous, previous, 8);
    memcpy(_current, current, 8);

    advanceToNextChange();
}

/**
 * Checks whether there are more changed keys to iterate over.
 */
bool KeyboardChangeIterator::hasNext() const
{
    return _found;
}

/**
 * Moves to the next changed key.
 */
void KeyboardChangeIterator::next()
{
    _bitMask <<= 1;

    if (_bitMask == 0)
    {
        _bitMask = 0x01;
        _byteIndex++;
    }

    advanceToNextChange();
}

/**
 * Returns the flat index (0-63) of the current key.
 */
uint8_t KeyboardChangeIterator::keyIndex() const
{
    return _byteIndex * 8 + bitIndex();
}

/**
 * Returns the row of the current key.
 */
uint8_t KeyboardChangeIterator::row() const
{
    return _byteIndex;
}

/**
 * Returns the column of the current key.
 */
uint8_t KeyboardChangeIterator::column() const
{
    return bitIndex();
}

/**
 * Returns whether the key was pressed in the previous state.
 */
bool KeyboardChangeIterator::wasPressed() const
{
    return (_previous[_byteIndex] & _bitMask) != 0;
}

/**
 * Returns whether the key is pressed now.
 */
bool KeyboardChangeIterator::isPressed() const
{
    return (_current[_byteIndex] & _bitMask) != 0;
}

/**
 * True if the key went from released to pressed.
 */
bool KeyboardChangeIterator::wasJustPressed() const
{
    return !wasPressed() && isPressed();
}

/**
 * True if the key went from pressed to released.
 */
bool KeyboardChangeIterator::wasReleased() const
{
    return wasPressed() && !isPressed();
}

/**
 * Returns the bit index of the current key within its byte.
 */
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

/**
 * Checks whether the shift key is pressed.
 */
bool KeyboardChangeIterator::isShiftPressed() const
{
    return (_current[7] & 0x01) > 0;
}

/**
 * Advances to the next changed key by checking the current and previous states.
 */
void KeyboardChangeIterator::advanceToNextChange()
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

/**
 * Returns the bit index of the current key within its byte.
 */
uint8_t KeyboardChangeIterator::bitIndex() const
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
