#include <GC2D/SelectShine2.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <System/Application.hpp>
#include <JSystem/JMath.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DMaterialAnm.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DDrawBuffer.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DAnmLoader.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/JParticle/JPAEmitterManager.hpp>

// rogue include needed for matching the .rodata string pool prefix
#include <System/DummyStrings.hpp>

JGeometry::TVec3<f32> TSelectShineManager::cCenter(300.0f, 160.0f, -9000.0f);

// The member initialiser list is what puts the mPositions array construction
// after every scalar store; assigning in the body emits __construct_array
// first.
TSelectShineManager::TSelectShineManager(const char* name)
    : JDrama::TViewObj(name)
    , mOpaBuffer(nullptr)
    , mXluBuffer(nullptr)
    , unk78(0.0f)
    , unk7C(0.0f)
    , mShineNum(0)
    , unk90(0.0f)
    , unk94(0.0f)
    , unk98(0)
    , mScroll(0)
    , mScrollSpeed(0.0f)
    , mIncreasing(false)
    , mDecreasing(false)
    , unkA6(0)
    , mClosed(false)
{
}

void TSelectShineManager::initData(u8* shine_states, u8 shine_num, u8 index,
                                   JPAEmitterManager* emitter_manager)
{
	mOpaBuffer = new J3DDrawBuffer(0x400);
	j3dSys.setDrawBuffer(mOpaBuffer, 0);
	mXluBuffer = new J3DDrawBuffer(0x400);
	j3dSys.setDrawBuffer(mXluBuffer, 1);

	void* modelRes = JKRGetResource("/select/shine_menu.bmd");
	void* anmRes   = JKRGetResource("/select/shine_menu.bpk");

	J3DModelData* modelData = J3DModelLoaderDataBase::load(
	    modelRes, J3DMLF_MaterialColorLightOn | J3DMLF_MaterialPEFull
	                  | J3DMLF_MaterialUseIndirect
	                  | (4 << J3DMLF_TevStageNumShift));
	J3DAnmColor* anmColor = (J3DAnmColor*)J3DAnmLoaderDataBase::load(anmRes);
	anmColor->searchUpdateMaterialID(modelData);
	for (u16 i = 0; i < modelData->getMaterialNum(); ++i) {
		J3DMaterialAnm* matAnm = new J3DMaterialAnm;
		modelData->getMaterialNodePointer(i)->change();
		modelData->getMaterialNodePointer(i)->setMaterialAnm(matAnm);
	}
	modelData->entryMatColorAnimator(anmColor);

	void* emptyModelRes = JKRGetResource("/select/shine_menu_empty.bmd");
	void* emptyAnmRes   = JKRGetResource("/select/shine_menu_empty.bpk");

	J3DModelData* emptyModelData = J3DModelLoaderDataBase::load(
	    emptyModelRes, J3DMLF_MaterialColorLightOn | J3DMLF_MaterialPEFull
	                       | J3DMLF_MaterialUseIndirect
	                       | (4 << J3DMLF_TevStageNumShift));
	J3DAnmColor* emptyAnmColor
	    = (J3DAnmColor*)J3DAnmLoaderDataBase::load(emptyAnmRes);
	emptyAnmColor->searchUpdateMaterialID(emptyModelData);
	// NOTE: the bound really is the first model's material count in retail,
	// not emptyModelData's. Both models have the same materials, so the bug
	// never fires.
	for (u16 i = 0; i < modelData->getMaterialNum(); ++i) {
		J3DMaterialAnm* matAnm = new J3DMaterialAnm;
		emptyModelData->getMaterialNodePointer(i)->change();
		emptyModelData->getMaterialNodePointer(i)->setMaterialAnm(matAnm);
	}
	emptyModelData->entryMatColorAnimator(emptyAnmColor);

	mShineNum = shine_num;
	mIndex    = index;
	mScroll   = mIndex * -40;

	for (int i = 0; i < 8; ++i) {
		JGeometry::TVec3<f32> pos = getPosition(mScroll + i * 40);
		s16 angle                 = getAngle(pos);
		if (pos.x > cCenter.x)
			angle *= -1;

		if (shine_states[i] == 3) {
			mShines[i] = new TSelectShine(
			    modelData, anmColor, emitter_manager, pos, angle, 0,
			    (f32)(int)(4000.0f * MsRandF()) / 1000.0f, 0.01f, 10.0f);
		} else if (shine_states[i] == 1 || shine_states[i] == 2) {
			mShines[i] = new TSelectShine(
			    emptyModelData, emptyAnmColor, emitter_manager, pos, angle, 1,
			    (f32)(int)(4000.0f * MsRandF()) / 1000.0f, 0.01f, 10.0f);
		} else {
			mShines[i] = nullptr;
		}
	}

	mShines[mIndex]->playEmitters();
}

