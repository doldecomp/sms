#include <dolphin/types.h>
#include <dolphin/gd/GDBase.h>
#include <dolphin/gx/GXEnum.h>

#define SET_REG_FIELD(reg, size, shift, val)                                   \
	do {                                                                       \
		(reg) = ((u32)(reg) & ~(((1 << (size)) - 1) << (shift)))               \
		        | ((u32)(val) << (shift));                                     \
	} while (0)

extern void GDOverflowed();

inline static void GDOverflowCheck(u32 size)
{
	if (__GDCurrentDL->ptr + size > __GDCurrentDL->top) {
		GDOverflowed();
	}
}

inline static void __GDWrite(u8 data) { *__GDCurrentDL->ptr++ = data; }

inline static void GDWrite_u8(u8 data)
{
	GDOverflowCheck(sizeof(u8));
	__GDWrite(data & 0xFF);
}

inline static void GDWrite_u16(u16 data)
{
	GDOverflowCheck(sizeof(u16));
	__GDWrite(data >> 8);
	__GDWrite(data & 0xFF);
}

inline static void GDWrite_u32(u32 data)
{
	GDOverflowCheck(sizeof(u32));
	__GDWrite(data >> 24);
	__GDWrite((data >> 16) & 0xFF);
	__GDWrite((data >> 8) & 0xFF);
	__GDWrite(data & 0xFF);
}

inline static void GDWrite_f32(float data)
{
	union {
		float f;
		u32 u;
	} fid;
	fid.f = data;
	GDWrite_u32(fid.u);
}

inline static void GDWriteXFCmdHdr(u16 addr, u8 len)
{
	GDWrite_u8(16);
	GDWrite_u16(len - 1);
	GDWrite_u16(addr);
}
inline static void GDWriteXFCmd(u16 addr, u32 val)
{
	GDWrite_u8(0x10);
	GDWrite_u16(0);
	GDWrite_u16(addr);
	GDWrite_u32(val);
}

inline static void GDWriteCPCmd(u8 addr, u32 val)
{
	GDWrite_u8(8);
	GDWrite_u8(addr);
	GDWrite_u32(val);
}

inline static void GDWriteBPCmd(u32 regval)
{
	GDWrite_u8(0x61);
	GDWrite_u32(regval);
}

inline static u16 __GDLightID2Index(GXLightID id)
{
	u16 idx;

	idx = 0x1F - __cntlzw(id);
	if (idx > 7) {
		idx = 0;
	}
	return idx;
}

inline static u16 __GDLightID2Offset(GXLightID id)
{
	return __GDLightID2Index(id) * 16;
}
