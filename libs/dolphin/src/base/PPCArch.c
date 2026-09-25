#include <dolphin.h>
#include <dolphin/base/PPCArch.h>

asm u32 PPCMfmsr()
{
#ifdef __MWERKS__ // clang-format off
	nofralloc
	mfmsr r3
	blr
#endif // clang-format on
}

asm void PPCMtmsr(register u32 newMSR)
{
#ifdef __MWERKS__ // clang-format off
	nofralloc
	mtmsr newMSR
	blr
#endif // clang-format on
}

asm u32 PPCMfhid0()
{
#ifdef __MWERKS__ // clang-format off
	nofralloc
	mfspr r3, HID0
	blr
#endif // clang-format on
}

asm void PPCMthid0(register u32 newHID0)
{
#ifdef __MWERKS__ // clang-format off
	nofralloc
	mtspr HID0, newHID0
	blr
#endif // clang-format on
}

asm u32 PPCMfl2cr()
{
#ifdef __MWERKS__ // clang-format off
	nofralloc
	mfspr r3, L2CR
	blr
#endif // clang-format on
}

asm void PPCMtl2cr(register u32 newL2cr)
{
#ifdef __MWERKS__ // clang-format off
	nofralloc
	mtspr L2CR, newL2cr
	blr
#endif // clang-format on
}

asm void PPCMtdec(register u32 newDec)
{
#ifdef __MWERKS__ // clang-format off
	nofralloc
	mtdec newDec
	blr
#endif // clang-format on
}

asm void PPCSync()
{
#ifdef __MWERKS__ // clang-format off
	nofralloc
	sc
	blr
#endif // clang-format on
}

asm void PPCHalt()
{
#ifdef __MWERKS__ // clang-format off
	nofralloc
	sync
loop:
	nop
	li r3, 0
	nop
	b loop
#endif // clang-format on
}

asm u32 PPCMfhid2()
{
#ifdef __MWERKS__ // clang-format off
	nofralloc
	mfspr r3, HID2
	blr
#endif // clang-format on
}

asm void PPCMthid2(register u32 newhid2)
{
#ifdef __MWERKS__ // clang-format off
	nofralloc
	mtspr HID2, newhid2
	blr
#endif // clang-format on
}

asm void PPCMtwpar(register u32 newwpar)
{
#ifdef __MWERKS__ // clang-format off
	nofralloc
	mtspr WPAR, newwpar
	blr
#endif // clang-format on
}

void PPCDisableSpeculation() { PPCMthid0(PPCMfhid0() | 0x200); }

asm void PPCSetFpNonIEEEMode()
{
#ifdef __MWERKS__ // clang-format off
	nofralloc
	mtfsb1 29
	blr
#endif // clang-format on
}
