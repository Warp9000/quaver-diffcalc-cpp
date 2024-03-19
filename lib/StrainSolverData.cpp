#include "StrainSolverData.h"

StrainSolverData::StrainSolverData(StrainSolverHitObject hitObject, float rate)
{
    HitObjects = vector<StrainSolverHitObject>();
    NextStrainSolverDataOnCurrentHand = nullptr;
    ActionStrainCoefficient = 1;
    PatternStrainMultiplier = 1;
    RollManipulationStrainMultiplier = 1;
    JackManipulationStrainMultiplier = 1;
    TotalStrainValue = 0;
    Action = FingerAction::NoFingerAction;
    State = FingerState::NoFingerState;

    StartTime = hitObject.HitObject.StartTime / rate;
    EndTime = hitObject.HitObject.EndTime / rate;
    HitObjects.push_back(hitObject);
}

void StrainSolverData::CalculateStrainValue()
{
    for (auto &hitObject : HitObjects)
    {
        hitObject.StrainValue = ActionStrainCoefficient * PatternStrainMultiplier * RollManipulationStrainMultiplier * JackManipulationStrainMultiplier * hitObject.LnStrainMultiplier;
        TotalStrainValue += hitObject.StrainValue;
    }

    TotalStrainValue /= HitObjects.size();
}

void StrainSolverData::SolveFingerState()
{
    for (auto hitObject : HitObjects)
    {
        State |= hitObject.State;
    }
}
