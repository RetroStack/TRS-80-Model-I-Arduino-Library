# DisplayRenderTarget

`DisplayRenderTarget` is the built-in [render target](RenderTarget.md) that
draws through a [DisplayProvider](DisplayProvider.md) - a physical panel.
`M1Shield.begin()` creates one for the shield's own panel and registers it as
target 0.

## Table of Contents

- [API](#api)
- [Adding a Second Panel](#adding-a-second-panel)
- [See Also](#see-also)

## API

```cpp
explicit DisplayRenderTarget(DisplayProvider& provider);

void setDisplayProvider(DisplayProvider& provider);
DisplayProvider& getDisplayProvider() const;
```

The provider is taken by reference and is required - a target with no surface to draw on has no valid meaning, so it cannot be constructed in that state. The provider is not owned by the target.

## Adding a Second Panel

Use `M1Shield.addDisplay()` rather than registering the target directly: it
validates the reset pin and calls `begin()` on the target, neither of which
`RenderManager::addRenderTarget()` does.

A second panel must not share the primary's reset line. The Adafruit driver
toggles reset while initialising, which would reset the primary panel and leave
it blank with nothing to re-initialise it. `addDisplay()` refuses that case
rather than letting it happen silently; pass `-1` for a panel with no reset
line of its own.

## See Also

- [RenderTarget](RenderTarget.md) - the interface this implements
- [RenderManager](RenderManager.md) - the registry it is added to
- [DisplayProvider](DisplayProvider.md) - what it draws through
