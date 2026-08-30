/*
 * test_RenderManager.cpp - Native tests for RenderManager
 *
 * RenderManager depends only on the RenderTarget interface, so it can be
 * compiled and run on the host. Everything else in the library reaches AVR
 * registers and can only be compile-checked.
 *
 * Run with: make -C test
 *
 * Authors: Marcel Erz (RetroStack)
 * License: MIT
 */

#include "RenderManager.h"
#include <cstdio>
#include <string>
#include <vector>

static std::vector<std::string> pushed;

class FakeTarget : public RenderTarget {
    const char *_name; bool _enabled; Adafruit_GFX _gfx;
public:
    FakeTarget(const char *n, bool e = true) : _name(n), _enabled(e) {}
    const char *getName() const override { return _name; }
    bool isEnabled() const override { return _enabled; }
    void setEnabled(bool e) override { _enabled = e; }
    Adafruit_GFX &getGFX() override { return _gfx; }
    uint16_t getScreenWidth() const override { return 320; }
    uint16_t getScreenHeight() const override { return 240; }
    uint16_t convertColor(uint16_t c) override { return c; }
    bool display() override { pushed.push_back(std::string("display:") + _name); return true; }
};

static int failures = 0;
#define CHECK(cond, msg) do { if (!(cond)) { printf("  FAIL %s\n", msg); failures++; } else printf("  ok   %s\n", msg); } while (0)

int main() {
    printf("RenderManager behaviour\n");
    RenderManager m;
    FakeTarget a("A"), b("B"), c("C", false);

    CHECK(m.getRenderTargetCount() == 0, "starts empty");
    CHECK(m.getPrimaryRenderTarget() == nullptr, "no primary when empty");
    CHECK(m.getActiveTarget() == nullptr, "no active target when empty");
    CHECK(!m.inRenderPass(), "not in a pass initially");
    CHECK(m.getPassTarget() == nullptr, "pass target null outside a pass");

    CHECK(m.addRenderTarget(&a), "add A");
    CHECK(m.addRenderTarget(&b), "add B");
    CHECK(!m.addRenderTarget(&a), "duplicate add rejected");
    CHECK(m.addRenderTarget(&c), "add C (disabled)");
    CHECK(m.getRenderTargetCount() == 3, "three registered");
    CHECK(m.getPrimaryRenderTarget() == &a, "A is primary");
    CHECK(m.getActiveTarget() == &a, "active falls back to primary");

    // Pass nomination
    m.beginRenderPass(&b);
    CHECK(m.inRenderPass(), "in a pass after begin");
    CHECK(m.getActiveTarget() == &b, "active follows nomination");
    CHECK(m.getPassTarget() == &b, "pass target is B");
    m.endRenderPass();
    CHECK(!m.inRenderPass(), "pass ended");
    CHECK(m.getActiveTarget() == &a, "active back to primary");

    // Primary stays the layout authority even when disabled
    a.setEnabled(false);
    CHECK(m.getActiveTarget() == &a, "disabled primary is still the layout authority");
    a.setEnabled(true);

    // displayAll skips disabled targets
    pushed.clear();
    CHECK(m.displayAll(), "displayAll succeeds");
    CHECK(pushed.size() == 2, "displayAll pushed only the two enabled targets");
    CHECK(pushed[0] == "display:A" && pushed[1] == "display:B", "pushed A and B, not C");

    // Removing the active target must clear the nomination
    m.beginRenderPass(&b);
    CHECK(m.removeRenderTarget(&b), "remove B while it is active");
    CHECK(!m.inRenderPass(), "removing the active target ends the pass");
    CHECK(m.getActiveTarget() == &a, "active falls back after removal");
    CHECK(m.getRenderTargetCount() == 2, "two left");
    CHECK(m.getRenderTarget(0) == &a && m.getRenderTarget(1) == &c, "order preserved after shift");
    CHECK(m.getRenderTarget(2) == nullptr, "out-of-range index is null");

    // Removing a non-member
    CHECK(!m.removeRenderTarget(&b), "removing a non-member fails");

    // clear resets the nomination too
    m.beginRenderPass(&a);
    m.clearRenderTargets();
    CHECK(m.getRenderTargetCount() == 0, "cleared");
    CHECK(!m.inRenderPass(), "clear ends any pass");

    // Capacity
    RenderManager full;
    FakeTarget many[MAX_RENDER_TARGETS + 1] = {
        {"0"},{"1"},{"2"},{"3"},{"4"},{"5"},{"6"},{"7"},{"8"}};
    for (uint8_t i = 0; i < MAX_RENDER_TARGETS; i++) full.addRenderTarget(&many[i]);
    CHECK(full.getRenderTargetCount() == MAX_RENDER_TARGETS, "fills to capacity");
    CHECK(!full.addRenderTarget(&many[MAX_RENDER_TARGETS]), "rejects past capacity");
    CHECK(!full.addRenderTarget(nullptr), "rejects null");

    printf(failures ? "\n%d FAILURE(S)\n" : "\nall checks passed\n", failures);
    return failures ? 1 : 0;
}
