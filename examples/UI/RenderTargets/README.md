# Render Targets Example

Draws one UI to several destinations at once, and lets you switch each one on and off at runtime.

Out of the box this runs on a single panel plus a **serial mirror** target, so it is useful with no extra hardware — you can watch the mirrored UI in the Serial Monitor.

## What It Demonstrates

- Registering more than one `RenderTarget`
- One draw pass reaching every enabled target, with no screen code aware of it
- Toggling a target at runtime with `setEnabled()`
- Writing a custom `RenderTarget` (`MirrorTarget.h`)
- Adding a second physical panel with `M1Shield.addDisplay()`

## Controls

| Input | Action |
|---|---|
| UP / DOWN | Select a render target |
| SELECT (or LEFT) | Toggle the selected target on/off |

The screen lists each registered target's name, its dimensions, and whether it is enabled.

## Files

| File | Purpose |
|---|---|
| `RenderTargets.ino` | Providers, targets, registration, second-panel pin block |
| `MirrorTarget.h` | A custom `RenderTarget` backed by a `GFXcanvas1`, printed to Serial as ASCII |
| `TargetsScreen.h/.cpp` | The `ContentScreen` that lists and toggles targets |

## The Serial Mirror

`MirrorTarget` owns a 1-bit offscreen canvas, so it receives the same drawing the panel does, and prints it to Serial as ASCII art (throttled to once every 5 seconds so it cannot flood the port).

**It costs 1024 bytes of SRAM** for the default 128×64 canvas — 12.5% of the Mega's 8 KB. Construct it as `MirrorTarget(64, 32)` for 256 bytes if your sketch is tight.

A canvas allocation can fail on an 8 KB part. `MirrorTarget::isValid()` reports that, and `isEnabled()` returns false when the buffer is missing so the render pass skips it — `GFXcanvas1`'s line-drawing helpers do not null-check their buffer.

## Adding a Second Physical Panel

Set `SECOND_PANEL_PRESENT` to `true` in the sketch and wire a panel to the pins in the block above it.

It is a `constexpr bool` rather than an `#ifdef` on purpose: the compiler type-checks the registration code either way, so CI catches API breakage even though the default build has no second panel, and `-Os` removes the dead branch.

### Choosing pins

The defaults are CS=5, DC=6, RST=7. Those are free in the **ATmega2560's** pin assignment — the TRS-80 bus claims ports A, C, F, L, J, H, E and D; the shield claims 4, 8–13, 38–43, 49 and A8–A15; and 50–53 are the hardware SPI bus. That leaves 5, 6, 7, 17, 20, 21 and 53.

> **Whether those pins are physically broken out on your board is yours to verify.** Check your shield before wiring anything.

### The reset pin matters

The second panel needs **its own reset line, or none at all** (`-1`).

It must not share the primary's reset pin. `Adafruit_SPITFT::initSPI()` toggles reset unconditionally while initializing, so creating the second panel on the primary's reset line would reset the primary too — clearing its sleep-out, rotation and colour mode — and nothing re-initializes it afterwards. `M1Shield.addDisplay()` refuses this rather than letting it happen silently.

## How It Works

`M1Shield.getGFX()`, `getScreenWidth()`, `getScreenHeight()` and `convertColor()` resolve through the render manager to whichever target is currently being drawn. `M1Shield.renderAll()` nominates each enabled target in turn and runs the drawing once per target, so **existing screen code reaches every target without changing a line**.

Layout adapts per target automatically: `isSmallDisplay()` reads the active target's height, so a 320×240 UI mirrored onto a 128×64 target picks up the small header, footer and progress bar on its own.

## Limits

- **Target 0 is the layout authority.** Disabling the primary stops it being drawn and pushed, but it still defines the layout — otherwise turning the panel off would reflow the whole UI.
- **Drawing outside a `Screen`** (directly in your sketch's `loop()`) reaches only the primary. Wrap it in `M1Shield.renderAll([]{ ... })` if you want it mirrored.
- **Pagination follows the primary.** `MenuScreen`'s current page and `TextFileViewer`'s page slice are decided once, outside the render pass, so a differently-sized secondary shows the primary's slice — possibly clipped or short.
- **Draw time scales with target count.** Two targets means two full draw passes.

## See Also

- [RenderTarget documentation](../../../docs/RenderTarget.md)
- [M1Shield documentation](../../../docs/M1Shield.md)
- [DisplayProvider documentation](../../../docs/DisplayProvider.md)
