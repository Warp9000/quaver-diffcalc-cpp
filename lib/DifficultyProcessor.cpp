#include "DifficultyProcessor.h"
#include "ModHelper.h"
#include "math.h"
#include <numeric>
#include <algorithm>

DifficultyProcessor::DifficultyProcessor(Qua *map, ModIdentifier mods, bool detailedSolve)
{
    Map = map;

    // Don't bother calculating map difficulty if there's less than 2 hit objects
    if (map->HitObjects.size() < 2)
        return;

    // Solve for difficulty
    CalculateDifficulty(mods);

    // If detailed solving is enabled, expand calculation
    if (detailedSolve)
    {
        // ComputeNoteDensityData();
        ComputeForPatternFlags();
    }
}

void DifficultyProcessor::CalculateDifficulty(ModIdentifier mods)
{
    // If map does not exist, ignore calculation.
    if (Map == nullptr)
    {
        printf("Map is null\n");
        return;
    }

    // Get song rate from selected mods
    float rate = ModHelper::GetRateFromMods(mods);

    // Compute for overall difficulty
    switch (Map->Mode)
    {
    case GameMode::Keys4:
        OverallDifficulty = ComputeForOverallDifficulty(rate);
        break;
    case GameMode::Keys7:
        OverallDifficulty = (ComputeForOverallDifficulty(rate, Hand::Left) + ComputeForOverallDifficulty(rate, Hand::Right)) / 2;
        break;
    }
}

Hand DifficultyProcessor::LaneToHand(long lane)
{
    long keyCount = Map->GetKeyCount();
    long half = keyCount / 2;

    if (keyCount % 2 == 0)
    {
        if (lane <= half)
            return Hand::Left;
        else
            return Hand::Right;
    }
    else
    {
        if (lane < half)
            return Hand::Left;
        else if (lane > half)
            return Hand::Right;
        else
            return Hand::Ambiguous;
    }
}

FingerState DifficultyProcessor::LaneToFinger(long lane)
{
    switch (Map->Mode)
    {
    case GameMode::Keys4:
        switch (lane)
        {
        case 1:
            return FingerState::Middle;
            break;
        case 2:
            return FingerState::Index;
            break;
        case 3:
            return FingerState::Index;
            break;
        case 4:
            return FingerState::Middle;
            break;
        }
        break;
    case GameMode::Keys7:
        switch (lane)
        {
        case 1:
            return FingerState::Ring;
            break;
        case 2:
            return FingerState::Middle;
            break;
        case 3:
            return FingerState::Index;
            break;
        case 4:
            return FingerState::Thumb;
            break;
        case 5:
            return FingerState::Index;
            break;
        case 6:
            return FingerState::Middle;
            break;
        case 7:
            return FingerState::Ring;
            break;
        }
        break;
    }

    throw "Invalid lane";
}

float DifficultyProcessor::ComputeForOverallDifficulty(float rate, Hand assumeHand)
{
    ComputeNoteDensityData(rate);
    ComputeBaseStrainStates(rate, assumeHand);
    ComputeForChords();
    ComputeForFingerActions();
    // todo: use ComputeForActionPatterns();
    ComputeForRollManipulation();
    ComputeForJackManipulation();
    ComputeForLnMultiplier();
    return CalculateOverallDifficulty();
}

void DifficultyProcessor::ComputeBaseStrainStates(float rate, Hand assumeHand)
{
    // Add hit objects from qua map to qssData
    for (long i = 0; i < static_cast<long>(Map->HitObjects.size()); i++)
    {
        if (Map->HasScratchKey && Map->HitObjects[i].Lane == Map->GetKeyCount())
            continue;

        auto curHitOb = StrainSolverHitObject(Map->HitObjects[i]);
        auto curStrainData = StrainSolverData(curHitOb, rate);

        // Assign Finger and Hand States
        switch (Map->Mode)
        {
        case GameMode::Keys4:
            curStrainData.HitObjects[0].State = LaneToFinger(Map->HitObjects[i].Lane);
            curStrainData.hand = LaneToHand(Map->HitObjects[i].Lane);
            break;
        case GameMode::Keys7:
            curStrainData.HitObjects[0].State = LaneToFinger(Map->HitObjects[i].Lane);
            curStrainData.hand = LaneToHand(Map->HitObjects[i].Lane) == Hand::Ambiguous
                                     ? assumeHand
                                     : LaneToHand(Map->HitObjects[i].Lane);
            break;
        }

        // Add Strain Solver Data to list
        SolverData.push_back(curStrainData);
    }
}

