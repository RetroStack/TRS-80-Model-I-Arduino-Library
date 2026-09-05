/*
 * AddressBus.h - Class for accessing the address bus
 * Authors: Ven Reddy, Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef ADDRESS_BUS_H
#define ADDRESS_BUS_H

#include <Arduino.h>
#include "ILogger.h"
#include "BusBase.h"

class AddressBus : public BusBase<AddressBus, uint16_t>
{
    friend class BusBase<AddressBus, uint16_t>; // Calls _configurePort()

private:
    void _configurePort(uint16_t config); // Set port configuration for direction control

public:
    uint16_t readMemoryAddress();              // Read 16-bit memory address from bus
    void writeMemoryAddress(uint16_t address); // Write 16-bit memory address to bus
    void writeRefreshAddress(uint8_t address); // Write 8-bit refresh address for DRAM operations

    uint8_t readIOAddress();              // Read 8-bit I/O port address from bus
    void writeIOAddress(uint8_t address); // Write 8-bit I/O port address to bus

    char *getState(); // Get current bus state as string for debugging
};

#endif /* ADDRESS_BUS_H */