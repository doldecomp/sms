#include <GC2D/SelectShine2.hpp>
#include <math.h>
#include <stdlib.h>
#include <dolphin/mtx.h>
#include <JSystem/JMath.hpp>
#include <JSystem/JGeometry/JGVec2.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DMaterialAnm.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DDrawBuffer.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DAnmLoader.hpp>
#include <JSystem/JDrama/JDRGraphics.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/JParticle/JPAEmitterManager.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <System/Application.hpp>
#include <System/DummyStrings.hpp>

JGeometry::TVec3<f32> TSelectShineManager::cCenter(300.0f, 160.0f, -9000.0f);

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
    , mRingAngle(0)
    , mRingSpeed(0.0f)
    , mIsIncreasing(0)
    , mIsDecreasing(0)
    , unkA6(0)
    , mIsClosing(0)
{
}

// TODO: Inlined vector copies, frame size, and register allocation differ.
void TSelectShineManager::initData(u8* states, u8 shine_num, u8 selected,
                                   JPAEmitterManager* emitter_manager)
{
	mOpaBuffer = new J3DDrawBuffer(0x400);
	j3dSys.setDrawBuffer(mOpaBuffer, 0);
	mXluBuffer = new J3DDrawBuffer(0x400);
	j3dSys.setDrawBuffer(mXluBuffer, 1);

	void* bmd = JKRFileLoader::getGlbResource("/select/shine_menu.bmd");
	void* bpk = JKRFileLoader::getGlbResource("/select/shine_menu.bpk");
	J3DModelData* modelData = J3DModelLoaderDataBase::load(
	    bmd, J3DMLF_MaterialColorLightOn | J3DMLF_MaterialPEFull
	             | J3DMLF_MaterialUseIndirect | (4 << J3DMLF_TevStageNumShift));
	J3DAnmColor* anmColor = (J3DAnmColor*)J3DAnmLoaderDataBase::load(bpk);
	anmColor->searchUpdateMaterialID(modelData);
	for (u16 i = 0; i < modelData->getMaterialNum(); ++i) {
		J3DMaterialAnm* anm = new J3DMaterialAnm;
		modelData->getMaterialNodePointer(i)->change();
		modelData->getMaterialNodePointer(i)->setMaterialAnm(anm);
	}
	modelData->entryMatColorAnimator(anmColor);

	void* emptyBmd
	    = JKRFileLoader::getGlbResource("/select/shine_menu_empty.bmd");
	void* emptyBpk
	    = JKRFileLoader::getGlbResource("/select/shine_menu_empty.bpk");
	J3DModelData* emptyModelData = J3DModelLoaderDataBase::load(
	    emptyBmd, J3DMLF_MaterialColorLightOn | J3DMLF_MaterialPEFull
	                  | J3DMLF_MaterialUseIndirect
	                  | (4 << J3DMLF_TevStageNumShift));
	J3DAnmColor* emptyAnmColor
	    = (J3DAnmColor*)J3DAnmLoaderDataBase::load(emptyBpk);
	emptyAnmColor->searchUpdateMaterialID(emptyModelData);
	for (u16 i = 0; i < modelData->getMaterialNum(); ++i) {
		J3DMaterialAnm* anm = new J3DMaterialAnm;
		emptyModelData->getMaterialNodePointer(i)->change();
		emptyModelData->getMaterialNodePointer(i)->setMaterialAnm(anm);
	}
	emptyModelData->entryMatColorAnimator(emptyAnmColor);

	mShineNum  = shine_num;
	mSelected  = selected;
	mRingAngle = mSelected * -40;
	u8* state  = states;
	s16 angle;
	for (int i = 0; i < 8; ++i) {
		JGeometry::TVec3<f32> pos = getPosition(mRingAngle + i * 40);
		angle                     = getAngle(pos);
		if (*state == 3) {
			mShines[i] = new TSelectShine(
			    modelData, anmColor, emitter_manager, pos, angle, 0,
			    (s32)(4000.0f * MsRandF()) / 1000.0f, 0.01f, 10.0f);
		} else if (*state == 1 || *state == 2) {
			mShines[i] = new TSelectShine(
			    emptyModelData, emptyAnmColor, emitter_manager, pos, angle, 1,
			    (s32)(4000.0f * MsRandF()) / 1000.0f, 0.01f, 10.0f);
		} else {
			mShines[i] = nullptr;
		}
		++state;
	}

	TSelectShine* shine = mShines[mSelected];
	if (shine->mType != 2) {
		if (shine->mType == 0) {
			shine->mEmitter1->playCreateParticle();
			shine->mEmitter0->playCreateParticle();
		}
		shine->mEmitter2->playCreateParticle();
	}
}

