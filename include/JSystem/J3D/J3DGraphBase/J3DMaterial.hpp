#ifndef J3D_MATERIAL_HPP
#define J3D_MATERIAL_HPP

#include <types.h>
#include <JSystem/J3d/J3DGraphBase/Blocks/J3DTevBlocks.hpp>
#include <JSystem/J3D/J3DGraphBase/Blocks/J3DTexGenBlocks.hpp>

class J3DMaterialAnm;
class J3DShape;

class J3DColorBlock;
class J3DTexGenBlock;
class J3DTevBlock;
class J3DIndBlock;
class J3DPEBlock;
class J3DDisplayListObj;

class J3DMaterial {
public:
	J3DMaterial() { initialize(); }
	~J3DMaterial() { }

	static J3DColorBlock* createColorBlock(int);
	static J3DTexGenBlock* createTexGenBlock(int);
	static J3DTevBlock* createTevBlock(int);
	static J3DIndBlock* createIndBlock(int);
	static J3DPEBlock* createPEBlock(int, u32);

	void initialize();
	void addShape(J3DShape*);

	s32 countDLSize();

	void makeDisplayList();
	void load();
	void patch(); // Unused
	void safeMakeDisplayList();
	void safeLoad(); // Unused

	void calc(MtxPtr);
	void setCurrentMtx();
	void copy(J3DMaterial*);

	void reset(); // Unused
	void change();
	J3DDisplayListObj* newSharedDisplayList(u32);

	void setMaterialAnm(J3DMaterialAnm* v) { unk38 = v; }

	u16 getTexNo(u32 idx) { return mTevBlock->getTexNo(idx); }

	J3DColorBlock* getColorBlock() { return mColorBlock; }
	J3DTexGenBlock* getTexGenBlock() { return mTexGenBlock; }
	J3DTevBlock* getTevBlock() { return mTevBlock; }
	J3DPEBlock* getPEBlock() { return mPEBlock; }

	J3DGXColor* getTevKColor(u32 idx) { return mTevBlock->getTevKColor(idx); }
	J3DGXColorS10* getTevColor(u32 idx) { return mTevBlock->getTevColor(idx); }
	J3DTexMtx* getTexMtx(u32 idx) { return mTexGenBlock->getTexMtx(idx); }
	J3DTexCoord* getTexCoord(u32 idx) { return mTexGenBlock->getTexCoord(idx); }
	J3DNBTScale* getNBTScale() { return mTexGenBlock->getNBTScale(); }

	J3DMaterial* getNext() { return mNext; }
	void setNext(J3DMaterial* material) { mNext = material; }
	J3DShape* getShape() { return mShape; }
	u16 getIndex() { return unkC; }

	J3DMaterialAnm* getMaterialAnm()
	{
		if ((u32)unk38 < 0xC0000000) {
			return unk38;
		} else {
			return nullptr;
		}
	}

	GXBool isDrawModeOpaTexEdge() { return (unk8 & 3) ? GX_TRUE : GX_FALSE; }

	// TODO: presumably this is something called diff flag?
	BOOL getSomeFlag() { return unk1C & 1 ? TRUE : FALSE; }
	void setSomeFlag() { unk1C |= 1; }

public:
	/* 0x0 */ J3DMaterial* mNext;
	/* 0x4 */ J3DShape* mShape;
	/* 0x8 */ u32 unk8;
	/* 0xC */ u16 unkC;
	/* 0x10 */ u32 unk10;
	/* 0x14 */ char unk14[4];
	/* 0x18 */ u32 unk18;
	/* 0x1C */ u32 unk1C;
	/* 0x20 */ J3DColorBlock* mColorBlock;
	/* 0x24 */ J3DTexGenBlock* mTexGenBlock;
	/* 0x28 */ J3DTevBlock* mTevBlock;
	/* 0x2C */ J3DIndBlock* mIndBlock;
	/* 0x30 */ J3DPEBlock* mPEBlock;
	/* 0x34 */ J3DMaterial* mOriginalMaterial;
	/* 0x38 */ J3DMaterialAnm* unk38;
	/* 0x3C */ J3DDisplayListObj* unk3C;
};

#endif
