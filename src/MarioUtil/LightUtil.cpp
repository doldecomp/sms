// The 12-byte zero object retail's .rodata opens with is @1490,
// dummyMactorStringValue1's string; LightUtil is one of the three TUs that
// carry it without SMS_NO_MEMORY_MESSAGE.
#include <System/DummyMactorString.hpp>

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

JDrama::TAmbAry* TLightCommon::mAmbAry;
JDrama::TLightAry* TLightCommon::mLightAry;
Vec* TLightCommon::mLightPos;

TLightWithDBSetManager* gpLightManager;

TLightCommon::TLightCommon(const char* name)
    : JDrama::TViewObj(name)
    , mShininess(0.0f)
    , unk14(1.0f)
    , unk18(1.0f)
    , unk1C(1.0f)
    , mAmbIndex(0)
    , mLightIndex(0)
    , unk28(0)
    , unk41(0)
{
	mAmbAry    = nullptr;
	mLightAry  = nullptr;
	mLightPos  = nullptr;
	mShininess = 50.0f;
}

// TODO: 97.7%. Two residues. (1) 32 bytes of frame too much (0xc0 vs 0xa0),
// all of it low region: our named block has the same shape (the GXGetLightColor
// out-parameter 0x10 below the register saves in both). (2) the ROM
// round-trips the first `unk29` colour through a 4-byte temporary at 0x18(r1)
// (`stw r0, 0x18(r1); lwz r0, 0x18(r1); stw r0, 0x29(r30)`) while the second
// stores straight to 0x2d -- the signature of one extra inline level on the
// FIRST amb read only (the batch-91 "+4 low goes on the earliest expansion"
// family). JDRLighting.hpp's TAmbColor::getColor() returns
// `const JUtility::TColor&`; a by-value return there would bind that
// temporary, but it is a shared header and 23 other TUs read it.
void TLightCommon::loadAfter()
{
	mAmbAry    = (JDrama::TAmbAry*)JDrama::TNameRefGen::search2(
	    "Ambient Group");
	mLightAry  = (JDrama::TLightAry*)JDrama::TNameRefGen::search2(
	    "Light Group");
	mLightPos  = &mLightAry->getLight(0)->mPosition;
	mShininess = 50.0f;
	for (int i = 0; i < 4; ++i) {
		unk31[i] = mLightAry->getLight(i + mLightIndex)->getColor();
		unk44[i] = mLightAry->getLight(i + mLightIndex)->mPosition;
	}
	unk29[0] = mAmbAry->getAmb(mAmbIndex)->getColor();
	unk29[1] = mAmbAry->getAmb(mAmbIndex + 1)->getColor();
}

GXColor TLightCommon::getLightColor(int index) const
{
	if (unk28) {
		if (index >= 4)
			index = 0;
		return unk31[index];
	}
	index += mLightIndex;
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
	index += mAmbIndex;
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
	index += mLightIndex;
	return &mLightAry->getLight(index)->mPosition;
}

// TODO: 99.1%, frame now exact (the ROM reuses one `Vec pos` for both
// MTXMultVec results -- slot 0x3c is written twice). What is left is a pure
// callee-saved rotation (ROM this=r29/gfx=r26/viewMtx=r28/manager=r27, ours
// r28/r29/r27/r26) plus the two `addi`s of the inlined setEffectLight's
// MTXMultVec in the opposite order (ROM computes the view matrix argument
// first, i.e. left to right). Rejected: a named `MtxPtr viewMtx` inside
// setEffectLight (+4 low, breaks the frame, and leaves its UNUSED size at
// 0xf4 vs the map's 0xf8); a `getShininess()` accessor (97.1%).
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

	MTXMultVec(gfx->getViewMtx(), getLightPosition(lightIndex), &pos);
	VECNormalize(&pos, &pos);
	GXInitSpecularDir(&light, -pos.x, -pos.y, -pos.z);
	GXInitLightColor(&light, getLightColor(lightIndex));
	GXInitLightShininess(&light, mShininess);
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

	MTXMultVec(gfx->getViewMtx(), getLightPosition(lightIndex), &pos);
	VECNormalize(&pos, &pos);
	GXInitSpecularDir(&light, -pos.x, -pos.y, -pos.z);
	GXInitLightColor(&light, getLightColor(lightIndex));
	GXInitLightShininess(&light, mShininess);
	GXLoadLightObjImm(&light, GX_LIGHT2);

	GXSetChanAmbColor(GX_COLOR0A0, getAmbColor(index));
}

