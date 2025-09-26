/*
 * Model1.cpp - Class for accessing the TRS-80 Model 1 low-level signals
 * Authors: Ven Reddy, Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#include "Model1.h"
#include "utils.h"
#include <SD.h>
#include "Model1LowLevel.h"

// Refresh trigger
//
// Fasted possible with current code:
//   (71+1) x 62.5ns = 4.5us -> 4.5us * 128 rows => 0.576ms
//
// Reasonable:
//   (89+1) x 62.5ns = 5.625us -> 5.625us * 128 rows => 0.720ms
//
// Reasonable:
//   (100+1) x 62.5ns = 6.3125us -> 6.3125us * 128 rows => 0.808ms
//
// Minimum needed:
//   (124+1) x 62.5ns = 7.8125us -> 7.8125us * 128 rows => 1.0ms
#define CTC_TRIGGER 89

// Version constants
#define M1_VERSION_MAJOR 1
#define M1_VERSION_MINOR 4
#define M1_VERSION_REVISION 0

// Define global instance
Model1Class Model1;

// Constructor
Model1Class::Model1Class()
{
    _logger = nullptr;
    _timer = -1; // Set to default off

    // Defines the mutability of the bus systems and signals (e.g. activate TEST signal)
    _mutability = false;

    // Initializes memory refresh to default values
    deactivateMemoryRefresh();
}

// Initialize the Model1 interface with memory refresh timer
void Model1Class::begin(int refreshTimer)
{
    // Validate refresh timer parameter
    if (refreshTimer != -1 && refreshTimer != 1 && refreshTimer != 2)
    {
        if (_logger)
            _logger->warnF(F("Model1: Invalid refresh timer %d. Valid values are -1 (disabled), 1, or 2. Using disabled."), refreshTimer);
        refreshTimer = -1;
    }

    _addressBus.begin();
    _dataBus.begin();

    _initSystemControlSignals();
    _initExternalControlSignals();

    _deactivateBusControlSignals();
    _deactivateBusAccessSignals();

    if (refreshTimer != -1)
    {
        _timer = refreshTimer;
    }

    if (refreshTimer == 1)
    {
        _setupMemoryRefreshTimer1();
    }
    else if (refreshTimer == 2)
    {
        _setupMemoryRefreshTimer2();
    }
    else
    {
        deactivateMemoryRefresh();
    }
}

// Shutdown the Model1 interface
void Model1Class::end()
{
    _addressBus.end();
    _dataBus.end();

    _deactivateBusControlSignals();
    _deactivateBusAccessSignals();

    deactivateMemoryRefresh();
}

// Set the logger for debugging output
void Model1Class::setLogger(ILogger &logger)
{
    _logger = &logger;
    _addressBus.setLogger(logger);
    _dataBus.setLogger(logger);
}

// ----------------------------------------
// ---------- Address Space
// ----------------------------------------

// Check if address is in ROM range (0x0000-0x2FFF)
bool Model1Class::isROMAddress(uint16_t address)
{
    return (address <= 0x2FFF);
}

// Check if address is in unused range (0x3000-0x37DF)
bool Model1Class::isUnusedAddress(uint16_t address)
{
    return (address >= 0x3000 && address <= 0x37DF);
}

// Check if address is memory-mapped I/O (0x37E0-0x37FF)
bool Model1Class::isMemoryMappedIOAddress(uint16_t address)
{
    return (address >= 0x37E0 && address <= 0x37FF);
}

// Check if address is keyboard range (0x3800-0x3BFF)
bool Model1Class::isKeyboardAddress(uint16_t address)
{
    return (address >= 0x3800 && address <= 0x3BFF); // memory space is shadowed from 0x3900 to 0x3BFF (3x)
}

// Check if address is video memory range (0x3C00-0x3FFF)
bool Model1Class::isVideoAddress(uint16_t address)
{
    return (address >= 0x3C00 && address <= 0x3FFF);
}

// Check if address is system memory range (0x4000-0x41FF)
bool Model1Class::isSystemAddress(uint16_t address)
{
    return (address >= 0x4000 && address <= 0x41FF);
}

// Check if address is lower memory range (0x4200-0x7FFF)
bool Model1Class::isLowerMemoryAddress(uint16_t address)
{
    return (address >= 0x4200 && address <= 0x7FFF);
}

// Check if address is higher memory range (0x8000-0xFFFF)
bool Model1Class::isHigherMemoryAddress(uint16_t address)
{
    return (address >= 0x8000 && address <= 0xFFFF);
}

// ----------------------------------------
// ---------- Mutability
// ----------------------------------------

// Set mutable
void Model1Class::_setMutable()
{
    _setMutability(true);
}

// Set immutability
void Model1Class::_setImmutable()
{
    _setMutability(false);
}

// Set mutability
void Model1Class::_setMutability(bool value)
{
    _mutability = value;
}

// Check mutability
bool Model1Class::_isMutable()
{
    return _mutability;
}

// Check mutability
bool Model1Class::_checkMutability()
{
    bool mutability = _isMutable();
    if (!_isMutable())
    {
        if (_logger)
            _logger->errF(F("System is not mutable, but a request to access the system was made."));
    }
    return mutability;
}

// ----------------------------------------
// ---------- Refresh
// ----------------------------------------

// Setup memory refresh timer 1
void Model1Class::_setupMemoryRefreshTimer1()
{
    uint8_t oldSREG = SREG;
    noInterrupts();

    // Reset and init
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;
    TIMSK1 &= ~(1 << OCIE1A);

    OCR1A = CTC_TRIGGER;

    TCCR1B |= (1 << WGM12); // Turn on CTC mode
    TCCR1B |= (1 << CS10);  // Set prescaler to 1

    SREG = oldSREG;
}

// Setup memory refresh timer 2
void Model1Class::_setupMemoryRefreshTimer2()
{
    uint8_t oldSREG = SREG;
    noInterrupts();

    // Reset and init
    TCCR2A = 0;
    TCCR2B = 0;
    TCNT2 = 0;
    TIMSK2 &= ~(1 << OCIE2A);

    OCR2A = CTC_TRIGGER;

    TCCR2A |= (1 << WGM21); // Turn on CTC mode
    TCCR2B |= (1 << CS20);  // Set prescaler to 1

    SREG = oldSREG;
}

// Activate memory refresh
void Model1Class::activateMemoryRefresh()
{
    _activeRefresh = true;
    if (_timer == 1)
    {
        TCNT1 = 0;
        TIMSK1 |= (1 << OCIE1A); // Enable timer compare interrupt
    }
    else if (_timer == 2)
    {
        TCNT2 = 0;
        TIMSK2 |= (1 << OCIE2A); // Enable timer compare interrupt
    }
}

// Deactivate memory refresh
void Model1Class::deactivateMemoryRefresh()
{
    if (_timer == 1)
    {
        TIMSK1 &= ~(1 << OCIE1A); // Disable timer compare interrupt
    }
    else if (_timer == 2)
    {
        TIMSK2 &= ~(1 << OCIE2A); // Disable timer compare interrupt
    }
    _activeRefresh = false;
}

// Refresh the next memory row
void Model1Class::nextUpdate()
{
    _refreshNextMemoryRow();
}

// Refresh the next memory row
void Model1Class::_refreshNextMemoryRow()
{
    // This function expects mutibility as well as that the refresh is activated
    uint8_t currentRefreshRow = _nextMemoryRefreshRow;

    // Prepare next address; quicker way to manage overflow
    _nextMemoryRefreshRow = (_nextMemoryRefreshRow + 1) & 0x7F;

    // Set address
    _addressBus.writeRefreshAddress(currentRefreshRow);

    // Timing of various signals
    Model1LowLevel::writeRAS(LOW); // 45ns (62.5ns, but when the pulse is down)
    asmNoop();                     // 125ns
    asmNoop();                     // 125ns
    asmNoop();                     // 125ns

    // Reset, leaving address as-is
    Model1LowLevel::writeRAS(HIGH); // 45ns (62.5ns, but when the pulse is down)
}

// ----------------------------------------
// ---------- Memory
// ----------------------------------------

// Read memory with single byte
uint8_t Model1Class::readMemory(uint16_t address)
{
    // Verification of access
    if (!_checkMutability())
        return 0;

    uint8_t oldSREG = SREG;
    noInterrupts();

    // Set address and then data
    _addressBus.writeMemoryAddress(address);

    // Timing of various signals
    Model1LowLevel::writeRAS(LOW);
    Model1LowLevel::writeRD(LOW);
    Model1LowLevel::writeMUX(HIGH);
    Model1LowLevel::writeCAS(LOW);
    asmWait(3); // 772 ns

    // Read data
    uint8_t data = _dataBus.readData();

    // Reset, leaving address as-is
    Model1LowLevel::writeCAS(HIGH);
    Model1LowLevel::writeRD(HIGH);
    Model1LowLevel::writeRAS(HIGH);
    Model1LowLevel::writeMUX(LOW);

    SREG = oldSREG;

    return data;
}

// Write memory with single byte
void Model1Class::writeMemory(uint16_t address, uint8_t data)
{
    // Verification of access
    if (!_checkMutability())
        return;

    uint8_t oldSREG = SREG;
    noInterrupts();

    // Configure bus
    _dataBus.setAsWritable();

    // Set address and then data
    _addressBus.writeMemoryAddress(address);
    _dataBus.writeData(data);

    // Timing of various signals
    Model1LowLevel::writeRAS(LOW);
    asmNoop();
    asmNoop();
    asmNoop();
    Model1LowLevel::writeWR(LOW);
    Model1LowLevel::writeMUX(HIGH);
    Model1LowLevel::writeCAS(LOW);
    asmWait(1); // 252 ns

    // Reset, leaving address as-is, removing data
    Model1LowLevel::writeWR(HIGH);
    Model1LowLevel::writeCAS(HIGH);
    Model1LowLevel::writeRAS(HIGH);
    Model1LowLevel::writeMUX(LOW);
    _dataBus.setAsReadable();

    SREG = oldSREG;
}

// Read memory with length
uint8_t *Model1Class::readMemory(uint16_t address, uint16_t length)
{
    if (length == 0)
        return nullptr;

    uint8_t *buffer = (uint8_t *)malloc(length * sizeof(uint8_t));
    if (!buffer)
    {
        if (_logger)
        {
            _logger->errF(F("Model1: Failed to allocate memory buffer for readMemory (%u bytes)"), length);
        }
        return nullptr;
    }

    for (uint16_t i = 0; i < length; i++)
    {
        buffer[i] = readMemory(address + i);
    }

    return buffer;
}

// Write memory block
void Model1Class::writeMemory(uint16_t address, uint8_t *data, uint16_t length)
{
    if (!data)
    {
        if (_logger)
            _logger->errF(F("Model1: writeMemory called with null data pointer"));
        return;
    }
    writeMemory(address, data, length, 0);
}

// Write memory with offset
void Model1Class::writeMemory(uint16_t address, uint8_t *data, uint16_t length, uint16_t offset)
{
    if (!data)
    {
        if (_logger)
            _logger->errF(F("Model1: writeMemory called with null data pointer"));
        return;
    }
    if (length == 0)
    {
        if (_logger)
            _logger->warnF(F("Model1: writeMemory called with length 0"));
        return;
    }
    for (uint16_t i = 0; i < length; i++)
    {
        writeMemory(address + i, data[offset + i]);
    }
}

// Copy memory from one location to another
void Model1Class::copyMemory(uint16_t src_address, uint16_t dst_address, uint16_t length)
{
    if (length == 0)
    {
        if (_logger)
            _logger->warnF(F("Model1: Copy memory called with length 0 - no action taken"));
        return;
    }
    if (dst_address == src_address)
    {
        if (_logger)
            _logger->warnF(F("Model1: Copy memory called with same src and dst address 0x%04X - no action taken"), src_address);
        return;
    }

    // Check for potential dangerous address overlap
    if ((src_address < dst_address && src_address + length > dst_address) ||
        (dst_address < src_address && dst_address + length > src_address))
    {
        if (_logger)
            _logger->warnF(F("Model1: Memory copy addresses overlap - src:0x%04X dst:0x%04X len:%d may cause data corruption"), src_address, dst_address, length);
    }

    for (uint16_t i = 0; i < length; i++)
    {
        writeMemory(dst_address + i, readMemory(src_address + i));
    }
}

// Fill memory with a single value
void Model1Class::fillMemory(uint8_t fill_data, uint16_t address, uint16_t length)
{
    for (uint16_t i = 0; i < length; i++)
    {
        writeMemory(address + i, fill_data);
    }
}

// Fill memory with data from a buffer
void Model1Class::fillMemory(uint8_t *fill_data, uint16_t length, uint16_t address, uint16_t address_length)
{
    if (!fill_data)
    {
        if (_logger)
            _logger->errF(F("Model1: fillMemory called with null fill_data pointer"));
        return;
    }
    if (length == 0)
    {
        if (_logger)
            _logger->warnF(F("Model1: fillMemory called with length 0"));
        return;
    }
    if (address_length == 0)
    {
        if (_logger)
            _logger->warnF(F("Model1: fillMemory called with address_length 0"));
        return;
    }
    for (uint16_t i = 0; i < address_length; i += length)
    {
        for (uint16_t j = 0; j < length; j++)
        {
            writeMemory(address + i + j, fill_data[j]);

            // Check if we reached the end address
            if (i + j >= address_length)
                return;
        }
    }
}

// ----------------------------------------
// ---------- IO
// ----------------------------------------

// Read from I/O port
uint8_t Model1Class::readIO(uint8_t address)
{
    // Verification of access
    if (!_checkMutability())
        return 0;

    uint8_t oldSREG = SREG;
    noInterrupts();

    // Set address and then data
    _addressBus.writeIOAddress(address);

    // Timing of various signals
    Model1LowLevel::writeIN(LOW);
    Model1LowLevel::writeMUX(HIGH);
    Model1LowLevel::writeCAS(LOW);

    // Read data
    uint8_t data = _dataBus.readData();

    // Reset, leaving address as-is
    Model1LowLevel::writeCAS(HIGH);
    Model1LowLevel::writeIN(HIGH);
    Model1LowLevel::writeMUX(LOW);

    SREG = oldSREG;

    return data;
}

// Write to I/O port
void Model1Class::writeIO(uint8_t address, uint8_t data)
{
    // Verification of access
    if (!_checkMutability())
        return;

    uint8_t oldSREG = SREG;
    noInterrupts();

    // Configure bus
    _dataBus.setAsWritable();

    // Set address and then data
    _addressBus.writeMemoryAddress(address);
    _dataBus.writeData(data);

    // Timing of various signals
    Model1LowLevel::writeOUT(LOW);
    Model1LowLevel::writeMUX(HIGH);
    Model1LowLevel::writeCAS(LOW);
    asmWait(1); // 252 ns

    // Reset, leving address as-is, removing data
    Model1LowLevel::writeCAS(HIGH);
    Model1LowLevel::writeOUT(HIGH);
    Model1LowLevel::writeMUX(LOW);
    _dataBus.setAsReadable();

    SREG = oldSREG;
}

// ----------------------------------------
// ---------- System Control Signals
// ----------------------------------------

// Initialize system control signals
void Model1Class::_initSystemControlSignals()
{
    Model1LowLevel::writeSYS_RES(LOW);
    Model1LowLevel::writeINT_ACK(LOW);

    Model1LowLevel::configWriteSYS_RES(INPUT);
    Model1LowLevel::configWriteINT_ACK(INPUT);
}

// Read the system reset signal
bool Model1Class::readSystemResetSignal()
{
    return Model1LowLevel::readSYS_RES() == LOW ? true : false;
}

// Read the INT_ACK signal
bool Model1Class::readInterruptAcknowledgeSignal()
{
    return Model1LowLevel::readINT_ACK() == LOW ? true : false;
}

// ----------------------------------------
// ---------- Memory Control Signals
// ----------------------------------------

// Activate bus control signals
void Model1Class::_activateBusControlSignals()
{
    _resetBusControlSignals();

    Model1LowLevel::configWriteRAS(OUTPUT);
    Model1LowLevel::configWriteMUX(OUTPUT);
    Model1LowLevel::configWriteCAS(OUTPUT);
}

// Deactivate bus control signals
void Model1Class::_deactivateBusControlSignals()
{
    Model1LowLevel::configWriteRAS(INPUT);
    Model1LowLevel::configWriteMUX(INPUT);
    Model1LowLevel::configWriteCAS(INPUT);
}

// Reset bus control signals
void Model1Class::_resetBusControlSignals()
{
    Model1LowLevel::writeRAS(HIGH);
    Model1LowLevel::writeMUX(LOW);
    Model1LowLevel::writeCAS(HIGH);
}

// ----------------------------------------
// ---------- Memory Access Signals
// ----------------------------------------

// Activate bus access signals
void Model1Class::_activateBusAccessSignals()
{
    _resetBusAccessSignals();

    Model1LowLevel::configWriteRD(OUTPUT);
    Model1LowLevel::configWriteWR(OUTPUT);

    Model1LowLevel::configWriteIN(OUTPUT);
    Model1LowLevel::configWriteOUT(OUTPUT);
}

// Deactivate bus access signals
void Model1Class::_deactivateBusAccessSignals()
{
    Model1LowLevel::configWriteRD(INPUT);
    Model1LowLevel::configWriteWR(INPUT);

    Model1LowLevel::configWriteIN(INPUT);
    Model1LowLevel::configWriteOUT(INPUT);
}

// Reset bus access signals
void Model1Class::_resetBusAccessSignals()
{
    Model1LowLevel::writeRD(HIGH);
    Model1LowLevel::writeWR(HIGH);
    Model1LowLevel::writeIN(HIGH);
    Model1LowLevel::writeOUT(HIGH);
}

// ----------------------------------------
// ---------- External Control Signals
// ----------------------------------------

// Initialize external control signals
void Model1Class::_initExternalControlSignals()
{
    Model1LowLevel::writeINT(HIGH);
    Model1LowLevel::writeTEST(HIGH);
    Model1LowLevel::writeWAIT(HIGH);

    Model1LowLevel::configWriteINT(OUTPUT);
    Model1LowLevel::configWriteTEST(OUTPUT);
    Model1LowLevel::configWriteWAIT(OUTPUT);
}

// ---------- Interrupt Request Signal

// Set the INT* signal
void Model1Class::_setInterruptRequestSignal(bool value)
{
    Model1LowLevel::writeINT(value ? LOW : HIGH);
}

// Trigger an interrupt
bool Model1Class::triggerInterrupt(uint8_t interrupt, uint16_t timeout)
{
    activateInterruptRequestSignal();

    for (uint16_t i = 0; i < timeout; i++)
    {
        if (Model1LowLevel::readINT_ACK() == LOW)
        {
            _dataBus.setAsWritable();
            _dataBus.writeData(interrupt);

            asmWait(3);
            deactivateInterruptRequestSignal();
            asmWait(3);

            _dataBus.setAsReadable();

            return true;
        }
        asmNoop();
    }

    deactivateInterruptRequestSignal();

    if (_logger)
        _logger->errF(F("Model1: Interrupt trigger timeout - CPU did not respond within %d cycles"), timeout);
    return false; // CPU did not respond within timeout
}

// Activate the INT* signal
void Model1Class::activateInterruptRequestSignal()
{
    if (Model1LowLevel::readINT() == LOW)
    {
        if (_logger)
            _logger->warnF(F("INT* signal already active."));
        return;
    }

    _setInterruptRequestSignal(true);
}

// Deactivate the INT* signal
void Model1Class::deactivateInterruptRequestSignal()
{
    if (Model1LowLevel::readINT() == HIGH)
    {
        if (_logger)
            _logger->warnF(F("INT* signal already deactivated."));
        return;
    }

    _setInterruptRequestSignal(false);
}

// ---------- Test Signal

// Set the TEST* signal
void Model1Class::_setTestSignal(bool value)
{
    Model1LowLevel::writeTEST(value ? LOW : HIGH);
}

// Check if TEST signal is active
bool Model1Class::hasActiveTestSignal()
{
    return Model1LowLevel::readTEST() == LOW;
}

// Get current state as a string
void Model1Class::activateTestSignal()
{
    if (Model1LowLevel::readTEST() == LOW)
    {
        if (_logger)
            _logger->warnF(F("TEST* signal already active."));
        return;
    }

    // Activate the TEST* signal
    _setTestSignal(true);

    // Wait to avoid contention
    asmWait(16); // Wait 4us. Only need 3 (value 12) for 5T, but this is to be safe

    // Set the signals as active from external system
    _addressBus.setAsWritable();
    _dataBus.setAsReadable();
    _activateBusControlSignals();
    _activateBusAccessSignals();

    // Mark bus as mutable, enabling write requests from this code
    _setMutable();

    // Activate background services
    if (_timer != -1)
    {
        activateMemoryRefresh();
    }
}

// Deactivate TEST signal
void Model1Class::deactivateTestSignal()
{
    if (Model1LowLevel::readTEST() == HIGH)
    {
        if (_logger)
            _logger->warnF(F("TEST* signal already deactivated."));
        return;
    }

    // Deactivate background services
    if (_timer != -1)
    {
        deactivateMemoryRefresh();
    }

    // Set bus as immutable, blocking write requests from this code
    _setImmutable();

    // Set the signals as inactive from external system
    _deactivateBusAccessSignals();
    _deactivateBusControlSignals();
    _dataBus.setAsReadable();
    _addressBus.setAsReadable();

    // Deactivate TEST* signal
    _setTestSignal(false);

    // Wait to avoid contention
    asmWait(16); // Wait 4us. Only need 3 (value 12) for 5T, but this is to be safe
}

// ---------- Wait Signal

// Set the WAIT* signal
void Model1Class::_setWaitSignal(bool value)
{
    Model1LowLevel::writeWAIT(value ? LOW : HIGH);
}

// Activate WAIT signal
void Model1Class::activateWaitSignal()
{
    if (Model1LowLevel::readWAIT() == LOW)
    {
        if (_logger)
            _logger->warnF(F("WAIT* signal already active."));
        return;
    }

    _setWaitSignal(true);
}

// Get current state as a string
void Model1Class::deactivateWaitSignal()
{
    if (Model1LowLevel::readWAIT() == HIGH)
    {
        if (_logger)
            _logger->warnF(F("WAIT* signal already deactivated."));
        return;
    }

    _setWaitSignal(false);
}

// ---------- State

// Get current state as a string
char *Model1Class::getState()
{
    char *addrStatus = _addressBus.getState();
    char *dataStatus = _dataBus.getState();

    const int LEN = 255;
    char *buffer = (char *)malloc(LEN * sizeof(char));
    if (!buffer)
    {
        if (_logger)
        {
            _logger->errF(F("Model1: Failed to allocate memory for state string"));
        }
        free(addrStatus);
        free(dataStatus);
        return nullptr;
    }

    // Handle null returns from bus getState methods
    if (!addrStatus || !dataStatus)
    {
        if (_logger)
        {
            _logger->errF(F("Model1: Failed to get bus state information"));
        }
        free(buffer);
        free(addrStatus);
        free(dataStatus);
        return nullptr;
    }

    snprintf(
        buffer,
        LEN,
        "Mut<%c>, RfshEn<%c>, RfshRow<%3.d>, RD<%c>(%d), WR<%c>(%d), IN<%c>(%d), OUT<%c>(%d), %s, %s, RAS<%c>(%d), CAS<%c>(%d), MUX<%c>(%d), SYS_RES<%c>(%d), INT_ACK<%c>(%d), INT<%c>(%d), TEST<%c>(%d), WAIT<%c>(%d)",

        _mutability ? 'T' : 'F',
        _activeRefresh ? 'T' : 'F',
        _nextMemoryRefreshRow,

        pinStatus(Model1LowLevel::configReadRD()), Model1LowLevel::readRD(),
        pinStatus(Model1LowLevel::configReadWR()), Model1LowLevel::readWR(),
        pinStatus(Model1LowLevel::configReadIN()), Model1LowLevel::readIN(),
        pinStatus(Model1LowLevel::configReadOUT()), Model1LowLevel::readOUT(),

        addrStatus,
        dataStatus,

        pinStatus(Model1LowLevel::configReadRAS()), Model1LowLevel::readRAS(),
        pinStatus(Model1LowLevel::configReadCAS()), Model1LowLevel::readCAS(),
        pinStatus(Model1LowLevel::configReadMUX()), Model1LowLevel::readMUX(),

        pinStatus(Model1LowLevel::configReadSYS_RES()), Model1LowLevel::readSYS_RES(),
        pinStatus(Model1LowLevel::configReadINT_ACK()), Model1LowLevel::readINT_ACK(),

        pinStatus(Model1LowLevel::configReadINT()), Model1LowLevel::readINT(),
        pinStatus(Model1LowLevel::configReadTEST()), Model1LowLevel::readTEST(),
        pinStatus(Model1LowLevel::configReadWAIT()), Model1LowLevel::readWAIT());

    free(addrStatus);
    free(dataStatus);

    return buffer;
}

// Get current state data
uint64_t Model1Class::getStateData()
{
    return Model1LowLevel::getStateData();
}

// Get current state configuration data
uint64_t Model1Class::getStateConfigData()
{
    return Model1LowLevel::getStateConfigData();
}

// Get current state as a string
void Model1Class::logState()
{
    if (_logger)
    {
        char *state = getState();
        _logger->infoF(F("State: %s"), state);
        free(state);
    }
}

// ---------- Version

// Get version major
uint8_t Model1Class::getVersionMajor()
{
    return M1_VERSION_MAJOR;
}

// Get version minor
uint8_t Model1Class::getVersionMinor()
{
    return M1_VERSION_MINOR;
}

// Get version revision
uint8_t Model1Class::getVersionRevision()
{
    return M1_VERSION_REVISION;
}

// Get version string
char *Model1Class::getVersion()
{
    const int LEN = 255;
    char *buffer = (char *)malloc(LEN * sizeof(char));
    if (!buffer)
    {
        if (_logger)
        {
            _logger->errF(F("Model1: Failed to allocate memory for version string"));
        }
        return nullptr;
    }

    snprintf(
        buffer,
        LEN,
        "%d.%d.%d",
        M1_VERSION_MAJOR,
        M1_VERSION_MINOR,
        M1_VERSION_REVISION);
    return buffer;
}

// Print memory contents with specified formatting options
void Model1Class::printMemoryContents(uint16_t start, uint16_t length, PRINT_STYLE style, bool relative, uint16_t bytesPerLine)
{
    if (!_logger)
        return;

    printMemoryContents(*_logger, start, length, style, relative, bytesPerLine);
}

// Print memory contents with specified formatting options
void Model1Class::printMemoryContents(Print &output, uint16_t start, uint16_t length, PRINT_STYLE style, bool relative, uint16_t bytesPerLine)
{
    if (bytesPerLine == 0 || bytesPerLine > 60)
    {
        if (_logger)
            _logger->errF(F("Unsupported value for bytesPerLine with %d."), bytesPerLine);
        return;
    }

    uint8_t *buffer = (uint8_t *)malloc(bytesPerLine);
    if (!buffer)
    {
        if (_logger)
            _logger->errF(F("Cannot allocate byte buffer."));
        return;
    }

    uint16_t lineLength = 6; // "0000: "
    if (style == HEXADECIMAL || style == BOTH)
        lineLength += 3 * bytesPerLine;
    if (style == BOTH)
        lineLength += 3; // " |" and closing '|'
    if (style == ASCII || style == BOTH)
        lineLength += bytesPerLine;
    lineLength += 1;

    char *lineBuffer = (char *)malloc(lineLength);
    if (!lineBuffer)
    {
        if (_logger)
            _logger->errF(F("Cannot allocate line buffer."));
        free(buffer);
        return;
    }

    for (uint16_t offset = 0; offset < length; offset += bytesPerLine)
    {
        uint16_t lineLengthActual = (offset + bytesPerLine <= length)
                                        ? bytesPerLine
                                        : (length - offset);

        // Load from ROM
        for (uint16_t i = 0; i < lineLengthActual; ++i)
        {
            buffer[i] = readMemory(start + offset + i);
        }

        // Build the line string
        char *p = lineBuffer;

        // Address
        uint16_t addr = relative ? offset : (start + offset);
        p += sprintf(p, "%04X: ", addr);

        // Hex bytes
        if (style == BOTH || style == HEXADECIMAL)
        {
            for (uint16_t i = 0; i < bytesPerLine; ++i)
            {
                if (i < lineLengthActual)
                {
                    p += sprintf(p, "%02X ", buffer[i]);
                }
                else
                {
                    p += sprintf(p, "   "); // Padding for short lines
                }
            }
        }

        if (style == BOTH)
        {
            p += sprintf(p, " |");
        }

        // ASCII representation
        if (style == BOTH || style == ASCII)
        {
            for (uint16_t i = 0; i < lineLengthActual; ++i)
            {
                uint8_t b = buffer[i];
                if (b >= 32 && b <= 126)
                {
                    *p++ = b; // Printable ASCII
                }
                else
                {
                    *p++ = '.'; // Non-printable
                }
            }
        }

        if (style == BOTH)
        {
            *p++ = '|';
        }

        *p = '\0'; // Null-terminate

        // Print the entire line
        output.println(lineBuffer);
    }

    free(buffer);
    free(lineBuffer);
}

// Dump memory region to SD card file as binary
bool Model1Class::dumpMemoryToSD(uint16_t address, uint16_t length, const char *filename)
{
    if (!filename)
    {
        if (_logger)
            _logger->errF(F("Model1: dumpMemoryToSD() called with null filename"));
        return false;
    }

    if (length == 0)
    {
        if (_logger)
            _logger->errF(F("Model1: dumpMemoryToSD() called with zero length"));
        return false;
    }

    // Initialize SD card if not already done
    if (!SD.begin(M1Shield.getSDCardSelectPin()))
    {
        if (_logger)
            _logger->errF(F("Model1: Failed to initialize SD card"));
        return false;
    }

    // Open file for writing (binary mode)
    File memoryFile = SD.open(filename, FILE_WRITE);
    if (!memoryFile)
    {
        if (_logger)
            _logger->errF(F("Model1: Failed to open file %s for writing"), filename);
        return false;
    }

    if (_logger)
        _logger->infoF(F("Model1: Dumping memory from address 0x%04X, length %u bytes to %s"), address, length, filename);

    // Read and write memory in chunks to manage memory usage
    const uint16_t CHUNK_SIZE = 64; // Read in 64-byte chunks
    uint16_t bytesWritten = 0;

    for (uint16_t offset = 0; offset < length; offset += CHUNK_SIZE)
    {
        uint16_t chunkSize = (offset + CHUNK_SIZE <= length) ? CHUNK_SIZE : (length - offset);
        uint16_t currentAddress = address + offset;

        // Read chunk from memory
        uint8_t *chunk = readMemory(currentAddress, chunkSize);
        if (!chunk)
        {
            if (_logger)
                _logger->errF(F("Model1: Failed to read memory at address 0x%04X"), currentAddress);
            memoryFile.close();
            return false;
        }

        // Write chunk to file
        size_t written = memoryFile.write(chunk, chunkSize);
        if (written != chunkSize)
        {
            if (_logger)
                _logger->errF(F("Model1: Failed to write chunk to file (wrote %u of %u bytes)"), written, chunkSize);
            free(chunk);
            memoryFile.close();
            return false;
        }

        bytesWritten += written;
        free(chunk); // Free the allocated buffer

        // Optional progress logging for large dumps
        if (_logger && length > 1024 && (offset % 256 == 0))
        {
            _logger->infoF(F("Model1: Progress: %u / %u bytes written"), bytesWritten, length);
        }
    }

    memoryFile.close();

    if (_logger)
        _logger->infoF(F("Model1: Successfully dumped %u bytes to %s"), bytesWritten, filename);

    return true;
}