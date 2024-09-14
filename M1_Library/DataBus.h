#ifndef DATA_BUS_H
#define DATA_BUS_H

#include <Arduino.h>
#include "./ILogger.h"

class DataBus
{
private:
    ILogger *_logger;

    volatile bool _writable;

    void _setBus(bool writableOption);
    void _configurePort(uint8_t config);

public:
    DataBus(ILogger *logger = nullptr);

    bool isReadable();
    bool isWritable();
    void setAsReadable();
    void setAsWritable();

    uint8_t readData();
    void writeData(uint8_t data);

    char *getState();
};

#endif /* DATA_BUS_H */