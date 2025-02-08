#ifndef J3D_PACKET_HPP
#define J3D_PACKET_HPP

#include <types.h>
#include <dolphin/gd.h>
#include <dolphin/mtx.h>

class J3DMatPacket;
class J3DDrawBuffer;
class J3DShape;
class J3DMaterial;
class J3DMaterialAnm;
class J3DShapePacket;
class J3DTexture;

class J3DDisplayListObj {
public:
	J3DDisplayListObj()
	{
		unk0 = nullptr;
		unk4 = nullptr;
		unk8 = 0;
		unkC = 0;
	}

	void newDisplayList(u32);
	void swapBuffer();
	void callDL();

public:
	void* unk0;
	void* unk4;
	u32 unk8;
	u32 unkC;
};

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

	void drawClear()
	{
		unk4 = nullptr;
		unk8 = nullptr;
	}

	J3DPacket* getNextPacket() const { return unk4; }
	void setNextPacket(J3DPacket* packet) { unk4 = packet; }

public:
	J3DPacket* unk4;
	J3DPacket* unk8;
	u32 unkC;
};

class J3DCallBackPacket : public J3DPacket {
public:
	typedef void (*CallbackT)(J3DPacket*, u32);

	J3DCallBackPacket() { unk10 = (CallbackT)nullptr; }

	virtual void draw();
	virtual ~J3DCallBackPacket() { }

	CallbackT unk10;
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

	J3DDisplayListObj* getDisplayListObj() const { return unk30; }

	bool checkFlag(u32 flag) const { return (unk10 & flag) != 0; }
	bool isLocked() const { return checkFlag(1) ? TRUE : FALSE; }

	static int sInterruptFlag;

public:
	u32 unk10;
	char padding0[0xc];
	GDLObj unk20;
	J3DDisplayListObj* unk30;
};

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

	J3DShapePacket* getShapePacket() const { return unk34; }
	J3DMaterial* getMaterial() const { return unk38; }

public:
	J3DShapePacket* unk34; // TODO: might be part of DrawPacket
	J3DMaterial* unk38;
	u32 unk3C; // TODO: unk3C is something weird, probably not u32
	J3DTexture* unk40;
	J3DMaterialAnm* unk44;
};

class J3DShapePacket : public J3DCallBackPacket {
public:
	J3DShapePacket();

	virtual void draw();
	virtual ~J3DShapePacket();

	J3DShape* unk14;
	Mtx** unk18;
	Mtx33** unk1C;
	u32* unk20;
	void* unk24;
	void* unk28;
	void* unk2C;
	u8 unk30;
	char unk31[0x3];
};

#endif
