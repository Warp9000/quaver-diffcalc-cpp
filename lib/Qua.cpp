#include "Qua.h"
#include "yaml-cpp/yaml.h"
#include "YamlParser.h"
#include <fstream>
#include "DifficultyProcessorKeys.h"

Qua Qua::ParseBuffer(string buffer, bool checkValidity)
{
    Qua qua = YAML::Load(buffer).as<Qua>();

    if (checkValidity && !qua.IsValid())
        throw std::runtime_error("Invalid qua file.");

    return qua;
}

Qua Qua::ParseFile(string path, bool checkValidity)
{
    Qua qua = YAML::LoadFile(path).as<Qua>();

    if (checkValidity && !qua.IsValid())
        throw std::runtime_error("Invalid qua file.");

    return qua;
}

string Qua::Serialize()
{
    // Sort the object before saving.
    Sort();

    // Set default values to zero so they don't waste space in the .qua file.
    auto originalTimingPoints = TimingPoints;
    auto originalHitObjects = HitObjects;
    auto originalSoundEffects = SoundEffects;
    auto originalBookmarks = Bookmarks;

    TimingPoints = vector<TimingPointInfo>();
    for (auto tp : originalTimingPoints)
    {
        if (tp.Signature == TimeSignature::Quadruple)
        {
            auto t = TimingPointInfo();
            t.Bpm = tp.Bpm;
            t.Signature = (TimeSignature)0;
            t.StartTime = tp.StartTime;
            t.Hidden = tp.Hidden;

            TimingPoints.push_back(t);
        }
        else
        {
            TimingPoints.push_back(tp);
        }
    }

    HitObjects = vector<HitObjectInfo>();
    for (auto obj : originalHitObjects)
    {
        auto keySoundsWithDefaults = vector<KeySoundInfo>();
        for (auto keySound : obj.KeySounds)
        {
            auto k = KeySoundInfo();
            k.Sample = keySound.Sample;
            k.Volume = keySound.Volume == 100 ? 0 : keySound.Volume;

            keySoundsWithDefaults.push_back(k);
        }

        auto h = HitObjectInfo();
        h.EndTime = obj.EndTime;
        h.HitSound = obj.HitSound == HitSounds::Normal ? (HitSounds)0 : obj.HitSound;
        h.KeySounds = keySoundsWithDefaults;
        h.Lane = obj.Lane;
        h.StartTime = obj.StartTime;
        h.EditorLayer = obj.EditorLayer;

        HitObjects.push_back(h);
    }

    SoundEffects = vector<SoundEffectInfo>();
    for (auto info : originalSoundEffects)
    {
        if (info.Volume == 100)
        {
            auto s = SoundEffectInfo();
            s.StartTime = info.StartTime;
            s.Sample = info.Sample;
            s.Volume = 0;
            SoundEffects.push_back(s);
        }
        else
        {
            SoundEffects.push_back(info);
        }
    }

    // Doing this to keep compatibility with older versions of .qua (.osu and .sm file conversions). It won't serialize
    // the bookmarks in the file.
    // if (Bookmarks.size() == 0)
    //     Bookmarks = null;

    YAML::Node node = YAML::Node();

    node = *this;

    string serialized = YAML::Dump(node);

    // Restore the original lists.
    TimingPoints = originalTimingPoints;
    HitObjects = originalHitObjects;
    SoundEffects = originalSoundEffects;
    Bookmarks = originalBookmarks;

    return serialized;
}

void Qua::Save(string path)
{
    std::ofstream file;
    file.open(path);
    file << Serialize();
    file.close();
}

bool Qua::IsValid()
{
    // If there aren't any HitObjects
    if (HitObjects.size() == 0)
        return false;

    // If there aren't any TimingPoints
    if (TimingPoints.size() == 0)
        return false;

    // Check if the mode is actually valid
    if (Mode < 0 || Mode > 3)
        return false;

    // Check that sound effects are valid.
    for (auto info : SoundEffects)
    {
        // Sample should be a valid array index.
        if (info.Sample < 1 || info.Sample >= static_cast<long>(CustomAudioSamples.size()) + 1)
            return false;

        // The sample volume should be between 1 and 100.
        if (info.Volume < 1 || info.Volume > 100)
            return false;
    }

    // Check that hit objects are valid.
    for (auto info : HitObjects)
    {
        // LN end times should be > start times.
        if (info.IsLongNote() && info.EndTime <= info.StartTime)
            return false;

        // Check that key sounds are valid.
        for (auto keySound : info.KeySounds)
        {
            // Sample should be a valid array index.
            if (keySound.Sample < 1 || keySound.Sample >= static_cast<long>(CustomAudioSamples.size()) + 1)
                return false;

            // The sample volume should be above 0.
            if (keySound.Volume < 1)
                return false;
        }
    }

    return true;
}

void Qua::Sort()
{
    std::sort(HitObjects.begin(), HitObjects.end(), [](HitObjectInfo a, HitObjectInfo b)
              { return a.StartTime < b.StartTime; });
    std::sort(TimingPoints.begin(), TimingPoints.end(), [](TimingPointInfo a, TimingPointInfo b)
              { return a.StartTime < b.StartTime; });
    std::sort(SliderVelocities.begin(), SliderVelocities.end(), [](SliderVelocityInfo a, SliderVelocityInfo b)
              { return a.StartTime < b.StartTime; });
    std::sort(SoundEffects.begin(), SoundEffects.end(), [](SoundEffectInfo a, SoundEffectInfo b)
              { return a.StartTime < b.StartTime; });
    std::sort(Bookmarks.begin(), Bookmarks.end(), [](BookmarkInfo a, BookmarkInfo b)
              { return a.StartTime < b.StartTime; });
}

float Qua::AverageNotesPerSecond(float rate)
{
    return HitObjects.size() / (GetLength() / (1000.0f * rate));
}

float Qua::GetActionsPerSecond(float rate)
{
    vector<long> actions = vector<long>();

    for (auto ho : HitObjects)
    {
        actions.push_back(ho.StartTime);

        if (ho.IsLongNote())
            actions.push_back(ho.EndTime);
    }

    if (actions.size() == 0)
        return 0;

    std::sort(actions.begin(), actions.end());

    long length = actions[actions.size() - 1];

    // Remove empty intro time
    length -= actions[0];

    // Exclude break times from the total length
    for (long i = 0; i < static_cast<long>(actions.size()); i++)
    {
        long action = actions[i];

        if (i == 0)
            continue;

        long previousAction = actions[i - 1];

        long difference = action - previousAction;

        if (difference >= 1000)
            length -= difference;
    }

    return actions.size() / (length / (1000.0f * rate));
}

long Qua::GetKeyCount()
{
    switch (Mode)
    {
    case GameMode::Keys4:
        return 4;
        break;
    case GameMode::Keys7:
        return 7;
        break;

    default:
        return 4;
        break;
    }
}

// DifficultyProcessorKeys Qua::SolveDifficulty(ModIdentifier mods, bool applyMods)
// {
//     return *new DifficultyProcessorKeys(this, mods, applyMods);
// }

long Qua::GetLength()
{
    if (HitObjects.size() == 0)
        return 0;

    long max = 0;
    for (auto ho : HitObjects)
    {
        long l = std::max(ho.StartTime, ho.EndTime);

        if (l > max)
            max = l;
    }

    return max;
}
