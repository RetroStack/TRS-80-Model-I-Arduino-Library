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
    ILogger *_logger;

    volatile bool _writable;

    void _setBus(bool writableOption);
    void _configurePort(uint8_t config);

public:
    DataBus();

    void begin();
    void setLogger(ILogger &logger);

    bool isReadable();
    bool isWritable();
    void setAsReadable();
    void setAsWritable();

    uint8_t readData();
    void writeData(uint8_t data);

    char *getState();
};

#endif /* DATA_BUS_H */