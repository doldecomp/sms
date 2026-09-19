#include <Camera/SunModel.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JDrama/JDRViewObjPtrList.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DMaterialAnm.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DAnmLoader.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <Map/MapStaticObject.hpp>
#include <Camera/cameralib.hpp>
#include <Camera/CameraMarioData.hpp>
#include <Camera/Camera.hpp>
#include <System/Resolution.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <stdio.h>

static const char* dummyMactorStringValue1 = "\0\0\0\0\0\0\0\0\0\0\0";
static const char* SMS_NO_MEMORY_MESSAGE   = "メモリが足りません\n";

TSunModel* gpSunModel;

TSunModel::TSunModel(bool param_1, const char* name)
    : JDrama::TActor(name)
    , unk44(nullptr)
    , unk48(nullptr)
    , unk4C(nullptr)
    , unk64(nullptr)
    , unk68(-1)
    , unk6C(0.1f)
    , unk70(0.01f)
    , unk74(50)
    , unk78(0.1f)
    , unk7C(0.01f)
    , unk80(100)
    , unk84(3.0f)
    , unk88(2.0f)
    , unk9C(0.0f)
    , unkA0(0.0f)
    , unkA4(0.0f)
    , unkA8(0.0f)
    , unkAC(0.0f)
    , unkB0(0.0f)
    , unk191(0)
    , unk194(0.0f)
    , unk1A4(0.014f)
    , unk1A8(3.0f)
    , unk1AC(0)
{
	gpSunModel = this;
	if (param_1) {
		unk1AC |= 0x4;
		unk80 = 48;   // retail `li r0, 0x30`, not 0
	}

	JGeometry::TVec2<s16>* it1 = unkB4;
	JGeometry::TVec2<f32>* it2 = unkF8;
	bool* it3                  = unk180;
	// The three pointer increments belong in the for-increment clause, not in
	// the body: with them in the body MWCC unrolls this loop eight times
	// (`li r0, 2; mtctr` plus an extra offset induction variable, 51 extra
	// instructions), while retail keeps the single 9-instruction body and
	// `li r0, 0x11`.  Every other 17-iteration loop in this file carries a
	// call or a branch and so was never unrollable, which is why this is the
	// only place it shows.
	for (int i = 0; i < 17; ++i, ++it1, ++it2, ++it3) {
		it1->x = it1->y = -1;
		it2->x = it2->y = 10000.0f;
		*it3            = false;
	}
}

// Binding level worth +8 of low region, landing TSunModel::load's frame at
// 0x1f8 (batch 121).
static inline s16 SunmodelGetFrameMax(const J3DAnmTextureSRTKey* p)
{
	s16 frameMax = p->getFrameMax();
	return frameMax;
}

void TSunModel::load(JSUMemoryInputStream& param_1)
{
	JDrama::TActor::load(param_1);

	mScaling *= 0.4f;

	const char* volumeName = cSunVolumeName;
	u32 modelFlags         = 0x10020000;
	if (unk1AC & 4) {
		modelFlags |= 0x1000000;
		volumeName = cSunsetVolumeName;
	}

	char path[0x100];
	snprintf(path, sizeof(path), "%s/%s", volumeName, "model.bmd");
	unk44 = J3DModelLoaderDataBase::load(JKRGetResource(path), modelFlags);
	unk48 = new J3DModel(unk44, 0, 1);

	snprintf(path, sizeof(path), "%s/%s", volumeName, "model.btk");
	unk4C = (J3DAnmTextureSRTKey*)J3DAnmLoaderDataBase::load(
	    JKRGetResource(path));
	unk4C->searchUpdateMaterialID(unk44);

	u16 i;
	int e = unk44->getMaterialNum();
	for (i = 0; i < e; ++i) {
		J3DMaterialAnm* materialAnm = new J3DMaterialAnm;

		unk44->getMaterialNodePointer(i)->change();
		unk44->getMaterialNodePointer(i)->setMaterialAnm(materialAnm);
	}

	unk44->entryTexMtxAnimator(unk4C);
	unk8C = *unk44->getMaterialNodePointer(0)->getTevColor(0);
	unk94 = *unk44->getMaterialNodePointer(1)->getTevColor(0);

	unk9C = unkA4 = (f32)unk68;
	unkA0 = unkA8 = (f32)unk74;
	unk50.init(SunmodelGetFrameMax(unk4C));
	unk50.setRate(SMSGetAnmFrameRate());
	unk50.setAttribute(J3DFrameCtrl::ATTR_LOOP);

	unk198 = mPosition;
	unk64  = new TMapStaticObj("太陽in鏡");
	unk64->init("sun_mirror");
	unk64->mPosition = mPosition;
	unk64->mRotation = mRotation;
	unk64->mScaling  = mScaling;

	JDrama::TViewObjPtrListT<JDrama::TViewObj>* mirrorScene
	    = JDrama::TNameRefGen::search<
	        JDrama::TViewObjPtrListT<JDrama::TViewObj> >("鏡シーン");
	mirrorScene->getChildren().push_back(unk64);
}

