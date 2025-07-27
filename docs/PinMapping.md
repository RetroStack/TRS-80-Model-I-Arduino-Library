# TRS-80 Model I Arduino Pin Mapping

This document provides the complete pin mapping between the TRS-80 Model I 40-pin edge connector and the Arduino Mega 2560. This mapping is essential for users who want to connect their Arduino directly to the TRS-80 without using the dedicated M1Shield.

## Table of Contents

1. [Overview](#overview)
2. [Safety Warnings](#safety-warnings)
3. [Pin Mapping Table](#pin-mapping-table)
4. [Connection Diagram](#connection-diagram)
5. [Verification Procedure](#verification-procedure)
6. [Troubleshooting](#troubleshooting)
7. [M1Shield Alternative](#m1shield-alternative)

## Overview

The TRS-80 Model I uses a 40-pin edge connector to interface with external devices. This library maps these signals to specific pins on the Arduino Mega 2560, allowing direct control of the TRS-80's address bus, data bus, and control signals.

**Important:** This mapping is designed specifically for the Arduino Mega 2560. Other Arduino boards do not have sufficient GPIO pins for this application.

## Safety Warnings

⚠️ **CRITICAL SAFETY INFORMATION:**

1. **Power Sequence:** Always power on the TRS-80 Model I first and let it fully boot before connecting or powering the Arduino
2. **Signal Levels:** Both systems operate at 5V logic levels, but ensure proper ground connections
3. **Double-Check Wiring:** Incorrect connections can damage both the TRS-80 and Arduino
4. **Use the M1Shield:** For beginners, the dedicated M1Shield is much safer and easier to use

## Pin Mapping Table

### Address Bus (16-bit)

| TRS-80 Edge Pin | Signal | Arduino Mega Pin | Arduino Port |
| --------------- | ------ | ---------------- | ------------ |
| A01             | A0     | 22               | PA0          |
| A02             | A1     | 23               | PA1          |
| A03             | A2     | 24               | PA2          |
| A04             | A3     | 25               | PA3          |
| A05             | A4     | 26               | PA4          |
| A06             | A5     | 27               | PA5          |
| A07             | A6     | 28               | PA6          |
| A08             | A7     | 29               | PA7          |
| A09             | A8     | 37               | PC0          |
| A10             | A9     | 36               | PC1          |
| A11             | A10    | 35               | PC2          |
| A12             | A11    | 34               | PC3          |
| A13             | A12    | 33               | PC4          |
| A14             | A13    | 32               | PC5          |
| A15             | A14    | 31               | PC6          |
| A16             | A15    | 30               | PC7          |

### Data Bus (8-bit)

| TRS-80 Edge Pin | Signal | Arduino Mega Pin | Arduino Port |
| --------------- | ------ | ---------------- | ------------ |
| B01             | D0     | 39               | PG2          |
| B02             | D1     | 40               | PG1          |
| B03             | D2     | 41               | PG0          |
| B04             | D3     | 42               | PL7          |
| B05             | D4     | 43               | PL6          |
| B06             | D5     | 44               | PL5          |
| B07             | D6     | 45               | PL4          |
| B08             | D7     | 46               | PL3          |

### Control Signals

| TRS-80 Edge Pin | Signal | Arduino Mega Pin | Arduino Port | Description                 |
| --------------- | ------ | ---------------- | ------------ | --------------------------- |
| A17             | /MREQ  | 47               | PL2          | Memory Request (active low) |
| A18             | /IORQ  | 48               | PL1          | I/O Request (active low)    |
| A19             | /RD    | 49               | PL0          | Read (active low)           |
| A20             | /WR    | 50               | PB3          | Write (active low)          |
| B09             | /WAIT  | 51               | PB2          | Wait State (active low)     |
| B10             | /INT   | 52               | PB1          | Interrupt (active low)      |
| B11             | /TEST  | 53               | PB0          | Test Signal (active low)    |
| B12             | /RESET | 2                | PE4          | Reset (active low)          |

### Clock and Special Signals

| TRS-80 Edge Pin | Signal | Arduino Mega Pin | Arduino Port | Description                  |
| --------------- | ------ | ---------------- | ------------ | ---------------------------- |
| B13             | CLK    | 3                | PE5          | System Clock                 |
| B14             | /RFSH  | 4                | PG5          | DRAM Refresh (active low)    |
| B15             | /HALT  | 5                | PE3          | Halt State (active low)      |
| B16             | /BUSAK | 6                | PH3          | Bus Acknowledge (active low) |
| B17             | /BUSRQ | 7                | PH4          | Bus Request (active low)     |

### Power and Ground

| TRS-80 Edge Pin    | Signal | Arduino Connection | Notes              |
| ------------------ | ------ | ------------------ | ------------------ |
| A39, A40           | +5V    | 5V                 | Power supply       |
| A38, B38, B39, B40 | GND    | GND                | Ground connections |

## Connection Diagram

```
TRS-80 Model I Edge Connector (40-pin)
=====================================

      A Side (Component Side)           B Side (Solder Side)
   +5V  A40 ●●●●●●●●●●●●●●●●●●●● A01  A0     +5V  B40 ●●●●●●●●●●●●●●●●●●●● B01  D0
   +5V  A39 ●●●●●●●●●●●●●●●●●●●● A02  A1     GND  B39 ●●●●●●●●●●●●●●●●●●●● B02  D1
   GND  A38 ●●●●●●●●●●●●●●●●●●●● A03  A2     GND  B38 ●●●●●●●●●●●●●●●●●●●● B03  D2
        ...                     ...              ...                     ...
        A20 ●●●●●●●●●●●●●●●●●●●● A17 /MREQ      B12 ●●●●●●●●●●●●●●●●●●●● B09 /WAIT
```

**Pin Numbering:**

- A01-A40: Component side (top)
- B01-B40: Solder side (bottom)
- Numbers increase from edge connector fingers toward the board

## Verification Procedure

Before connecting your TRS-80 to the Arduino, follow this verification procedure:

### 1. Visual Inspection

```arduino
// Test sketch to verify pin assignments
void setup() {
  Serial.begin(115200);
  Serial.println("TRS-80 Arduino Pin Mapping Test");

  // Configure all pins as inputs with pullups for safety
  for (int pin = 2; pin <= 53; pin++) {
    pinMode(pin, INPUT_PULLUP);
  }

  Serial.println("All pins configured as inputs with pullups");
  Serial.println("Safe to connect TRS-80 edge connector");
}

void loop() {
  // Read and display some key signals
  Serial.print("A0: "); Serial.print(digitalRead(22));
  Serial.print(" D0: "); Serial.print(digitalRead(39));
  Serial.print(" /MREQ: "); Serial.print(digitalRead(47));
  Serial.print(" /IORQ: "); Serial.print(digitalRead(48));
  Serial.println();

  delay(1000);
}
```

### 2. Continuity Testing

1. Power off both systems
2. Use a multimeter to verify connections between edge connector and Arduino pins
3. Check for short circuits between adjacent pins
4. Verify ground connections

### 3. Signal Testing

1. Power on TRS-80 first
2. Let it boot completely
3. Connect Arduino (powered separately)
4. Run the test sketch above
5. Verify signals change when TRS-80 operates

## Troubleshooting

### Common Issues

**No Communication:**

- Verify ground connections first
- Check that TRS-80 is powered and booted
- Ensure correct pin mapping
- Test with multimeter for signal presence

**Erratic Behavior:**

- Check for loose connections
- Verify power supply stability
- Ensure proper signal timing
- Consider signal integrity issues on long wires

**System Crashes:**

- Never assert /TEST signal during TRS-80 boot
- Use proper power sequencing
- Check for short circuits

### Signal Quality Tips

1. **Use short wires** (< 6 inches) to minimize noise
2. **Twist address/data pairs** to reduce crosstalk
3. **Keep power and ground wires thick** for stable supply
4. **Use bypass capacitors** near connection points
5. **Consider level shifters** for critical signals

## M1Shield Alternative

**Strongly Recommended:** Instead of direct wiring, consider using the dedicated TRS-80 Model I Arduino Shield:

### Advantages of M1Shield:

- **Pre-verified connections** - No wiring errors
- **Built-in protection** - Overcurrent and ESD protection
- **Integrated display** - ST7789/ST7735/ILI9341 support
- **User interface** - Buttons, joystick, LED
- **Professional reliability** - PCB quality connections
- **Easier debugging** - Known-good hardware

### When to Use Direct Wiring:

- Custom applications requiring modified pin assignments
- Educational purposes to understand the interface
- Prototyping new shield designs
- Budget constraints (though reliability costs more long-term)

## Example Connection Code

Here's how the library uses these pins:

```arduino
#include "Model1.h"

void setup() {
  Serial.begin(115200);

  // Initialize the Model1 interface
  // This configures all pins according to the mapping above
  Model1.begin();

  // Take control of the TRS-80 bus
  Model1.activateTestSignal();

  // Read from address 0x0000 (ROM)
  uint8_t data = Model1.readByte(0x0000);
  Serial.print("ROM byte 0: 0x");
  Serial.println(data, HEX);

  // Release control
  Model1.deactivateTestSignal();
}

void loop() {
  // Main application code
}
```

## Pin Assignment Rationale

The pin assignments were chosen based on:

1. **Port Grouping:** Address and data buses use complete ports for efficient bit manipulation
2. **Signal Integrity:** Critical signals avoid pins with special functions (PWM, interrupts)
3. **Arduino Constraints:** Avoid pins 0-1 (Serial), 13 (LED), and other special functions
4. **Performance:** Group related signals on same ports for atomic operations
5. **Future Expansion:** Reserve some pins for additional features

This mapping provides reliable operation while maintaining compatibility with standard Arduino libraries and development practices.
