#ifndef ADDRESS_BUS_H
#define ADDRESS_BUS_H

#include <Arduino.h>
#include "./ILogger.h"

class AddressBus
{
private:
    ILogger *_logger;

    volatile bool _writable;

    void _setBus(bool writableOption);
    void _configurePort(uint8_t config);

public:
    AddressBus(ILogger *logger = nullptr);

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