s16 TSelectShineManager::getAngle(const JGeometry::TVec3<f32>& pos)
{
	JGeometry::TVec2<f32> dir(300.0f, 1300.0f);
	dir = dir - JGeometry::TVec2<f32>(pos.x, pos.z);
	JGeometry::TVec2<f32> forward(0.0f, 1.0f);
	f32 cross = dir.cross(forward);
	s16 angle = 57.295776f * fabsf(atan2f(cross, dir.dot(forward)));
	if (pos.x > cCenter.x)
		angle *= -1;
	return angle;
}

JGeometry::TVec3<f32> TSelectShineManager::getPosition(s16 angle)
{
	s16 a = 57.295776f * angle;
	return JGeometry::TVec3<f32>(1500.0f * JMASSin(a) + cCenter.x, cCenter.y,
	                             9000.0f * JMASCos(a) + cCenter.z);
}

void TSelectShineManager::startClose()
{
	mShines[mSelected]->mIsSelected = false;
	for (int i = 0; i < 8; ++i) {
		TSelectShine* shine = mShines[i];
		if (shine != nullptr && i != mSelected && !shine->unk49) {
			shine->unk49 = true;
			shine->unk48 = false;
		}
	}
	mIsClosing = true;
}

void TSelectShineManager::startIncrease(int step)
{
	TSelectShine* shine = mShines[mSelected];
	if (shine->mType != 2) {
		if (shine->mType == 0) {
			shine->mEmitter1->stopCreateParticle();
			shine->mEmitter0->stopCreateParticle();
		}
		shine->mEmitter2->stopCreateParticle();
	}

	mIsIncreasing = true;
	mRingSpeed    = (step * -40) / 10;
	mSelected += step;

	shine = mShines[mSelected];
	if (shine->mType != 2) {
		if (shine->mType == 0) {
			shine->mEmitter1->playCreateParticle();
			shine->mEmitter0->playCreateParticle();
		}
		shine->mEmitter2->playCreateParticle();
	}
}

void TSelectShineManager::startDecrease(int step)
{
	TSelectShine* shine = mShines[mSelected];
	if (shine->mType != 2) {
		if (shine->mType == 0) {
			shine->mEmitter1->stopCreateParticle();
			shine->mEmitter0->stopCreateParticle();
		}
		shine->mEmitter2->stopCreateParticle();
	}

	mIsDecreasing = true;
	mRingSpeed    = (step * 40) / 10;
	mSelected -= step;

	shine = mShines[mSelected];
	if (shine->mType != 2) {
		if (shine->mType == 0) {
			shine->mEmitter1->playCreateParticle();
			shine->mEmitter0->playCreateParticle();
		}
		shine->mEmitter2->playCreateParticle();
	}
}

// TODO: fabricated, get rid of this
inline void TSelectShineManager::updateShine(int i)
{
	// Copy as the pre-merge TVec3::operator= did (Vec struct copy).
	JGeometry::TVec3<f32> pos     = getPosition(mRingAngle + i * 40);
	*(Vec*)&mShines[i]->mPos      = *(Vec*)&pos;
	JGeometry::TVec3<f32> realPos = mShines[i]->mPos + mShines[i]->mOffset;
	s16 angle                     = getAngle(realPos);
	TSelectShine* shine           = mShines[i];
	MtxPtr mtx                    = shine->mModel->getBaseTRMtx();
	Mtx rot;
	MTXRotRad(rot, 'y', 0.017453292f * (angle - shine->mAngleY));
	MTXConcat(mtx, rot, mtx);
	shine->mAngleY = angle;
}

// TODO: Vector-copy chains and frame layout differ from the target.
void TSelectShineManager::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & 1) {
		for (int i = 0; i < mShineNum; ++i)
			mShines[i]->move();

		if (mIsIncreasing || mIsDecreasing) {
			mRingAngle += mRingSpeed;
			if (mIsIncreasing) {
				if (mRingAngle < mSelected * -40) {
					mRingAngle    = mSelected * -40;
					mIsIncreasing = false;
				}
			}
			if (mIsDecreasing) {
				if (mRingAngle > mSelected * -40) {
					mRingAngle    = mSelected * -40;
					mIsDecreasing = false;
				}
			}

			for (int i = 0; i < mShineNum; ++i)
				updateShine(i);
		}
	}

	if (cue & 2) {
		for (int i = 0; i < mShineNum; ++i) {
			mShines[i]->mAnmColor->setFrame(mShines[i]->mAnmFrame);
			mShines[i]->mModel->update();
			mShines[i]->mModel->viewCalc();
		}
	}

	if (cue & 8) {
		j3dSys.setViewMtx(graphics->mViewMtx);
		j3dSys.drawInit();
		j3dSys.unk4C = 3;
		mOpaBuffer->draw();
		j3dSys.unk4C = 4;
		mXluBuffer->draw();
		mOpaBuffer->frameInit();
		mXluBuffer->frameInit();
	}
}

