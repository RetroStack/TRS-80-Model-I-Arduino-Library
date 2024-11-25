// Various utilties to support print, input, high precision wait, etc.

#include "utils.h"

/**
 * Converts an 8-bit value to a string
 */
char *uint8ToBinary(uint8_t value, char *buffer)
{
  for (int i = 7; i >= 0; i--)
  {
    buffer[7 - i] = ((value >> i) & 1) ? '1' : '0';
  }
  buffer[8] = '\0'; // Null-terminate the string
  return buffer;
}

/**
 * Returns the pin status
 */
char pinStatus(bool value)
{
  return value ? 'o' : 'i';
}
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

/**
 * Wait two noop cycles
 */
void asmNoop()
{
  __asm__ volatile("nop\nnop");
}

// This assumes ATMega 2560, 1 cycle = 62.5ns
// The main loop is 3 cycles, excluding the mov
// Min = 250ns, Max = 12266812.5ns (12.2668125 ms)
// Example: wait = 1, then delay will be 250ns
void asmWait(uint16_t wait)
{
  if (wait == 0)
    return;
  __asm__ volatile(
      " mov r16,%0\n"
      "1: nop\n"
      " dec r16\n"
      " brne 1b\n" // 1 cycle if branching, 2 if not
      :
      : "r"(wait) // input operands if any, here
      : "r16"     // clobbered regs here
  );
}

void asmWait(uint16_t outerLoopCount, uint16_t innerLoopCount)
{
  asm volatile(
      "outer_loop_start: \n\t"                   // Outer loop start label
      "movw r24, %A0 \n\t"                       // Copy outer loop count to r24:r25
      "inner_loop_start: \n\t"                   // Inner loop start label
      "movw r26, %A1 \n\t"                       // Copy inner loop count to r26:r27
      "inner_loop: \n\t"                         // Inner loop label
      "sbiw r26, 1 \n\t"                         // Subtract one from the inner loop count
      "brne inner_loop \n\t"                     // Branch to Inner loop label if zero flag is clear
      "sbiw r24, 1 \n\t"                         // Subtract one from the outer loop count
      "brne inner_loop_start \n\t"               // Branch to Inner loop start label if zero flag is clear
      :                                          //"+w"(outerLoopCount), "+w"(innerLoopCount) // Outputs: modified in place
      : "r"(outerLoopCount), "r"(innerLoopCount) // Inputs
      : "r24", "r25", "r26", "r27"               // Clobbers
  );
}