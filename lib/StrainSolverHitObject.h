#pragma once

#include "HitObjectInfo.h"
#include "FingerState.h"
#include "LnLayerType.h"

struct StrainSolverHitObject
{
    HitObjectInfo HitObject;
    FingerState State;
    LnLayerType LnType;
    float LnStrainMultiplier;
    float StrainValue;

    StrainSolverHitObject(HitObjectInfo hitObject);
};
