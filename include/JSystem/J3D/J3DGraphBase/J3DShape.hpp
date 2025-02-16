#ifndef J3D_SHAPE_HPP
#define J3D_SHAPE_HPP

#include <JSystem/J3D/J3DGraphBase/J3DVertex.hpp>
#include <dolphin/mtx.h>
#include <dolphin/gx.h>

class J3DShapeMtx {
public:
	typedef void (J3DShapeMtx::*LoadPipeline)(int, u16) const;

	J3DShapeMtx(u16 useMtxIndex)
	    : unk4(useMtxIndex)
	{
	}

	virtual ~J3DShapeMtx() { }
	virtual int getType() const { return 'SMTX'; }
	virtual u32 getUseMtxNum() const { return 1; }
	virtual u16 getUseMtxIndex(u16) const { return unk4; }
	virtual void load() const;
	virtual void calcNBTScale(const Vec&, float (*)[3][3], float (*)[3][3]);

	void loadMtxIndx_PNGP(int, u16) const;
	void loadMtxIndx_PCPU(int, u16) const;
	void loadMtxIndx_NCPU(int, u16) const;
	void loadMtxIndx_PNCPU(int, u16) const;

	static LoadPipeline mtxLoadPipeline[4];
	static u32 currentPipeline;

public:
	u16 unk4;
};

class J3DShapeMtxDL : public J3DShapeMtx {
public:
	J3DShapeMtxDL(u16);

	virtual ~J3DShapeMtxDL() { }
	virtual void load() const;
	virtual void calcNBTScale(const Vec&, float (*)[3][3], float (*)[3][3]) { }

public:
	void* mDisplayList;
};

class J3DShapeMtxMulti : public J3DShapeMtx {
public:
	J3DShapeMtxMulti(u16 useMtxIndex, u16 useMtxNum, u16* useMtxIndexTable)
	    : J3DShapeMtx(useMtxIndex)
	    , unk8(useMtxNum)
	    , unkC(useMtxIndexTable)
	{
	}

	virtual ~J3DShapeMtxMulti() { }
	virtual int getType() const { return 'SMML'; }
	virtual u32 getUseMtxNum() const { return unk8; }
	virtual u16 getUseMtxIndex(u16 i) const { return unkC[i]; }
	virtual void load() const;
	virtual void calcNBTScale(const Vec&, float (*)[3][3], float (*)[3][3]);

public:
	u16 unk8;
	u16* unkC;
};

class J3DShapeDraw {
public:
	J3DShapeDraw(const u8*, u32);

	virtual ~J3DShapeDraw() { }

	void draw() const;

	u8* getDisplayList() const { return (u8*)mDisplayList; }
	u32 getDisplayListSize() const { return mDisplayListSize; }

public:
	u32 mDisplayListSize;
	const u8* mDisplayList;
};

class J3DShape {
public:
	enum {
		kVcdVatDLSize = 0xC0,
	};

	J3DShape()
	{
		unk3C[0] = 0x3C;
		unk3C[1] = 0x3C;
		unk3C[2] = 0x3C;
		unk3C[3] = 0x3C;
		unk3C[4] = 0x3C;
		unk3C[5] = 0x3C;
		unk3C[6] = 0x3C;
		unk3C[7] = 0x3C;
		initialize();
	}
	~J3DShape();
	void initialize();
	void calcNBTScale(const Vec&, float (*)[3][3], float (*)[3][3]);
	int countBumpMtxNum() const;
	void makeVtxArrayCmd();
	void makeVcdVatCmd();
	void loadVtxArray() const;
	void draw() const;

	void setUnk3C(u8 a, u8 b, u8 c, u8 d, u8 e, u8 f, u8 g, u8 h)
	{
		unk3C[0] = a;
		unk3C[1] = b;
		unk3C[2] = c;
		unk3C[3] = d;
		unk3C[4] = e;
		unk3C[5] = f;
		unk3C[6] = g;
		unk3C[7] = h;
	}

	bool checkFlag(u32 flag) const { return (unk8 & flag) ? TRUE : FALSE; }
	void onFlag(u32 flag) { unk8 |= flag; }
	void offFlag(u32 flag) { unk8 &= ~flag; }

	u32 getIndex() const { return unk4; }
	GXVtxDescList* getVtxDesc() const { return unk2C; }
	u32 getMtxGroupNum() const { return mElementCount; }
	J3DShapeMtx* getShapeMtx(u16 idx) const { return mMatrices[idx]; }
	J3DShapeDraw* getShapeDraw(u16 idx) const { return mDraws[idx]; }
	u32 getBumpMtxOffset() const { return unk5C; }

	void setScaleFlagArray(u8* pScaleFlagArray) { unk4C = pScaleFlagArray; }

public:
	/* 0x0 */ u32 unk0;
	/* 0x4 */ u16 unk4;
	/* 0x6 */ u16 mElementCount;
	/* 0x8 */ u32 unk8;
	/* 0xC */ float unkC;
	/* 0x10 */ Vec unk10;
	/* 0x1C */ Vec unk1C;
	/* 0x28 */ void* mGDCommands;
	/* 0x2C */ GXVtxDescList* unk2C;
	/* 0x30 */ bool unk30;
	/* 0x31 */ char unk31[3];
	/* 0x34 */ J3DShapeMtx** mMatrices; // mElementCount entries
	/* 0x38 */ J3DShapeDraw** mDraws;   // mElementCount entries
	/* 0x3C */ u8 unk3C[8];
	/* 0x44 */ J3DVertexData* unk44;
	/* 0x48 */ J3DDrawMtxData* unk48;
	/* 0x4C */ u8* unk4C;
	/* 0x50 */ Mtx** mDrawMatrices;
	/* 0x54 */ Mtx33** mNormMatrices;
	/* 0x58 */ u32* mCurrentViewNo;
	/* 0x5C */ u32 unk5C;
};

#endif