GXColor TLightMario::getLightColor(int index) const
{
	GXColor color = TLightCommon::getLightColor(index + mLightIndex);
	color.a *= unk14;
	return color;
}

GXColor TLightMario::getAmbColor(int index) const
{
	index += mLightIndex;
	GXColor color = TLightCommon::getAmbColor(index);
	color.a *= unk14;
	return color;
}

#pragma dont_inline on
TLightDrawBuffer::TLightDrawBuffer(int param_1, u32 param_2, const char* name)
    : JDrama::TViewObj(name)
    , mLight(nullptr)
    , mOpaDrawBufferObject(nullptr)
    , mXluDrawBufferObject(nullptr)
    , unk80(param_1)
{
	snprintf(unk1C, 0x32, "%s%s", name, "opa");
	JDrama::TDrawBufObj* opa = new JDrama::TDrawBufObj(3, param_2, unk1C);
	mOpaDrawBufferObject     = opa;

	snprintf(unk4E, 0x32, "%s%s", name, "xlu");
	mXluDrawBufferObject = new JDrama::TDrawBufObj(4, param_2, unk4E);
}
#pragma dont_inline reset

void TLightDrawBuffer::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_LIGHT)
		mLight->setLight(graphics, unk80);
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

// UNUSED in the map at 0x28/0x28/0x20. All three are exactly four
// instructions longer than the bare indexed read, and that shared prefix is
// the `index > unk1C` clamp changeLightDrawBuffer spells out for itself.
// Nothing in the retail image reaches them (changeLightDrawBuffer duplicates
// their bodies), so they cannot be makeDrawBuffer's frame lever: the clamp
// would add four instructions per site there.
J3DDrawBuffer* TLightWithDBSet::getOpaDrawBuffer(int index)
{
	if (index > unk1C)
		index = 0;
	return unk10[index]->getOpaDbo()->getDrawBuffer();
}

J3DDrawBuffer* TLightWithDBSet::getXluDrawBuffer(int index)
{
	if (index > unk1C)
		index = 0;
	return unk10[index]->getXluDbo()->getDrawBuffer();
}

TLightDrawBuffer* TLightWithDBSet::getLightDrawBuffer(int index)
{
	if (index > unk1C)
		index = 0;
	return unk10[index];
}

int TLightWithDBSet::getLightIndex(const char* name)
{
	for (int i = 0; i < TLightCommon::mLightAry->getLightNum(); ++i) {
		JDrama::TLightAry* lightAry = TLightCommon::mLightAry;
		JDrama::TIdxLight* light    = lightAry->getLight(i);
		const char* lightName    = light->getName();
		if (strcmp(name, lightName) == 0)
			return i;
	}
	return -1;
}

int TLightWithDBSet::getAmbIndex(const char* name)
{
	for (int i = 0; i < TLightCommon::mAmbAry->getAmbNum(); ++i) {
		JDrama::TAmbAry* ambAry = TLightCommon::mAmbAry;
		JDrama::TAmbColor* amb  = ambAry->getAmb(i);
		const char* ambName    = amb->getName();
		if (strcmp(name, ambName) == 0)
			return i;
	}
	return -1;
}

// The 32 bytes of low region the four makeDrawBuffer bodies were short come
// from the two UNUSED carriers they expand: naming the array and the light in
// getLightIndex/getAmbIndex is +8 per expansion and a third chain step a
// further +8 (24 in all), and the `mLight` binder below closes the last 8.
// TODO: the binder is +8 in the two className bodies but +0x10 in
// TPlayer/TObject (both the naming and the plain form), so those two are
// still 8 over and keep the raw `unk10[i]->mLight` spelling, 8 short.
static inline TLightCommon* LightWithDBSetLight(TLightDrawBuffer* buffer)
{
	TLightCommon* light = buffer->mLight;
	return light;
}

void TPlayerLightWithDBSet::makeDrawBuffer()
{
	static const char lightName[] = "太陽（プレイヤー）";
	static const char ambName[]   = "太陽アンビエント（プレイヤー）";

	int lightIndex = getLightIndex(lightName);
	int ambIndex   = getAmbIndex(ambName);
	unk10 = new TLightDrawBuffer*[unk1C];
	for (int i = 0; i < unk1C; ++i) {
		unk10[i] = new TLightDrawBuffer(
		    i, 0x80, TLightCommon::mAmbAry->getAmb(ambIndex + i)->getName());
		TLightMario* light = new TLightMario();
		unk10[i]->setLight(light);
		unk10[i]->mLight->mAmbIndex   = ambIndex;
		unk10[i]->mLight->mLightIndex = lightIndex;
		unk10[i]->mLight->loadAfter();
	}
}

