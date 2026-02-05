#ifndef J3D_PACKET_HPP
#define J3D_PACKET_HPP

#include <types.h>
#include <dolphin/gd.h>
#include <dolphin/mtx.h>
#include <JSystem/J3D/J3DGraphBase/J3DDrawBuffer.hpp>

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
		unk4 = nullptr;
		unk8 = nullptr;
		unkC = nullptr;
	}

	virtual bool isSame(J3DMatPacket*) const;
	virtual bool entry(J3DDrawBuffer*);
	void addChildPacket(J3DPacket* packet);
	virtual void draw() { }

	virtual ~J3DPacket() { }

	void drawClear()
	{
		unk4 = nullptr;
		unk8 = nullptr;
	}

	J3DPacket* getNextPacket() const { return unk4; }
	void setNextPacket(J3DPacket* packet) { unk4 = packet; }

	// Weird, but TP debug says this is the signature
	void setUserArea(u32 area) { unkC = (void*)area; }
	u32 getUserArea() const { return (u32)unkC; }

public:
	J3DPacket* unk4;
	J3DPacket* unk8;
	void* unkC;
};

class J3DCallBackPacket : public J3DPacket {
public:
	typedef void (*CallbackT)(J3DCallBackPacket*, int);

	J3DCallBackPacket() { unk10 = nullptr; }

	virtual void draw();
	virtual ~J3DCallBackPacket() { }

	void setCallback(CallbackT cb) { unk10 = cb; }

public:
	CallbackT unk10;
};

class J3DDrawPacket : public J3DPacket {
public:
	J3DDrawPacket();

	virtual void draw();
	virtual ~J3DDrawPacket();

	void beginDL();
	u32 endDL();
	void beginPatch();
	void endPatch();

	J3DDisplayListObj* getDisplayListObj() const { return unk30; }
	void setDisplayListObj(J3DDisplayListObj* pObj) { unk30 = pObj; }

	bool checkFlag(u32 flag) const { return (unk10 & flag) != 0; }
	void onFlag(u32 flag) { unk10 |= flag; }
	void offFlag(u32 flag) { unk10 &= ~flag; }
	bool isLocked() const { return checkFlag(1) ? TRUE : FALSE; }
	void lock() { onFlag(0x01); }
	void unlock() { offFlag(0x01); }

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

	virtual bool isSame(J3DMatPacket* other) const
	{
		return unk3C == other->unk3C && (unk3C >> 31) == 0;
	}
	virtual bool entry(J3DDrawBuffer* buffer)
	{
		J3DDrawBuffer::sortFunc func
		    = J3DDrawBuffer::sortFuncTable[buffer->mSortType];
		return (buffer->*func)(this);
	};
	virtual void draw();
	virtual ~J3DMatPacket();

	void addShapePacket(J3DShapePacket* packet);
	void isHideAllShapePacket_();

	J3DShapePacket* getShapePacket() const { return unk34; }
	void setShapePacket(J3DShapePacket* packet) { unk34 = packet; }
	J3DMaterial* getMaterial() const { return unk38; }
	void setTexture(J3DTexture* pTexture) { mTexture = pTexture; }

	void setMaterialAnmID(J3DMaterialAnm* materialAnm) { unk44 = materialAnm; }

public:
	J3DShapePacket* unk34; // TODO: might be part of DrawPacket
	J3DMaterial* unk38;
	u32 unk3C; // TODO: unk3C is something weird, probably not u32
	J3DTexture* mTexture;
	J3DMaterialAnm* unk44;
};

class J3DShapePacket : public J3DCallBackPacket {
public:
	J3DShapePacket();

	virtual void draw();
	virtual ~J3DShapePacket();

	void setShape(J3DShape* pShape) { unk14 = pShape; }
	void setDrawMtx(Mtx** mtx) { unk18 = mtx; }
	void setNrmMtx(Mtx33** mtx) { unk1C = mtx; }
	void setCurrentViewNoPtr(u32* pCurrentViewNo) { unk20 = pCurrentViewNo; }

	// fabriacted
	void hide() { unk30 = 0; }
	void show() { unk30 = 1; }

public:
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
