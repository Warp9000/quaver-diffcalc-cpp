#pragma once

#include "yaml-cpp/yaml.h"
#include "Qua.h"

namespace YAML
{
    template <>
    struct convert<Qua>
    {
    public:
        static Node encode(const Qua &qua)
        {
            Node node = SerializeInfo(qua);

            SerializeList(node, "EditorLayers", qua.EditorLayers, SerializeEditorLayer);
            SerializeList(node, "Bookmarks", qua.Bookmarks, SerializeBookmark);
            SerializeList(node, "CustomAudioSamples", qua.CustomAudioSamples, SerializeCustomAudioSample);
            SerializeList(node, "SoundEffects", qua.SoundEffects, SerializeSoundEffect);
            SerializeList(node, "TimingPoints", qua.TimingPoints, SerializeTimingPoint);
            SerializeList(node, "SliderVelocities", qua.SliderVelocities, SerializeSliderVelocity);
            SerializeList(node, "HitObjects", qua.HitObjects, SerializeHitObject);

            return node;
        }

        static bool decode(const Node &node, Qua &qua)
        {
            ParseInfo(node, qua);

            ParseList(node, "EditorLayers", qua.EditorLayers, ParseEditorLayer);
            ParseList(node, "Bookmarks", qua.Bookmarks, ParseBookmark);
            ParseList(node, "CustomAudioSamples", qua.CustomAudioSamples, ParseCustomAudioSample);
            ParseList(node, "SoundEffects", qua.SoundEffects, ParseSoundEffect);
            ParseList(node, "TimingPoints", qua.TimingPoints, ParseTimingPoint);
            ParseList(node, "SliderVelocities", qua.SliderVelocities, ParseSliderVelocity);
            ParseList(node, "HitObjects", qua.HitObjects, ParseHitObject);

            return true;
        }

    private:
        template <typename T>
        static void SerializeList(Node &node, string key, const vector<T> &list, Node (*func)(const T &item))
        {
            for (const auto &item : list)
            {
                node[key].push_back(func(item));
            }
        }

        static Node SerializeInfo(const Qua &qua)
        {
            Node node;
            node["AudioFile"] = qua.AudioFile;
            node["SongPreviewTime"] = qua.SongPreviewTime;
            node["BackgroundFile"] = qua.BackgroundFile;
            node["BannerFile"] = qua.BannerFile;
            node["MapId"] = qua.MapId;
            node["MapSetId"] = qua.MapSetId;
            node["Mode"] = qua.Mode;
            node["Title"] = qua.Title;
            node["Artist"] = qua.Artist;
            node["Source"] = qua.Source;
            node["Tags"] = qua.Tags;
            node["Creator"] = qua.Creator;
            node["DifficultyName"] = qua.DifficultyName;
            node["Description"] = qua.Description;
            node["Genre"] = qua.Genre;
            node["BPMDoesNotAffectScrollVelocity"] = qua.BPMDoesNotAffectScrollVelocity;
            node["InitialScrollVelocity"] = qua.InitialScrollVelocity;
            node["HasScratchKey"] = qua.HasScratchKey;
            return node;
        }

        static Node SerializeEditorLayer(const EditorLayerInfo &editorLayer)
        {
            Node node;
            node["Name"] = editorLayer.Name;
            node["Hidden"] = editorLayer.Hidden;
            node["ColorRgb"] = editorLayer.ColorRgb;
            return node;
        }

        static Node SerializeBookmark(const BookmarkInfo &bookmark)
        {
            Node node;
            node["StartTime"] = bookmark.StartTime;
            node["Note"] = bookmark.Note;
            return node;
        }

        static Node SerializeCustomAudioSample(const CustomAudioSampleInfo &customAudioSample)
        {
            Node node;
            node["Path"] = customAudioSample.Path;
            node["UnaffectedByRate"] = customAudioSample.UnaffectedByRate;
            return node;
        }

        static Node SerializeSoundEffect(const SoundEffectInfo &soundEffect)
        {
            Node node;
            node["StartTime"] = soundEffect.StartTime;
            node["Sample"] = soundEffect.Sample;
            node["Volume"] = soundEffect.Volume;
            return node;
        }

        static Node SerializeTimingPoint(const TimingPointInfo &timingPoint)
        {
            Node node;
            node["StartTime"] = timingPoint.StartTime;
            node["Bpm"] = timingPoint.Bpm;
            node["Hidden"] = timingPoint.Hidden;
            return node;
        }

