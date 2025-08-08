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
    ILogger *_logger;

    volatile bool _writable;

    void _setBus(bool writableOption);
    void _configurePort(uint16_t config);

public:
    AddressBus();

    void begin();
    void end();

    void setLogger(ILogger &logger);

    bool isReadable();
    bool isWritable();
    void setAsReadable();
    void setAsWritable();

    uint16_t readMemoryAddress();
    void writeMemoryAddress(uint16_t address);

    void writeRefreshAddress(uint8_t address);

    uint8_t readIOAddress();
    void writeIOAddress(uint8_t address);

    char *getState();
};

#endif /* ADDRESS_BUS_H */