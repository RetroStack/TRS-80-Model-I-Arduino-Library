/*
 * test_utils.cpp - Native tests for the pure helpers in utils
 *
 * These three helpers carry logic that used to live inline in Model1 and
 * FileBrowser, where it could only be compile-checked. Pulled out as free
 * functions over plain types, they run on the host in milliseconds.
 *
 * Run with: make -C extras/test
 *
 * Authors: Marcel Erz (RetroStack)
 * License: MIT
 */

#include "utils.h"
#include <cstdio>
#include <cstring>

static int failures = 0;
#define CHECK(cond, msg)                     \
    do                                       \
    {                                        \
        if (!(cond))                         \
        {                                    \
            printf("  FAIL %s\n", msg);      \
            failures++;                      \
        }                                    \
        else                                 \
            printf("  ok   %s\n", msg);      \
    } while (0)

// Walk a range exactly as the chunked loops in Model1 do, and report how many
// bytes were covered. Returns false if the walk did not terminate promptly,
// which is the defect this helper exists to prevent.
static bool walkRange(uint16_t total, uint16_t chunkSize, uint32_t &covered, uint32_t &iterations)
{
    covered = 0;
    iterations = 0;

    for (uint32_t offset = 0;; offset += chunkSize)
    {
        uint16_t length = chunkLength(offset, total, chunkSize);
        if (length == 0)
        {
            break;
        }

        covered += length;
        if (++iterations > 70000u)
        {
            return false; // Did not terminate
        }
    }

    return true;
}

static void testChunkLength()
{
    printf("chunkLength\n");

    CHECK(chunkLength(0, 100, 64) == 64, "full chunk at the start");
    CHECK(chunkLength(64, 100, 64) == 36, "short final chunk");
    CHECK(chunkLength(100, 100, 64) == 0, "exhausted at exactly total");
    CHECK(chunkLength(200, 100, 64) == 0, "exhausted past total");
    CHECK(chunkLength(0, 0, 64) == 0, "empty range");
    CHECK(chunkLength(0, 100, 0) == 0, "zero chunk size does not spin");

    // The wrap that made dumpMemoryToSD(0x0000, 65535) restart the range: the
    // last full chunk starts at 65472, and a uint16_t offset stepping past it
    // returned to 0 instead of ending the walk.
    CHECK(chunkLength(65472, 65535, 64) == 63, "final 63-byte chunk at the 16-bit boundary");
    CHECK(chunkLength(65535, 65535, 64) == 0, "walk ends at 65535 rather than wrapping");

    uint32_t covered = 0;
    uint32_t iterations = 0;

    CHECK(walkRange(65535, 64, covered, iterations), "64-byte walk of 65535 terminates");
    CHECK(covered == 65535, "64-byte walk covers every byte exactly once");

    CHECK(walkRange(65535, 32, covered, iterations), "32-byte walk of 65535 terminates");
    CHECK(covered == 65535, "32-byte walk covers every byte exactly once");

    CHECK(walkRange(65472, 64, covered, iterations), "walk of an exact multiple terminates");
    CHECK(covered == 65472, "exact multiple covers every byte");
    CHECK(iterations == 1023, "exact multiple takes the expected iteration count");
}

static void testNormalizePath()
{
    char out[64];

    printf("normalizePath\n");

    CHECK(normalizePath("/logs", out, sizeof(out)) && strcmp(out, "/logs") == 0, "already normal");
    CHECK(normalizePath("logs", out, sizeof(out)) && strcmp(out, "/logs") == 0, "leading separator added");
    CHECK(normalizePath("/logs/", out, sizeof(out)) && strcmp(out, "/logs") == 0, "trailing separator removed");
    CHECK(normalizePath("//logs///a", out, sizeof(out)) && strcmp(out, "/logs/a") == 0, "repeated separators squeezed");
    CHECK(normalizePath("/", out, sizeof(out)) && strcmp(out, "/") == 0, "root survives");
    CHECK(normalizePath("", out, sizeof(out)) && strcmp(out, "/") == 0, "empty becomes root");
    CHECK(normalizePath("/logs/./a", out, sizeof(out)) && strcmp(out, "/logs/a") == 0, "dot segment dropped");

    // The traversal the containment check used to miss entirely.
    CHECK(normalizePath("/logs/../secret", out, sizeof(out)) && strcmp(out, "/secret") == 0, "parent segment collapsed");
    CHECK(normalizePath("/logs/a/../b", out, sizeof(out)) && strcmp(out, "/logs/b") == 0, "parent segment collapsed mid-path");
    CHECK(normalizePath("/../..", out, sizeof(out)) && strcmp(out, "/") == 0, "climbing above root clamps at root");
    CHECK(normalizePath("/logs/../../etc", out, sizeof(out)) && strcmp(out, "/etc") == 0, "over-climbing still clamps");

    char small[6];
    CHECK(!normalizePath("/logs/toolong", small, sizeof(small)), "overflow is reported, not truncated");
    CHECK(!normalizePath(nullptr, out, sizeof(out)), "null path rejected");
    CHECK(!normalizePath("/logs", nullptr, sizeof(out)), "null output rejected");
    CHECK(!normalizePath("/logs", out, 1), "capacity below the root case rejected");
}

static void testPathIsWithin()
{
    printf("pathIsWithin\n");

    CHECK(pathIsWithin("/logs", "/logs"), "root itself is contained");
    CHECK(pathIsWithin("/logs/a", "/logs"), "child is contained");
    CHECK(pathIsWithin("/logs/a/b", "/logs"), "grandchild is contained");
    CHECK(pathIsWithin("/anything", "/"), "everything is under root");

    // The prefix-match bypass: "/logsecret" shares five characters with
    // "/logs" but is a sibling, not a child.
    CHECK(!pathIsWithin("/logsecret", "/logs"), "sibling sharing a prefix is not contained");
    CHECK(!pathIsWithin("/logs2", "/logs"), "sibling with a trailing digit is not contained");
    CHECK(!pathIsWithin("/secret", "/logs"), "unrelated path is not contained");
    CHECK(!pathIsWithin("/log", "/logs"), "shorter path is not contained");

    CHECK(pathIsWithin("/logs/a", "/logs/"), "trailing separator on root tolerated");
    CHECK(!pathIsWithin(nullptr, "/logs"), "null path rejected");
    CHECK(!pathIsWithin("/logs", nullptr), "null root rejected");
}

int main()
{
    testChunkLength();
    testNormalizePath();
    testPathIsWithin();

    if (failures)
    {
        printf("%d check(s) failed\n", failures);
        return 1;
    }

    printf("all checks passed\n");
    return 0;
}
