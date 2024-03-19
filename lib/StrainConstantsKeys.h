#pragma once

struct StrainConstantsKeys
{
    // When Long Notes start/end after this threshold, it will be considered for a specific multiplier.
    // Non-Dyanmic Constant. Do not use for optimization.
    const float LnEndThresholdMs = 42;

    // When seperate notes are under this threshold, it will count as a chord.
    // Non-Dyanmic Constant. Do not use for optimization.
    const float ChordClumpToleranceMs = 8;

    // Size of each graph partition in miliseconds.
    // Non-Dyanmic Constant. Do not use for optimization.
    const long GraphIntervalSizeMs = 500;

    // Offset between each graph partition in miliseconds.
    // Non-Dyanmic Constant. Do not use for optimization.
    const long GraphIntervalOffsetMs = 100;

    // Simple Jack
    const float SJackLowerBoundaryMs = 40.0f;
    const float SJackUpperBoundaryMs = 320.0f;
    const float SJackMaxStrainValue = 68.0f;
    const float SJackCurveExponential = 1.17f;

    // Tech Jack
    const float TJackLowerBoundaryMs = 40.0f;
    const float TJackUpperBoundaryMs = 330.0f;
    const float TJackMaxStrainValue = 70.0f;
    const float TJackCurveExponential = 1.14f;

    // Roll/Trill
    const float RollLowerBoundaryMs = 30.0f;
    const float RollUpperBoundaryMs = 230.0f;
    const float RollMaxStrainValue = 55.0f;
    const float RollCurveExponential = 1.13f;

    // Bracket
    const float BracketLowerBoundaryMs = 30.0f;
    const float BracketUpperBoundaryMs = 230.0f;
    const float BracketMaxStrainValue = 56.0f;
    const float BracketCurveExponential = 1.13f;

    // LN
    const float LnBaseMultiplier = 0.6f;
    const float LnLayerToleranceMs = 60.0f;
    const float LnLayerThresholdMs = 93.7f;
    const float LnReleaseAfterMultiplier = 1.0f;
    const float LnReleaseBeforeMultiplier = 1.3f;
    const float LnTapMultiplier = 1.05f;

    // LongJack Manipulation
    const float VibroActionDurationMs = 88.2f;
    const float VibroActionToleranceMs = 88.2f;
    const float VibroMultiplier = 0.75f;
    const float VibroLengthMultiplier = 0.3f;
    const float VibroMaxLength = 6.0f;

    // Roll Manipulation
    const float RollRatioToleranceMs = 2.0f;
    const float RollRatioMultiplier = 0.25f;
    const float RollLengthMultiplier = 0.6f;
    const float RollMaxLength = 14.0f;
};