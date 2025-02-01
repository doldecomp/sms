#include <dolphin/types.h>
#include <dolphin/gd/GDBase.h>
#include <dolphin/gx/GXEnum.h>

#define SET_REG_FIELD(reg, size, shift, val)                                   \
	do {                                                                       \
		(reg) = ((u32)(reg) & ~(((1 << (size)) - 1) << (shift)))               \
		        | ((u32)(val) << (shift));                                     \
	} while (0)

extern void GDOverflowed();

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
