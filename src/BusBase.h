/*
 * BusBase.h - Shared behaviour for the address and data buses
 * Authors: Ven Reddy, Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef BUS_BASE_H
#define BUS_BASE_H

#include <Arduino.h>
#include "ILogger.h"

// AddressBus and DataBus are the same class at two widths: the direction flag,
// the logger, the readable/writable predicates and the direction switch are
// identical, and only the port width and the mask differ.
//
// This uses CRTP rather than a virtual _configurePort() deliberately. These
// objects sit on Model1's bus-cycle path, where the timing is measured in
// processor cycles; static dispatch inlines away to exactly the code the
// hand-written classes produced, where a vtable would add an indirect call to
// every direction switch and a pointer to every instance.
template <typename TDerived, typename TWidth>
class BusBase
{
protected:
    ILogger *_logger;        // Logger instance for debugging output
    volatile bool _writable; // Flag indicating if bus is configured for writing

    // All ones for output, all zeros for input, at whatever width TWidth is.
    void _setBus(bool writableOption)
    {
        if (_writable == writableOption)
            return;

        static_cast<TDerived *>(this)->_configurePort(
            writableOption ? (TWidth)~(TWidth)0 : (TWidth)0);
        _writable = writableOption;
    }

public:
    BusBase() : _logger(nullptr), _writable(false) {}

    void begin() { setAsReadable(); } // Initialize bus pins and configuration
    void end() { setAsReadable(); }   // Reset bus to default state

    void setLogger(ILogger &logger) { _logger = &logger; } // Set logger for debugging output

    bool isReadable() const { return !_writable; } // Check if bus is configured for reading
    bool isWritable() const { return _writable; }  // Check if bus is configured for writing

    void setAsReadable() { _setBus(false); } // Configure bus pins as inputs
    void setAsWritable() { _setBus(true); }  // Configure bus pins as outputs
};

#endif /* BUS_BASE_H */
