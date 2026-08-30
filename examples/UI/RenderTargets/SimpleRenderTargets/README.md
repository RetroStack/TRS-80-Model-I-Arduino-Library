# Simple Render Targets

The smallest multi-target case: the panel, and the render manager that owns it.

## What You'll Learn

- That `M1Shield.begin()` registers the display panel as render target 0
- That target 0 is the **layout authority** -- every size the UI computes comes
  from it, no matter what else is registered afterwards
- That ordinary drawing code reaches every enabled target without changing a
  single call site

## What It Does

Opens a console screen and writes three lines. Those lines are drawn once by
your code and pushed to every registered target. The sketch prints the target
count to Serial so you can see the registry is real.

## Wiring

Nothing beyond the shield and a display.

## Next

`ComprehensiveRenderTargets` adds a second physical panel on its own pins, a
target that mirrors the UI to Serial, and turning targets on and off at
runtime.
