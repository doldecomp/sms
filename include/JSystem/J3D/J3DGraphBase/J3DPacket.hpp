#ifndef J3D_PACKET_HPP
#define J3D_PACKET_HPP

#include <stdint.h>
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
		mpData[0] = nullptr;
		mpData[1] = nullptr;
		mSize     = 0;
		mCapacity = 0;
	}

	void newDisplayList(u32);
	void swapBuffer();
	void callDL();

	u8* getDisplayList(int idx) const { return (u8*)mpData[idx]; }
	u32 getDisplayListSize() const { return mSize; }

public:
	/* 0x0 */ void* mpData[2];
	/* 0x8 */ u32 mSize;
	/* 0xC */ u32 mCapacity;
};

class J3DPacket {
public:
	J3DPacket()
	{
		mpNext       = nullptr;
		mpFirstChild = nullptr;
		mpUserData   = nullptr;
	}

	virtual bool isSame(J3DMatPacket*) const;
	virtual bool entry(J3DDrawBuffer*);
	void addChildPacket(J3DPacket* packet);
	virtual void draw() { }

	virtual ~J3DPacket() { }

	void drawClear()
	{
		mpNext       = nullptr;
		mpFirstChild = nullptr;
	}

	J3DPacket* getNextPacket() const { return mpNext; }
	void setNextPacket(J3DPacket* packet) { mpNext = packet; }

	void setUserArea(uintptr_t area) { mpUserData = (void*)area; }
	uintptr_t getUserArea() const { return (uintptr_t)mpUserData; }

protected:
	/* 0x4 */ J3DPacket* mpNext;
	/* 0x8 */ J3DPacket* mpFirstChild;
	/* 0xC */ void* mpUserData;
};

class J3DCallBackPacket : public J3DPacket {
public:
	typedef void (*CallbackT)(J3DCallBackPacket*, int);

	J3DCallBackPacket() { mpCallBack = nullptr; }

	virtual void draw();
	virtual ~J3DCallBackPacket() { }

	void setCallback(CallbackT cb) { mpCallBack = cb; }

protected:
	/* 0x10 */ CallbackT mpCallBack;
};

class J3DDrawPacket : public J3DPacket {
public:
	J3DDrawPacket();

	virtual void draw();
	virtual ~J3DDrawPacket();

	void beginDL();
	u32 endDL();
	void beginPatch();
	u32 endPatch();

	J3DDisplayListObj* getDisplayListObj() const { return mpDisplayListObj; }
	void setDisplayListObj(J3DDisplayListObj* pObj) { mpDisplayListObj = pObj; }

	enum {
		LOCKED = 0x1,
	};

	bool checkFlag(u32 flag) const { return (mFlags & flag) != 0; }
	void onFlag(u32 flag) { mFlags |= flag; }
	void offFlag(u32 flag) { mFlags &= ~flag; }
	bool isLocked() const { return checkFlag(LOCKED) ? TRUE : FALSE; }
	void lock() { onFlag(LOCKED); }
	void unlock() { offFlag(LOCKED); }

	static int sInterruptFlag;

protected:
	/* 0x10 */ u32 mFlags;
	/* 0x14 */ char unk14[0xC];
	/* 0x20 */ GDLObj mGDList;
	/* 0x30 */ J3DDisplayListObj* mpDisplayListObj;
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
		    = J3DDrawBuffer::sortFuncTable[buffer->getSortMode()];
		return (buffer->*func)(this);
	};
	virtual void draw();
	virtual ~J3DMatPacket();

	void addShapePacket(J3DShapePacket* packet);

	// from TP
	bool isChanged() const { return unk3C & 0x80000000; }

	J3DShapePacket* getShapePacket() const { return mpShapePacket; }
	void setShapePacket(J3DShapePacket* packet) { mpShapePacket = packet; }

	J3DMaterial* getMaterial() const { return mpMaterial; }
	void setMaterial(J3DMaterial* pMaterial) { mpMaterial = pMaterial; }

	void setTexture(J3DTexture* pTexture) { mTexture = pTexture; }

	void setMaterialAnmID(J3DMaterialAnm* materialAnm) { unk44 = materialAnm; }

private:
	bool isHideAllShapePacket_();

public:
	/* 0x34 */ J3DShapePacket* mpShapePacket;
	/* 0x38 */ J3DMaterial* mpMaterial;
	/* 0x3C */ u32 unk3C; // TODO: unk3C is something weird, probably not u32
	/* 0x40 */ J3DTexture* mTexture;
	/* 0x44 */ J3DMaterialAnm* unk44;
};

class J3DShapePacket : public J3DCallBackPacket {
public:
	J3DShapePacket();

	virtual void draw();
	virtual ~J3DShapePacket();

	// from TP debug
	void setShape(J3DShape* pShape) { mpShape = pShape; }
	void setDrawMtx(Mtx** mtx) { mDrawMatrices = mtx; }
	void setNrmMtx(Mtx33** mtx) { mNormMatrices = mtx; }
	void setCurrentViewNoPtr(u32* pCurrentViewNo)
	{
		mpCurrentViewNo = pCurrentViewNo;
	}

	// fabricated
	void setVtxPos(void* pVtxPos) { mpVertexPositions = pVtxPos; }
	void setVtxNrm(void* pVtxNrm) { mpVertexNormals = pVtxNrm; }
	void setVtxCol(GXColor* pVtxCol) { mpVertexColors = pVtxCol; }

	// fabriacted
	void hide() { mVisible = false; }
	void show() { mVisible = true; }
	bool isVisible() const { return mVisible; }

private:
	J3DShape* mpShape;
	Mtx** mDrawMatrices;
	Mtx33** mNormMatrices;
	u32* mpCurrentViewNo;
	void* mpVertexPositions;
	void* mpVertexNormals;
	GXColor* mpVertexColors;
	bool mVisible;
};

#endif
