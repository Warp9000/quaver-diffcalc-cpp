#include "StrainSolverHitObject.h"

StrainSolverHitObject::StrainSolverHitObject(HitObjectInfo hitObject)
{
    State = FingerState::NoFingerState;
    LnType = LnLayerType::None;
    LnStrainMultiplier = 1;
    StrainValue = 0;

    HitObject = hitObject;
}