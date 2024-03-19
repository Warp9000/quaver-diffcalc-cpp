#pragma once

enum ModIdentifier : long long
{
    NoMod = -1LL,
    NoSliderVelocity = 1LL << 0, // No Slider Velocity
    Speed05X = 1LL << 1,         // Speed 0.5x,
    Speed06X = 1LL << 2,         // Speed 0.6x
    Speed07X = 1LL << 3,         // Speed 0.7x
    Speed08X = 1LL << 4,         // Speed 0.8x
    Speed09X = 1LL << 5,         // Speed 0.9x
    Speed11X = 1LL << 6,         // Speed 1.1x
    Speed12X = 1LL << 7,         // Speed 1.2x
    Speed13X = 1LL << 8,         // Speed 1.3x
    Speed14X = 1LL << 9,         // Speed 1.4x
    Speed15X = 1LL << 10,        // Speed 1.5x
    Speed16X = 1LL << 11,        // Speed 1.6x
    Speed17X = 1LL << 12,        // Speed 1.7x
    Speed18X = 1LL << 13,        // Speed 1.8x
    Speed19X = 1LL << 14,        // Speed 1.9x
    Speed20X = 1LL << 15,        // Speed 2.0x
    Strict = 1LL << 16,          // Makes the accuracy hit windows harder
    Chill = 1LL << 17,           // Makes the accuracy hit windows easier
    NoPause = 1LL << 18,         // Disallows pausing.
    Autoplay = 1LL << 19,        // The game automatically plays it.
    Paused = 1LL << 20,          // The user paused during gameplay.
    NoFail = 1LL << 21,          // Unable to fail during gameplay.
    NoLongNotes = 1LL << 22,     // Converts LNs into regular notes.
    Randomize = 1LL << 23,       // Randomizes the playfield's lanes.
    Speed055X = 1LL << 24,       // Speed 0.55x,
    Speed065X = 1LL << 25,       // Speed 0.65x
    Speed075X = 1LL << 26,       // Speed 0.75x
    Speed085X = 1LL << 27,       // Speed 0.85x
    Speed095X = 1LL << 28,       // Speed 0.95x
    Inverse = 1LL << 29,         // Converts regular notes into LNs and LNs into gaps.
    FullLN = 1LL << 30,          // Converts regular notes into LNs, keeps existing LNs.
    Mirror = 1LL << 31,          // Flips the map horizontally
    Coop = 1LL << 32,            // Allows multiple people to play together on one client
    Speed105X = 1LL << 33,       // Speed 1.05x
    Speed115X = 1LL << 34,       // Speed 1.15x
    Speed125X = 1LL << 35,       // Speed 1.25x
    Speed135X = 1LL << 36,       // Speed 1.35x
    Speed145X = 1LL << 37,       // Speed 1.45x
    Speed155X = 1LL << 38,       // Speed 1.55x
    Speed165X = 1LL << 39,       // Speed 1.65x
    Speed175X = 1LL << 40,       // Speed 1.75x
    Speed185X = 1LL << 41,       // Speed 1.85x
    Speed195X = 1LL << 42,       // Speed 1.95x
    HeatlthAdjust = 1LL << 43,   // Test mod for making long note windows easier
    NoMiss = 1LL << 44           // You miss, you die
};