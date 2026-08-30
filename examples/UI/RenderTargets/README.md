# Render Target Examples

`M1Shield` keeps a list of render targets. The display panel is target 0;
anything else registered is drawn to as well, from the same drawing code and
without changing a single call site.

Target 0 is the **layout authority**: every size the UI computes comes from it,
whatever else is registered afterwards, and whether or not it is enabled.

## Examples Overview

### SimpleRenderTargets

**Location:** `SimpleRenderTargets/`
**Purpose:** The registry, with one target in it
**Features:**

- `M1Shield.begin()` registering the panel as target 0
- Reading the target count back from the render manager
- Ordinary drawing code reaching every enabled target

**Best for:** Understanding what a render target is before adding a second one.

### ComprehensiveRenderTargets

**Location:** `ComprehensiveRenderTargets/`
**Purpose:** More than one target at once
**Features:**

- A second physical panel on its own chip-select, data/command and reset pins
- A custom `RenderTarget` that mirrors the UI to Serial
- Enabling and disabling targets at runtime
- What happens when the targets are different sizes

**Best for:** Driving two panels, or capturing the UI somewhere other than a
screen.

## A Warning About Pins

The second panel must not share the primary's reset line. The Adafruit driver
toggles reset while initialising, which would reset the primary panel and leave
it blank. `M1Shield.addDisplay()` refuses that case rather than letting it
happen silently.