// TODO: open, but no longer a JGVec2.hpp item. Retail copies the TVec2 with
// lfs/stfs here and ours copies with lwz/stw, leaving the UNUSED body 0x20 over
// the map's 0x88 -- yet TVec2 cannot have the operator= that would fix it:
// std::__copy<TCameraMapTool> (System/MarNameRefGen.cpp) moves the TVec2 that
// class holds at 0x18 with a paired lwz/lwz/stw/stw, the aggregate copy MWCC
// only emits when there is no user-declared operator=, and declaring one takes
// that exact function to 88.3%. Enabling it here is worth +0.7 on initData and
// +0.7 on perform, so the float moves are real; they have to come from a
// different spelling of this function -- most likely a component-wise
// sub() against a TVec2 temporary instead of `toCenter = toCenter - ...`.
// The trial table is in JGVec2.hpp.
// The (0, 1) axis is a named local declared *after* the subtraction, not two
// call temporaries: retail hoists 0.0f into the higher callee-saved FPR and
// 1.0f into the lower one in both loops that inline this function (initData
// f28/f27, perform f31/f30), which is left-to-right initialisation of one
// object; two `TVec2<f32>(0.0f, 1.0f)` arguments evaluate right to left and
// load 1.0f first. Declared before `toCenter` the pair outranks 1500/300/1300
// instead (initData -1.4).
// TODO: the name costs 0x18 of frame that retail spends (initData 0x188
// against 0x1b0, and retail's `bl TVec2<f>::sub` -- the unit's one MISSING map
// symbol -- is still inlined here), so initData is 97.3 -> 97.1 while perform
// is 82.7 -> 83.7. The spelling that keeps the order without dropping the
// temporaries is unfound.
s16 TSelectShineManager::getAngle(const JGeometry::TVec3<f32>& position)
{
	JGeometry::TVec2<f32> toCenter(300.0f, 1300.0f);
	toCenter.set(toCenter - JGeometry::TVec2<f32>(position.x, position.z));
	JGeometry::TVec2<f32> up(0.0f, 1.0f);
	return (s16)(57.295776f
	             * fabsf(atan2f(toCenter.cross(up), toCenter.dot(up))));
}

JGeometry::TVec3<f32> TSelectShineManager::getPosition(s16 angle)
{
	s16 shortAngle = (s16)(57.295776f * (f32)angle);
	return JGeometry::TVec3<f32>(cCenter.x + 1500.0f * JMASSin(shortAngle),
	                             cCenter.y,
	                             cCenter.z + 9000.0f * JMASCos(shortAngle));
}

void TSelectShineManager::startClose()
{
	mShines[mIndex]->mSpinning = false;

	for (int i = 0; i < 8; ++i) {
		TSelectShine* shine = mShines[i];
		if (shine != nullptr && i != mIndex && !shine->mDisappearing) {
			shine->mDisappearing = true;
			shine->mAppearing    = false;
		}
	}

	mClosed = true;
}

void TSelectShineManager::startIncrease(int count)
{
	mShines[mIndex]->stopEmitters();

	mIncreasing  = true;
	mScrollSpeed = count * -40 / 10;
	mIndex += count;

	mShines[mIndex]->playEmitters();
}

void TSelectShineManager::startDecrease(int count)
{
	mShines[mIndex]->stopEmitters();

	mDecreasing  = true;
	mScrollSpeed = count * 40 / 10;
	mIndex -= count;

	mShines[mIndex]->playEmitters();
}

// The per-shine update is its own inline level: that puts getPosition's
// TVec3 constructor at depth 3 (so TVec3::set<f> is retail's `bl`) and
// JMASSin/JMASCos and getAngle's TVec2::sub at the depth where retail calls
// them out of line, which initData (one level shallower) inlines.
// TVec3::add is not part of that puzzle: naming the sum gives it the
// `a = b + c` depth (copy ctor 1, operator+ 2, operator+= 3, add 4) that
// retail's `bl` measures, and the two copies retail makes out of operator+
// say the sum was a local even though getAngle takes it by reference (the
// map's getAngle__19TSelectShineManagerFRCQ29JGeometry8TVec3<f> settles
// that).
// TODO: 94.0, frame 0x1b8 against 0x220. getAngle's `toCenter.set(...)`
// gives retail's lfs/stfs copy, but retail copies the difference twice and
// never stores `up` (cross/dot read its constants from f30/f31), and it
// copies getPosition's result through a low temporary (0x1c) into both `pos`
// and mPosition. Inert: a named difference, a functional-cast copy, `-=`,
// chained `mPosition = pos = ...` either way round, mPosition.set(pos).
static inline void TSelectShineUpdate(TSelectShineManager* m, int i)
{
	JGeometry::TVec3<f32> pos = m->getPosition(m->mScroll + i * 40);
	m->mShines[i]->mPosition  = pos;

	JGeometry::TVec3<f32> anglePos
	    = m->mShines[i]->mPosition + m->mShines[i]->mOffset;
	s16 angle = m->getAngle(anglePos);
	if (pos.x > TSelectShineManager::cCenter.x)
		angle *= -1;

	m->mShines[i]->setAngle(angle);
}

