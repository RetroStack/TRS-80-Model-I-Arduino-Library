/*
 * Adafruit_GFX.h - Minimal stub for native (host) tests
 *
 * RenderTarget's interface returns an Adafruit_GFX reference but the manager
 * never calls through it, so an empty polymorphic type is enough.
 *
 * Authors: Marcel Erz (RetroStack)
 * License: MIT
 */

#ifndef TEST_STUB_ADAFRUIT_GFX_H
#define TEST_STUB_ADAFRUIT_GFX_H

#include <cstdint>

class Adafruit_GFX
{
public:
    virtual ~Adafruit_GFX() {}
};

#endif // TEST_STUB_ADAFRUIT_GFX_H
