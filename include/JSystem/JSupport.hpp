#ifndef J_SUPPORT_HPP
#define J_SUPPORT_HPP

#include <types.h>

inline u8 JSULoByte(u16 in) { return in & 0xff; }
inline u8 JSUHiByte(u16 in) { return in >> 8; }

#endif
