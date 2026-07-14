#include <MarioUtil/LightUtil.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <MarioUtil/ReinitGX.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JDrama/JDRLighting.hpp>
#include <System/MarDirector.hpp>
#include <Player/MarioAccess.hpp>
#include <stdio.h>
#include <string.h>

// TODO: figure out headers & PCH
static const char dummy1[] = "\0\0\0\0\0\0\0\0\0\0\0";

JDrama::TAmbAry* TLightCommon::mAmbAry;
JDrama::TLightAry* TLightCommon::mLightAry;
Vec* TLightCommon::mLightPos;

TLightWithDBSetManager* gpLightManager;

TLightCommon::TLightCommon(const char* name)
    : JDrama::TViewObj(name)
    , unk10(0.0f)
    , unk14(1.0f)
    , unk18(1.0f)
    , unk1C(1.0f)
    , unk20(0)
    , unk24(0)
    , unk28(0)
    , unk41(0)
{
	mAmbAry   = nullptr;
	mLightAry = nullptr;
	mLightPos = nullptr;
	unk10     = 50.0f;
}

void TLightCommon::loadAfter()
{
	mAmbAry   = JDrama::TNameRefGen::search<JDrama::TAmbAry>("Ambient Group");
	mLightAry = JDrama::TNameRefGen::search<JDrama::TLightAry>("Light Group");
	mLightPos = &mLightAry->getLight(0)->mPosition;
	unk10     = 50.0f;
	for (int i = 0; i < 4; ++i) {
		unk31[i] = mLightAry->getLight(unk24 + i)->getColor();
		unk44[i] = mLightAry->getLight(unk24 + i)->mPosition;
	}
	unk29[0] = mAmbAry->getAmb(unk20)->getColor();
	unk29[1] = mAmbAry->getAmb(unk20 + 1)->getColor();
}

GXColor TLightCommon::getLightColor(int index) const
{
	if (unk28) {
		if (index >= 4)
			index = 0;
		return unk31[index];
	}
	index += unk24;
	GXColor color = mLightAry->getLight(index)->getColor();
	color.a *= unk1C;
	return color;
}

GXColor TLightCommon::getAmbColor(int index) const
{
	if (unk28) {
		if (index >= 2)
			index = 0;
		return unk29[index];
	}
	index += unk20;
	GXColor color = mAmbAry->getAmb(index)->getColor();
	color.a *= unk18;
	return color;
}

Vec* TLightCommon::getLightPosition(int index)
{
	if (unk41) {
		if (index >= 4)
			index = 0;
		return unk44[index];
	}
	index += unk24;
	return &mLightAry->getLight(index)->mPosition;
}

void TLightCommon::setLight(const JDrama::TGraphics* gfx, int index)
{
	ReInitializeGX();
	SMS_DrawInit();
	int lightIndex = index * 2;

	GXLightObj light;
	Vec pos;
	MTXMultVec(gfx->getViewMtx(), getLightPosition(lightIndex), &pos);
	GXInitLightPos(&light, pos.x, pos.y, pos.z);
	GXInitLightColor(&light, getLightColor(lightIndex));
	GXInitLightAttn(&light, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	GXLoadLightObjImm(&light, GX_LIGHT0);

	gpLightManager->setEffectLight(gfx, &light);

	Vec spos;
	MTXMultVec(gfx->getViewMtx(), getLightPosition(lightIndex), &spos);
	VECNormalize(&spos, &spos);
	GXInitSpecularDir(&light, -spos.x, -spos.y, -spos.z);
	GXInitLightColor(&light, getLightColor(lightIndex));
	GXInitLightAttn(&light, 0.0f, 0.0f, 1.0f, unk10 / 2.0f, 0.0f,
	                1.0f - unk10 / 2.0f);
	GXLoadLightObjImm(&light, GX_LIGHT2);

	GXSetChanAmbColor(GX_COLOR0A0, getAmbColor(index));
}

void TLightCommon::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_DRAW_INIT) {
		ReInitializeGX();
		SMS_DrawInit();
		GXLightObj light;
		GXInitLightPos(&light, getLightPosition(0)->x, getLightPosition(0)->y,
		               getLightPosition(0)->z);
		GXInitLightColor(&light, getLightColor(0));
		GXInitLightAttn(&light, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		GXLoadLightObjImm(&light, GX_LIGHT0);
		GXLoadLightObjImm(&light, GX_LIGHT1);
		GXLoadLightObjImm(&light, GX_LIGHT2);
	}
	if (cue & CUE_LIGHT)
		setLight(graphics, 0);
}

