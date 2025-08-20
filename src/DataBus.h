/*
 * DataBus.h - Class for accessing the data bus
 * Authors: Ven Reddy, Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef DATA_BUS_H
#define DATA_BUS_H

#include <Arduino.h>
#include "ILogger.h"

class DataBus
{
private:
    ILogger *_logger; // Logger instance for debugging output

    volatile bool _writable; // Flag indicating if bus is configured for writing

    void _setBus(bool writableOption);   // Configure bus direction (true for write, false for read)
    void _configurePort(uint8_t config); // Set port configuration for direction control

public:
    DataBus(); // Constructor

    void begin(); // Initialize data bus pins and configuration
    void end();   // Reset data bus to default state

    void setLogger(ILogger &logger); // Set logger for debugging output

    bool isReadable(); // Check if data bus is configured for reading
    bool isWritable(); // Check if data bus is configured for writing

    void setAsReadable(); // Configure data bus pins as inputs for reading
    void setAsWritable(); // Configure data bus pins as outputs for writing

    uint8_t readData();           // Read 8-bit data value from bus
    void writeData(uint8_t data); // Write 8-bit data value to bus

    char *getState(); // Get current bus state as string for debugging
};

#endif /* DATA_BUS_H */