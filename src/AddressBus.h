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
    // Constructor
    AddressBus();

    // Initialize address bus pins and configuration
    void begin();

    // Reset address bus to default state
    void end();

    // Set logger for debugging output
    void setLogger(ILogger &logger);

    // Check if address bus is configured for reading
    bool isReadable();

    // Check if address bus is configured for writing
    bool isWritable();

    // Configure address bus pins as inputs for reading
    void setAsReadable();

    // Configure address bus pins as outputs for writing
    void setAsWritable();

    // Read 16-bit memory address from bus
    uint16_t readMemoryAddress();

    // Write 16-bit memory address to bus
    void writeMemoryAddress(uint16_t address);

    // Write 8-bit refresh address for DRAM operations
    void writeRefreshAddress(uint8_t address);

    // Read 8-bit I/O port address from bus
    uint8_t readIOAddress();

    // Write 8-bit I/O port address to bus
    void writeIOAddress(uint8_t address);

    // Get current bus state as string for debugging
    char *getState();
};

#endif /* ADDRESS_BUS_H */