void TSunModel::calcOtherFPosFromCenterAndRadius_(
    JGeometry::TVec2<f32>* param_1, const JGeometry::TVec2<f32>& param_2,
    f32 param_3)
{
	f32 fVar1    = param_3 * 0.707f;
	param_1[0].x = param_2.x;
	param_1[0].y = param_2.y + param_3;
	param_1[1].x = param_2.x - fVar1;
	param_1[1].y = param_2.y + fVar1;
	param_1[2].x = param_2.x - param_3;
	param_1[2].y = param_2.y;
	param_1[3].x = param_2.x - fVar1;
	param_1[3].y = param_2.y - fVar1;
	param_1[4].x = param_2.x;
	param_1[4].y = param_2.y - param_3;
	param_1[5].x = param_2.x + fVar1;
	param_1[5].y = param_2.y - fVar1;
	param_1[6].x = param_2.x + param_3;
	param_1[6].y = param_2.y;
	param_1[7].x = param_2.x + fVar1;
	param_1[7].y = param_2.y + fVar1;
}

// The screen-position loop is an inline *level*, not a convenience: the map
// carries CLBScreenFPosToSPos as a (func,weak) body of its own in this object
// (0x114 at 0x29a50), i.e. retail `bl`s it from calcDispRatioAndScreenPos_
// instead of expanding it.  A namespace-scope `inline` is unlimited at depth 1
// and the loop sits directly in calcDispRatioAndScreenPos_'s body, so one
// level has to separate them; with it the callee is at depth 2, its ~10
// statements exceed the 9-statement allowance, and the `bl` appears.  That
// lands calcDispRatioAndScreenPos_ byte-exact, which is the evidence that the
// level is real -- only its name and receiver form are guesses, so it is
// parked TU-local rather than added to the shared header.
// The three declarations are in retail's order, not ours: callee-saved GPRs go
// out r31 downward in reverse declaration order, and retail's r31/r30/r29 hold
// i / it2 / it1, so `it1` must be declared first.
static inline void SunModelCalcScreenPos(TSunModel* p)
{
	JGeometry::TVec2<s16>* it1;
	JGeometry::TVec2<f32>* it2;
	int i;

	it1 = p->unkB4;
	it2 = p->unkF8;
	for (i = 0; i < 17; ++i) {
		CLBScreenFPosToSPos(it1, *it2);
		++it1;
		++it2;
	}
}

// `inline` is load-bearing: the map has this as (func,weak), which for a
// member defined in the .cpp means the `inline` keyword, and retail `bl`s it
// from perform (the call is in perform's relocation list).  See moveSun_ for
// why that call survives.
inline void TSunModel::calcDispRatioAndScreenPos_()
{
	unk191   = 0;
	bool* it = unk180;
	for (int i = 0; i < 17; ++i, ++it)
		if (*it)
			unk191 += 1;

	unk194 = (f32)unk191 * (1.0f / 17.0f);

	CLBCalc2DFPos(unkF8, gpCamera->getUnk16C(), gpCamera->getUnk1EC(), unk198,
	              nullptr, false);

	f32 radius = unk1A4 * mScaling.y;
	calcOtherFPosFromCenterAndRadius_(&unkF8[1], unkF8[0], radius);
	radius /= 2.0f;
	calcOtherFPosFromCenterAndRadius_(&unkF8[9], unkF8[0], radius);

	SunModelCalcScreenPos(this);
}

