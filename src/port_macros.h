/*
 * port_macros.h - File to manage macros for port/pin access
 * Authors: Ven Reddy, Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef PORT_MACROS_H
#define PORT_MACROS_H

// Utility macros
#define PORT_CONCAT(a, b) a##b
#define PORT_EXPAND(a, b) PORT_CONCAT(a, b)

// Macros for pin mode and pin operations
#define PIN_CONFIG_OUTPUT(_port, _pin) PORT_CONCAT(DDR, _port) = PORT_CONCAT(DDR, _port) | PIN_##_pin##_ON
#define PIN_CONFIG_INPUT(_port, _pin) PORT_CONCAT(DDR, _port) = PORT_CONCAT(DDR, _port) & PIN_##_pin##_OFF
#define PIN_CONFIG_READ(_port, _pin) ((PORT_CONCAT(DDR, _port) & PIN_##_pin##_ON) > 0)
#define PIN_WRITE_HIGH(_port, _pin) PORT_CONCAT(PORT, _port) = PORT_CONCAT(PORT, _port) | PIN_##_pin##_ON
#define PIN_WRITE_LOW(_port, _pin) PORT_CONCAT(PORT, _port) = PORT_CONCAT(PORT, _port) & PIN_##_pin##_OFF
#define PIN_READ(_port, _pin) (((PORT_CONCAT(PIN, _port) & PIN_##_pin##_ON) > 0) ? HIGH : LOW)

// Main macros for setting pin mode, writing, and reading
#define pinConfigWrite(_pin, _mode) PIN_CONFIG_##_mode(PIN_##_pin##_PORT, _pin)
#define pinConfigRead(_pin) (PIN_CONFIG_READ(PIN_##_pin##_PORT, _pin))
#define pinWrite(_pin, _value) PIN_WRITE_##_value(PIN_##_pin##_PORT, _pin)
#define pinRead(_pin) PIN_READ(PIN_##_pin##_PORT, _pin)

// Main macros for setting bus mode, writing, and reading
#define busConfigWrite(_bus, _config) PORT_EXPAND(DDR, BUS_##_bus##_PORT) = _config
#define busConfigRead(_bus) (PORT_EXPAND(DDR, BUS_##_bus##_PORT))
#define busWrite(_bus, _value) PORT_EXPAND(PORT, BUS_##_bus##_PORT) = _value
#define busRead(_bus) PORT_EXPAND(PIN, BUS_##_bus##_PORT)

#endif // PORT_MACROS_H