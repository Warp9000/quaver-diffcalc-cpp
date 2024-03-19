#pragma once

#include <vector>
#include "StrainSolverHitObject.h"
#include "Hand.h"
#include "FingerAction.h"
#include <string>
#include "FingerState.h"

using std::string;
using std::vector;

class StrainSolverData
{
public:
    vector<StrainSolverHitObject> HitObjects;
    StrainSolverData *NextStrainSolverDataOnCurrentHand;
    float StartTime;
    float EndTime;

    float ActionStrainCoefficient;
    float PatternStrainMultiplier;
    float RollManipulationStrainMultiplier;
    float JackManipulationStrainMultiplier;
    float TotalStrainValue;
    Hand hand;
    FingerAction Action;
    float FingerActionDurationMs;
    string Pattern;
    bool HandChord()
    {
        return HitObjects.size() > 1;
    }
    FingerState State;

    StrainSolverData(StrainSolverHitObject hitObject, float rate = 1);
    void CalculateStrainValue();
    void SolveFingerState();
};
