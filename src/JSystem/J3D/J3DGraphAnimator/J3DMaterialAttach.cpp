#include <JSystem/J3D/J3DGraphAnimator/J3DMaterialAttach.hpp>

void J3DMaterialTable::clear()
{
	mMaterialNum  = 0;
	mMaterials    = nullptr;
	mTexture      = nullptr;
	mMaterialName = nullptr;
	mTextureName  = nullptr;
}

J3DMaterialTable::J3DMaterialTable() { clear(); }

J3DMaterialTable::~J3DMaterialTable() { }