void TLightShadow::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_LIGHT)
		setLight(graphics, 1);
}

void TLightMario::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_LIGHT)
		setLight(graphics, *gpMarioLightID);
}

void TLightMario::setLight(const JDrama::TGraphics* gfx, int index)
{
	ReInitializeGX();
	SMS_DrawInit();
	int lightIndex = index * 2;

	GXLightObj light;
	Vec pos;
	MTXMultVec(gfx->getViewMtx(), getLightPosition(lightIndex), &pos);
	GXInitLightPos(&light, pos.x, pos.y, pos.z);
	GXInitLightColor(&light, getLightColor(lightIndex));
	GXInitLightAttn(&light, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	GXLoadLightObjImm(&light, GX_LIGHT0);

	gpLightManager->setEffectLight(gfx, &light);

	Vec spos;
	MTXMultVec(gfx->getViewMtx(), getLightPosition(lightIndex), &spos);
	VECNormalize(&spos, &spos);
	GXInitSpecularDir(&light, -spos.x, -spos.y, -spos.z);
	GXInitLightColor(&light, getLightColor(lightIndex));
	GXInitLightAttn(&light, 0.0f, 0.0f, 1.0f, unk10 / 2.0f, 0.0f,
	                1.0f - unk10 / 2.0f);
	GXLoadLightObjImm(&light, GX_LIGHT2);

	GXSetChanAmbColor(GX_COLOR0A0, getAmbColor(index));
}

GXColor TLightMario::getLightColor(int index) const
{
	GXColor color = TLightCommon::getLightColor(index + unk24);
	color.a *= unk14;
	return color;
}

GXColor TLightMario::getAmbColor(int index) const
{
	index += unk24;
	GXColor color = TLightCommon::getAmbColor(index);
	color.a *= unk14;
	return color;
}

#pragma dont_inline on
TLightDrawBuffer::TLightDrawBuffer(int param_1, u32 param_2, const char* name)
    : JDrama::TViewObj(name)
    , unk10(nullptr)
    , mOpaDrawBufferObject(nullptr)
    , mXluDrawBufferObject(nullptr)
    , unk80(param_1)
{
	snprintf(unk1C, 0x32, "%s%s", name, "opa");
	mOpaDrawBufferObject = new JDrama::TDrawBufObj(3, param_2, unk1C);

	snprintf(unk4E, 0x32, "%s%s", name, "xlu");
	mXluDrawBufferObject = new JDrama::TDrawBufObj(4, param_2, unk4E);
}
#pragma dont_inline reset

void TLightDrawBuffer::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_LIGHT)
		unk10->setLight(graphics, unk80);
}

TLightWithDBSet::TLightWithDBSet(int param_1, const char* name)
    : JDrama::TViewObj(name)
{
	unk10 = nullptr;
	unk14 = nullptr;
	unk18 = nullptr;
	unk1C = param_1;
	unk20 = 0;
}

void TLightWithDBSet::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_LIGHT) {
		for (int i = 0; i < unk1C; ++i) {
			unk10[i]->perform(CUE_LIGHT, graphics);
			if (cue & CUE_UNK10000)
				unk10[i]->getOpaDbo()->perform(CUE_DRAW, graphics);
			if (cue & CUE_UNK20000)
				unk10[i]->getXluDbo()->perform(CUE_DRAW, graphics);
		}
	}
	if (cue & CUE_SET_DRAW_BUFFER) {
		for (int i = 0; i < unk1C; ++i) {
			unk10[i]->getOpaDbo()->perform(CUE_SET_DRAW_BUFFER | CUE_DRAW_INIT,
			                               graphics);
			unk10[i]->getXluDbo()->perform(CUE_SET_DRAW_BUFFER | CUE_DRAW_INIT,
			                               graphics);
		}
	}
}

void TLightWithDBSet::addChildGroupObj(
    JDrama::TViewObjPtrListT<JDrama::TViewObj>* list)
{
	if (unk20) {
		for (int j = 0; j < unk1C; ++j) {
			list->insert(unk10[j]->getOpaDbo());
			list->insert(unk10[j]->getXluDbo());
		}
	}
}

