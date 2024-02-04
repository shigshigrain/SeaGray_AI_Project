#pragma once

#ifdef SCPDICPP_EXPORTS
#define SCPDICPP_API __declspec(dllexport)
#else
#define SCPDICPP_API __declspec(dllimport)
#endif

namespace ScpdiCpp
{
enum class SCPDICPP_API X360Buttons : int
{
	NONE = 0,

	UP = 1 << 0,
	DOWN = 1 << 1,
	LEFT = 1 << 2,
	RIGHT = 1 << 3,

	START = 1 << 4,
	BACK = 1 << 5,

	LEFT_STICK = 1 << 6,
	RIGHT_STICK = 1 << 7,

	LFET_BUMPER = 1 << 8,
	RIGHT_BUMPER = 1 << 9,

	LOGO = 1 << 10,

	A = 1 << 12,
	B = 1 << 13,
	X = 1 << 14,
	Y = 1 << 15
};

inline X360Buttons operator|(X360Buttons a, X360Buttons b)
{
	return static_cast<X360Buttons>(static_cast<int>(a) | static_cast<int>(b));
}
inline X360Buttons operator&(X360Buttons a, X360Buttons b)
{
	return static_cast<X360Buttons>(static_cast<int>(a) & static_cast<int>(b));
}
inline X360Buttons operator^(X360Buttons a, X360Buttons b)
{
	return static_cast<X360Buttons>(static_cast<int>(a) ^ static_cast<int>(b));
}
inline X360Buttons operator~(X360Buttons a)
{
	return static_cast<X360Buttons>(~static_cast<int>(a));
}
inline X360Buttons operator|=(X360Buttons& lhs, X360Buttons rhs)
{
	return lhs = static_cast<X360Buttons>(static_cast<int>(lhs) | static_cast<int>(rhs));
}
inline X360Buttons operator&=(X360Buttons& lhs, X360Buttons rhs)
{
	return lhs = static_cast<X360Buttons>(static_cast<int>(lhs) & static_cast<int>(rhs));
}
inline X360Buttons operator^=(X360Buttons& lhs, X360Buttons rhs)
{
	return lhs = static_cast<X360Buttons>(static_cast<int>(lhs) ^ static_cast<int>(rhs));
}
}