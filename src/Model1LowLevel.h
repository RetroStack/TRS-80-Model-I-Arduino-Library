/*
 * Model1LowLevel.h - Class for accessing the TRS-80 Model 1 low-level signals
 * Authors: Ven Reddy, Marcel Erz (RetroStack)
 * Released under the MIT License.
 * 
 * This class provides direct, low-level access to all TRS-80 Model 1 hardware control signals
 * and buses, bypassing the safety mechanisms of the main Model1 class. Use with caution.
 * 
 * Features:
 * - Direct pin configuration (INPUT/OUTPUT mode setting)
 * - Pin configuration reading (check current INPUT/OUTPUT state)
 * - Signal control (HIGH/LOW state setting)
 * - Signal reading (current HIGH/LOW state)
 * - Complete address and data bus control
 * - Static inline functions for maximum performance (zero call overhead)
 * 
 * Control Signals Available:
 * - RAS (Row Address Strobe)
 * - CAS (Column Address Strobe)  
 * - MUX (Address Multiplexer)
 * - RD (Read signal)
 * - WR (Write signal)
 * - IN (Input signal)
 * - OUT (Output signal)
 * - INT (Interrupt signal)
 * - TEST (Test signal)
 * - WAIT (Wait signal)
 * - SYS_RES (System Reset)
 * - INT_ACK (Interrupt Acknowledge)
 * 
 * Bus Control:
 * - 16-bit Address Bus (ADDR_LOW, ADDR_HIGH)
 * - 8-bit Data Bus (DATA)
 * 
 * Usage Example:
 *   // Configure RAS pin as output and set it high
 *   Model1LowLevel::configRAS(OUTPUT);
 *   Model1LowLevel::setRAS(HIGH);
 *   
 *   // Read current state
 *   uint8_t rasState = Model1LowLevel::readRAS();
 *   uint8_t rasConfig = Model1LowLevel::configReadRAS();
 *   
 *   // Bus operations
 *   Model1LowLevel::setAddressBus(0x1234);
 *   Model1LowLevel::setDataBus(0xFF);
 * 
 * WARNING: This class bypasses all safety checks and can potentially damage
 * hardware if used incorrectly. Only use if you understand the TRS-80 Model 1
 * hardware architecture and timing requirements.
 */

#ifndef MODEL1_LOWLEVEL_H
#define MODEL1_LOWLEVEL_H

#include <Arduino.h>
#include "port_config.h"
#include "port_macros.h"

class Model1LowLevel
{
public:
    // ========== Control Signal Configuration Functions ==========
    // Configure individual pins as INPUT or OUTPUT
    // Parameters: outputMode - Use OUTPUT or INPUT constants
    
    /** Configure RAS (Row Address Strobe) pin direction */
    static inline void configRAS(uint8_t outputMode)
    {
        if (outputMode == OUTPUT)
            pinConfigWrite(RAS, OUTPUT);
        else
            pinConfigWrite(RAS, INPUT);
    }

    static inline void configCAS(uint8_t outputMode)
    {
        if (outputMode == OUTPUT)
            pinConfigWrite(CAS, OUTPUT);
        else
            pinConfigWrite(CAS, INPUT);
    }

    static inline void configMUX(uint8_t outputMode)
    {
        if (outputMode == OUTPUT)
            pinConfigWrite(MUX, OUTPUT);
        else
            pinConfigWrite(MUX, INPUT);
    }

    static inline void configRD(uint8_t outputMode)
    {
        if (outputMode == OUTPUT)
            pinConfigWrite(RD, OUTPUT);
        else
            pinConfigWrite(RD, INPUT);
    }

    static inline void configWR(uint8_t outputMode)
    {
        if (outputMode == OUTPUT)
            pinConfigWrite(WR, OUTPUT);
        else
            pinConfigWrite(WR, INPUT);
    }

    static inline void configIN(uint8_t outputMode)
    {
        if (outputMode == OUTPUT)
            pinConfigWrite(IN, OUTPUT);
        else
            pinConfigWrite(IN, INPUT);
    }

    static inline void configOUT(uint8_t outputMode)
    {
        if (outputMode == OUTPUT)
            pinConfigWrite(OUT, OUTPUT);
        else
            pinConfigWrite(OUT, INPUT);
    }

    static inline void configINT(uint8_t outputMode)
    {
        if (outputMode == OUTPUT)
            pinConfigWrite(INT, OUTPUT);
        else
            pinConfigWrite(INT, INPUT);
    }

    static inline void configTEST(uint8_t outputMode)
    {
        if (outputMode == OUTPUT)
            pinConfigWrite(TEST, OUTPUT);
        else
            pinConfigWrite(TEST, INPUT);
    }

    static inline void configWAIT(uint8_t outputMode)
    {
        if (outputMode == OUTPUT)
            pinConfigWrite(WAIT, OUTPUT);
        else
            pinConfigWrite(WAIT, INPUT);
    }

    static inline void configSYS_RES(uint8_t outputMode)
    {
        if (outputMode == OUTPUT)
            pinConfigWrite(SYS_RES, OUTPUT);
        else
            pinConfigWrite(SYS_RES, INPUT);
    }

    static inline void configINT_ACK(uint8_t outputMode)
    {
        if (outputMode == OUTPUT)
            pinConfigWrite(INT_ACK, OUTPUT);
        else
            pinConfigWrite(INT_ACK, INPUT);
    }

    // ========== Control Signal Setters ==========
    // Set individual pins to HIGH or LOW state
    // Parameters: value - Use HIGH or LOW constants
    // Note: Pin must be configured as OUTPUT before setting
    
    /** Set RAS (Row Address Strobe) signal state */
    static inline void setRAS(uint8_t value)
    {
        if (value == HIGH)
            pinWrite(RAS, HIGH);
        else
            pinWrite(RAS, LOW);
    }

