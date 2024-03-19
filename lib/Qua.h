#pragma once

#include <string>
#include <vector>
#include "GameMode.h"
#include "BookmarkInfo.h"
#include "CustomAudioSampleInfo.h"
#include "EditorLayerInfo.h"
#include "HitObjectInfo.h"
#include "SliderVelocityInfo.h"
#include "SoundEffectInfo.h"
#include "TimingPointInfo.h"
#include "ModIdentifier.h"

class Qua
{
public:
    string AudioFile;
    long SongPreviewTime;
    string BackgroundFile;
    string BannerFile;
    long MapId = -1;
    long MapSetId = -1;
    GameMode Mode;
    string Title;
    string Artist;
    string Source;
    string Tags;
    string Creator;
    string DifficultyName;
    string Description;
    string Genre;
    bool BPMDoesNotAffectScrollVelocity;
    float InitialScrollVelocity;
    bool HasScratchKey;
    vector<EditorLayerInfo> EditorLayers;
    vector<BookmarkInfo> Bookmarks;
    vector<CustomAudioSampleInfo> CustomAudioSamples;
    vector<SoundEffectInfo> SoundEffects;
    vector<TimingPointInfo> TimingPoints;
    vector<SliderVelocityInfo> SliderVelocities;
    vector<HitObjectInfo> HitObjects;

    static Qua ParseBuffer(string buffer, bool checkValidity = true);
    static Qua ParseFile(string path, bool checkValidity = true);
    string Serialize();
    void Save(string path);
    bool IsValid();
    void Sort();
    float AverageNotesPerSecond(float rate = 1.0f);
    float GetActionsPerSecond(float rate = 1.0f);
    long GetKeyCount();
    float GetCommonBpm();
    TimingPointInfo GetTimingPointAt(double time);
    SliderVelocityInfo GetSliderVelocityAt(double time);
    double GetTimingPointLength(TimingPointInfo timingPoint);
    // DifficultyProcessorKeys SolveDifficulty(ModIdentifier mods = ModIdentifier::NoMod, bool applyMods = false);
    double SVFactor();
    string ToString();
    void ReplaceLongNotesWithRegularNotes();
    void ApplyInverse();
    void ApplyMods(ModIdentifier mods);
    void RandomizeLanes(long seed);
    void MirrorHitObjects();
    void SortSliderVelocities();
    void SortTimingPoints();
    long GetHitObjectJudgementIndex(HitObjectInfo hitObject);
    HitObjectInfo GetHitObjectAtJudgementIndex(long index);
    static void RestoreDefaultValues(Qua qua);
    void NormalizeSVs();
    void DenormalizeSVs();
    Qua WithNormalizedSVs();
    Qua WithDenormalizedSVs();
    string GetBackgroundPath();
    string GetBannerPath();
    string GetAudioPath();
    long GetLength();

private:
    string FilePath;
    static void AfterLoad(Qua qua, bool checkValidity);
    string GetFullPath(string file);
};