// Fabricated: retail's name for the CUE_MOVE body is unknown and it leaves no
// symbol, because an inline that is expanded at every call site is emitted
// nowhere.  What is known is that *some* level wraps this block: retail `bl`s
// the weak calcDispRatioAndScreenPos_, and a weak body is unlimited at depth 1,
// so the call cannot be a statement of perform itself.  One level puts it at
// depth 2, where its ~22 statements are far over the 9-statement allowance,
// and the `bl` appears; without this wrapper the body is expanded into perform
// instead (perform 95.1% -> 70.8%, no out-of-line copy, and CLBScreenFPosToSPos
// then takes perform's own depth-2 slot).  Splitting the block at exactly the
// `if (cue & CUE_MOVE)` boundary is the smallest shape that does it.
inline void TSunModel::moveSun_()
{
	unkA4 = CLBLinearInbetween<f32>((f32)unk68, 255.0f, unk194);
	unkA8 = CLBEaseOutInbetween<f32>((f32)unk74, 255.0f, unk194);

	f32 chase1;
	if (unk9C < unkA4)
		chase1 = unk6C;
	else
		chase1 = unk70;
	CLBChaseDecrease(&unk9C, unkA4, chase1, 0.0f);
	unk8C.color.a = (s16)unk9C;

	f32 chase2;
	if (unkA0 < unkA8)
		chase2 = unk78;
	else
		chase2 = unk7C;
	CLBChaseDecrease(&unkA0, unkA8, chase2, 0.0f);
	unk94.color.a = (s16)unkA0;

	if (gpCameraMario->isMarioIndoor()) {
		unkB0 = 0.0f;
	} else {
		f32 distSq = unkF8[0].squared();
		if (distSq > 2.0f) {
			unkB0 = 0.0f;
		} else {
			f32 nearness = 2.0f - distSq;
			unkB0        = CLBLinearInbetween<f32>(
			           0.0f, (f32)unk80, 0.5f * nearness * unk194);
		}
	}

	f32 chase3;
	if (unkAC < unkB0)
		chase3 = unk84;
	else
		chase3 = unk88;
	CLBChaseGeneralConstantSpecifySpeed<f32>(&unkAC, unkB0, chase3);

	// Retail loads all six operands before storing any component, which
	// `dir.sub(mPosition, camPos)` cannot do (it stores each component as
	// soon as it is computed): the three differences are arguments of
	// `set`, so they are all evaluated before the body runs.
	CPolarSubCamera* camera = SMSGetCamera();
	JGeometry::TVec3<f32> dir;
	dir.set(mPosition.x - camera->getUnk124().x,
	    mPosition.y - camera->getUnk124().y,
	    mPosition.z - camera->getUnk124().z);
	MsVECNormalize(&dir, &dir);

	// Header round 30 closed this: the out-of-line `set(const Vec&)` is the
	// *implicit* conversion at scaleAdd's `const TVec3<f32>&` parameter.
	// `getUnk124Vec()` is typed `const Vec&`, so the argument takes the
	// converting constructor `TVec3(const Vec&)`, which puts `set` at inline
	// depth 4 (moveSun_ 1, scaleAdd 2, the constructor 3) - the depth at
	// which a 3-statement in-class member stops expanding - and the
	// constructor's stack temporary is the copy retail reads the three
	// fmadds addends out of.  Everything from the `lfsu` through the last
	// `stfs` is instruction-exact; only the displacements differ.
	// The overload is the whole point: a `const TVec3<f32>&` binds directly
	// (no temporary, no call) and `.set()` on a named local picks the
	// `set<TY>` member template, which is why header round 24's forwarder
	// chains never reached the `bl` (see the trial list in JGVec3.hpp).
	// TODO: the frame is exact now and only two differences are left: the
	// `Mtx mtx` slot (retail 0x78, ours 0x70) and the `unkB0` argument
	// scheduling in cluster 1.  Research batch 207 explains the slot order:
	// an inlined callee's class-object locals form their own block, and the
	// blocks stack downward in *expansion* order below the caller's own
	// named locals, so a `Mtx` declared in perform can never sit below this
	// expansion's `dir`/temp pair - it has to be a local of a *second*
	// inlined callee, which is what calcAnim_ is.  What is left is a
	// constant 4 bytes at the top of calcAnim_'s block (ours is
	// dir - mtx = 0x34, retail 0x30): measured invariant under a 4- or
	// 8-byte local declared before or after `mtx`, hoisting `calc()` out,
	// a `J3DModel*` binder, and `MTXCopy(mtx, getBaseTRMtx())` in place of
	// `setBaseTRMtx`, and a scratch TU with the same two-callee shape shows
	// no such pad, so it comes from something inside this body.  The named
	// `camera` is worth -8 of low region and is what makes the frame exact.
	// Measured on the four camera reads with no binder: `gpCamera->` in
	// place of `SMSGetCamera()` is -4 at each of the three subtraction
	// operands and +0 at the scaleAdd argument, and any single one of the
	// three reaches the same 7 markers with `mtx` at 0x74.  With the binder
	// the scaleAdd argument has to stay a fresh read of the global (retail
	// reloads `gpCamera` there after MsVECNormalize, so it was never a
	// named local in retail): `camera->getUnk124Vec()` costs 3 extra
	// instructions and `SMSGetCamera()->` gives the 8 bytes back.
	unk198.scaleAdd(250000.0f, dir, gpCamera->getUnk124Vec());

	if (unk64)
		unk64->mPosition = unk198;

	calcDispRatioAndScreenPos_();
}

