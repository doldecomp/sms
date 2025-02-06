#ifndef J3D_MATERIAL_HPP
#define J3D_MATERIAL_HPP

#include <types.h>
#include <JSystem/J3d/J3DGraphBase/J3DTevBlocks.hpp>

class J3DMaterialAnm;

class J3DMaterial {
public:
	typedef void IDK;
	IDK makeDisplayList();
	IDK load();
	IDK patch();
	IDK safeMakeDisplayList();
	IDK safeLoad();
	IDK calc(float (*)[4]);
	IDK setCurrentMtx();
	IDK copy(J3DMaterial*);
	IDK reset();
	IDK change();
	IDK newSharedDisplayList(u32);

	void setMaterialAnm(J3DMaterialAnm* v) { unk38 = v; }

	~J3DMaterial();

	u16 getTexNo(u32 idx) { return mTevBlock->getTexNo(idx); }

	/* 0x0 */ char unk0[0x28];
	/* 0x28 */ J3DTevBlock* mTevBlock;
	/* 0x2C */ char unk2C[0xC];
	/* 0x38 */ J3DMaterialAnm* unk38;
};

#endif
