#include <dolphin/gd/GDTransform.h>
#include <dolphin/gd/GDBase.h>

void GDSetCurrentMtx(u32 pn, u32 t0, u32 t1, u32 t2, u32 t3, u32 t4, u32 t5,
                     u32 t6, u32 t7)
{
	u32 regA;
	u32 regB;

	regA = CP_MTX_REG_A(pn, t0, t1, t2, t3);
	regB = CP_MTX_REG_B(t4, t5, t6, t7);

	GDWriteCPCmd(CP_MTX_REG_A_ID, regA);
	GDWriteCPCmd(CP_MTX_REG_B_ID, regB);
	GDWriteXFCmdHdr(XF_REG_MATRIXINDEX0_ID, 2);
	GDWrite_u32(regA);
	GDWrite_u32(regB);
}