void DifficultyProcessor::ComputeForChords()
{
    // Search through whole hit object list and find chords
    for (long i = 0; i < static_cast<long>(SolverData.size()) - 1; i++)
    {
        for (long j = i + 1; j < static_cast<long>(SolverData.size()); j++)
        {
            // Check if next hit object is way past the tolerance
            float msDiff = SolverData[j].StartTime - SolverData[i].StartTime;
            if (msDiff > StrainConstants.ChordClumpToleranceMs)
                break;

            // Check if the next and current hit objects are chord-able
            if (abs(msDiff) <= StrainConstants.ChordClumpToleranceMs)
            {
                if (SolverData[i].hand == SolverData[j].hand)
                {
                    // Search through every hit object for chords
                    for (auto k : SolverData[j].HitObjects)
                    {
                        // Check if the current data point will have duplicate finger state to prevent stacked notes
                        bool sameStateFound = false;
                        for (auto l : SolverData[i].HitObjects)
                        {
                            if (l.State == k.State)
                            {
                                sameStateFound = true;
                            }
                        }

                        // Add hit object to chord list if its not stacked
                        if (!sameStateFound)
                            SolverData[i].HitObjects.push_back(k);
                    }

                    // Remove un-needed data point because it has been merged with the current point
                    SolverData.erase(SolverData.begin() + j);
                }
            }
        }
    }

    // Solve finger state of every object once chords have been found and applied.
    for (long i = 0; i < static_cast<long>(SolverData.size()); i++)
    {
        SolverData[i].SolveFingerState();
    }
}

void DifficultyProcessor::ComputeForFingerActions()
{
    // Solve for Finger Action
    for (long i = 0; i < static_cast<long>(SolverData.size()) - 1; i++)
    {
        auto curHitOb = &SolverData[i];

        // Find the next Hit Object in the current Hit Object's Hand
        for (long j = i + 1; j < static_cast<long>(SolverData.size()); j++)
        {
            auto nextHitOb = &SolverData[j];
            if (curHitOb->hand == nextHitOb->hand && nextHitOb->StartTime > curHitOb->StartTime)
            {
                // Determined by if there's a minijack within 2 set of chords/single notes
                bool actionJackFound = (curHitOb->State & nextHitOb->State) != 0;

                // Determined by if a chord is found in either finger state
                bool actionChordFound = curHitOb->HandChord() || nextHitOb->HandChord();

                // Determined by if both fingerstates are exactly the same
                bool actionSameState = curHitOb->State == nextHitOb->State;

                // Determined by how long the current finger action is
                float actionDuration = nextHitOb->StartTime - curHitOb->StartTime;

                // Apply the "NextStrainSolverDataOnCurrentHand" value on the current hit object and also apply action duration.
                curHitOb->NextStrainSolverDataOnCurrentHand = nextHitOb;
                curHitOb->FingerActionDurationMs = actionDuration;

                // Trill/Roll
                if (!actionChordFound && !actionSameState)
                {
                    curHitOb->Action = FingerAction::Roll;
                    curHitOb->ActionStrainCoefficient = GetCoefficientValue(actionDuration,
                                                                            StrainConstants.RollLowerBoundaryMs,
                                                                            StrainConstants.RollUpperBoundaryMs,
                                                                            StrainConstants.RollMaxStrainValue,
                                                                            StrainConstants.RollCurveExponential);
                }

                // Simple Jack
                else if (actionSameState)
                {
                    curHitOb->Action = FingerAction::SimpleJack;
                    curHitOb->ActionStrainCoefficient = GetCoefficientValue(actionDuration,
                                                                            StrainConstants.SJackLowerBoundaryMs,
                                                                            StrainConstants.SJackUpperBoundaryMs,
                                                                            StrainConstants.SJackMaxStrainValue,
                                                                            StrainConstants.SJackCurveExponential);
                }

                // Tech Jack
                else if (actionJackFound)
                {
                    curHitOb->Action = FingerAction::TechnicalJack;
                    curHitOb->ActionStrainCoefficient = GetCoefficientValue(actionDuration,
                                                                            StrainConstants.TJackLowerBoundaryMs,
                                                                            StrainConstants.TJackUpperBoundaryMs,
                                                                            StrainConstants.TJackMaxStrainValue,
                                                                            StrainConstants.TJackCurveExponential);
                }

                // Bracket
                else
                {
                    curHitOb->Action = FingerAction::Bracket;
                    curHitOb->ActionStrainCoefficient = GetCoefficientValue(actionDuration,
                                                                            StrainConstants.BracketLowerBoundaryMs,
                                                                            StrainConstants.BracketUpperBoundaryMs,
                                                                            StrainConstants.BracketMaxStrainValue,
                                                                            StrainConstants.BracketCurveExponential);
                }

                break;
            }
        }
    }
}

