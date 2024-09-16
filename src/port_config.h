/*
 * port_config.h - File to configure all ports and pins
 * Authors: Ven Reddy, Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef PORT_CONFIG_H
#define PORT_CONFIG_H

#include <Arduino.h>

// Low- and high-byte port for Address Bus
#define BUS_ADDR_LOW_PORT A
#define BUS_ADDR_HIGH_PORT C

// Byte port for Data Bus
#define BUS_DATA_PORT F

// Port L, Bit(0) 4, Pin 45
const uint8_t PIN_SYS_RES_ON = 0b00000001 << 4;
const uint8_t PIN_SYS_RES_OFF = ~(0b00000001 << 4);
#define PIN_SYS_RES_PORT L

// Port L, Bit(0) 5, Pin 44
const uint8_t PIN_INT_ACK_ON = 0b00000001 << 5;
const uint8_t PIN_INT_ACK_OFF = ~(0b00000001 << 5);
#define PIN_INT_ACK_PORT L

// Port E, Bit(0) 4, Pin 2
const uint8_t PIN_RAS_ON = 0b00000001 << 4;
const uint8_t PIN_RAS_OFF = ~(0b00000001 << 4);
#define PIN_RAS_PORT E

// Port E, Bit(0) 5, Pin 3
const uint8_t PIN_CAS_ON = 0b00000001 << 5;
const uint8_t PIN_CAS_OFF = ~(0b00000001 << 5);
#define PIN_CAS_PORT E

// Port L, Bit(0) 0, Pin 49
const uint8_t PIN_MUX_ON = 0b00000001 << 0;
const uint8_t PIN_MUX_OFF = ~(0b00000001 << 0);
#define PIN_MUX_PORT L

// Port D, Bit(0) 2, Pin 19
const uint8_t PIN_RD_ON = 0b00000001 << 2;
const uint8_t PIN_RD_OFF = ~(0b00000001 << 2);
#define PIN_RD_PORT D
#define PIN_RD 19 // for Interrupt

// Port G, Bit(0) 1, Pin 40
const uint8_t PIN_WR_ON = 0b00000001 << 1;
const uint8_t PIN_WR_OFF = ~(0b00000001 << 1);
#define PIN_WR_PORT G
#define PIN_WR 40 // for Interrupt

// Port D, Bit(0) 3, Pin 18
const uint8_t PIN_IN_ON = 0b00000001 << 3;
const uint8_t PIN_IN_OFF = ~(0b00000001 << 3);
#define PIN_IN_PORT D
#define PIN_IN 18 // for Interrupt

// Port D, Bit(0) 7, Pin 38
const uint8_t PIN_OUT_ON = 0b00000001 << 7;
const uint8_t PIN_OUT_OFF = ~(0b00000001 << 7);
#define PIN_OUT_PORT D
#define PIN_OUT 38 // for Interrupt

// Port L, Bit(0) 1, Pin 48
const uint8_t PIN_INT_ON = 0b00000001 << 1;
const uint8_t PIN_INT_OFF = ~(0b00000001 << 1);
#define PIN_INT_PORT L

// Port L, Bit(0) 2, Pin 47
const uint8_t PIN_TEST_ON = 0b00000001 << 2;
const uint8_t PIN_TEST_OFF = ~(0b00000001 << 2);
#define PIN_TEST_PORT L

// Port L, Bit(0) 3, Pin 46
const uint8_t PIN_WAIT_ON = 0b00000001 << 3;
const uint8_t PIN_WAIT_OFF = ~(0b00000001 << 3);
#define PIN_WAIT_PORT L

#endif // PORT_CONFIG_H