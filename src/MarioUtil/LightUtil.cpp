#include <MarioUtil/LightUtil.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>

TLightWithDBSetManager* gpLightManager;

TLightCommon::TLightCommon(const char* name)
    : JDrama::TViewObj(name)
{
}

void TLightCommon::loadAfter() { }

void TLightCommon::getLightColor(int) const { }

void TLightCommon::getAmbColor(int) const { }

void TLightCommon::getLightPosition(int) { }

void TLightCommon::setLight(const JDrama::TGraphics*, int) { }

void TLightCommon::perform(u32, JDrama::TGraphics*) { }

void TLightShadow::perform(u32, JDrama::TGraphics*) { }

void TLightMario::perform(u32, JDrama::TGraphics*) { }

void TLightMario::setLight(const JDrama::TGraphics*, int) { }

void TLightMario::getLightColor(int) const { }

void TLightMario::getAmbColor(int) const { }

TLightDrawBuffer::TLightDrawBuffer(int, u32, const char* name)
    : JDrama::TViewObj(name)
{
}

void TLightDrawBuffer::perform(u32, JDrama::TGraphics*) { }

void TLightWithDBSet::perform(u32, JDrama::TGraphics*) { }

void TLightWithDBSet::changeLightDrawBuffer(int param_1)
{
	unk14 = nullptr;
	unk18 = nullptr;
	if (param_1 > unk1C)
		param_1 = 0;

	unk14 = j3dSys.getDrawBuffer(0);
	unk18 = j3dSys.getDrawBuffer(1);

	j3dSys.setDrawBuffer(unk10[param_1]->unk14->mDrawBuffer, 0);
	j3dSys.setDrawBuffer(unk10[param_1]->unk18->mDrawBuffer, 1);
}

void TLightWithDBSet::resetLightDrawBuffer()
{
	if (!unk14)
		return;
	if (!unk18)
		return;

	j3dSys.setDrawBuffer(unk14, 0);
	j3dSys.setDrawBuffer(unk18, 1);
	unk14 = nullptr;
	unk18 = nullptr;
}

void TPlayerLightWithDBSet::makeDrawBuffer() { }

void TObjectLightWithDBSet::makeDrawBuffer() { }

void TMapObjectLightWithDBSet::makeDrawBuffer() { }

void TIndirectLightWithDBSet::makeDrawBuffer() { }

TLightWithDBSetManager::TLightWithDBSetManager(const char* name)
    : JDrama::TViewObj(name)
{
}

void TLightWithDBSetManager::loadAfter() { }

void TLightWithDBSetManager::perform(u32, JDrama::TGraphics*) { }

void TLightWithDBSetManager::addChildGroupObj(
    JDrama::TViewObjPtrListT<JDrama::TViewObj, JDrama::TViewObj>*)
{
}

void TLightWithDBSetManager::makeDrawBuffer()
{
	for (int i = 0; i < 4; ++i)
		if (unk14[i]->unk20)
			unk14[i]->makeDrawBuffer();
}

void TLightWithDBSetManager::getLightPos() const { }
