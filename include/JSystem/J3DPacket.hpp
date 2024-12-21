#ifndef J3D_PACKET_HPP
#define J3D_PACKET_HPP

#include <dolphin/types.h>

class J3DMatPacket;
class J3DDrawBuffer;

class J3DPacket {
public:
	J3DPacket()
	{
		unk4 = 0;
		unk8 = 0;
		unkC = 0;
	}

	typedef void IDK;

	virtual bool isSame(J3DMatPacket*) const;
	virtual bool entry(J3DDrawBuffer*);
	void addChildPacket(J3DPacket* packet);
	virtual IDK draw() { }

	virtual ~J3DPacket() {};

	J3DPacket* unk4;
	J3DPacket* unk8;
	u32 unkC;
};

class J3DCallBackPacket : public J3DPacket {
public:
	J3DCallBackPacket() { unk10 = NULL; }

	virtual void draw();
	virtual ~J3DCallBackPacket() { }

	void (*unk10)(J3DPacket*, u32);
};

class J3DShape;

class J3DShapePacket : public J3DCallBackPacket {
public:
	J3DShapePacket();

	virtual void draw();
	virtual ~J3DShapePacket();

	J3DShape* unk14;
	u32 unk18;
	u32 unk1C;
	void* unk20;
	u32 unk24;
	u32 unk28;
	u32 unk2C;
	u8 unk30;
	char unk31[0x3];
};

class J3DDrawPacket : public J3DPacket {
public:
	J3DDrawPacket();

	typedef void IDK;
	virtual void draw();
	virtual ~J3DDrawPacket();

	void beginDL();
	u32 endDL();
	IDK beginPatch();
	IDK endPatch();

	u32 unk10;
	char padding0[0xc];
	u32 unk20;
	char padding1[0x4];
	u32 unk28;
	char padding2[0x4];

	struct Unknown {
		void* unk0;
		void* unk4;
		u32 unk8;
		u32 unkC;
	};
	Unknown* unk30;

	static int sInterruptFlag;
};

class J3DMaterial;

class J3DMatPacket : public J3DDrawPacket {
public:
	J3DMatPacket();

	typedef void IDK;
	virtual bool isSame(J3DMatPacket* other) const
	{
		return unk3C == other->unk3C && (unk3C >> 31) == 0;
	}
	virtual bool entry(J3DDrawBuffer*) { return true; }; // TODO: not matched
	virtual void draw();
	virtual ~J3DMatPacket();

	void addShapePacket(J3DShapePacket* packet);
	IDK isHideAllShapePacket_();

	J3DShapePacket* unk34; // TODO: might be part of DrawPacket
	J3DMaterial* unk38;
	u32 unk3C; // TODO: unk3C is something weird, probably not u32
	u32 unk40;
	u32 unk44;
};

class J3DDisplayListObj {
public:
	typedef void IDK;
	void newDisplayList(u32);
	IDK swapBuffer();
	void callDL();

	void* unk0;
	void* unk4;
	u32 unk8;
	u32 unkC;
};

#endif