void TSelectShineManager::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_MOVE) {
		for (int i = 0; i < mShineNum; ++i)
			mShines[i]->move();

		if (mIncreasing || mDecreasing) {
			mScroll = (int)((f32)mScroll + mScrollSpeed);

			if (mIncreasing && mScroll < mIndex * -40) {
				mScroll     = mIndex * -40;
				mIncreasing = false;
			}
			if (mDecreasing && mScroll > mIndex * -40) {
				mScroll     = mIndex * -40;
				mDecreasing = false;
			}

			for (int i = 0; i < mShineNum; ++i)
				TSelectShineUpdate(this, i);
		}
	}

	if (cue & CUE_CALC_ANIM) {
		for (int i = 0; i < mShineNum; ++i) {
			mShines[i]->mAnmColor->setFrame((f32)mShines[i]->mAnmFrame);
			mShines[i]->getModel()->update();
			mShines[i]->getModel()->viewCalc();
		}
	}

	if (cue & CUE_DRAW) {
		MTXCopy(graphics->getViewMtx(), j3dSys.getViewMtx());
		j3dSys.drawInit();
		j3dSys.setUnk4C(3);
		mOpaBuffer->draw();
		j3dSys.setUnk4C(4);
		mXluBuffer->draw();
		mOpaBuffer->frameInit();
		mXluBuffer->frameInit();
	}
}

TSelectShine::TSelectShine(J3DModelData* model_data, J3DAnmColor* anm_color,
                           JPAEmitterManager* emitter_manager,
                           JGeometry::TVec3<f32>& position, s16 angle, u8 type,
                           f32 bound_phase, f32 bound_speed, f32 bound_height)
{
	mSpinning       = false;
	mBoundTimer     = 0.0f;
	mBoundHeight    = 0.0f;
	mBoundSpeed     = 0.0f;
	mSpinAngle      = 0;
	mSpinSpeed      = 0;
	mAngle          = angle;
	mAnmFrame       = 0;
	unk3E           = 0;
	unk40           = 0.0f;
	unk44           = 0.0f;
	mAppearing      = false;
	mDisappearing   = false;
	mType           = 0;
	mEmitterManager = nullptr;
	mEmitter0       = nullptr;
	mEmitter1       = nullptr;
	mEmitter2       = nullptr;

	mModel     = new J3DModel(model_data, 0, 1);
	mAnmColor  = anm_color;
	mPosition.x = position.x;
	mPosition.y = position.y;
	mPosition.z = position.z;
	mOffset.x   = 0.0f;
	mOffset.y   = 0.0f;
	mOffset.z   = 0.0f;

	mEmitterManager = emitter_manager;
	mBoundSpeed     = bound_speed;
	mBoundHeight    = bound_height;
	mBoundTimer     = bound_phase;
	mSpinSpeed      = 3;

	// TODO: instruction-identical from here on; the frame is 0x108 against
	// our 0xd8, i.e. one 48-byte (Mtx-sized) inline-expansion temporary in
	// retail that no spelling of this body has reproduced.
	// Retail's pool is 0x34 higher between the operator+ temporary (0x4c)
	// and emitPos (0x8c). Tried (cc41): the rotation in a TU-local helper
	// with its own Mtx (moves the matrix into the pool), `facing` declared at
	// the top, set()/zero()/chained-zero spellings of mPosition/mOffset, and
	// emitPos declared then assigned.
	MtxPtr mtx = mModel->getBaseTRMtx();
	mtx[0][3]  = mPosition.x;
	mtx[1][3]  = mPosition.y;
	mtx[2][3]  = mPosition.z;

	Mtx facing;
	MTXRotRad(facing, 'y', 0.017453292f * (f32)mAngle);
	MTXConcat(mtx, facing, mtx);

	mType = type;
	if (mType != 2) {
		JGeometry::TVec3<f32> emitPos = mPosition + mOffset;
		if (mType == 0) {
			mEmitterManager->createEmitter(emitPos, 0, nullptr, nullptr);
			mEmitter0 = mEmitterManager->unkC8[0][0];
			mEmitterManager->createEmitter(emitPos, 1, nullptr, nullptr);
			mEmitter1 = mEmitterManager->unkC8[0][0];
			mEmitterManager->createEmitter(emitPos, 2, nullptr, nullptr);
		} else {
			mEmitterManager->createEmitter(emitPos, 3, nullptr, nullptr);
		}
		mEmitter2 = mEmitterManager->unkC8[0][0];

		stopEmitters();
	}
}

