#ifndef J_SUPPORT_HPP
#define J_SUPPORT_HPP

#include <types.h>

template <typename T> T* JSUConvertOffsetToPtr(const void* ptr, u32 offset)
{
	if (offset == nullptr) {
		return nullptr;
	} else {
		return (T*)((u32)ptr + offset);
	}
}

template <typename T>
T* JSUConvertOffsetToPtr(const void* ptr, const void* offset)
{
	if (offset == nullptr) {
		return nullptr;
	} else {
		return (T*)((u32)ptr + (u32)offset);
	}
}

inline u16 JSULoHalf(u32 in) { return in & 0xffff; }
inline u8 JSULoByte(u16 in) { return in & 0xff; }
inline u8 JSUHiByte(u16 in) { return in >> 8; }

#endif
