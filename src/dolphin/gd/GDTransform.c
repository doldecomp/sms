#include "__gd.h"

void GDSetCurrentMtx(u32 pn, u32 t0, u32 t1, u32 t2, u32 t3, u32 t4, u32 t5,
                     u32 t6, u32 t7)
{
	u32 regA;
	u32 regB;

	// clang-format off
	regA = pn << 0
		| t0 << 6
		| t1 << 12
		| t2 << 18
		| t3 << 24;
	// clang-format on

	// clang-format off
	regB = t4 << 0
	  | t5 << 6
	  | t6 << 12
	  | t7 << 18;
	// clang-format on

	GDWriteCPCmd(0x30, regA);
	GDWriteCPCmd(0x40, regB);
	GDWriteXFCmdHdr(0x1018, 2);
	GDWrite_u32(regA);
	GDWrite_u32(regB);
}
