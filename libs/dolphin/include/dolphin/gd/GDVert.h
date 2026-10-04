#ifndef _DOLPHIN_GDVERT_H_
#define _DOLPHIN_GDVERT_H_

#include <dolphin/gd/GDBase.h>

// Basically, copy-paste of GX stuff but writing via GD functions instead

#define FUNC_1PARAM(name, T)                                                   \
	static inline void name##1##T(T x) { GDWrite_##T(x); }

#define FUNC_2PARAM(name, T)                                                   \
	static inline void name##2##T(T x, T y)                                    \
	{                                                                          \
		GDWrite_##T(x);                                                        \
		GDWrite_##T(y);                                                        \
	}

#define FUNC_3PARAM(name, T)                                                   \
	static inline void name##3##T(T x, T y, T z)                               \
	{                                                                          \
		GDWrite_##T(x);                                                        \
		GDWrite_##T(y);                                                        \
		GDWrite_##T(z);                                                        \
	}

#define FUNC_4PARAM(name, T)                                                   \
	static inline void name##4##T(T x, T y, T z, T w)                          \
	{                                                                          \
		GDWrite_##T(x);                                                        \
		GDWrite_##T(y);                                                        \
		GDWrite_##T(z);                                                        \
		GDWrite_##T(w);                                                        \
	}

#define FUNC_INDEX8(name)                                                      \
	static inline void name##1x8(u8 x) { GDWrite_u8(x); }

#define FUNC_INDEX16(name)                                                     \
	static inline void name##1x16(u16 x) { GDWrite_u16(x); }

// GDParam
FUNC_1PARAM(GDParam, u8)
FUNC_1PARAM(GDParam, u16)
FUNC_1PARAM(GDParam, u32)
FUNC_1PARAM(GDParam, s8)
FUNC_1PARAM(GDParam, s16)
FUNC_1PARAM(GDParam, s32)
FUNC_1PARAM(GDParam, f32)
FUNC_3PARAM(GDParam, f32)
FUNC_4PARAM(GDParam, f32)

// GDPosition
FUNC_3PARAM(GDPosition, f32)
FUNC_3PARAM(GDPosition, u8)
FUNC_3PARAM(GDPosition, s8)
FUNC_3PARAM(GDPosition, u16)
FUNC_3PARAM(GDPosition, s16)
FUNC_2PARAM(GDPosition, f32)
FUNC_2PARAM(GDPosition, u8)
FUNC_2PARAM(GDPosition, s8)
FUNC_2PARAM(GDPosition, u16)
FUNC_2PARAM(GDPosition, s16)
FUNC_INDEX16(GDPosition)
FUNC_INDEX8(GDPosition)

// GDNormal
FUNC_3PARAM(GDNormal, f32)
FUNC_3PARAM(GDNormal, s16)
FUNC_3PARAM(GDNormal, s8)
FUNC_INDEX16(GDNormal)
FUNC_INDEX8(GDNormal)

// GDColor
FUNC_4PARAM(GDColor, u8)
FUNC_1PARAM(GDColor, u32)
FUNC_3PARAM(GDColor, u8)
FUNC_1PARAM(GDColor, u16)
FUNC_INDEX16(GDColor)
FUNC_INDEX8(GDColor)

// GDTexCoord
FUNC_2PARAM(GDTexCoord, f32)
FUNC_2PARAM(GDTexCoord, s16)
FUNC_2PARAM(GDTexCoord, u16)
FUNC_2PARAM(GDTexCoord, s8)
FUNC_2PARAM(GDTexCoord, u8)
FUNC_1PARAM(GDTexCoord, f32)
FUNC_1PARAM(GDTexCoord, s16)
FUNC_1PARAM(GDTexCoord, u16)
FUNC_1PARAM(GDTexCoord, s8)
FUNC_1PARAM(GDTexCoord, u8)
FUNC_INDEX16(GDTexCoord)
FUNC_INDEX8(GDTexCoord)

// GDMatrixIndex
FUNC_1PARAM(GDMatrixIndex, u8)

#undef FUNC_1PARAM
#undef FUNC_2PARAM
#undef FUNC_3PARAM
#undef FUNC_4PARAM
#undef FUNC_INDEX8
#undef FUNC_INDEX16

#endif
