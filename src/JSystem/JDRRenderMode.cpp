#include "JSystem/JDrama.hpp"
#include <cstring>


bool JDrama::IsEqualRenderModeVIParams(const GXRenderModeObj &rmo1, const GXRenderModeObj &rmo2)
{
	return	(rmo1.viTVmode	== rmo2.viTVmode)
    &&		(rmo1.fbWidth	== rmo2.fbWidth)
	&&		(rmo1.efbHeight	== rmo2.efbHeight)
	&&		(rmo1.xfbHeight	== rmo2.xfbHeight)
	&&		(rmo1.viXOrigin	== rmo2.viXOrigin)
	&&		(rmo1.viYOrigin	== rmo2.viYOrigin)
	&&		(rmo1.viWidth	== rmo2.viWidth)
	&&		(rmo1.viHeight	== rmo2.viHeight)
    &&		(rmo1.xFBmode	== rmo2.xFBmode);
}

bool JDrama::IsRenderModeHalfAspectRatio(const GXRenderModeObj &rmo)
{
	return rmo.xfbHeight * 2 == rmo.viHeight;
}


#ifdef NONMATCHING
// TODO
f32 JDrama::GetRenderModeYScale(const GXRenderModeObj &rmo)
{
	f32 ret = 1.0f;
	bool first = false;
	if (rmo.aa == 1)
	{
		bool second = true;
		if (rmo.xFBmode != VI_XFBMODE_DF)
			if (rmo.viTVmode != VI_TVMODE_NTSC_PROG)
				second = first;
		if (second)
			first = true;
	}
	if (first)
#line 55 "JDRRenderMode.cpp"
		OSHalt("future not implemented!");
	else
		ret = GXGetYScaleFactor(rmo.efbHeight, rmo.xfbHeight);
	return ret;
}

void JDrama::CalcRenderModeXFBHeight(GXRenderModeObj *rmo, u16 p16)
{
	bool first = false;
	if (rmo->aa == 1)
	{
		bool second = true;

		if(rmo->xFBmode != VI_XFBMODE_DF)
			if(rmo->viTVmode != VI_TVMODE_NTSC_PROG)
				second = first;

	if (second)
		first = true;
	}
	if (first)
#line 72 "JDRRenderMode.cpp"
		OSHalt("future not implemented!\n");

	s32 fr = (rmo->field_rendering != 0) ? 2 : 1;
	u32 p32 = p16 / fr;
    GXGetYScaleFactor(rmo->efbHeight,p32);
    u16 ln = GXGetNumXfbLines(rmo->efbHeight,p32);
    rmo->xfbHeight = ln;
    rmo->viHeight = rmo->xfbHeight * fr;

	return;
}
#else

/* I'm sure there is a much cleaner way to inline assembly,
   but this gets the job done
*/
#pragma push

const float one[2] = {1.0f, 0.0f}; // Trailing zeroes for alignment

const char file[] = "JDRRenderMode.cpp";
const char future1[] = "future not implemented!";

asm f32 JDrama::GetRenderModeYScale(const GXRenderModeObj &rmo)
{
		nofralloc
	mflr r0
	mr r5, r3
	stw r0, 4(r1)
	stwu r1, -0x18(r1)
	stfd f31, 0x10(r1)
	lbz r0, 0x19(r3)
	li r3, 0
	lfs f31, one(r2)
	cmplwi r0, 1
	bne lbl_80045C24
	lwz r0, 0x14(r5)
	li r4, 1
	cmpwi r0, 1
	beq lbl_80045C18
	lwz r0, 0(r5)
	cmpwi r0, 2
	beq lbl_80045C18
	mr r4, r3
lbl_80045C18:
	clrlwi. r0, r4, 24
	beq lbl_80045C24
	li r3, 1
lbl_80045C24:
	clrlwi. r0, r3, 24
	beq lbl_80045C4C
	lis r3, file@ha
	crclr 6
	lis r4, future1@ha
	addi r5, r4, future1@l
	addi r3, r3, file@l
	li r4, 55
	bl OSPanic
	b lbl_80045C5C
lbl_80045C4C:
	lhz r3, 6(r5)
	lhz r4, 8(r5)
	bl GXGetYScaleFactor
	fmr f31, f1
lbl_80045C5C:
	lwz r0, 28(r1)
	fmr f1, f31
	lfd f31, 16(r1)
	addi r1, r1, 24
	mtlr r0
	blr
}

const char future2[] = "future not implemented!\n\0\0\0"; // Trailing zeroes for allignment

asm void JDrama::CalcRenderModeXFBHeight(GXRenderModeObj *rmo, u16 p16)
{
		nofralloc
	mflr r0
	stw r0, 4(r1)
	stwu r1, -32(r1)
	stw r31, 28(r1)
	mr r31, r3
	stw r30, 24(r1)
	stw r29, 20(r1)
	addi r29, r4, 0
	lbz r0, 25(r3)
	li r3, 0
	cmplwi r0, 1
	bne lbl_80045CD0
	lwz r0, 20(r31)
	li r4, 1
	cmpwi r0, 1
	beq lbl_80045CC4
	lwz r0, 0(r31)
	cmpwi r0, 2
	beq lbl_80045CC4
	mr r4, r3
lbl_80045CC4:
	clrlwi. r0, r4, 0x18
	beq lbl_80045CD0
	li r3, 1
lbl_80045CD0:
	clrlwi. r0, r3, 0x18
	beq lbl_80045CF4
	lis r3, file@ha
	crclr 6
	lis r4, future2@ha
	addi r5, r4, future2@l
	addi r3, r3, file@l
	li r4, 0x48
	bl OSPanic
lbl_80045CF4:
	lbz r0, 0x18(r31)
	cmplwi r0, 0
	beq lbl_80045D08
	li r30, 2
	b lbl_80045D0C
lbl_80045D08:
	li r30, 1
lbl_80045D0C:
	clrlwi r0, r29, 0x10
	lhz r3, 6(r31)
	divw r0, r0, r30
	clrlwi r4, r0, 0x10
	bl GXGetYScaleFactor
	lhz r3, 6(r31)
	bl GXGetNumXfbLines
	sth r3, 8(r31)
	lhz r0, 8(r31)
	mullw r0, r0, r30
	sth r0, 0x10(r31)
	lwz r0, 0x24(r1)
	lwz r31, 0x1c(r1)
	lwz r30, 0x18(r1)
	lwz r29, 0x14(r1)
	addi r1, r1, 0x20
	mtlr r0
	blr 
}
#pragma pop
#endif


void JDrama::CalcRenderModeVIXOrigin(GXRenderModeObj *rmo)
{
	u32 dif = static_cast<u16>(JDrama::GetVIWidthMax()) - rmo->viWidth;
	rmo->viXOrigin = (((s32)dif / 2) + ((dif < 0) && ((dif & 1) != 0)));
}

void JDrama::CalcRenderModeVIYOrigin(GXRenderModeObj *rmo)
{
	u32 dif = static_cast<u16>(JDrama::GetVIHeightMax()) - rmo->viHeight;
	rmo->viYOrigin = (((s32)dif / 2) + ((dif < 0) && ((dif & 1) != 0)));
}

void JDrama::CopyRenderModeSamplePattern(GXRenderModeObj *rmo, const u8 (*s)[2])
{
	memcpy(rmo->sample_pattern,
		   s,
		   sizeof(rmo->sample_pattern));
}

void JDrama::CopyRenderModeVFilter(GXRenderModeObj *rmo, const u8 *s)
{
	memcpy(rmo->vfilter,
		   s,
		   sizeof(rmo->vfilter));
}
