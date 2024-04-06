#include "Qua.h"
#include "QssPatternFlags.h"
#include "StrainConstantsKeys.h"
#include "StrainSolverData.h"

class DifficultyProcessor
{
private:
    Qua *Map;

    Hand LaneToHand(long lane);
    FingerState LaneToFinger(long lane);

    float VibroInaccuracyConfidence;
    float RollInaccuracyConfidence;

    float ComputeForOverallDifficulty(float rate, Hand assumeHand = Hand::Right);
    void ComputeBaseStrainStates(float rate, Hand assumeHand);
    void ComputeForChords();
    void ComputeForFingerActions();
    void ComputeForActionPatterns();
    void ComputeForRollManipulation();
    void ComputeForJackManipulation();
    void ComputeForLnMultiplier();
    void ComputeForPatternFlags();
    float CalculateOverallDifficulty();
    void ComputeNoteDensityData(float rate);
    float GetCoefficientValue(float duration, float xMin, float xMax, float strainMax, float exp);

public:
    float OverallDifficulty;
    QssPatternFlags PatternFlags;
    const float SECONDS_TO_MILLISECONDS = 1000;
    const string Version = "0.0.5";
    StrainConstantsKeys StrainConstants;
    float AverageNoteDensity;
    vector<StrainSolverData> SolverData;

    // private Dictionary<int, Hand> LaneToHand4K; = new Dictionary<int, Hand>()
    // {
    //     {1, Hand.Left},
    //     {2, Hand.Left},
    //     {3, Hand.Right},
    //     {4, Hand.Right}
    // };
    // private Dictionary<int, Hand> LaneToHand7K; = new Dictionary<int, Hand>()
    // {
    //     {1, Hand.Left},
    //     {2, Hand.Left},
    //     {3, Hand.Left},
    //     {4, Hand.Ambiguous},
    //     {5, Hand.Right},
    //     {6, Hand.Right},
    //     {7, Hand.Right}
    // };
    // private Dictionary<int, FingerState> LaneToFinger4K; = new Dictionary<int, FingerState>()
    // {
    //     {1, FingerState.Middle},
    //     {2, FingerState.Index},
    //     {3, FingerState.Index},
    //     {4, FingerState.Middle}
    // };
    // private Dictionary<int, FingerState> LaneToFinger7K; = new Dictionary<int, FingerState>()
    // {
    //     {1, FingerState.Ring},
    //     {2, FingerState.Middle},
    //     {3, FingerState.Index},
    //     {4, FingerState.Thumb},
    //     {5, FingerState.Index},
    //     {6, FingerState.Middle},
    //     {7, FingerState.Ring}
    // };

    DifficultyProcessor(Qua *map, ModIdentifier mods = ModIdentifier::NoMod, bool detailedSolve = false);
    void CalculateDifficulty(ModIdentifier mods);
};