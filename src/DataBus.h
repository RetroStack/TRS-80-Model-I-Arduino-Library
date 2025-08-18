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
    // Constructor
    DataBus();

    // Initialize data bus pins and configuration
    void begin();

    // Reset data bus to default state
    void end();

    // Set logger for debugging output
    void setLogger(ILogger &logger);

    // Check if data bus is configured for reading
    bool isReadable();

    // Check if data bus is configured for writing
    bool isWritable();

    // Configure data bus pins as inputs for reading
    void setAsReadable();

    // Configure data bus pins as outputs for writing
    void setAsWritable();

    // Read 8-bit data value from bus
    uint8_t readData();

    // Write 8-bit data value to bus
    void writeData(uint8_t data);

    // Get current bus state as string for debugging
    char *getState();
};

#endif /* DATA_BUS_H */