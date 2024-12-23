#include <JSystem/J3D/J3DPacket.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <JSystem/J3D/J3DSys.hpp>
#include <JSystem/J3D/J3DMaterial.hpp>
#include <JSystem/J3D/J3DShape.hpp>
#include <dolphin/os.h>
#include <dolphin/gd.h>
#include <dolphin/gx.h>


int J3DDrawPacket::sInterruptFlag;

void J3DDisplayListObj::newDisplayList(u32 param_1)
{
	this->unkC = param_1 + 0x1f & 0xffffffe0;
	this->unk0 = new (0x20) char[this->unkC];
	this->unk4 = new (0x20) char[this->unkC];
	this->unk8 = 0;
}

void J3DDisplayListObj::callDL() { GXCallDisplayList(this->unk0, this->unk8); }

bool J3DPacket::isSame(J3DMatPacket*) const { return false; }

bool J3DPacket::entry(J3DDrawBuffer*) { return true; }

void J3DPacket::addChildPacket(J3DPacket* packet)
{
	if (unk8 == nullptr) {
		unk8 = packet;
		return;
	}
	packet->unk4 = this->unk8;
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

void J3DDrawPacket::draw()
{
	GXCallDisplayList(this->unk30->unk0, this->unk30->unk8);
}

J3DDrawPacket::~J3DDrawPacket() { }

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

void J3DShapePacket::draw()
{
	char
	    trash[0x20]; // TODO: probably shares inlines w/ J3DCallBackPacket::draw
	if ((unk14 != 0) && (unk30 != 0)) {
		if (unk10 != nullptr) {
			unk10(this, 0);
		}
		j3dSys._10C_4_ = unk24;
		j3dSys._110_4_ = unk28;
		j3dSys._114_4_ = unk2C;
		unk14->unk50   = unk18;
		unk14->unk54   = unk1C;
		unk14->unk58   = unk20;
		unk14->draw();
		if (unk10 != nullptr) {
			unk10(this, 1);
		}
	}
}

J3DShapePacket::~J3DShapePacket() { }

J3DMatPacket::J3DMatPacket()
{
	unk38 = 0;
	unk3C = -1;
	unk40 = 0;
	unk44 = 0;
}

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
		j3dSys._054_4_ = this->unk40;
		j3dSys._03C_4_ = this;
		this->unk38->load();
		for (J3DPacket* j = unk34; j != nullptr; j = j->unk4) {
			j->draw();
		}
	}
}

J3DMatPacket::~J3DMatPacket() { }

template <class T> inline void swap(T& a, T& b)
{
	T c = a;
	a   = b;
	b   = c;
}

void J3DDrawPacket::beginDL()
{
	swap(unk30->unk0, unk30->unk4);
	sInterruptFlag = OSDisableInterrupts();
	GDInitGDLObj(&unk20, unk30->unk0, unk30->unkC);
	__GDCurrentDL = &unk20;
}

u32 J3DDrawPacket::endDL()
{
	GDPadCurr32();
	OSRestoreInterrupts(sInterruptFlag);
	unk30->unk8 = unk28 - unk20;
	GDFlushCurrToMem();
	__GDCurrentDL = 0;
	return unk30->unk8;
}