// Fabricated, like moveSun_: retail leaves no symbol for it, but `mtx` sits
// *below* moveSun_'s expansion in retail's frame (0x78 against dir's 0xa8),
// and an inlined callee's class-object locals always rank above the caller's
// own named locals (research batch 207), so the Mtx block cannot be a local of
// perform itself -- it has to be the local of a second inlined callee.
inline void TSunModel::calcAnim_()
{
	Mtx mtx;
	MsMtxSetTRS(mtx, unk198.x, unk198.y, unk198.z, mRotation.x, mRotation.y,
	            mRotation.z, mScaling.x, mScaling.y, mScaling.z);
	unk48->setBaseTRMtx(mtx);
	unk48->calc();
}

void TSunModel::perform(u32 cue, JDrama::TGraphics*)
{
	bool sunInBounds;
	if (gpCameraMario->isMarioIndoor()) {
		sunInBounds = false;
	} else {
		f32 bounds = unk1A8;
		sunInBounds = -bounds <= unkF8[0].x && unkF8[0].x <= bounds
		              && -bounds <= unkF8[0].y && unkF8[0].y <= bounds
		                  ? true
		                  : false;
	}

	if (cue & CUE_MOVE) {
		moveSun_();
	}

	if (cue & CUE_CALC_ANIM) {
		unk50.update();
		if (sunInBounds)
			calcAnim_();
	}

	if (cue & CUE_ENTRY && sunInBounds) {
		unk4C->setFrame(unk50.getFrame());
		unk44->mMaterials[0]->getTevBlock()->setTevColor(0, &unk8C);
		unk44->mMaterials[1]->getTevBlock()->setTevColor(0, &unk94);
		unk48->entry();
	}

	if (cue & CUE_CALC_VIEW && sunInBounds)
		unk48->viewCalc();
}

void TSunModel::getZBufValue()
{
	bool cVar1 = gpCameraMario->isMarioIndoor();

	JGeometry::TVec2<s16>* it = unkB4;
	bool* it2                 = unk180;
	for (int i = 0; i < 17; ++i, ++it, ++it2) {
		*it2 = 0;
		if (!cVar1 && it->x != -1 && it->y != -1) {
			u32 depth;
			GXPeekZ(it->x, it->y, &depth);
			if (depth == 0xffffff)
				*it2 = 1;
		}
	}
}
