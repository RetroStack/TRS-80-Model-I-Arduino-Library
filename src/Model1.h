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

enum PRINT_STYLE
{
    ASCII,
    HEXADECIMAL,
    BOTH
};

class Model1Class
{
private:
    ILogger *_logger;
    AddressBus _addressBus;
    DataBus _dataBus;

    volatile bool _mutability;
    uint8_t _nextMemoryRefreshRow;
    volatile bool _activeRefresh;
    int _timer;

    void _setMutable();
    void _setImmutable();
    void _setMutability(bool value);
    bool _isMutable();
    bool _checkMutability();

    void _refreshNextMemoryRow();

    void _initSystemControlSignals();
    void _initExternalControlSignals();

    void _setupMemoryRefreshTimer1();
    void _setupMemoryRefreshTimer2();
    void _activateMemoryRefresh();
    void _deactivateMemoryRefresh();

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
    Model1Class();

    void begin(int refreshTimer = -1);
    void end();

    void setLogger(ILogger &logger);

    // ---------- Address Space
    bool isROMAddress(uint16_t address);
    bool isUnusedAddress(uint16_t address);
    bool isMemoryMappedIOAddress(uint16_t address);
    bool isKeyboardAddress(uint16_t address);
    bool isVideoAddress(uint16_t address);
    bool isSystemAddress(uint16_t address);
    bool isLowerMemoryAddress(uint16_t address);
    bool isHigherMemoryAddress(uint16_t address);

    // ---------- Update
    void nextUpdate();

    // ---------- Memory
    uint8_t readMemory(uint16_t address);
    void writeMemory(uint16_t address, uint8_t data);
    // Returns a newly allocated buffer; caller must delete[] the result
    uint8_t *readMemory(uint16_t address, uint16_t length);
    void writeMemory(uint16_t address, uint8_t *data, uint16_t length);
    void writeMemory(uint16_t address, uint8_t *data, uint16_t length, uint16_t offset);
    void copyMemory(uint16_t src_address, uint16_t dst_address, uint16_t length);
    void fillMemory(uint8_t fill_data, uint16_t address, uint16_t length);
    void fillMemory(uint8_t *fill_data, uint16_t length, uint16_t start_address, uint16_t end_address);

    // ---------- IO
    uint8_t readIO(uint8_t address);
    void writeIO(uint8_t address, uint8_t data);

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

    void printMemoryContents(uint16_t start, uint16_t length, PRINT_STYLE style = BOTH, bool relative = false, uint16_t bytesPerLine = 32);
    void printMemoryContents(Print &output, uint16_t start, uint16_t length, PRINT_STYLE style = BOTH, bool relative = false, uint16_t bytesPerLine = 32);
};

extern Model1Class Model1;

#endif /* MODEL1_H */
