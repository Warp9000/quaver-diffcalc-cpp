#pragma once

enum FingerState
{
    NoFingerState = 0,
    Index = 1 << 0,
    Middle = 1 << 1,
    Ring = 1 << 2,
    Pinkie = 1 << 3,
    Thumb = 1 << 4
};

inline FingerState operator~(FingerState a) { return static_cast<FingerState>(~static_cast<std::underlying_type<FingerState>::type>(a)); }
inline FingerState operator|(FingerState a, FingerState b) { return static_cast<FingerState>(static_cast<std::underlying_type<FingerState>::type>(a) | static_cast<std::underlying_type<FingerState>::type>(b)); }
inline FingerState operator&(FingerState a, FingerState b) { return static_cast<FingerState>(static_cast<std::underlying_type<FingerState>::type>(a) & static_cast<std::underlying_type<FingerState>::type>(b)); }
inline FingerState operator^(FingerState a, FingerState b) { return static_cast<FingerState>(static_cast<std::underlying_type<FingerState>::type>(a) ^ static_cast<std::underlying_type<FingerState>::type>(b)); }
inline FingerState &operator|=(FingerState &a, FingerState b) { return reinterpret_cast<FingerState &>(reinterpret_cast<std::underlying_type<FingerState>::type &>(a) |= static_cast<std::underlying_type<FingerState>::type>(b)); }
inline FingerState &operator&=(FingerState &a, FingerState b) { return reinterpret_cast<FingerState &>(reinterpret_cast<std::underlying_type<FingerState>::type &>(a) &= static_cast<std::underlying_type<FingerState>::type>(b)); }
inline FingerState &operator^=(FingerState &a, FingerState b) { return reinterpret_cast<FingerState &>(reinterpret_cast<std::underlying_type<FingerState>::type &>(a) ^= static_cast<std::underlying_type<FingerState>::type>(b)); }
