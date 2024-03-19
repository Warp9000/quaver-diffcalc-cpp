#pragma once

#include "TimeSignature.h"

struct TimingPointInfo
{
    float StartTime;
    float Bpm;
    TimeSignature Signature;
    bool Hidden;
};
