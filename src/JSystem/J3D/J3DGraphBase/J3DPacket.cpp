#include <JSystem/J3D/J3DGraphBase/J3DPacket.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DShape.hpp>
#include <dolphin/os.h>
#include <dolphin/gd.h>
#include <dolphin/gx.h>

int J3DDrawPacket::sInterruptFlag;

void J3DDisplayListObj::newDisplayList(u32 param_1)
{
	unkC = param_1 + 0x1f & 0xffffffe0;
	unk0 = new (0x20) char[unkC];
	unk4 = new (0x20) char[unkC];
	unk8 = 0;
}

void J3DDisplayListObj::swapBuffer()
{
	void* tmp = unk0;
	unk0      = unk4;
	unk4      = tmp;
}

void J3DDisplayListObj::callDL() { GXCallDisplayList(unk0, unk8); }

bool J3DPacket::isSame(J3DMatPacket*) const { return false; }

bool J3DPacket::entry(J3DDrawBuffer*) { return true; }

void J3DPacket::addChildPacket(J3DPacket* packet)
{
	if (unk8 == nullptr) {
		unk8 = packet;
		return;
	}
	packet->unk4 = unk8;
	unk8         = packet;
}

void J3DCallBackPacket::draw()
{
	if (unk10 != nullptr) {
		unk10(this, 0);
	}
	for (J3DPacket* packet = unk8; packet != nullptr; packet = packet->unk4) {
		packet->draw();
	}
	if (unk10 != nullptr) {
		unk10(this, 1);
	}
}

J3DDrawPacket::J3DDrawPacket()
{
	unk10 = 0;
	unk30 = nullptr;
}

J3DDrawPacket::~J3DDrawPacket() { }

void J3DDrawPacket::draw() { GXCallDisplayList(unk30->unk0, unk30->unk8); }

void J3DDrawPacket::beginDL()
{
	unk30->swapBuffer();
	sInterruptFlag = OSDisableInterrupts();
	GDInitGDLObj(&unk20, unk30->unk0, unk30->unkC);
	__GDCurrentDL = &unk20;
}

u32 J3DDrawPacket::endDL()
{
	GDPadCurr32();
	OSRestoreInterrupts(sInterruptFlag);
	unk30->unk8 = unk20.ptr - unk20.start;
	GDFlushCurrToMem();
	__GDCurrentDL = 0;
	return unk30->unk8;
}

J3DMatPacket::J3DMatPacket()
{
	unk38 = 0;
	unk3C = -1;
	unk40 = 0;
	unk44 = 0;
}

J3DMatPacket::~J3DMatPacket() { }

void J3DMatPacket::addShapePacket(J3DShapePacket* packet)
{
	if (unk34 == nullptr) {
		unk34 = packet;
		return;
	}
	packet->unk4 = unk34;
	unk34        = packet;
}

inline bool checkThing(J3DShapePacket* p)
{
	bool ret = true;

	for (J3DShapePacket* i = p; i != nullptr; i = (J3DShapePacket*)i->unk4) {
		if (i->unk30 != 0) {
			ret = false;
			break;
		}
	}

	return ret;
}

void J3DMatPacket::draw()
{
	char trash[0x20];
	if (!checkThing(unk34)) {
		j3dSys.mTexture   = unk40;
		j3dSys.mMatPacket = this;
		unk38->load();
		for (J3DPacket* j = unk34; j != nullptr; j = j->unk4) {
			j->draw();
		}
	}
}

J3DShapePacket::J3DShapePacket()
{
	unk14 = 0;
	unk18 = 0;
	unk1C = 0;
	unk20 = &j3dDefaultViewNo;
	unk24 = 0;
	unk28 = 0;
	unk2C = 0;
	unk30 = 1;
}

J3DShapePacket::~J3DShapePacket() { }

void J3DShapePacket::draw()
{
	char
	    trash[0x20]; // TODO: probably shares inlines w/ J3DCallBackPacket::draw
	if ((unk14 != 0) && (unk30 != 0)) {
		if (unk10 != nullptr) {
			unk10(this, 0);
		}
		j3dSys.unk10C         = unk24;
		j3dSys.unk110         = unk28;
		j3dSys.unk114         = unk2C;
		unk14->mDrawMatrices  = unk18;
		unk14->mNormMatrices  = unk1C;
		unk14->mCurrentViewNo = unk20;
		unk14->draw();
		if (unk10 != nullptr) {
			unk10(this, 1);
		}
	}
}
