# KeyboardChangeIterator

Walks the differences between two snapshots of the TRS-80 keyboard matrix, one
changed key at a time. `Keyboard::changes()` hands you one; you rarely
construct it yourself.

The matrix is 8 rows of 8 columns, held as eight bytes. Comparing two snapshots
byte by byte would mean 64 tests and a lot of bookkeeping in the caller; this
does that walk once and reports only the keys that actually moved.

## Table of Contents

- [Iterating](#iterating)
- [Identifying the Key](#identifying-the-key)
- [What Changed](#what-changed)
- [Notes](#notes)
- [See Also](#see-also)

## Iterating

```cpp
bool hasNext() const; // Another change is available
void next();          // Advance to it
```

`hasNext()` must be true before any accessor below is read. The iterator starts
positioned before the first change, so the loop shape is:

```cpp
auto it = Keyboard.changes();
while (it.hasNext()) {
    it.next();
    // ... read the accessors here
}
```

## Identifying the Key

```cpp
uint8_t keyIndex() const; // Linear index, 0-63
uint8_t row() const;      // Matrix row, 0-7
uint8_t column() const;   // Matrix column, 0-7
uint8_t keyValue() const; // TRS-80 scan code
String keyName() const;   // Human-readable name, e.g. "ENTER", "BREAK"
```

`keyName()` returns the name printed on the key, which is what you want for
logging. `keyValue()` returns the scan code the machine itself uses.

## What Changed

```cpp
bool wasPressed() const;     // Down in the previous snapshot
bool isPressed() const;      // Down in the current snapshot
bool wasJustPressed() const; // Released -> pressed
bool wasReleased() const;    // Pressed -> released
bool isShiftPressed() const; // Shift is down right now
```

`wasJustPressed()` and `wasReleased()` are the two you normally want; the other
two are the raw states they are derived from.

`isShiftPressed()` reports the current state of shift regardless of which key
the iterator is sitting on, so a shifted keypress can be recognised without
tracking shift separately.

## Notes

- **Read only while `hasNext()` is true.** The accessors index the snapshot
  arrays by the iterator's current position; past the end that position is no
  longer inside them.
- The snapshots are copied into the iterator on construction, so the caller's
  buffers may be reused immediately.
- Both snapshots are 8 bytes; the iterator adds a few bytes of position state.

## See Also

- [Keyboard](Keyboard.md) - produces the snapshots and hands out the iterator
- [Model1](Model1.md) - the bus access underneath
