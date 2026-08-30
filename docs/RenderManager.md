# RenderManager

`RenderManager` holds the registered [render targets](RenderTarget.md) and
drives them as a group. `M1Shield` owns one; `M1Shield.getRenderManager()`
returns it.

## Table of Contents

- [API](#api)
- [How M1Shield Uses It](#how-m1shield-uses-it)
- [See Also](#see-also)

## API

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

## See Also

- [RenderTarget](RenderTarget.md) - the interface each registered target implements
- [DisplayRenderTarget](DisplayRenderTarget.md) - the built-in panel target
- [M1Shield](M1Shield.md) - `addDisplay()`, `renderAll()`, `getRenderManager()`