void DifficultyProcessor::ComputeForActionPatterns()
{
}

void DifficultyProcessor::ComputeForRollManipulation()
{
    long manipulationIndex = 0;

    for (auto &data : SolverData)
    {
        // Reset manipulation found
        bool manipulationFound = false;

        // Check to see if the current data point has two other following points
        if (data.NextStrainSolverDataOnCurrentHand != nullptr &&
            data.NextStrainSolverDataOnCurrentHand->NextStrainSolverDataOnCurrentHand != nullptr)
        {
            auto middle = data.NextStrainSolverDataOnCurrentHand;
            auto last = data.NextStrainSolverDataOnCurrentHand->NextStrainSolverDataOnCurrentHand;

            if (data.Action == FingerAction::Roll && middle->Action == FingerAction::Roll)
            {
                if (data.State == last->State)
                {
                    // Get action duration ratio from both actions
                    float durationRatio = std::max(data.FingerActionDurationMs / middle->FingerActionDurationMs, middle->FingerActionDurationMs / data.FingerActionDurationMs);

                    // If the ratio is above this threshold, count it as a roll manipulation
                    if (durationRatio >= StrainConstants.RollRatioToleranceMs)
                    {
                        // Apply multiplier
                        // todo: catch possible arithmetic error (division by 0)
                        float durationMultiplier = 1 / (1 + (durationRatio - 1) * StrainConstants.RollRatioMultiplier);

                        float manipulationFoundRatio = 1 - manipulationIndex / StrainConstants.RollMaxLength * (1 - StrainConstants.RollLengthMultiplier);
                        data.RollManipulationStrainMultiplier = durationMultiplier * manipulationFoundRatio;

                        // Count manipulation
                        manipulationFound = true;
                        RollInaccuracyConfidence++;

                        if (manipulationIndex < StrainConstants.RollMaxLength)
                            manipulationIndex++;
                    }
                }
            }
        }

        // subtract manipulation index if manipulation was not found
        if (!manipulationFound && manipulationIndex > 0)
            manipulationIndex--;
    }
}

void DifficultyProcessor::ComputeForJackManipulation()
{
    long longJackSize = 0;

    for (auto &data : SolverData)
    {
        // Reset manipulation found
        bool manipulationFound = false;

        // Check to see if the current data point has a following data point
        if (data.NextStrainSolverDataOnCurrentHand != nullptr)
        {
            auto next = data.NextStrainSolverDataOnCurrentHand;

            if (data.Action == FingerAction::SimpleJack && next->Action == FingerAction::SimpleJack)
            {
                // Apply multiplier
                // todo: catch possible arithmetic error (division by 0)
                // note:    83.3ms = 180bpm 1/4 vibro
                //          88.2ms = 170bpm 1/4 vibro
                //          93.7ms = 160bpm 1/4 vibro

                // 35f = 35ms tolerance before hitting vibro point (88.2ms, 170bpm vibro)
                float durationValue = std::min(1.0f, std::max(0.0f, (StrainConstants.VibroActionDurationMs + StrainConstants.VibroActionToleranceMs - data.FingerActionDurationMs) / StrainConstants.VibroActionToleranceMs));

                float durationMultiplier = 1 - durationValue * (1 - StrainConstants.VibroMultiplier);
                float manipulationFoundRatio = 1 - longJackSize / StrainConstants.VibroMaxLength * (1 - StrainConstants.VibroLengthMultiplier);
                data.RollManipulationStrainMultiplier = durationMultiplier * manipulationFoundRatio;

                // Count manipulation
                manipulationFound = true;
                VibroInaccuracyConfidence++;

                if (longJackSize < StrainConstants.VibroMaxLength)
                    longJackSize++;
            }
        }

        // Reset manipulation count if manipulation was not found
        if (!manipulationFound)
            longJackSize = 0;
    }
}