void TLightWithDBSet::changeLightDrawBuffer(int param_1)
{
	unk14 = nullptr;
	unk18 = nullptr;
	if (param_1 > unk1C)
		param_1 = 0;

	unk14 = j3dSys.getDrawBuffer(0);
	unk18 = j3dSys.getDrawBuffer(1);

	j3dSys.setDrawBuffer(unk10[param_1]->getOpaDbo()->getDrawBuffer(), 0);
	j3dSys.setDrawBuffer(unk10[param_1]->getXluDbo()->getDrawBuffer(), 1);
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

void TLightWithDBSet::getOpaDrawBuffer(int) { }

void TLightWithDBSet::getXluDrawBuffer(int) { }

void TLightWithDBSet::getLightDrawBuffer(int) { }

int TLightWithDBSet::getLightIndex(const char* name)
{
	for (int i = 0; i < TLightCommon::mLightAry->getLightNum(); ++i)
		if (strcmp(name, TLightCommon::mLightAry->getLight(i)->getName()) == 0)
			return i;
	return -1;
}

int TLightWithDBSet::getAmbIndex(const char* name)
{
	for (int i = 0; i < TLightCommon::mAmbAry->getAmbNum(); ++i)
		if (strcmp(name, TLightCommon::mAmbAry->getAmb(i)->getName()) == 0)
			return i;
	return -1;
}

void TPlayerLightWithDBSet::makeDrawBuffer()
{
	static const char lightName[] = "太陽（プレイヤー）";
	static const char ambName[]   = "太陽アンビエント（プレイヤー）";

	int lightIndex = getLightIndex(lightName);
	int ambIndex   = getAmbIndex(ambName);
	unk10          = new TLightDrawBuffer*[unk1C];
	for (int i = 0; i < unk1C; ++i) {
		unk10[i] = new TLightDrawBuffer(
		    i, 0x80, TLightCommon::mAmbAry->getAmb(ambIndex + i)->getName());
		TLightMario* light = new TLightMario();
		unk10[i]->setLight(light);
		unk10[i]->unk10->unk20 = ambIndex;
		unk10[i]->unk10->unk24 = lightIndex;
		unk10[i]->unk10->loadAfter();
	}
}

void TObjectLightWithDBSet::makeDrawBuffer()
{
	static const char lightName[] = "太陽（オブジェクト）";
	static const char ambName[]   = "太陽アンビエント（オブジェクト）";

	int lightIndex = getLightIndex(lightName);
	int ambIndex   = getAmbIndex(ambName);
	unk10          = new TLightDrawBuffer*[unk1C];
	for (int i = 0; i < unk1C; ++i) {
		unk10[i] = new TLightDrawBuffer(
		    i, 0x100, TLightCommon::mAmbAry->getAmb(ambIndex + i)->getName());
		TLightCommon* light = new TLightCommon();
		unk10[i]->setLight(light);
		unk10[i]->unk10->unk20 = ambIndex;
		unk10[i]->unk10->unk24 = lightIndex;
		unk10[i]->unk10->loadAfter();
	}
}

void TMapObjectLightWithDBSet::makeDrawBuffer()
{
	static const char lightName[] = "太陽（オブジェクト）";
	static const char ambName[]   = "太陽アンビエント（オブジェクト）";

	int lightIndex = getLightIndex(lightName);
	int ambIndex   = getAmbIndex(ambName);
	static const char* className[]
	    = { "マップオブジェ太陽", "マップオブジェ影" };
	unk10 = new TLightDrawBuffer*[unk1C];
	for (int i = 0; i < unk1C; ++i) {
		unk10[i]            = new TLightDrawBuffer(i, 0x100, className[i]);
		TLightCommon* light = new TLightCommon();
		unk10[i]->setLight(light);
		unk10[i]->unk10->unk20 = ambIndex;
		unk10[i]->unk10->unk24 = lightIndex;
		unk10[i]->unk10->loadAfter();
	}
}

void TIndirectLightWithDBSet::makeDrawBuffer()
{
	static const char lightName[] = "太陽（オブジェクト）";
	static const char ambName[]   = "太陽アンビエント（オブジェクト）";

	int lightIndex = getLightIndex(lightName);
	int ambIndex   = getAmbIndex(ambName);
	static const char* className[]
	    = { "インダイレクト太陽", "インダイレクト影" };
	unk10 = new TLightDrawBuffer*[unk1C];
	for (int i = 0; i < unk1C; ++i) {
		unk10[i]            = new TLightDrawBuffer(i, 0x100, className[i]);
		TLightCommon* light = new TLightCommon();
		unk10[i]->setLight(light);
		unk10[i]->unk10->unk20 = ambIndex;
		unk10[i]->unk10->unk24 = lightIndex;
		unk10[i]->unk10->loadAfter();
	}
}

TPlayerLightWithDBSet::TPlayerLightWithDBSet()
    : TLightWithDBSet(2, "プレイヤー用ライト")
{
}

TObjectLightWithDBSet::TObjectLightWithDBSet()
    : TLightWithDBSet(2, "オブジェクト用ライト")
{
}

TMapObjectLightWithDBSet::TMapObjectLightWithDBSet()
    : TLightWithDBSet(2, "マップオブジェクト用ライト")
{
}

TIndirectLightWithDBSet::TIndirectLightWithDBSet()
    : TLightWithDBSet(2, "インダイレクトモデル用ライト")
{
}

TLightWithDBSetManager::TLightWithDBSetManager(const char* name)
    : JDrama::TViewObj(name)
{
	unk10          = nullptr;
	unk14          = nullptr;
	unk54          = 0;
	unk55          = 1;
	unk14          = new TLightWithDBSet*[4];
	unk14[0]       = new TPlayerLightWithDBSet();
	unk14[1]       = new TObjectLightWithDBSet();
	unk14[2]       = new TMapObjectLightWithDBSet();
	unk14[3]       = new TIndirectLightWithDBSet();
	gpLightManager = this;
	unk48.x        = 0.0f;
	unk48.y        = 0.0f;
	unk48.z        = 0.0f;
	unk28          = 1.0f;
	unk2C          = 100.0f;
	unk30          = 400.0f;
	unk34          = 1000.0f;
	unk38          = 1.80535f;
	unk3C          = -0.012058f;
	unk40          = 0.00003f;
	unk44          = 90.0f;
	calcLightBorder();
}

void TLightWithDBSetManager::loadAfter()
{
	JDrama::TLightAry* group
	    = JDrama::TNameRefGen::search<JDrama::TLightAry>("Light Group");
	unk18 = group->getLight(0)->getColor();
	unk1C = group->getLight(0)->mPosition;
}

void TLightWithDBSetManager::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_LIGHT) {
		int start;
		int end;
		if (cue & CUE_UNK80000) {
			start = 3;
			end   = 4;
		} else if (cue & CUE_UNK40000) {
			start = 2;
			end   = 3;
		} else {
			start = 0;
			end   = 2;
		}
		for (int i = start; i < end; ++i)
			if (unk14[i]->isEnabled())
				unk14[i]->perform(cue, graphics);
	}
	if (cue & CUE_SET_DRAW_BUFFER) {
		for (int i = 0; i < 4; ++i)
			if (unk14[i]->isEnabled())
				unk14[i]->perform(cue, graphics);
	}
}