// TODO: the inlined middle product multiplies 0.9f first (retail: the
// height first, four sites) and the frame is 0x140, retail 0x158.
void TSelectShine::move()
{
	f32 newY;
	if (mBoundTimer < 1.0f) {
		newY = makeNewPosition(mBoundTimer, 0.0f, mBoundHeight * 0.9f,
		                       mBoundHeight);
	} else if (mBoundTimer < 2.0f) {
		newY = makeNewPosition(mBoundTimer - 1.0f, mBoundHeight,
		                       mBoundHeight * 0.9f, 0.0f);
	} else if (mBoundTimer < 3.0f) {
		newY = makeNewPosition(mBoundTimer - 2.0f, 0.0f, -mBoundHeight * 0.9f,
		                       -mBoundHeight);
	} else if (mBoundTimer < 4.0f) {
		newY = makeNewPosition(mBoundTimer - 3.0f, -mBoundHeight,
		                       -mBoundHeight * 0.9f, 0.0f);
	}
	mOffset.y = newY;

	MtxPtr mtx                = mModel->getBaseTRMtx();
	JGeometry::TVec3<f32> pos = mPosition + mOffset;
	mtx[0][3]                 = pos.x;
	mtx[1][3]                 = pos.y;
	mtx[2][3]                 = pos.z;

	s16 frame = (s16)(-0.05f * pos.z);
	// TODO: retail keeps pos.z in the register it stored into the matrix; both
	// `pos.z` and a readback of mtx[2][3] re-load it here (one extra lfs).
	if (frame < 30)
		frame = 30;

	if (mAppearing) {
		mAnmFrame -= 8;
		if (mAnmFrame < frame)
			mAnmFrame = frame;
	} else if (mDisappearing) {
		mAnmFrame += 8;
		if (mAnmFrame > mAnmColor->getFrameMax() - 1)
			mAnmFrame = mAnmColor->getFrameMax() - 1;
	} else {
		mAnmFrame = frame;
		if (frame < 0)
			mAnmFrame = 0;
		if (frame > mAnmColor->getFrameMax())
			mAnmFrame = mAnmColor->getFrameMax();
	}

	u8 alpha = 255.0f
	           * (1.0f - (f32)(mAnmFrame / mAnmColor->getFrameMax()));
	if (mType != 2) {
		if (mType == 0) {
			mEmitter0->setGlobalRTMatrix(mtx);
			mEmitter1->setGlobalRTMatrix(mtx);
			mEmitter0->setGlobalAlpha(alpha);
			mEmitter1->setGlobalAlpha(alpha);
		}
		mEmitter2->setGlobalRTMatrix(mtx);
		mEmitter2->setGlobalAlpha(alpha);
	}

	if (mSpinning) {
		Mtx spin;
		MTXRotRad(spin, 'y',
		          0.017453292f * ((f32)mSpinSpeed * SMSGetAnmFrameRate()));
		mSpinAngle = (int)((f32)mSpinSpeed * SMSGetAnmFrameRate()
		                   + (f32)mSpinAngle);
		if (mSpinAngle > 360)
			mSpinAngle -= 360;
		if (mSpinAngle < 0)
			mSpinAngle += 360;
		MTXConcat(mtx, spin, mtx);
	} else if (mSpinAngle != 0) {
		s16 step   = (s16)((f32)mSpinSpeed * SMSGetAnmFrameRate());
		mSpinAngle = mSpinAngle + step;
		if (mSpinAngle > 360) {
			step       = step - mSpinAngle + 360;
			mSpinAngle = 0;
		}
		Mtx spin;
		MTXRotRad(spin, 'y', 0.017453292f * (f32)step);
		MTXConcat(mtx, spin, mtx);
	}

	mBoundTimer = mBoundTimer + mBoundSpeed;
	if (mBoundTimer > 4.0f)
		mBoundTimer = 0.0f;
}

f32 TSelectShine::makeNewPosition(f32 t, f32 start, f32 middle, f32 end)
{
	f32 s = 1.0f - t;
	return start * (s * s) + middle * (2.0f * s * t) + end * (t * t);
}
