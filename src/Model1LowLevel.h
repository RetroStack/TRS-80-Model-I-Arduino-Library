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
 *   Model1LowLevel::configWriteRAS(OUTPUT);
 *   Model1LowLevel::writeRAS(HIGH);
 *   
 *   // Read current state
 *   uint8_t rasState = Model1LowLevel::readRAS();
 *   uint8_t rasConfig = Model1LowLevel::configReadRAS();
 *   
 *   // Bus operations
 *   Model1LowLevel::writeAddressBus(0x1234);
 *   Model1LowLevel::writeDataBus(0xFF);
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
    static inline void configWriteRAS(uint8_t outputMode)
    {
        if (outputMode == OUTPUT)
            pinConfigWrite(RAS, OUTPUT);
        else
            pinConfigWrite(RAS, INPUT);
    }

    static inline void configWriteCAS(uint8_t outputMode)
    {
        if (outputMode == OUTPUT)
            pinConfigWrite(CAS, OUTPUT);
        else
            pinConfigWrite(CAS, INPUT);
    }

    static inline void configWriteMUX(uint8_t outputMode)
    {
        if (outputMode == OUTPUT)
            pinConfigWrite(MUX, OUTPUT);
        else
            pinConfigWrite(MUX, INPUT);
    }

    static inline void configWriteRD(uint8_t outputMode)
    {
        if (outputMode == OUTPUT)
            pinConfigWrite(RD, OUTPUT);
        else
            pinConfigWrite(RD, INPUT);
    }

    static inline void configWriteWR(uint8_t outputMode)
    {
        if (outputMode == OUTPUT)
            pinConfigWrite(WR, OUTPUT);
        else
            pinConfigWrite(WR, INPUT);
    }

    static inline void configWriteIN(uint8_t outputMode)
    {
        if (outputMode == OUTPUT)
            pinConfigWrite(IN, OUTPUT);
        else
            pinConfigWrite(IN, INPUT);
    }

    static inline void configWriteOUT(uint8_t outputMode)
    {
        if (outputMode == OUTPUT)
            pinConfigWrite(OUT, OUTPUT);
        else
            pinConfigWrite(OUT, INPUT);
    }

    static inline void configWriteINT(uint8_t outputMode)
    {
        if (outputMode == OUTPUT)
            pinConfigWrite(INT, OUTPUT);
        else
            pinConfigWrite(INT, INPUT);
    }

    static inline void configWriteTEST(uint8_t outputMode)
    {
        if (outputMode == OUTPUT)
            pinConfigWrite(TEST, OUTPUT);
        else
            pinConfigWrite(TEST, INPUT);
    }

    static inline void configWriteWAIT(uint8_t outputMode)
    {
        if (outputMode == OUTPUT)
            pinConfigWrite(WAIT, OUTPUT);
        else
            pinConfigWrite(WAIT, INPUT);
    }

    static inline void configWriteSYS_RES(uint8_t outputMode)
    {
        if (outputMode == OUTPUT)
            pinConfigWrite(SYS_RES, OUTPUT);
        else
            pinConfigWrite(SYS_RES, INPUT);
    }

    static inline void configWriteINT_ACK(uint8_t outputMode)
    {
        if (outputMode == OUTPUT)
            pinConfigWrite(INT_ACK, OUTPUT);
        else
            pinConfigWrite(INT_ACK, INPUT);
    }

    // ========== Control Signal Writers ==========
    // Set individual pins to HIGH or LOW state
    // Parameters: value - Use HIGH or LOW constants
    // Note: Pin must be configured as OUTPUT before setting
    
    /** Write RAS (Row Address Strobe) signal state */
    static inline void writeRAS(uint8_t value)
    {
        if (value == HIGH)
            pinWrite(RAS, HIGH);
        else
            pinWrite(RAS, LOW);
    }

    static inline void writeCAS(uint8_t value)
    {
        if (value == HIGH)
            pinWrite(CAS, HIGH);
        else
            pinWrite(CAS, LOW);
    }

    static inline void writeMUX(uint8_t value)
    {
        if (value == HIGH)
            pinWrite(MUX, HIGH);
        else
            pinWrite(MUX, LOW);
    }

    static inline void writeRD(uint8_t value)
    {
        if (value == HIGH)
            pinWrite(RD, HIGH);
        else
            pinWrite(RD, LOW);
    }

    static inline void writeWR(uint8_t value)
    {
        if (value == HIGH)
            pinWrite(WR, HIGH);
        else
            pinWrite(WR, LOW);
    }

    static inline void writeIN(uint8_t value)
    {
        if (value == HIGH)
            pinWrite(IN, HIGH);
        else
            pinWrite(IN, LOW);
    }

    static inline void writeOUT(uint8_t value)
    {
        if (value == HIGH)
            pinWrite(OUT, HIGH);
        else
            pinWrite(OUT, LOW);
    }

    static inline void writeINT(uint8_t value)
    {
        if (value == HIGH)
            pinWrite(INT, HIGH);
        else
            pinWrite(INT, LOW);
    }

    static inline void writeTEST(uint8_t value)
    {
        if (value == HIGH)
            pinWrite(TEST, HIGH);
        else
            pinWrite(TEST, LOW);
    }

    static inline void writeWAIT(uint8_t value)
    {
        if (value == HIGH)
            pinWrite(WAIT, HIGH);
        else
            pinWrite(WAIT, LOW);
    }

    static inline void writeSYS_RES(uint8_t value)
    {
        if (value == HIGH)
            pinWrite(SYS_RES, HIGH);
        else
            pinWrite(SYS_RES, LOW);
    }

    static inline void writeINT_ACK(uint8_t value)
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
    
    /** Write complete 16-bit address on address bus */
    static inline void writeAddressBus(uint16_t address)
    {
        busWrite(ADDR_LOW, address & 0xff);
        busWrite(ADDR_HIGH, (address & 0xff00) >> 8);
    }

    static inline uint16_t readAddressBus()
    {
        return (busRead(ADDR_HIGH) << 8) | busRead(ADDR_LOW);
    }

    static inline void configWriteAddressBus(uint16_t outputMode)
    {
        busConfigWrite(ADDR_LOW, outputMode & 0xff);
        busConfigWrite(ADDR_HIGH, (outputMode >> 8) & 0xff);
    }

    static inline void writeDataBus(uint8_t data)
    {
        busWrite(DATA, data);
    }

    static inline uint8_t readDataBus()
    {
        return busRead(DATA);
    }

    static inline void configWriteDataBus(uint8_t outputMode)
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

    // ========== State Data Functions ==========
    
    /**
     * Returns the current state as packed data
     *
     * Bit layout (64-bit):
     * Bits 63-48: Address (16 bits) - Memory address bus
     * Bits 47-40: Data (8 bits) - Data bus value
     * Bits 39-32: Memory control signals (8 bits) - RD, WR, IN, OUT, RAS, CAS, MUX, (1 spare)
     * Bits 31-24: System signals (8 bits) - SYS_RES, INT_ACK, INT, TEST, WAIT, (3 spare)
     * Bits 23-0:  Reserved for future use (24 bits)
     */
    static inline uint64_t getStateData()
    {
        uint16_t addr = readAddressBus();
        uint8_t data = readDataBus();

        uint64_t state = 0;

        // Pack address (16 bits) in bits 63-48
        state |= ((uint64_t)addr) << 48;

        // Pack data (8 bits) in bits 47-40
        state |= ((uint64_t)data) << 40;

        // Pack memory control signals (8 bits) in bits 39-32
        state |= ((uint64_t)readRD()) << 39;
        state |= ((uint64_t)readWR()) << 38;
        state |= ((uint64_t)readIN()) << 37;
        state |= ((uint64_t)readOUT()) << 36;
        state |= ((uint64_t)readRAS()) << 35;
        state |= ((uint64_t)readCAS()) << 34;
        state |= ((uint64_t)readMUX()) << 33;
        // Bit 32 reserved for future memory control signal

        // Pack system signals (8 bits) in bits 31-24
        state |= ((uint64_t)readSYS_RES()) << 31;
        state |= ((uint64_t)readINT_ACK()) << 30;
        state |= ((uint64_t)readINT()) << 29;
        state |= ((uint64_t)readTEST()) << 28;
        state |= ((uint64_t)readWAIT()) << 27;
        // Bits 26-24 reserved for future system signals

        // Bits 23-0 reserved for future expansion

        return state;
    }

    /**
     * Returns the current configuration state as packed data
     *
     * Bit layout (64-bit):
     * Bits 63-48: Address Bus Config (16 bits) - Memory address bus pin configurations
     * Bits 47-40: Data Bus Config (8 bits) - Data bus pin configurations  
     * Bits 39-32: Memory control signal configs (8 bits) - RD, WR, IN, OUT, RAS, CAS, MUX, (1 spare)
     * Bits 31-24: System signal configs (8 bits) - SYS_RES, INT_ACK, INT, TEST, WAIT, (3 spare)
     * Bits 23-0:  Reserved for future use (24 bits)
     *
     * Note: Configuration values represent pin direction (INPUT=0, OUTPUT=1)
     */
    static inline uint64_t getStateConfigData()
    {
        uint16_t addrConfig = configReadAddressBus();
        uint8_t dataConfig = configReadDataBus();

        uint64_t state = 0;

        // Pack address config (16 bits) in bits 63-48
        state |= ((uint64_t)addrConfig) << 48;

        // Pack data config (8 bits) in bits 47-40
        state |= ((uint64_t)dataConfig) << 40;

        // Pack memory control signal configs (8 bits) in bits 39-32
        state |= ((uint64_t)configReadRD()) << 39;
        state |= ((uint64_t)configReadWR()) << 38;
        state |= ((uint64_t)configReadIN()) << 37;
        state |= ((uint64_t)configReadOUT()) << 36;
        state |= ((uint64_t)configReadRAS()) << 35;
        state |= ((uint64_t)configReadCAS()) << 34;
        state |= ((uint64_t)configReadMUX()) << 33;
        // Bit 32 reserved for future memory control signal

        // Pack system signal configs (8 bits) in bits 31-24
        state |= ((uint64_t)configReadSYS_RES()) << 31;
        state |= ((uint64_t)configReadINT_ACK()) << 30;
        state |= ((uint64_t)configReadINT()) << 29;
        state |= ((uint64_t)configReadTEST()) << 28;
        state |= ((uint64_t)configReadWAIT()) << 27;
        // Bits 26-24 reserved for future system signals

        // Bits 23-0 reserved for future expansion

        return state;
    }
};

#endif /* MODEL1_LOWLEVEL_H */
