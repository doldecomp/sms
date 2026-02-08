#include <JSystem/JDrama/JDRLighting.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <dolphin/gx.h>
#include <dolphin/os/OSCache.h>

using namespace JDrama;

void TLight::load(JSUMemoryInputStream& stream)
{
	TPlacement::load(stream);
	JUtility::TColor color = stream.readU32();
	GXInitLightColor(&unk24, color);
}

void TLight::perform(u32 param_1, TGraphics* param_2)
{
	if (!(param_1 & 0x20))
		return;
	correct(param_2);
	GXLoadLightObjImm(&unk24, (GXLightID)(param_2->unkE4 + GX_LIGHT0));
}

void TLight::correct(TGraphics* param_1) const
{
	Vec pos;
	MTXMultVec(param_1->mViewMtx.mMtx, (Vec*)&mPosition, &pos);
	// Ewwwwwwwwwwwwwwwwwwwwwww
	GXInitLightPos(const_cast<GXLightObj*>(&unk24), pos.x, pos.y, pos.z);
}

JStage::TELight TLight::JSGGetLightType() const { return mLightType; }

void TLight::JSGSetLightType(JStage::TELight type)
{
	if (type != JStage::TELIGHT_Unk1)
		return;

	mLightType = type;
}

void TLight::JSGGetPosition(Vec* v) const { *v = mPosition; }

void TLight::JSGSetPosition(const Vec& v)
{
	if (mLightType == JStage::TELIGHT_Unk2)
		return;

	mPosition.x = v.x;
	mPosition.y = v.y;
	mPosition.z = v.z;
}

GXColor TLight::JSGGetColor() const
{
	GXColor result;
	GXGetLightColor(&unk24, &result);
	return result;
}

void TLight::JSGSetColor(GXColor color) { GXInitLightColor(&unk24, color); }

void TLightAry::load(JSUMemoryInputStream& stream)
{
	TNameRef::load(stream);
	setLightNum(stream.readU32());
	for (int i = 0; i < mLightCount; ++i) {
		JSUMemoryInputStream stream2(nullptr, 0);
		TNameRef::getType(stream, stream2);
		mLights[i].load(stream2);
	}
}

TNameRef* TLightAry::searchF(u16 key, const char* name)
{

	TNameRef* result = TNameRef::searchF(key, name);

	if (result)
		return result;

	for (int i = 0; i < mLightCount; ++i) {
		TNameRef* res = mLights[i].searchF(key, name);
		if (res)
			return res;
	}

	return nullptr;
}

void TLightAry::setLightNum(s32 num)
{
	mLightCount = num;
	if (num > 0)
		mLights = new TIdxLight[mLightCount];

	for (int i = 0; i < mLightCount; ++i)
		mLights[i].unk68 = i;
}

void TLightAry::perform(u32 param_1, TGraphics* param_2)
{
	if (!(param_1 & 0x20))
		return;

	for (int i = 0; i < mLightCount; ++i) {
		TIdxLight& light = mLights[i];
		Vec pos;
		MTXMultVec(param_2->mViewMtx.mMtx, &light.mPosition, &pos);
		GXInitLightPos(&light.unk24, pos.x, pos.y, pos.z);
	}

	DCFlushRange(mLights, mLightCount * sizeof(TIdxLight));
	GXSetArray(GX_LIGHT_ARRAY, &mLights[0].unk24, sizeof(TIdxLight));
}

void TAmbColor::load(JSUMemoryInputStream& stream)
{
	TNameRef::load(stream);
	mColor.set(stream.readU32());
}

void TAmbColor::perform(u32 param_1, TGraphics* param_2)
{

	if (!(param_1 & 0x20))
		return;

	GXSetChanAmbColor(GX_COLOR0A0, mColor);
}

GXColor TAmbColor::JSGGetColor() const { return mColor; }

void TAmbColor::JSGSetColor(GXColor color) { mColor = color; }

void TAmbAry::load(JSUMemoryInputStream& stream)
{
	TNameRef::load(stream);
	setAmbNum(stream.readU32());
	for (int i = 0; i < mAmbColorCount; ++i) {
		JSUMemoryInputStream stream2(nullptr, 0);
		TNameRef::getType(stream, stream2);
		mAmbColors[i].load(stream2);
	}
}

TNameRef* TAmbAry::searchF(u16 key, const char* name)
{
	TNameRef* result = TNameRef::searchF(key, name);

	if (result)
		return result;

	for (int i = 0; i < mAmbColorCount; ++i) {
		TNameRef* res = mAmbColors[i].searchF(key, name);
		if (res)
			return res;
	}

	return nullptr;
}

void TAmbAry::setAmbNum(s32 num)
{
	mAmbColorCount = num;
	if (num > 0)
		mAmbColors = new TAmbColor[mAmbColorCount];
}

void TLightMap::load(JSUMemoryInputStream& stream)
{
	mLightInfoCount = stream.readS32();
	mLightInfos     = new TLightInfo[mLightInfoCount];

	for (int i = 0; i < mLightInfoCount; ++i) {
		mLightInfos[i].unk0 = stream.readU32();
		char buf[0x50];
		stream.readString(buf, 0x50);
		mLightInfos[i].unk4
		    = (TViewObj*)TNameRefGen::getInstance()->getRootNameRef()->search(
		        buf);
	}
}

void TLightMap::perform(u32 param_1, TGraphics* param_2)
{
	if (!(param_1 & 0x20))
		return;

	for (int i = 0; i < mLightInfoCount; ++i) {
		if (mLightInfos[i].unk4) {
			param_2->unkE4 = mLightInfos[i].unk0;
			mLightInfos[i].unk4->perform(param_1, param_2);
		}
	}
}
