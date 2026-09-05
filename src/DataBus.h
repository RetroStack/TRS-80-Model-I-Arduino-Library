/*
 * DataBus.h - Class for accessing the data bus
 * Authors: Ven Reddy, Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef DATA_BUS_H
#define DATA_BUS_H

#include <Arduino.h>
#include "ILogger.h"
#include "BusBase.h"

class DataBus : public BusBase<DataBus, uint8_t>
{
    friend class BusBase<DataBus, uint8_t>; // Calls _configurePort()

private:
    void _configurePort(uint8_t config); // Set port configuration for direction control

public:
    uint8_t readData();           // Read 8-bit data value from bus
    void writeData(uint8_t data); // Write 8-bit data value to bus

    char *getState(); // Get current bus state as string for debugging
};

#endif /* DATA_BUS_H */