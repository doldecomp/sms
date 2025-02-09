#ifndef J3D_MATERIAL_ATTACH_HPP
#define J3D_MATERIAL_ATTACH_HPP

#include <types.h>

class J3DMaterial;

class J3DMaterialTable {
public:
	J3DMaterialTable();
	virtual ~J3DMaterialTable();

	void clear();

	J3DMaterial* getMaterial(u16 idx) const { return mMaterials[idx]; }
	u16 getMaterialNum() const { return mMaterialNum; }

	/* 0x04 */ u16 mMaterialNum;
	/* 0x06 */ u16 mMaterialInitNum;
	/* 0x08 */ J3DMaterial** mMaterials;
	/* 0x0C */ void* unkC;
	/* 0x10 */ void* unk10;
	/* 0x14 */ void* unk14;
};

#endif