void DifficultyProcessor::ComputeForLnMultiplier()
{
    for (auto &data : SolverData)
    {
        // Check if data is LN
        if (data.EndTime > data.StartTime)
        {
            float durationValue = 1 - std::min(1.0f, std::max(0.0f, (StrainConstants.LnLayerThresholdMs + StrainConstants.LnLayerToleranceMs - (data.EndTime - data.StartTime)) / StrainConstants.LnLayerToleranceMs));
            float baseMultiplier = 1 + durationValue * StrainConstants.LnBaseMultiplier;

            for (auto k : data.HitObjects)
                k.LnStrainMultiplier = baseMultiplier;

            // Check if next data point exists on current hand
            if (data.NextStrainSolverDataOnCurrentHand != nullptr)
            {
                auto next = data.NextStrainSolverDataOnCurrentHand;

                // Check to see if the target hitobject is layered inside the current LN
                if (next->StartTime < data.EndTime - StrainConstants.LnEndThresholdMs)
                {
                    if (next->StartTime >= data.StartTime + StrainConstants.LnEndThresholdMs)
                    {
                        // Target hitobject's LN ends after current hitobject's LN end.
                        if (next->EndTime > data.EndTime + StrainConstants.LnEndThresholdMs)
                        {
                            for (auto &k : data.HitObjects)
                            {
                                k.LnType = LnLayerType::OutsideRelease;
                                k.LnStrainMultiplier *= StrainConstants.LnReleaseAfterMultiplier;
                            }
                        }

                        // Target hitobject's LN ends before current hitobject's LN end
                        else if (next->EndTime > 0)
                        {
                            for (auto &k : data.HitObjects)
                            {
                                k.LnType = LnLayerType::InsideRelease;
                                k.LnStrainMultiplier *= StrainConstants.LnReleaseBeforeMultiplier;
                            }
                        }

                        // Target hitobject is not an LN
                        else
                        {
                            for (auto &k : data.HitObjects)
                            {
                                k.LnType = LnLayerType::InsideTap;
                                k.LnStrainMultiplier *= StrainConstants.LnTapMultiplier;
                            }
                        }
                    }
                }
            }
        }
    }
}

void DifficultyProcessor::ComputeForPatternFlags()
{
    // If 10% or more of the map has longjack manip, flag it as vibro map
    if (VibroInaccuracyConfidence / SolverData.size() > 0.10)
        PatternFlags |= QssPatternFlags::SimpleVibro;

    // If 15% or more of the map has roll manip, flag it as roll map
    if (RollInaccuracyConfidence / SolverData.size() > 0.15)
        PatternFlags |= QssPatternFlags::Rolls;
}

