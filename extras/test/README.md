# Native Tests

Host-compiled tests for the parts of this library that do not touch hardware.

```sh
make -C extras/test
```

Most of `src/` reaches AVR registers or the Arduino core and can only be compile-checked. Two parts are exceptions and are tested here:

- `RenderManager` depends solely on the `RenderTarget` interface.
- The free helpers in `utils` operate on plain types, so the logic they carry runs on the host even though their callers cannot.

## What Is Covered

`test_RenderManager.cpp` exercises the render-target registry and the render-pass state:

- Registration: add, duplicate rejection, null rejection, capacity limit
- Removal: index shifting, non-member removal, `clearRenderTargets()`
- Pass state: `beginRenderPass()` / `endRenderPass()`, `inRenderPass()`, `getPassTarget()`
- `getActiveTarget()` falling back to the primary outside a pass
- The primary remaining the layout authority while disabled
- `displayAll()` pushing only enabled targets
- Removing the *active* target ending the pass, so a pass can never point at a target that is gone

## What Is Covered — `test_utils.cpp`

`chunkLength()`, `normalizePath()` and `pathIsWithin()` exist so that logic which used to sit inline in `Model1` and `FileBrowser` — where nothing could execute it — can be tested:

- Chunked walks of a 65535-byte range terminate and cover every byte exactly once. A `uint16_t` offset stepping by the chunk size wraps to 0 at 65536 and restarts the range; the tests pin the boundary cases that exposed it.
- Path normalization collapses `.` and `..`, squeezes repeated separators, and clamps a climb above root instead of escaping it.
- Containment compares whole path components, so `/logs` does not contain `/logsecret`.

## Stubs

`stub/Arduino.h` and `stub/Adafruit_GFX.h` provide the minimum the headers need — fixed-width integer types, and an empty polymorphic `Adafruit_GFX` that the manager never calls through. The stubs grow only when a test needs them; `utils.cpp` guards its inline AVR assembly behind `#ifdef __AVR__` so the translation unit compiles on the host untouched.

## Adding a Test

Add `test_<Thing>.cpp`, list it in `TESTS` in the `Makefile`, and give it a rule. A test returns non-zero on failure so `make` and CI stop.
