/*
 * Model1.h - Class for accessing the TRS-80 Model 1 low-level signals
 * Authors: Ven Reddy, Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef MODEL1_H
#define MODEL1_H

#include <Arduino.h>
#include "ILogger.h"
#include "AddressBus.h"
#include "DataBus.h"

const uint8_t EVENT_MEMORY_READ = 1;
const uint8_t EVENT_MEMORY_WRITE = 2;
const uint8_t EVENT_IO_READ = 3;
const uint8_t EVENT_IO_WRITE = 4;

struct EventData
{
    uint8_t type;
    uint16_t address;
    uint8_t data;
};

typedef void (*EventMemoryReadCallback)(const EventData &);
typedef void (*EventMemoryWriteCallback)(const EventData &);
typedef void (*EventIOReadCallback)(const EventData &);
typedef void (*EventIOWriteCallback)(const EventData &);

class Model1
{
private:
    ILogger *_logger;
    AddressBus *_addressBus;
    DataBus *_dataBus;

    volatile bool _mutability;
    volatile uint8_t _nextMemoryRefreshRow;
    volatile bool _activeRefresh;

    EventMemoryReadCallback _memoryReadCallback = nullptr;
    EventMemoryWriteCallback _memoryWriteCallback = nullptr;

    EventIOReadCallback _ioReadCallback = nullptr;
    EventIOWriteCallback _ioWriteCallback = nullptr;

    EventData *_createEventData(uint8_t type);

    void _setMutable();
    void _setImmutable();
    void _setMutability(bool value);
    bool _isMutable();
    bool _checkMutability();

    void _initSystemControlSignals();
    void _initExternalControlSignals();

    void _setupMemoryRefresh();
    void _activateMemoryRefresh();
    void _deactivateMemoryRefresh();

    void _setupMemoryInterrupts();
    void _removeMemoryInterrupts();
    void _setupIOInterrupts();
    void _removeIOInterrupts();

    void _activateBusControlSignals();
    void _deactivateBusControlSignals();
    void _resetBusControlSignals();

    void _activateBusAccessSignals();
    void _deactivateBusAccessSignals();
    void _resetBusAccessSignals();

    void _setInterruptRequestSignal(bool value);
    void _setTestSignal(bool value);
    void _setWaitSignal(bool value);

public:
    Model1(ILogger *logger = nullptr);
    ~Model1();

    void begin(bool memoryRefresh = false);
    void end();

    // ---------- Refresh DRAM
    void refreshNextMemoryRow();

    // ---------- Memory
    uint8_t readMemory(uint16_t address);
    void writeMemory(uint16_t address, uint8_t data);
    uint8_t *readMemory(uint16_t address, uint16_t length);
    void writeMemory(uint16_t address, uint8_t *data, uint16_t length);
    void writeMemory(uint16_t address, uint8_t *data, uint16_t length, uint16_t offset);
    void copyMemory(uint16_t src_address, uint16_t dst_address, uint16_t length);
    void fillMemory(uint8_t fill_data, uint16_t address, uint16_t length);
    void fillMemory(uint8_t *fill_data, uint16_t length, uint16_t start_address, uint16_t end_address);

    void setMemoryReadCallback(EventMemoryReadCallback callback);
    void triggerMemoryReadEvent();

    void setMemoryWriteCallback(EventMemoryWriteCallback callback);
    void triggerMemoryWriteEvent();

    // ---------- IO
    uint8_t readIO(uint8_t address);
    void writeIO(uint8_t address, uint8_t data);

    void setIOReadCallback(EventIOReadCallback callback);
    void triggerIOReadEvent();

    void setIOWriteCallback(EventIOWriteCallback callback);
    void triggerIOWriteEvent();

    // ---------- System Control Signals
    bool readSystemResetSignal();
    bool readInterruptAcknowledgeSignal();

    // ---------- Interrupt Request Signal
    bool triggerInterrupt(uint8_t interrupt, uint16_t timeout = 1000);
    void activateInterruptRequestSignal();
    void deactivateInterruptRequestSignal();

    // ---------- Test Signal
    void activateTestSignal();
    void deactivateTestSignal();

    // ---------- Wait Signal
    void activateWaitSignal();
    void deactivateWaitSignal();

    // ---------- State
    char *getState();
    void logState();

    uint8_t getVersionMajor();
    uint8_t getVersionMinor();
    uint8_t getVersionRevision();
    char *getVersion();
};

#endif /* MODEL1_H */
