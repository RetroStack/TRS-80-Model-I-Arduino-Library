# RenderTarget

`RenderTarget` is the interface for a destination that screens can be drawn to.
A panel is one; so is anything else that can accept the same drawing calls.

See [RenderManager](RenderManager.md) for the registry that holds them, and
[DisplayRenderTarget](DisplayRenderTarget.md) for the built-in implementation
that draws through a `DisplayProvider`.

## Table of Contents

- [Overview](#overview)
- [Interface](#interface)
- [Writing a Custom Target](#writing-a-custom-target)
- [Notes](#notes)
- [See Also](#see-also)

## Overview

`M1Shield.begin(provider)` creates a `DisplayRenderTarget` around the display provider and registers it as target 0. From then on, `M1Shield`'s display accessors read through the render manager rather than the provider directly:

- `getGFX()`, `getScreenWidth()`, `getScreenHeight()` and `convertColor()` resolve to the **active** target - the one currently being drawn during a render pass, or the primary outside one.
- `display()` pushes the active target during a pass, and **every enabled** target outside one.

`M1Shield.renderAll()` runs a drawing operation once per enabled target, nominating each in turn. Because every draw call in the framework already goes through those accessors, **screens reach every target without a single call-site change** - a screen written before render targets existed mirrors correctly.

Layout adapts per target for free: `Screen::isSmallDisplay()` reads the active target's height, so a 320x240 UI mirrored onto a 128x64 target picks up the small header, footer and progress bar on its own.

## Interface

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
- **Target 0 is the layout authority**, enabled or not. Disabling the primary stops it being drawn and pushed, but it still defines the layout - otherwise switching the panel off would reflow the whole UI to whatever target 1 happens to be.
- **Drawing outside a `Screen`** - directly in a sketch's `loop()` - reaches only the primary. Wrap it in `M1Shield.renderAll([]{ ... })` to mirror it.
- **Pagination follows the primary.** `MenuScreen`'s current page and `TextFileViewer`'s page slice are decided outside the render pass, so a differently-sized secondary shows the primary's slice, possibly clipped or short. Layout adapts per target; pagination cannot.
- **Draw time scales with target count** - two targets means two full draw passes.
- **Lifetime**: a target must outlive its registration. Remove it with `removeRenderTarget()` before destroying it.

## See Also

- [RenderManager](RenderManager.md) - the registry and the render pass
- [DisplayRenderTarget](DisplayRenderTarget.md) - the built-in panel target
- [DisplayProvider](DisplayProvider.md) - the panel abstraction a `DisplayRenderTarget` wraps
- [M1Shield](M1Shield.md) - `getRenderManager()`, `getGFX()`, `display()`
- [Screen](Screen.md) - what draws through these targets
