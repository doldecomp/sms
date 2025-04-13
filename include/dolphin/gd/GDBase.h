#ifndef _DOLPHIN_GD_BASE_H
#define _DOLPHIN_GD_BASE_H

#include <dolphin/types.h>
#include <dolphin/gx/GXEnum.h>

#ifdef __cplusplus
extern "C" {
#endif // ifdef __cplusplus

typedef void (*GDOverflowCb)();

typedef struct _GDLObj {
	u8* start;
	u32 length;
	u8* ptr;
	u8* top;
} GDLObj;

extern GDLObj* __GDCurrentDL;

void GDInitGDLObj(GDLObj* dl, void* start, u32 length);
void GDFlushCurrToMem(void);
void GDPadCurr32(void);
void GDOverflowed(void);
void GDSetOverflowCallback(GDOverflowCb callback);
void GDGetOverflowCallback();

inline void GDSetCurrent(GDLObj* dl) { __GDCurrentDL = dl; }

inline GDLObj* GDGetCurrent() { return __GDCurrentDL; }

inline u8* GDGetCurrPointer() { return __GDCurrentDL->ptr; }

inline s32 GDGetCurrOffset()
{
	return __GDCurrentDL->ptr - __GDCurrentDL->start;
}

inline void GDSetCurrOffset(s32 offs)
{
	__GDCurrentDL->ptr = __GDCurrentDL->start + offs;
}

// NOTE: yes, these were 100% "public" because JSystem uses them directly

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
	GDWrite_u8(GX_CMD_LOAD_XF_REG);
	GDWrite_u16(len - 1);
	GDWrite_u16(addr);
}

inline static void GDWriteXFCmd(u16 addr, u32 val)
{
	GDWrite_u8(GX_CMD_LOAD_XF_REG);
	GDWrite_u16(0);
	GDWrite_u16(addr);
	GDWrite_u32(val);
}

inline static void GDWriteCPCmd(u8 addr, u32 val)
{
	GDWrite_u8(GX_CMD_LOAD_CP_REG);
	GDWrite_u8(addr);
	GDWrite_u32(val);
}

inline static void GDWriteBPCmd(u32 regval)
{
	GDWrite_u8(GX_CMD_LOAD_BP_REG);
	GDWrite_u32(regval);
}

#ifdef __cplusplus
}
#endif // ifdef __cplusplus

#endif
