#ifndef J3D_MATERIAL_ATTACH_HPP
#define J3D_MATERIAL_ATTACH_HPP

#include <types.h>
#include <JSystem/J3D/J3DAssert.hpp>

class J3DMaterial;
class JUTNameTab;
class J3DTexture;

class J3DMaterialTable {
	friend class J3DModelLoader;
	friend class J3DModelLoader_v21;
	friend class J3DModelLoader_v26;

public:
	J3DMaterialTable();
	virtual ~J3DMaterialTable();

	void clear();

	J3DMaterial* getMaterialNodePointer(u16 idx) const
	{
		J3D_ASSERT_RANGE(idx < mMaterialNum);
		return mMaterials[idx];
	}
	u16 getMaterialNum() const { return mMaterialNum; }

	J3DTexture* getTexture() const { return mTexture; }
	void setTexture(J3DTexture* pTexture) { mTexture = pTexture; }

	JUTNameTab* getMaterialName() const { return mMaterialName; }
	JUTNameTab* getTextureName() const { return mTextureName; }

private:
	/* 0x04 */ u16 mMaterialNum;
	/* 0x06 */ u16 mMaterialInitNum;
	/* 0x08 */ J3DMaterial** mMaterials;
	/* 0x0C */ J3DTexture* mTexture;
	/* 0x10 */ JUTNameTab* mMaterialName;
	/* 0x14 */ JUTNameTab* mTextureName;
};

#endif
