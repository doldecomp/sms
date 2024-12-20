#ifndef J3D_MATERIAL_HPP
#define J3D_MATERIAL_HPP

#include <dolphin/types.h>

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

	char padding[0x38];
	J3DMaterialAnm* unk38;
};

#endif
