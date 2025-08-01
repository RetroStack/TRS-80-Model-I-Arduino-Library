#ifndef VRAMTESTSUITE_H
#define VRAMTESTSUITE_H

#include "RAMTestSuite.h"

/**
 * VRAMTestSuite - Specialized RAM testing for Video RAM (1KB at 0x3C00-0x3FFF)
 *
 * This class inherits from RAMTestSuite and focuses specifically on testing the
 * TRS-80 Model 1's video RAM. The VRAM consists of 1KB of memory mapped from
 * 0x3C00 to 0x3FFF, implemented using 8 SRAM chips that store the character
 * data displayed on screen.
 *
 * IC References:
 * - Z48, Z47, Z46, Z45, Z61, Z62, Z?, Z63 (8 VRAM chips)
 *
 * Features:
 * - Comprehensive VRAM-specific testing
 * - Progress tracking during tests
 * - Per-bit error analysis mapped to specific ICs
 * - Automatic TEST signal control
 * - Video memory pattern verification
 */
class VRAMTestSuite : public RAMTestSuite
{
public:
    VRAMTestSuite();
    Screen *actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY) override;

protected:
    void _executeOnce() override;
};

#endif
