/*
 * utils.cpp - Utility functions and helpers
 * Authors: Ven Reddy, Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#include "utils.h"

char *uint8ToBinary(uint8_t value, char *buffer)
{
  for (int i = 7; i >= 0; i--)
  {
    buffer[7 - i] = ((value >> i) & 1) ? '1' : '0';
  }
  buffer[8] = '\0'; // Null-terminate the string
  return buffer;
}

char *uint16ToBinary(uint16_t value, char *buffer)
{
  for (int i = 15; i >= 0; i--)
  {
    buffer[15 - i] = ((value >> i) & 1) ? '1' : '0';
  }
  buffer[16] = '\0'; // Null-terminate the string
  return buffer;
}

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
  __asm__ volatile(
      " mov r16,%0\n" // set wait countdown
      "1: nop\n"      // noop
      " dec r16\n"    // decrement
      " brne 1b\n"    // 1 cycle if branching, 2 if not
      :
      : "r"(wait) // input operands if any, here
      : "r16"     // clobbered regs here
  );
}

/**
 * Busy-wait delay loop using nested counters.
 *
 * Timing details (ATmega2560, 16 MHz CPU clock):
 *
 *  - Each inner loop iteration:
 *      - 4 cycles per iteration (2 cycles for sbiw + 2 cycles for brne when branching)
 *      - except the final iteration: 3 cycles (sbiw + brne fallthrough)
 *
 *  - Each outer loop iteration executes the inner loop fully, plus:
 *      - 4 cycles for outer sbiw/brne when branching
 *      - or 3 cycles when exiting after the final iteration
 *
 *  - Therefore:
 *
 *      innerLoopCycles = (innerCount - 1) * 4 + 3
 *
 *      totalCycles =
 *          (outerCount - 1) * (innerLoopCycles + 4)
 *          + (innerLoopCycles + 3)
 *
 *  - 1 cycle = 62.5 ns
 *  - Function call overhead: ~375 ns (before loop starts)
 *
 *  - Example delays (including call overhead):
 *
 *      outer=1, inner=1:
 *          innerLoopCycles = 3
 *          totalCycles = 3 + 3 = 6 cycles
 *          delay = (6 * 62.5 ns) + 375 ns = 750 ns
 *
 *      outer=1, inner=2:
 *          innerLoopCycles = 7
 *          totalCycles = 7 + 3 = 10 cycles
 *          delay = (10 * 62.5 ns) + 375 ns = 1.0 us
 *
 *      outer=1, inner=3:
 *          innerLoopCycles = 11
 *          totalCycles = 11 + 3 = 14 cycles
 *          delay = (14 * 62.5 ns) + 375 ns = 1.25 us
 *
 *      outer=1, inner=4:
 *          innerLoopCycles = 15
 *          totalCycles = 15 + 3 = 18 cycles
 *          delay = (18 * 62.5 ns) + 375 ns = 1.5 us
 *
 *      outer=2, inner=1:
 *          innerLoopCycles = 3
 *          totalCycles = (1)*(3+4) + (3+3) = (7) + (6) =13 cycles
 *          delay = (13 * 62.5 ns) + 375 ns =1.1875 us
 *
 *      outer=3, inner=1:
 *          innerLoopCycles = 3
 *          totalCycles = (2)*(3+4) + (3+3) = (2*7) +6 =14+6=20 cycles
 *          delay = (20 *62.5 ns) +375 ns =1.625 us
 *
 *      outer=10, inner=10:
 *          innerLoopCycles = (10-1)*4 +3 =39
 *          totalCycles = (9)*(39+4) + (39+3) = (9*43) +42 =387+42=429 cycles
 *          delay = (429*62.5 ns)+375 ns =26.8 us +375 ns ~27.2 us
 *
 * Use this function for precise longer delays.
 */
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
