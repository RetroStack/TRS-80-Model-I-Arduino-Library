/*
 * MirrorTarget.h - A render target that mirrors the UI to the serial monitor
 *
 * Demonstrates writing a custom RenderTarget. It owns a 1-bit offscreen
 * canvas, so it receives the same drawing the panel does, and prints that
 * canvas to Serial as ASCII art.
 *
 * This exists so the example is useful with no extra hardware: you can watch
 * the mirrored UI in the Serial Monitor and toggle it at runtime.
 *
 * MEMORY: a GFXcanvas1 costs width * height / 8 bytes of SRAM. The default
 * 128x64 costs 1024 bytes - 12.5% of the Mega's 8KB. Drop to 64x32 (256
 * bytes) if your sketch is tight.
 *
 * Authors: Marcel Erz (RetroStack)
 * License: MIT
 */

#ifndef MIRROR_TARGET_H
#define MIRROR_TARGET_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <RenderTarget.h>

class MirrorTarget : public RenderTarget
{
private:
    GFXcanvas1 _canvas;
    bool _enabled;
    unsigned long _lastDump;
    unsigned long _dumpIntervalMs;

public:
    MirrorTarget(uint16_t width = 128, uint16_t height = 64, unsigned long dumpIntervalMs = 5000)
        : _canvas(width, height), _enabled(true), _lastDump(0), _dumpIntervalMs(dumpIntervalMs)
    {
    }

    // True when the canvas actually allocated. Check this after construction:
    // on an 8KB part the allocation can fail, and GFXcanvas1's line-drawing
    // helpers do not null-check their buffer.
    bool isValid() const { return _canvas.getBuffer() != nullptr; }

    // ---- RenderTarget ----

    const char *getName() const override { return "Serial Mirror"; }

    // Reporting "not enabled" when the buffer is missing keeps renderAll from
    // ever drawing into a canvas that failed to allocate.
    bool isEnabled() const override { return _enabled && _canvas.getBuffer() != nullptr; }

    void setEnabled(bool enabled) override { _enabled = enabled; }

    Adafruit_GFX &getGFX() override { return _canvas; }

    uint16_t getScreenWidth() const override { return (uint16_t)_canvas.width(); }
    uint16_t getScreenHeight() const override { return (uint16_t)_canvas.height(); }

    // One bit per pixel: anything non-black becomes set.
    uint16_t convertColor(uint16_t color) override { return color ? 1 : 0; }

    // Print the canvas to Serial, throttled so it cannot flood the port.
    bool display() override
    {
        if (_canvas.getBuffer() == nullptr)
        {
            return false;
        }

        unsigned long now = millis();
        if (now - _lastDump < _dumpIntervalMs)
        {
            return true; // Drawn, just not printed this time
        }
        _lastDump = now;

        Serial.print(F("--- mirror "));
        Serial.print(_canvas.width());
        Serial.print('x');
        Serial.print(_canvas.height());
        Serial.println(F(" ---"));

        for (int16_t y = 0; y < _canvas.height(); y++)
        {
            for (int16_t x = 0; x < _canvas.width(); x++)
            {
                Serial.write(_canvas.getPixel(x, y) ? '#' : '.');
            }
            Serial.println();
        }

        return true;
    }
};

#endif // MIRROR_TARGET_H
