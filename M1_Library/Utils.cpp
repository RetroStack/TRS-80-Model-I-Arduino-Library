// Various utilties to support print, input, high precision wait, etc.

#include "./utils.h"

// This assumes ATMega 2560, 1 cycle = 62.5ns
// The main loop is 3 cycles, excluding the mov
// Min = 250ns, Max = 12266812.5ns (12.2668125 ms)
// Example: wait = 10, then delay will be 10us
void asmWait(uint16_t wait)
{
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
      "outer_loop_start: \n\t"                     // Outer loop start label
      "movw r24, %A0 \n\t"                         // Copy outer loop count to r24:r25
      "inner_loop_start: \n\t"                     // Inner loop start label
      "movw r26, %A1 \n\t"                         // Copy inner loop count to r26:r27
      "inner_loop: \n\t"                           // Inner loop label
      "sbiw r26, 1 \n\t"                           // Subtract one from the inner loop count
      "brne inner_loop \n\t"                       // Branch to Inner loop label if zero flag is clear
      "sbiw r24, 1 \n\t"                           // Subtract one from the outer loop count
      "brne inner_loop_start \n\t"                 // Branch to Inner loop start label if zero flag is clear
      : "+w"(outerLoopCount), "+w"(innerLoopCount) // Outputs: modified in place
      : "r"(outerLoopCount), "r"(innerLoopCount)   // Inputs
      : "r24", "r25", "r26", "r27"                 // Clobbers
  );
}