void TObjectLightWithDBSet::makeDrawBuffer()
{
	static const char lightName[] = "太陽（オブジェクト）";
	static const char ambName[]   = "太陽アンビエント（オブジェクト）";

	int lightIndex = getLightIndex(lightName);
	int ambIndex   = getAmbIndex(ambName);
	unk10 = new TLightDrawBuffer*[unk1C];
	for (int i = 0; i < unk1C; ++i) {
		unk10[i] = new TLightDrawBuffer(
		    i, 0x100, TLightCommon::mAmbAry->getAmb(ambIndex + i)->getName());
		TLightCommon* light = new TLightCommon();
		unk10[i]->setLight(light);
		unk10[i]->mLight->mAmbIndex   = ambIndex;
		unk10[i]->mLight->mLightIndex = lightIndex;
		unk10[i]->mLight->loadAfter();
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
		unk10[i]->mLight->mAmbIndex   = ambIndex;
		unk10[i]->mLight->mLightIndex = lightIndex;
		LightWithDBSetLight(unk10[i])->loadAfter();
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
		unk10[i]->mLight->mAmbIndex   = ambIndex;
		unk10[i]->mLight->mLightIndex = lightIndex;
		LightWithDBSetLight(unk10[i])->loadAfter();
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
	mMarioLight = nullptr;
	mLightSets  = nullptr;
	unk54       = false;
	unk55       = 1;

	mLightSets                       = new TLightWithDBSet*[LIGHT_TYPE_COUNT];
	mLightSets[LIGHT_TYPE_PLAYER]    = new TPlayerLightWithDBSet();
	mLightSets[LIGHT_TYPE_OBJECT]    = new TObjectLightWithDBSet();
	mLightSets[LIGHT_TYPE_MAPOBJECT] = new TMapObjectLightWithDBSet();
	mLightSets[LIGHT_TYPE_INDIRECT]  = new TIndirectLightWithDBSet();

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
	    = (JDrama::TLightAry*)JDrama::TNameRefGen::search2("Light Group");
	mEffectLightColor = group->getLight(0)->getColor();
	mEffectLightPos   = group->getLight(0)->mPosition;
}

void TLightWithDBSetManager::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_LIGHT) {
		int start;
		int end;
		if (cue & CUE_UNK80000) {
			start = LIGHT_TYPE_INDIRECT;
			end   = LIGHT_TYPE_COUNT;
		} else if (cue & CUE_UNK40000) {
			start = LIGHT_TYPE_MAPOBJECT;
			end   = LIGHT_TYPE_INDIRECT;
		} else {
			start = LIGHT_TYPE_PLAYER;
			end   = LIGHT_TYPE_MAPOBJECT;
		}
		for (int i = start; i < end; ++i)
			if (mLightSets[i]->isEnabled())
				mLightSets[i]->perform(cue, graphics);
	}

	if (cue & CUE_SET_DRAW_BUFFER) {
		for (int i = LIGHT_TYPE_FIRST; i < LIGHT_TYPE_COUNT; ++i)
			if (mLightSets[i]->isEnabled())
				mLightSets[i]->perform(cue, graphics);
	}
}

void TLightWithDBSetManager::addChildGroupObj(
    JDrama::TViewObjPtrListT<JDrama::TViewObj>* list)
{
	for (int i = LIGHT_TYPE_FIRST; i < LIGHT_TYPE_COUNT; ++i)
		mLightSets[i]->addChildGroupObj(list);
}

void TLightWithDBSetManager::makeDrawBuffer()
{
	for (int i = LIGHT_TYPE_FIRST; i < LIGHT_TYPE_COUNT; ++i)
		if (mLightSets[i]->isEnabled())
			mLightSets[i]->makeDrawBuffer();
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
		MTXMultVec(gfx->getViewMtx(), mEffectLightPos, &epos);
		GXInitLightPos(light, epos.x, epos.y, epos.z);
		GXInitLightColor(light, getEffectLightColor());
		GXInitLightAttnA(light, 1.0f, 0.0f, 0.0f);
		GXInitLightDistAttn(light, 1000.0f, 0.5f, GX_DA_STEEP);
		GXLoadLightObjImm(light, GX_LIGHT1);
	}
}

GXColor TLightWithDBSetManager::getEffectLightColor() const
{
	GXColor result = mEffectLightColor;
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
