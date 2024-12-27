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

	// TODO: stolen from mkdd, may be wrong
	u16 mMaterialNum;
	u16 mMaterialInitNum;
	J3DMaterial** mMaterials;
	// ...more stuff
};

#endif
