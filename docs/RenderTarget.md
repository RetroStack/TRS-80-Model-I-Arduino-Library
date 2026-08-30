# RenderTarget and RenderManager

`RenderTarget` is the interface for a destination that screens can be drawn to. `RenderManager` holds the registered targets and drives them as a group. `DisplayRenderTarget` is the built-in implementation that draws through a `DisplayProvider` - the panel attached to the shield.

## Table of Contents

- [Overview](#overview)
- [RenderTarget Interface](#rendertarget-interface)
- [RenderManager](#rendermanager)
- [DisplayRenderTarget](#displayrendertarget)
- [How M1Shield Uses It](#how-m1shield-uses-it)
- [Writing a Custom Target](#writing-a-custom-target)
- [Notes](#notes)
- [See Also](#see-also)

## Overview

`M1Shield.begin(provider)` creates a `DisplayRenderTarget` around the display provider and registers it as target 0. From then on, `M1Shield`'s display accessors read through the render manager rather than the provider directly:

- `getGFX()`, `getScreenWidth()`, `getScreenHeight()` and `convertColor()` use the **primary** target (index 0).
- `display()` pushes **every enabled** target.

Screens are unaffected - they keep calling `M1Shield.getGFX()` and `M1Shield.display()` as before. The indirection lets additional targets receive the same `display()` without touching any screen code.

## RenderTarget Interface

```cpp
class RenderTarget {
public:
    virtual ~RenderTarget() = default;

    virtual const char* getName() const = 0;
    virtual bool isEnabled() const = 0;
    virtual void setEnabled(bool enabled) = 0;

    virtual Adafruit_GFX& getGFX() = 0;
    virtual uint16_t getScreenWidth() const = 0;
    virtual uint16_t getScreenHeight() const = 0;
    virtual uint16_t convertColor(uint16_t color) = 0;

    virtual bool display() = 0;

    bool isSmallDisplay() const;  // getScreenHeight() <= 128
};
```

`isSmallDisplay()` is non-virtual and is the single definition of the small-display rule.

## RenderManager

```cpp
bool addRenderTarget(RenderTarget* target);     // false if full or already registered
bool removeRenderTarget(RenderTarget* target);  // false if not found
void clearRenderTargets();

uint8_t getRenderTargetCount() const;
RenderTarget* getRenderTarget(uint8_t index) const;   // nullptr when out of range
RenderTarget* getPrimaryRenderTarget() const;         // target 0, or nullptr

bool displayAll();  // push every enabled target
```

Up to `MAX_RENDER_TARGETS` (8) targets may be registered. **Registration order matters**: target 0 is the primary, and is what dimensions, colors and the graphics context are read from.

The manager does **not** own the targets - it never deletes them. `M1Shield` owns the `DisplayRenderTarget` it registers; you own anything you add.

## DisplayRenderTarget

```cpp
explicit DisplayRenderTarget(DisplayProvider& provider);

void setDisplayProvider(DisplayProvider& provider);
DisplayProvider& getDisplayProvider() const;
```

The provider is taken by reference and is required - a target with no surface to draw on has no valid meaning, so it cannot be constructed in that state. The provider is not owned by the target.

## How M1Shield Uses It

```cpp
RenderManager& getRenderManager();  // access the manager directly
```

`begin(provider)` registers the display target; the destructor unregisters and deletes it. To temporarily stop pushing to the panel while keeping other targets live:

```cpp
RenderTarget* panel = M1Shield.getRenderManager().getPrimaryRenderTarget();
if (panel) {
    panel->setEnabled(false);   // drawing still works; display() skips it
}
```

## Writing a Custom Target

A custom target must supply its own `Adafruit_GFX` surface. `GFXcanvas1` is the practical choice on an ATmega2560 - a full-color canvas is not affordable, but a 1-bit canvas is (128x64 is 1 KB).

```cpp
class SerialMirrorTarget : public RenderTarget {
private:
    GFXcanvas1 _canvas;
    bool _enabled;

public:
    SerialMirrorTarget() : _canvas(128, 64), _enabled(true) {}

    const char* getName() const override { return "Serial Mirror"; }
    bool isEnabled() const override      { return _enabled; }
    void setEnabled(bool e) override     { _enabled = e; }

    Adafruit_GFX& getGFX() override           { return _canvas; }
    uint16_t getScreenWidth() const override  { return _canvas.width(); }
    uint16_t getScreenHeight() const override { return _canvas.height(); }
    uint16_t convertColor(uint16_t c) override { return c ? 1 : 0; }

    bool display() override {
        // Emit the canvas buffer over serial, dump it to SD, etc.
        return true;
    }
};

SerialMirrorTarget mirror;

void setup() {
    M1Shield.begin(displayProvider);
    M1Shield.getRenderManager().addRenderTarget(&mirror);
}
```

## Notes

- **SRAM**: each registered target costs 2 bytes in the manager plus whatever the target itself holds. A `GFXcanvas1` costs `width * height / 8` bytes, so keep custom targets small on an 8 KB part.
- **One draw, one surface**: `M1Shield.getGFX()` returns the *primary* target's context, so a single draw pass fills the primary target only. A secondary target that needs its own pixels must render in its `display()`, or the sketch must redraw per target.
- **Lifetime**: a target must outlive its registration. Remove it with `removeRenderTarget()` before destroying it.

## See Also

- [DisplayProvider](DisplayProvider.md) - the panel abstraction a `DisplayRenderTarget` wraps
- [M1Shield](M1Shield.md) - `getRenderManager()`, `getGFX()`, `display()`
- [Screen](Screen.md) - what draws through these targets
