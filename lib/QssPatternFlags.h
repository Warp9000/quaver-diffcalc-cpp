#pragma once

#include <type_traits>

enum QssPatternFlags
{
    Unknown = 0,
    MiniJack = 1 << 0,
    ChordJack = 1 << 1,
    KoreaJack = 1 << 2,
    LongJack = 1 << 3,
    QuadJack = 1 << 4,
    Rolls = 1 << 5,
    LightStream = 1 << 6,
    JumpStream = 1 << 7,
    HandStream = 1 << 8,
    QuadStream = 1 << 9,
    InverseLN = 1 << 10,
    ReleaseLN = 1 << 11,
    Polyrhythm = 1 << 12,
    JumpTrill = 1 << 13,
    SplitTrill = 1 << 14,
    SimpleVibro = 1 << 15,
    ControlVibro = 1 << 16
};

inline QssPatternFlags operator~(QssPatternFlags a) { return static_cast<QssPatternFlags>(~static_cast<std::underlying_type<QssPatternFlags>::type>(a)); }
inline QssPatternFlags operator|(QssPatternFlags a, QssPatternFlags b) { return static_cast<QssPatternFlags>(static_cast<std::underlying_type<QssPatternFlags>::type>(a) | static_cast<std::underlying_type<QssPatternFlags>::type>(b)); }
inline QssPatternFlags operator&(QssPatternFlags a, QssPatternFlags b) { return static_cast<QssPatternFlags>(static_cast<std::underlying_type<QssPatternFlags>::type>(a) & static_cast<std::underlying_type<QssPatternFlags>::type>(b)); }
inline QssPatternFlags operator^(QssPatternFlags a, QssPatternFlags b) { return static_cast<QssPatternFlags>(static_cast<std::underlying_type<QssPatternFlags>::type>(a) ^ static_cast<std::underlying_type<QssPatternFlags>::type>(b)); }
inline QssPatternFlags &operator|=(QssPatternFlags &a, QssPatternFlags b) { return reinterpret_cast<QssPatternFlags &>(reinterpret_cast<std::underlying_type<QssPatternFlags>::type &>(a) |= static_cast<std::underlying_type<QssPatternFlags>::type>(b)); }
inline QssPatternFlags &operator&=(QssPatternFlags &a, QssPatternFlags b) { return reinterpret_cast<QssPatternFlags &>(reinterpret_cast<std::underlying_type<QssPatternFlags>::type &>(a) &= static_cast<std::underlying_type<QssPatternFlags>::type>(b)); }
inline QssPatternFlags &operator^=(QssPatternFlags &a, QssPatternFlags b) { return reinterpret_cast<QssPatternFlags &>(reinterpret_cast<std::underlying_type<QssPatternFlags>::type &>(a) ^= static_cast<std::underlying_type<QssPatternFlags>::type>(b)); }