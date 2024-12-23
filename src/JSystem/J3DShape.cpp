#include <JSystem/J3D/J3DShape.hpp>
#include <JSystem/J3D/J3DSys.hpp>
#include <JSystem/J3D/J3DTevs.hpp>
#include <dolphin/gx.h>

void J3DShape::initialize()
{
	unk0  = 0;
	unk4  = 0xffff;
	unk6  = 0;
	unk8  = 0;
	unkC  = 0;
	unk10 = 0;
	unk14 = 0;
	unk18 = 0;
	unk1C = 0;
	unk20 = 0;
	unk24 = 0;
	unk2c = 0;
	unk34 = 0;
	unk38 = 0;
	unk44 = 0;
	unk48 = 0;
	unk4c = 0;
	unk50 = 0;
	unk54 = 0;
	unk58 = &j3dDefaultViewNo;
	unk30 = 0;
}

void J3DShape::draw() const
{
}