void TLightWithDBSetManager::addChildGroupObj(
    JDrama::TViewObjPtrListT<JDrama::TViewObj>* list)
{
	for (int i = 0; i < 4; ++i)
		unk14[i]->addChildGroupObj(list);
}

void TLightWithDBSetManager::makeDrawBuffer()
{
	for (int i = 0; i < 4; ++i)
		if (unk14[i]->isEnabled())
			unk14[i]->makeDrawBuffer();
}

Vec* TLightWithDBSetManager::getLightPos() const
{
	return TLightCommon::mLightPos;
}

void TLightWithDBSetManager::setEffectLight(const JDrama::TGraphics* gfx,
                                            GXLightObj* light)
{
	if (unk54 && unk55) {
		Vec epos;
		MTXMultVec(gfx->getViewMtx(), unk1C, &epos);
		GXInitLightPos(light, epos.x, epos.y, epos.z);
		GXInitLightColor(light, getEffectLightColor());
		GXInitLightAttnA(light, 1.0f, 0.0f, 0.0f);
		GXInitLightDistAttn(light, 1000.0f, 0.5f, GX_DA_STEEP);
		GXLoadLightObjImm(light, GX_LIGHT1);
	}
}

GXColor TLightWithDBSetManager::getEffectLightColor() const
{
	GXColor result = unk18;
	result.a *= unk28;
	return result;
}

void TLightWithDBSetManager::calcLightBorder()
{
	f32 a[3] = { 0.9f, 0.5f, 0.05f };
	f32 b[3];
	b[0] = unk2C;
	b[1] = unk30;
	b[2] = unk34;

	f32 P[2];
	f32 Q[2];
	f32 R[2];
	for (int i = 0; i < 2; ++i) {
		P[i] = a[i + 1] * (a[i] * (b[i] * b[i] - b[i + 1] * b[i + 1]));
		Q[i] = a[i + 1] * (a[i] * (b[i] - b[i + 1]));
		R[i] = a[i + 1] - a[i];
	}

	unk40 = (R[0] * Q[1] - R[1] * Q[0]) / (P[0] * Q[1] - P[1] * Q[0]);
	unk3C = (R[0] - P[0] * unk40) / Q[0];
	unk38 = a[0] - (unk40 * (b[0] * b[0]) + b[0] * unk3C);
}