float DifficultyProcessor::CalculateOverallDifficulty()
{
    float calculatedDiff;

    // Solve strain value of every data point
    for (auto &data : SolverData)
    {
        data.CalculateStrainValue();
    }

    float sum = 0.0f;
    int count = 0;
    float max = __FLT_MIN__, min = __FLT_MAX__;
    for (const auto &s : SolverData)
    {
        if (s.hand == Hand::Left || s.hand == Hand::Right)
        {
            sum += s.TotalStrainValue;
            count++;
        }
        max = std::max(max, s.TotalStrainValue);
        min = std::min(min, s.TotalStrainValue);
    }
    calculatedDiff = count > 0 ? sum / count : 0.0f;

    vector<float> bins = vector<float>();
    const int binSize = 1000;

    // float mapStart = std::min_element(SolverData.begin(), SolverData.end(), [](const auto &s1, const auto &s2)
    //                                   { return s1.StartTime < s2.StartTime; })
    //                      ->StartTime;

    // float mapEnd = std::max_element(SolverData.begin(), SolverData.end(), [](const auto &s1, const auto &s2)
    //                                 { return std::max(s1.StartTime, s1.EndTime) < std::max(s2.StartTime, s2.EndTime); })
    //                    ->EndTime;
    float mapStart = __FLT_MAX__, mapEnd = __FLT_MIN__;

    for (auto s : SolverData)
    {
        mapStart = std::min(mapStart, s.StartTime);
        mapEnd = std::max(mapEnd, std::max(s.StartTime, s.EndTime));
    }

    for (float i = mapStart; i < mapEnd; i += binSize)
    {
        auto valuesInBin = vector<float>();
        for (auto s : SolverData)
        {
            if (s.StartTime >= i && s.StartTime < i + binSize)
            {
                valuesInBin.push_back(s.TotalStrainValue);
            }
        }
        float averageRating = valuesInBin.size() > 0 ? std::accumulate(valuesInBin.begin(), valuesInBin.end(), 0.0f) / valuesInBin.size() : 0;

        bins.push_back(averageRating);
    }

    bool hasPositiveStrain = std::any_of(bins.begin(), bins.end(), [](float strain)
                                         { return strain > 0; });

    if (!hasPositiveStrain)
        return 0;

    /*
     * Having a section where notes are relatively easy, compared to the hardest sections of the map, drops the rating way more than it should.
     *
     * This part computes a "continuity" value, the higher it is the less "easy" notes it has.
     * Maps with a higher continuity should get nerfed, since they are overrated right now.
     *
     * The continuity is computed by taking a comparison value, set as the average of the hardest 40% of all sections.
     * All sections are compared to that value, the average is the continuity.
     * A square root is used to decrease the gap between easy sections and hard sections and considered the core of this adjustment.
     */

    // Average of the hardest 40% of the map
    long cutoffPos = static_cast<long>(std::floor(bins.size() * 0.4));
    std::sort(bins.begin(), bins.end(), std::greater<float>());
    std::vector<float> top40(bins.begin(), bins.begin() + cutoffPos);
    std::vector<float> enumerable = top40;
    float easyRatingCutoff = (enumerable.size() > 0) ? std::accumulate(enumerable.begin(), enumerable.end(), 0.0f) / enumerable.size() : 0;

    // We do not consider sections without notes, since there are no "easy notes". Those sections have barely
    // affected the rating in the old difficulty calculator.
    float continuity = 0.0f;
    count = 0;
    for (float strain : bins)
    {
        if (strain > 0)
        {
            continuity += sqrt(strain / easyRatingCutoff);
            count++;
        }
    }
    if (count > 0)
    {
        continuity /= count;
    }

    // The average continuity of maps in our dataset has been observed to be around 0.85, so we take that
    // as the reference value to keep the rating the same.
    const float maxContinuity = 1.00f;
    const float avgContinuity = 0.85f;
    const float minContinuity = 0.60f;

    const float maxAdjustment = 1.05f;
    const float avgAdjustment = 1.00f;
    const float minAdjustment = 0.90f;

    float continuityAdjustment;

    if (continuity > avgContinuity)
    {
        float continuityFactor = 1 - (continuity - avgContinuity) / (maxContinuity - avgContinuity);
        continuityAdjustment = std::min(avgAdjustment, std::max(minAdjustment, continuityFactor * (avgAdjustment - minAdjustment) + minAdjustment));
    }
    else
    {
        float continuityFactor = 1 - (continuity - minContinuity) / (avgContinuity - minContinuity);
        continuityAdjustment = std::min(maxAdjustment, std::max(avgAdjustment, continuityFactor * (maxAdjustment - avgAdjustment) + avgAdjustment));
    }

    calculatedDiff *= continuityAdjustment;

    /*
     * Nerf short maps below 60 seconds of true drain time (time where a player is actually playing hard/moderate parts)
     * We derive the "true drain time" from the continuity.
     * There is no need to measure map length by taking the first/last note or strain data, since the bin size is already known.
     */
    const float maxShortMapAdjustment = 0.75f;
    const float shortMapThreshold = 60 * SECONDS_TO_MILLISECONDS; // Start applying the nerf once a map is shorter

    float trueDrainTime = bins.size() * continuity * binSize;
    float shortMapAdjustment = std::min(1.0f, std::max(maxShortMapAdjustment, 0.25f * sqrt(trueDrainTime / shortMapThreshold) + 0.75f));

    calculatedDiff *= shortMapAdjustment;

    return calculatedDiff;
}

void DifficultyProcessor::ComputeNoteDensityData(float rate)
{
    // MapLength = Qua.Length;
    AverageNoteDensity = SECONDS_TO_MILLISECONDS * Map->HitObjects.size() / (Map->GetLength() * (-.5f * rate + 1.5f));
}

float DifficultyProcessor::GetCoefficientValue(float duration, float xMin, float xMax, float strainMax, float exp)
{
    // todo: temp. Linear for now
    // todo: apply cosine curve
    const float lowestDifficulty = 1;
    const float densityMultiplier = .266f;
    const float densityDifficultyMin = .4f;

    // calculate ratio between min and max value
    float ratio = std::max(0.0f, 1 - (duration - xMin) / (xMax - xMin));

    // if ratio is too big and map isnt a beginner map (nps > 4) scale based on nps instead
    if (ratio == 0 && AverageNoteDensity < 4)
    {
        // if note density is too low dont bother calculating for density either
        if (AverageNoteDensity < 1)
            return densityDifficultyMin;

        return AverageNoteDensity * densityMultiplier + .134f;
    }

    // compute for difficulty
    return lowestDifficulty + (strainMax - lowestDifficulty) * std::pow(ratio, exp);
}