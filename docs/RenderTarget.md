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

- `getGFX()`, `getScreenWidth()`, `getScreenHeight()` and `convertColor()` resolve to the **active** target - the one currently being drawn during a render pass, or the primary outside one.
- `display()` pushes the active target during a pass, and **every enabled** target outside one.

`M1Shield.renderAll()` runs a drawing operation once per enabled target, nominating each in turn. Because every draw call in the framework already goes through those accessors, **screens reach every target without a single call-site change** - a screen written before render targets existed mirrors correctly.

Layout adapts per target for free: `Screen::isSmallDisplay()` reads the active target's height, so a 320x240 UI mirrored onto a 128x64 target picks up the small header, footer and progress bar on its own.

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

`begin(provider)` registers the display target; the destructor unregisters and deletes it.

```cpp
// Add a second physical panel on its own pins
bool addDisplay(DisplayRenderTarget &target, int8_t cs, int8_t dc, int8_t rst = -1);

// Run a drawing operation once per enabled target
template <typename F> void renderAll(F draw);
```

`addDisplay()` resets and creates the panel, then registers the target - and refuses to register one it could not initialize. It also refuses a panel sharing the primary's reset pin, which would reset the primary panel and leave it blank.

`renderAll()` is public so a sketch can mirror its own partial redraws:

```cpp
M1Shield.renderAll([]{
    Adafruit_GFX &gfx = M1Shield.getGFX();   // resolve INSIDE the callable
    gfx.fillRect(0, 0, 40, 10, M1Shield.convertColor(0x0000));
    M1Shield.display();                      // the callable pushes; the loop does not
});
```

> **Never capture by reference, and never let an `Adafruit_GFX&` cross into the callable.** Resolve `getGFX()` inside it. A reference captured beforehand binds to the primary's canvas, so every target draws into panel 1 - and it compiles cleanly while doing so.

To temporarily stop pushing to the panel while keeping other targets live:

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
- **Target 0 is the layout authority**, enabled or not. Disabling the primary stops it being drawn and pushed, but it still defines the layout - otherwise switching the panel off would reflow the whole UI to whatever target 1 happens to be.
- **Drawing outside a `Screen`** - directly in a sketch's `loop()` - reaches only the primary. Wrap it in `M1Shield.renderAll([]{ ... })` to mirror it.
- **Pagination follows the primary.** `MenuScreen`'s current page and `TextFileViewer`'s page slice are decided outside the render pass, so a differently-sized secondary shows the primary's slice, possibly clipped or short. Layout adapts per target; pagination cannot.
- **Draw time scales with target count** - two targets means two full draw passes.
- **Lifetime**: a target must outlive its registration. Remove it with `removeRenderTarget()` before destroying it.

## See Also

- [DisplayProvider](DisplayProvider.md) - the panel abstraction a `DisplayRenderTarget` wraps
- [M1Shield](M1Shield.md) - `getRenderManager()`, `getGFX()`, `display()`
- [Screen](Screen.md) - what draws through these targets