TSelectShine::TSelectShine(J3DModelData* model_data, J3DAnmColor* anm_color,
                           JPAEmitterManager* emitter_manager,
                           JGeometry::TVec3<f32>& pos, s16 angle, u8 type,
                           f32 bob_phase, f32 bob_speed, f32 bob_height)
    : mIsSelected(0)
    , mBobPhase(0.0f)
    , mBobHeight(0.0f)
    , mBobSpeed(0.0f)
    , mSpinAngle(0)
    , mSpinSpeed(0)
    , mAngleY(angle)
    , mAnmFrame(0)
    , unk3E(0)
    , unk40(0.0f)
    , unk44(0.0f)
    , unk48(0)
    , unk49(0)
    , mType(0)
    , mEmitterManager(nullptr)
    , mEmitter0(nullptr)
    , mEmitter1(nullptr)
    , mEmitter2(nullptr)
{
	mModel    = new J3DModel(model_data, 0, 1);
	mAnmColor = anm_color;
	mPos.set(pos);
	mOffset.x = 0.0f;
	mOffset.y = 0.0f;
	mOffset.z = 0.0f;

	mEmitterManager = emitter_manager;
	mBobSpeed       = bob_speed;
	mBobHeight      = bob_height;
	mBobPhase       = bob_phase;
	mSpinSpeed      = 3;

	MtxPtr mtx = mModel->getBaseTRMtx();
	mtx[0][3]  = mPos.x;
	mtx[1][3]  = mPos.y;
	mtx[2][3]  = mPos.z;

	Mtx rot;
	MTXRotRad(rot, 'y', 0.017453292f * mAngleY);
	MTXConcat(mtx, rot, mtx);

	mType = type;
	if (mType != 2) {
		JGeometry::TVec3<f32> emitPos = mPos + mOffset;
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

		if (mType != 2) {
			if (mType == 0) {
				mEmitter1->stopCreateParticle();
				mEmitter0->stopCreateParticle();
			}
			mEmitter2->stopCreateParticle();
		}
	}
}

// TODO: Frame-only mismatch; inline stack layout differs.
void TSelectShine::move()
{
	f32 height;
	f32 scale = 0.9f;
	if (mBobPhase < 1.0f) {
		height
		    = makeNewPosition(mBobPhase, 0.0f, mBobHeight * scale, mBobHeight);
	} else if (mBobPhase < 2.0f) {
		height = makeNewPosition(mBobPhase - 1.0f, mBobHeight,
		                         mBobHeight * scale, 0.0f);
	} else if (mBobPhase < 3.0f) {
		height = makeNewPosition(mBobPhase - 2.0f, 0.0f, -mBobHeight * scale,
		                         -mBobHeight);
	} else if (mBobPhase < 4.0f) {
		height = makeNewPosition(mBobPhase - 3.0f, -mBobHeight,
		                         -mBobHeight * scale, 0.0f);
	}
	mOffset.y = height;

	MtxPtr mtx                = mModel->getBaseTRMtx();
	JGeometry::TVec3<f32> pos = mPos + mOffset;
	mtx[0][3]                 = pos.x;
	mtx[1][3]                 = pos.y;
	f32 depth                 = pos.z;
	mtx[2][3]                 = depth;

	s16 frame = -0.05f * depth;
	if (frame < 30)
		frame = 30;

	if (unk48) {
		mAnmFrame -= 8;
		if (mAnmFrame < frame)
			mAnmFrame = frame;
	} else if (unk49) {
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

	u8 alpha = 255.0f * (1.0f - (f32)(mAnmFrame / mAnmColor->getFrameMax()));
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

	if (mIsSelected) {
		Mtx rot;
		MTXRotRad(rot, 'y', 0.017453292f * (mSpinSpeed * SMSGetAnmFrameRate()));
		mSpinAngle += mSpinSpeed * SMSGetAnmFrameRate();
		if (mSpinAngle > 360)
			mSpinAngle -= 360;
		if (mSpinAngle < 0)
			mSpinAngle += 360;
		MTXConcat(mtx, rot, mtx);
	} else if (mSpinAngle != 0) {
		s16 step = mSpinSpeed * SMSGetAnmFrameRate();
		mSpinAngle += step;
		if (mSpinAngle > 360) {
			step       = step - mSpinAngle + 360;
			mSpinAngle = 0;
		}
		Mtx rot;
		MTXRotRad(rot, 'y', 0.017453292f * step);
		MTXConcat(mtx, rot, mtx);
	}

	mBobPhase += mBobSpeed;
	if (mBobPhase > 4.0f)
		mBobPhase = 0.0f;
}

f32 TSelectShine::makeNewPosition(f32 t, f32 p0, f32 p1, f32 p2)
{
	f32 s  = 1.0f - t;
	f32 ss = s * s;
	return p0 * ss + p1 * (2.0f * s * t) + p2 * (t * t);
}