    static inline void setCAS(uint8_t value)
    {
        if (value == HIGH)
            pinWrite(CAS, HIGH);
        else
            pinWrite(CAS, LOW);
    }

    static inline void setMUX(uint8_t value)
    {
        if (value == HIGH)
            pinWrite(MUX, HIGH);
        else
            pinWrite(MUX, LOW);
    }

    static inline void setRD(uint8_t value)
    {
        if (value == HIGH)
            pinWrite(RD, HIGH);
        else
            pinWrite(RD, LOW);
    }

    static inline void setWR(uint8_t value)
    {
        if (value == HIGH)
            pinWrite(WR, HIGH);
        else
            pinWrite(WR, LOW);
    }

    static inline void setIN(uint8_t value)
    {
        if (value == HIGH)
            pinWrite(IN, HIGH);
        else
            pinWrite(IN, LOW);
    }

    static inline void setOUT(uint8_t value)
    {
        if (value == HIGH)
            pinWrite(OUT, HIGH);
        else
            pinWrite(OUT, LOW);
    }

    static inline void setINT(uint8_t value)
    {
        if (value == HIGH)
            pinWrite(INT, HIGH);
        else
            pinWrite(INT, LOW);
    }

    static inline void setTEST(uint8_t value)
    {
        if (value == HIGH)
            pinWrite(TEST, HIGH);
        else
            pinWrite(TEST, LOW);
    }

    static inline void setWAIT(uint8_t value)
    {
        if (value == HIGH)
            pinWrite(WAIT, HIGH);
        else
            pinWrite(WAIT, LOW);
    }

    static inline void setSYS_RES(uint8_t value)
    {
        if (value == HIGH)
            pinWrite(SYS_RES, HIGH);
        else
            pinWrite(SYS_RES, LOW);
    }

    static inline void setINT_ACK(uint8_t value)
    {
        if (value == HIGH)
            pinWrite(INT_ACK, HIGH);
        else
            pinWrite(INT_ACK, LOW);
    }

    // ========== Control Signal Configuration Readers ==========
    // Read current pin configuration (INPUT/OUTPUT state)
    // Returns: INPUT (0) or OUTPUT (1)
    
    /** Read RAS pin configuration state */
    static inline uint8_t configReadRAS() { return pinConfigRead(RAS); }
    static inline uint8_t configReadCAS() { return pinConfigRead(CAS); }
    static inline uint8_t configReadMUX() { return pinConfigRead(MUX); }
    static inline uint8_t configReadRD() { return pinConfigRead(RD); }
    static inline uint8_t configReadWR() { return pinConfigRead(WR); }
    static inline uint8_t configReadIN() { return pinConfigRead(IN); }
    static inline uint8_t configReadOUT() { return pinConfigRead(OUT); }
    static inline uint8_t configReadINT() { return pinConfigRead(INT); }
    static inline uint8_t configReadTEST() { return pinConfigRead(TEST); }
    static inline uint8_t configReadWAIT() { return pinConfigRead(WAIT); }
    static inline uint8_t configReadSYS_RES() { return pinConfigRead(SYS_RES); }
    static inline uint8_t configReadINT_ACK() { return pinConfigRead(INT_ACK); }

    // ========== Control Signal Readers ==========
    // Read current pin signal state (HIGH/LOW)
    // Returns: HIGH (1) or LOW (0)
    
    /** Read RAS signal current state */
    static inline uint8_t readRAS() { return pinRead(RAS); }
    static inline uint8_t readCAS() { return pinRead(CAS); }
    static inline uint8_t readMUX() { return pinRead(MUX); }
    static inline uint8_t readRD() { return pinRead(RD); }
    static inline uint8_t readWR() { return pinRead(WR); }
    static inline uint8_t readIN() { return pinRead(IN); }
    static inline uint8_t readOUT() { return pinRead(OUT); }
    static inline uint8_t readINT() { return pinRead(INT); }
    static inline uint8_t readTEST() { return pinRead(TEST); }
    static inline uint8_t readWAIT() { return pinRead(WAIT); }
    static inline uint8_t readSYS_RES() { return pinRead(SYS_RES); }
    static inline uint8_t readINT_ACK() { return pinRead(INT_ACK); }

    // ========== Bus Control Functions ==========
    // Control 16-bit address bus and 8-bit data bus
    // Address bus: pins 22-37 (ADDR_LOW: 22-29, ADDR_HIGH: 30-37)
    // Data bus: pins 14-21
    
    /** Set complete 16-bit address on address bus */
    static inline void setAddressBus(uint16_t address)
    {
        busWrite(ADDR_LOW, address & 0xff);
        busWrite(ADDR_HIGH, (address & 0xff00) >> 8);
    }

    static inline uint16_t readAddressBus()
    {
        return (busRead(ADDR_HIGH) << 8) | busRead(ADDR_LOW);
    }

    static inline void configAddressBus(uint16_t outputMode)
    {
        busConfigWrite(ADDR_LOW, outputMode & 0xff);
        busConfigWrite(ADDR_HIGH, (outputMode >> 8) & 0xff);
    }

    static inline void setDataBus(uint8_t data)
    {
        busWrite(DATA, data);
    }

    static inline uint8_t readDataBus()
    {
        return busRead(DATA);
    }

    static inline void configDataBus(uint8_t outputMode)
    {
        busConfigWrite(DATA, outputMode);
    }

    static inline uint16_t configReadAddressBus()
    {
        return (busConfigRead(ADDR_HIGH) << 8) | busConfigRead(ADDR_LOW);
    }

    static inline uint8_t configReadDataBus()
    {
        return busConfigRead(DATA);
    }
};

#endif /* MODEL1_LOWLEVEL_H */