        static Node SerializeSliderVelocity(const SliderVelocityInfo &sliderVelocity)
        {
            Node node;
            node["StartTime"] = sliderVelocity.StartTime;
            node["Multiplier"] = sliderVelocity.Multiplier;
            return node;
        }

        static Node SerializeHitObject(const HitObjectInfo &hitObject)
        {
            Node node;
            node["StartTime"] = hitObject.StartTime;
            node["Lane"] = hitObject.Lane;
            node["EndTime"] = hitObject.EndTime;
            for (const auto &keySound : hitObject.KeySounds)
            {
                Node keySoundNode;
                keySoundNode["Sample"] = keySound.Sample;
                keySoundNode["Volume"] = keySound.Volume;
                node["KeySounds"].push_back(keySoundNode);
            }
            node["EditorLayer"] = hitObject.EditorLayer;
            return node;
        }

        template <typename T>
        static void ParseList(const Node &node, string key, vector<T> &list, T (*func)(const Node &node))
        {
            if (node[key])
            {
                for (const auto &item : node[key])
                {
                    list.push_back(func(item));
                }
            }
        }

        static void ParseInfo(const Node &node, Qua &qua)
        {
            if (node["AudioFile"])
                qua.AudioFile = node["AudioFile"].as<string>();
            else
                qua.AudioFile = "";

            if (node["SongPreviewTime"])
                qua.SongPreviewTime = node["SongPreviewTime"].as<long>();
            else
                qua.SongPreviewTime = 0;

            if (node["BackgroundFile"])
                qua.BackgroundFile = node["BackgroundFile"].as<string>();
            else
                qua.BackgroundFile = "";

            if (node["BannerFile"])
                qua.BannerFile = node["BannerFile"].as<string>();
            else
                qua.BannerFile = "";

            if (node["MapId"])
                qua.MapId = node["MapId"].as<long>();
            else
                qua.MapId = 0;

            if (node["MapSetId"])
                qua.MapSetId = node["MapSetId"].as<long>();
            else
                qua.MapSetId = 0;

            if (node["Mode"])
                qua.Mode = node["Mode"].as<GameMode>();
            else
                qua.Mode = GameMode::Keys4;

            if (node["Title"])
                qua.Title = node["Title"].as<string>();
            else
                qua.Title = "";

            if (node["Artist"])
                qua.Artist = node["Artist"].as<string>();
            else
                qua.Artist = "";

            if (node["Source"])
                qua.Source = node["Source"].as<string>();
            else
                qua.Source = "";

            if (node["Tags"])
                qua.Tags = node["Tags"].as<string>();
            else
                qua.Tags = "";

            if (node["Creator"])
                qua.Creator = node["Creator"].as<string>();
            else
                qua.Creator = "";

            if (node["DifficultyName"])
                qua.DifficultyName = node["DifficultyName"].as<string>();
            else
                qua.DifficultyName = "";

            if (node["Description"])
                qua.Description = node["Description"].as<string>();
            else
                qua.Description = "";

            if (node["Genre"])
                qua.Genre = node["Genre"].as<string>();
            else
                qua.Genre = "";

            if (node["BPMDoesNotAffectScrollVelocity"])
                qua.BPMDoesNotAffectScrollVelocity = node["BPMDoesNotAffectScrollVelocity"].as<bool>();
            else
                qua.BPMDoesNotAffectScrollVelocity = false;

            if (node["InitialScrollVelocity"])
                qua.InitialScrollVelocity = node["InitialScrollVelocity"].as<float>();
            else
                qua.InitialScrollVelocity = 1.0f;

            if (node["HasScratchKey"])
                qua.HasScratchKey = node["HasScratchKey"].as<bool>();
            else
                qua.HasScratchKey = false;
        }

        static EditorLayerInfo ParseEditorLayer(const Node &node)
        {
            EditorLayerInfo editorLayer;
            if (node["Name"])
                editorLayer.Name = node["Name"].as<string>();
            else
                editorLayer.Name = "";

            if (node["Hidden"])
                editorLayer.Hidden = node["Hidden"].as<bool>();
            else
                editorLayer.Hidden = false;

            if (node["ColorRgb"])
                editorLayer.ColorRgb = node["ColorRgb"].as<string>();
            else
                editorLayer.ColorRgb = "";

            return editorLayer;
        }

