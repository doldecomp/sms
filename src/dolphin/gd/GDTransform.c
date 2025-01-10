#include "__gd.h"

void GDSetCurrentMtx(u32 pn, u32 t0, u32 t1, u32 t2, u32 t3, u32 t4, u32 t5,
                     u32 t6, u32 t7)
{
	u32 regA = 0;
	u32 regB = 0;

	regA |= pn << 0;
	regA |= t0 << 6;
	regA |= t1 << 12;
	regA |= t2 << 18;
	regA |= t3 << 24;

	regB |= t4 << 0;
	regB |= t5 << 6;
	regB |= t6 << 12;
	regB |= t7 << 18;

	GDWriteCPCmd(0x30, regA);
	GDWriteCPCmd(0x40, regB);
	GDWriteXFCmdHdr(0x1018, 1);
	GDWrite_u32(regA);
	GDWrite_u32(regB);
}
