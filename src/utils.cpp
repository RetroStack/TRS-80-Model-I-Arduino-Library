/*
 * utils.cpp - Utility functions and helpers
 * Authors: Ven Reddy, Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#include "utils.h"

// Convert an 8-bit unsigned integer to a binary string
char *uint8ToBinary(uint8_t value, char *buffer)
{
  if (!buffer)
  {
    return nullptr; // Cannot convert to null buffer
  }

  for (int i = 7; i >= 0; i--)
  {
    buffer[7 - i] = ((value >> i) & 1) ? '1' : '0';
  }
  buffer[8] = '\0'; // Null-terminate the string
  return buffer;
}

// Convert a 16-bit unsigned integer to a binary string
char *uint16ToBinary(uint16_t value, char *buffer)
{
  if (!buffer)
  {
    return nullptr; // Cannot convert to null buffer
  }

  for (int i = 15; i >= 0; i--)
  {
    buffer[15 - i] = ((value >> i) & 1) ? '1' : '0';
  }
  buffer[16] = '\0'; // Null-terminate the string
  return buffer;
}

// Convert a boolean value to a pin status character
char pinStatus(bool value)
{
  return value ? 'o' : 'i';
}

// Convert a bus status value to a character
// 'o' for output, 'i' for input, '?' for unknown state
char busStatus(uint8_t value)
{
  if (value == 0xff) // Output
  {
    return 'o';
  }
  else if (value == 0x00) // Input
  {
    return 'i';
  }
  else // Some unknown state (should not happen)
  {
    return '?';
  }
}

// Precise nanosecond delay using inline assembly for 16MHz ATMega
// 1 cycle = 62.5 ns, each loop iteration = 4 cycles except final (3 cycles)
// Total cycles = (wait - 1)*4 + 3 = 4*wait -1 cycles
// Calling overhead is 190ns
// Examples:
//   wait=1: ~252 ns total delay (3 cycles + call overhead)
//   wait=2: ~512 ns total delay (7 cycles + call overhead)
//   wait=3: ~772 ns total delay (11 cycles + call overhead)
//   wait=4: ~1032 ns total delay (15 cycles + call overhead)
//   wait=5: ~1292 ns total delay (19 cycles + call overhead)
// To get ~1 us delay, use wait=4
// To get ~2 us delay, use wait=8
// Usage: asmWait(3); // ~772 ns delay including overhead
void asmWait(uint16_t wait)
{
  if (wait == 0)
    return;
#ifdef __AVR__
  __asm__ volatile(
      " mov r16,%0\n" // set wait countdown
      "1: nop\n"      // noop
      " dec r16\n"    // decrement
      " brne 1b\n"    // 1 cycle if branching, 2 if not
      :
      : "r"(wait) // input operands if any, here
      : "r16"     // clobbered regs here
  );
#else
  (void)wait; // Host builds have no cycle-accurate delay to emulate
#endif
}


// Length of the chunk starting at `offset` within a `total`-byte range.
// Returns 0 once the range is exhausted. `offset` is 32-bit so that a caller
// stepping it by `chunkSize` cannot wrap at 65536 and restart the range.
uint16_t chunkLength(uint32_t offset, uint16_t total, uint16_t chunkSize)
{
  if (chunkSize == 0 || offset >= total)
  {
    return 0;
  }

  uint32_t remaining = (uint32_t)total - offset;
  return (remaining < chunkSize) ? (uint16_t)remaining : chunkSize;
}

// Collapse "." and ".." segments and squeeze repeated separators. The result
// always starts with "/" and never ends with one (except for root itself). A
// ".." that would climb above root is clamped there rather than escaping.
bool normalizePath(const char *path, char *out, size_t outSize)
{
  if (!path || !out || outSize < 2)
  {
    return false;
  }

  size_t len = 0;
  out[0] = '\0';

  const char *p = path;
  while (*p)
  {
    while (*p == '/')
    {
      p++;
    }
    if (!*p)
    {
      break;
    }

    const char *segment = p;
    while (*p && *p != '/')
    {
      p++;
    }
    size_t segmentLength = (size_t)(p - segment);

    if (segmentLength == 1 && segment[0] == '.')
    {
      continue;
    }

    if (segmentLength == 2 && segment[0] == '.' && segment[1] == '.')
    {
      while (len > 0 && out[len - 1] != '/')
      {
        len--;
      }
      if (len > 0)
      {
        len--; // Drop the separator that preceded the removed segment
      }
      out[len] = '\0';
      continue;
    }

    if (len + segmentLength + 2 > outSize)
    {
      return false;
    }

    out[len++] = '/';
    for (size_t i = 0; i < segmentLength; i++)
    {
      out[len++] = segment[i];
    }
    out[len] = '\0';
  }

  if (len == 0)
  {
    out[0] = '/';
    out[1] = '\0';
  }

  return true;
}

// Containment test comparing whole path components, so "/logs" does not
// contain "/logsecret". Both arguments must already be normalized.
bool pathIsWithin(const char *path, const char *root)
{
  if (!path || !root)
  {
    return false;
  }

  if (root[0] == '/' && root[1] == '\0')
  {
    return path[0] == '/'; // Root contains every absolute path
  }

  size_t rootLength = 0;
  while (root[rootLength])
  {
    rootLength++;
  }
  while (rootLength > 1 && root[rootLength - 1] == '/')
  {
    rootLength--;
  }

  for (size_t i = 0; i < rootLength; i++)
  {
    if (path[i] != root[i])
    {
      return false;
    }
  }

  // The match must land on a component boundary, not mid-name.
  return path[rootLength] == '\0' || path[rootLength] == '/';
}

// Copy a flash string into RAM
FlashBuffer::FlashBuffer(const __FlashStringHelper *text) : _buffer(nullptr)
{
  if (text == nullptr)
  {
    return;
  }

  size_t length = strlen_P((const char *)text);
  _buffer = (char *)malloc(length + 1);
  if (_buffer != nullptr)
  {
    strcpy_P(_buffer, (const char *)text);
  }
}

FlashBuffer::~FlashBuffer()
{
  if (_buffer != nullptr)
  {
    free(_buffer);
  }
}
