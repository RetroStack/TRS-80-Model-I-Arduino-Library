# Native Tests

Host-compiled tests for the parts of this library that do not touch hardware.

```sh
make -C extras/test
```

Everything else in `src/` reaches AVR registers or the Arduino core and can only be compile-checked. `RenderManager` is the exception: it depends solely on the `RenderTarget` interface, so it compiles and runs with a normal `g++` in about a second.

## What Is Covered

`test_RenderManager.cpp` exercises the render-target registry and the render-pass state:

- Registration: add, duplicate rejection, null rejection, capacity limit
- Removal: index shifting, non-member removal, `clearRenderTargets()`
- Pass state: `beginRenderPass()` / `endRenderPass()`, `inRenderPass()`, `getPassTarget()`
- `getActiveTarget()` falling back to the primary outside a pass
- The primary remaining the layout authority while disabled
- `displayAll()` pushing only enabled targets
- Removing the *active* target ending the pass, so a pass can never point at a target that is gone

## Stubs

`stub/Arduino.h` and `stub/Adafruit_GFX.h` provide the minimum the headers need — fixed-width integer types, and an empty polymorphic `Adafruit_GFX` that the manager never calls through.

## Adding a Test

Add `test_<Thing>.cpp`, list it in `TESTS` in the `Makefile`, and give it a rule. A test returns non-zero on failure so `make` and CI stop.
