#pragma once

#include <vector>
#include "KeySoundInfo.h"
#include "HitSounds.h"

using std::vector;

struct HitObjectInfo
{
    long StartTime;
    long Lane;
    long EndTime;
    HitSounds HitSound;
    vector<KeySoundInfo> KeySounds;
    long EditorLayer;

    bool IsLongNote()
    {
        return EndTime > StartTime;
    }
};
