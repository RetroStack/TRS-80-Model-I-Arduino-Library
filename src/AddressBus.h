/*
 * AddressBus.h - Class for accessing the address bus
 * Authors: Ven Reddy, Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef ADDRESS_BUS_H
#define ADDRESS_BUS_H

#include <Arduino.h>
#include "ILogger.h"

class AddressBus
{
private:
    ILogger *_logger; // Logger instance for debugging output

    volatile bool _writable; // Flag indicating if bus is configured for writing

    void _setBus(bool writableOption);    // Configure bus direction (true for write, false for read)
    void _configurePort(uint16_t config); // Set port configuration for direction control

public:
    AddressBus(); // Constructor

    void begin(); // Initialize address bus pins and configuration
    void end();   // Reset address bus to default state

    void setLogger(ILogger &logger); // Set logger for debugging output

    bool isReadable(); // Check if address bus is configured for reading
    bool isWritable(); // Check if address bus is configured for writing

    void setAsReadable(); // Configure address bus pins as inputs for reading
    void setAsWritable(); // Configure address bus pins as outputs for writing

    uint16_t readMemoryAddress();              // Read 16-bit memory address from bus
    void writeMemoryAddress(uint16_t address); // Write 16-bit memory address to bus
    void writeRefreshAddress(uint8_t address); // Write 8-bit refresh address for DRAM operations

    uint8_t readIOAddress();              // Read 8-bit I/O port address from bus
    void writeIOAddress(uint8_t address); // Write 8-bit I/O port address to bus

    char *getState(); // Get current bus state as string for debugging
};

#endif /* ADDRESS_BUS_H */