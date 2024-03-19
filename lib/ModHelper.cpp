#include "ModHelper.h"

// 1 << 1 to 1 << 15
const long long _05to20x = 0x7FFE;
// 1 << 24 to 1 << 28
const long long _055to095x = 0x1F000000;
// 1 << 33 to 1 << 42
const long long _105to195x = 0xFF80000000;

float ModHelper::GetRateFromMods(ModIdentifier mods)
{
    if (mods == ModIdentifier::NoMod)
        return 1.0f;

    if (mods & _05to20x)
    {
        int i = __builtin_ctzll(mods & _05to20x) + 1;
        return 0.5f + (i * 0.1f);
    }

    if (mods & _055to095x)
    {
        int i = __builtin_ctzll(mods & _055to095x) + 24;
        return 0.55f + ((i - 23) * 0.1f);
    }

    if (mods & _105to195x)
    {
        int i = __builtin_ctzll(mods & _105to195x) + 33;
        return 1.05f + ((i - 32) * 0.1f);
    }

    return 1.0f;
}