        static BookmarkInfo ParseBookmark(const Node &node)
        {
            BookmarkInfo bookmark;
            if (node["StartTime"])
                bookmark.StartTime = node["StartTime"].as<long>();
            else
                bookmark.StartTime = 0;

            if (node["Note"])
                bookmark.Note = node["Note"].as<string>();
            else
                bookmark.Note = "";

            return bookmark;
        }

        static CustomAudioSampleInfo ParseCustomAudioSample(const Node &node)
        {
            CustomAudioSampleInfo customAudioSample;
            if (node["Path"])
                customAudioSample.Path = node["Path"].as<string>();
            else
                customAudioSample.Path = "";

            if (node["UnaffectedByRate"])
                customAudioSample.UnaffectedByRate = node["UnaffectedByRate"].as<bool>();
            else
                customAudioSample.UnaffectedByRate = false;

            return customAudioSample;
        }

        static SoundEffectInfo ParseSoundEffect(const Node &node)
        {
            SoundEffectInfo soundEffect;
            if (node["StartTime"])
                soundEffect.StartTime = node["StartTime"].as<long>();
            else
                soundEffect.StartTime = 0;

            if (node["Sample"])
                soundEffect.Sample = node["Sample"].as<long>();
            else
                soundEffect.Sample = 0;

            if (node["Volume"])
                soundEffect.Volume = node["Volume"].as<float>();
            else
                soundEffect.Volume = 0;

            return soundEffect;
        };

        static TimingPointInfo ParseTimingPoint(const Node &node)
        {
            TimingPointInfo timingPoint;
            if (node["StartTime"])
                timingPoint.StartTime = node["StartTime"].as<float>();
            else
                timingPoint.StartTime = 0;

            if (node["Bpm"])
                timingPoint.Bpm = node["Bpm"].as<float>();
            else
                timingPoint.Bpm = 0;

            if (node["Hidden"])
                timingPoint.Hidden = node["Hidden"].as<bool>();
            else
                timingPoint.Hidden = false;

            return timingPoint;
        };

        static SliderVelocityInfo ParseSliderVelocity(const Node &node)
        {
            SliderVelocityInfo sliderVelocity;
            if (node["StartTime"])
                sliderVelocity.StartTime = node["StartTime"].as<float>();
            else
                sliderVelocity.StartTime = 0;

            if (node["Multiplier"])
                sliderVelocity.Multiplier = node["Multiplier"].as<float>();
            else
                sliderVelocity.Multiplier = 0;

            return sliderVelocity;
        };

        static HitObjectInfo ParseHitObject(const Node &node)
        {
            HitObjectInfo hitObject;
            if (node["StartTime"])
                hitObject.StartTime = node["StartTime"].as<float>();
            else
                hitObject.StartTime = 0;

            if (node["Lane"])
                hitObject.Lane = node["Lane"].as<long>();
            else
                hitObject.Lane = 0;

            if (node["EndTime"])
                hitObject.EndTime = node["EndTime"].as<float>();
            else
                hitObject.EndTime = 0;

            if (node["KeySounds"])
                for (const auto &keySound : node["KeySounds"])
                {
                    KeySoundInfo keySoundInfo;
                    if (keySound["Sample"])
                        keySoundInfo.Sample = keySound["Sample"].as<long>();
                    else
                        keySoundInfo.Sample = 0;
                    if (keySound["Volume"])
                        keySoundInfo.Volume = keySound["Volume"].as<long>();
                    else
                        keySoundInfo.Volume = 0;

                    hitObject.KeySounds.push_back(keySoundInfo);
                }
            else
                hitObject.KeySounds = vector<KeySoundInfo>();

            if (node["EditorLayer"])
                hitObject.EditorLayer = node["EditorLayer"].as<long>();
            else
                hitObject.EditorLayer = 0;

            return hitObject;
        };
    };

    template <>
    struct convert<GameMode>
    {
        static Node encode(const GameMode &mode)
        {
            Node node;
            string modeStr;

            switch (mode)
            {
            case GameMode::Keys4:
                modeStr = "Keys4";
                break;
            case GameMode::Keys7:
                modeStr = "Keys7";
                break;
            }

            node = modeStr;
            return node;
        }

        static bool decode(const Node &node, GameMode &mode)
        {
            string modeStr = node.as<string>();

            if (modeStr == "Keys4")
            {
                mode = GameMode::Keys4;
            }
            else if (modeStr == "Keys7")
            {
                mode = GameMode::Keys7;
            }
            else
            {
                return false;
            }

            return true;
        }
    };

}