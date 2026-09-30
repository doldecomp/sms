#include <GC2D/SelectShine2.hpp>
#include <macros.h>
#include <stdlib.h>
#include <math.h>
#include <dolphin/mtx.h>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DMaterialAnm.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DDrawBuffer.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DAnmLoader.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/JDrama/JDRGraphics.hpp>
#include <JSystem/JMath.hpp>

extern f32 SMSGetAnmFrameRate(); // avoid including Application.hpp

// TODO: work in progress. Matching: ~TSelectShine, the TSelectShineManager
// constructor and destructor, startClose, startIncrease, startDecrease,
// __sinit, TVec3::set<float> and TVec2::sub. Close: the TSelectShine
// constructor (99.8%). Remaining: move (96.7%), initData (96.2%) and perform
// (92.5%), which still differ in struct copies and register order (the target
// keeps one more TVec3 copy of the new position, reserves 0x18 more stack
// bytes in initData and 0x30 more in move), and the UNUSED makeNewPosition,
// whose body is unknown (the map only gives its size, 0x2c). getShinePosition
// and getShineAngle are fabricated inline layers: they reproduce the target's
// out-of-line JMASSin, JMASCos, TVec3::set and TVec2::sub calls in perform.

// TODO: removeme
static const char* dummy                 = "\0\0\0\0\0\0\0\0\0\0\0";
static const char* SMS_NO_MEMORY_MESSAGE = "メモリが足りません\n";

JGeometry::TVec3<f32> TSelectShineManager::cCenter(300.0f, 160.0f, -9000.0f);

TSelectShineManager::TSelectShineManager(const char* name)
    : JDrama::TViewObj(name)
    , mDrawBuffer1(nullptr)
    , mDrawBuffer2(nullptr)
    , unk78(0.0f)
    , unk7C(0.0f)
    , mShineNum(0)
    , unk90(0.0f)
    , unk94(0.0f)
    , unk98(0)
    , mAngle(0)
    , mAngleSpeed(0.0f)
    , unkA4(0)
    , unkA5(0)
    , unkA6(0)
    , unkA7(0)
{
}

void TSelectShineManager::initData(u8* kinds, u8 shine_num, u8 current,
                                   JPAEmitterManager* emitter_manager)
{
	mDrawBuffer1 = new J3DDrawBuffer(0x400);
	j3dSys.setDrawBuffer(mDrawBuffer1, 0);
	mDrawBuffer2 = new J3DDrawBuffer(0x400);
	j3dSys.setDrawBuffer(mDrawBuffer2, 1);

	void* bmd = JKRFileLoader::getGlbResource("/select/shine_menu.bmd");
	void* bpk = JKRFileLoader::getGlbResource("/select/shine_menu.bpk");
	J3DModelData* modelData = J3DModelLoaderDataBase::load(bmd, 0x51040000);
	J3DAnmColor* anmColor   = (J3DAnmColor*)J3DAnmLoaderDataBase::load(bpk);
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
	J3DModelData* emptyModelData
	    = J3DModelLoaderDataBase::load(emptyBmd, 0x51040000);
	J3DAnmColor* emptyAnmColor
	    = (J3DAnmColor*)J3DAnmLoaderDataBase::load(emptyBpk);
	emptyAnmColor->searchUpdateMaterialID(emptyModelData);
	for (u16 i = 0; i < modelData->getMaterialNum(); ++i) {
		J3DMaterialAnm* anm = new J3DMaterialAnm;
		emptyModelData->getMaterialNodePointer(i)->change();
		emptyModelData->getMaterialNodePointer(i)->setMaterialAnm(anm);
	}
	emptyModelData->entryMatColorAnimator(emptyAnmColor);

	mShineNum = shine_num;
	mCurrent  = current;
	mAngle    = mCurrent * -40;
	for (int i = 0; i < 8; ++i) {
		JGeometry::TVec3<f32> pos = getPosition(mAngle + i * 40);
		s16 angle                 = getAngle(pos);
		if (pos.x > cCenter.x)
			angle *= -1;
		if (kinds[i] == 3) {
			mShines[i] = new TSelectShine(
			    modelData, anmColor, emitter_manager, pos, angle, 0,
			    (f32)(s32)(4000.0f * (0.000030517578f * (f32)rand())) / 1000.0f,
			    0.01f, 10.0f);
		} else if ((u8)(kinds[i] - 1) <= 1) {
			mShines[i] = new TSelectShine(
			    emptyModelData, emptyAnmColor, emitter_manager, pos, angle, 1,
			    (f32)(s32)(4000.0f * (0.000030517578f * (f32)rand())) / 1000.0f,
			    0.01f, 10.0f);
		} else {
			mShines[i] = nullptr;
		}
	}

	TSelectShine* shine = mShines[mCurrent];
	if (shine->mKind != 2) {
		if (shine->mKind == 0) {
			shine->unk54->mStatus &= ~1;
			shine->unk50->mStatus &= ~1;
		}
		shine->unk58->mStatus &= ~1;
	}
}

s16 TSelectShineManager::getAngle(const JGeometry::TVec3<f32>& pos)
{
	JGeometry::TVec2<f32> center(300.0f, 1300.0f);
	JGeometry::TVec2<f32> p(pos.x, pos.z);
	JGeometry::TVec2<f32> diff = center - p;
	JGeometry::TVec2<f32> up(0.0f, 1.0f);
	return (s16)(57.295776f * fabsf(atan2f(diff.cross(up), diff.dot(up))));
}

JGeometry::TVec3<f32> TSelectShineManager::getPosition(s16 angle)
{
	s16 a = (s16)(57.295776f * (f32)angle);
	return JGeometry::TVec3<f32>(1500.0f * JMASSin(a) + cCenter.x, cCenter.y,
	                             9000.0f * JMASCos(a) + cCenter.z);
}

void TSelectShineManager::startClose()
{
	mShines[mCurrent]->mIsSpinning = 0;
	for (int i = 0; i < 8; ++i) {
		TSelectShine* shine = mShines[i];
		if (shine != nullptr && i != mCurrent && shine->unk49 == 0) {
			shine->unk49 = 1;
			shine->unk48 = 0;
		}
	}
	unkA7 = 1;
}

void TSelectShineManager::startIncrease(int count)
{
	// stack frame padding: the target reserves 0x18 more bytes here
	JGeometry::TVec3<f32> tmpA, tmpB;
	TSelectShine* shine = mShines[mCurrent];
	if (shine->mKind != 2) {
		if (shine->mKind == 0) {
			shine->unk54->mStatus |= 1;
			shine->unk50->mStatus |= 1;
		}
		shine->unk58->mStatus |= 1;
	}
	unkA4       = 1;
	mAngleSpeed = (f32)((count * -40) / 10);
	mCurrent += count;
	shine = mShines[mCurrent];
	if (shine->mKind != 2) {
		if (shine->mKind == 0) {
			shine->unk54->mStatus &= ~1;
			shine->unk50->mStatus &= ~1;
		}
		shine->unk58->mStatus &= ~1;
	}
}

void TSelectShineManager::startDecrease(int count)
{
	// stack frame padding: the target reserves 0x18 more bytes here
	JGeometry::TVec3<f32> tmpA, tmpB;
	TSelectShine* shine = mShines[mCurrent];
	if (shine->mKind != 2) {
		if (shine->mKind == 0) {
			shine->unk54->mStatus |= 1;
			shine->unk50->mStatus |= 1;
		}
		shine->unk58->mStatus |= 1;
	}
	unkA5       = 1;
	mAngleSpeed = (f32)((count * 40) / 10);
	mCurrent -= count;
	shine = mShines[mCurrent];
	if (shine->mKind != 2) {
		if (shine->mKind == 0) {
			shine->unk54->mStatus &= ~1;
			shine->unk50->mStatus &= ~1;
		}
		shine->unk58->mStatus &= ~1;
	}
}

void TSelectShineManager::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_MOVE) {
		for (int i = 0; i < mShineNum; ++i)
			mShines[i]->move();

		if (unkA4 != 0 || unkA5 != 0) {
			mAngle = (s32)((f32)mAngle + mAngleSpeed);
			if (unkA4 != 0) {
				int limit = mCurrent * -40;
				if (mAngle < limit) {
					mAngle = limit;
					unkA4  = 0;
				}
			}
			if (unkA5 != 0) {
				int limit = mCurrent * -40;
				if (mAngle > limit) {
					mAngle = limit;
					unkA5  = 0;
				}
			}
			for (int i = 0; i < mShineNum; ++i) {
				JGeometry::TVec3<f32> pos = getShinePosition(i);
				mShines[i]->mPosition     = pos;
				JGeometry::TVec3<f32> target
				    = mShines[i]->mPosition + mShines[i]->unk18;
				s16 angle = getShineAngle(target);
				if (pos.x > cCenter.x)
					angle *= -1;
				TSelectShine* shine = mShines[i];
				Mtx rot;
				PSMTXRotRad(rot, 'y',
				            0.017453292f * (f32)(angle - shine->unk3A));
				PSMTXConcat(shine->mModel->getBaseTRMtx(), rot,
				            shine->mModel->getBaseTRMtx());
				shine->unk3A = angle;
			}
		}
	}

	if (cue & CUE_CALC_ANIM) {
		for (int i = 0; i < mShineNum; ++i) {
			mShines[i]->mAnmColor->setFrame((f32)mShines[i]->unk3C);
			mShines[i]->mModel->calc();
			mShines[i]->mModel->viewCalc();
		}
	}

	if (cue & CUE_DRAW) {
		MTXCopy(graphics->getViewMtx(), j3dSys.mViewMtx);
		j3dSys.drawInit();
		j3dSys.unk4C = 3;
		mDrawBuffer1->draw();
		j3dSys.unk4C = 4;
		mDrawBuffer2->draw();
		mDrawBuffer1->frameInit();
		mDrawBuffer2->frameInit();
	}
}

TSelectShine::TSelectShine(J3DModelData* model_data, J3DAnmColor* anm_color,
                           JPAEmitterManager* emitter_manager,
                           JGeometry::TVec3<f32>& position, s16 angle, u8 kind,
                           f32 param_7, f32 param_8, f32 param_9)
{
	mIsSpinning     = 0;
	unk28           = 0.0f;
	unk2C           = 0.0f;
	unk30           = 0.0f;
	unk34           = 0;
	unk38           = 0;
	unk3A           = angle;
	unk3C           = 0;
	unk3E           = 0;
	unk40           = 0.0f;
	unk44           = 0.0f;
	unk48           = 0;
	unk49           = 0;
	mKind           = 0;
	mEmitterManager = nullptr;
	unk50           = nullptr;
	unk54           = nullptr;
	unk58           = nullptr;

	mModel          = new J3DModel(model_data, 0, 1);
	mAnmColor       = anm_color;
	mPosition.x     = position.x;
	mPosition.y     = position.y;
	mPosition.z     = position.z;
	unk18.x         = 0.0f;
	unk18.y         = 0.0f;
	unk18.z         = 0.0f;
	mEmitterManager = emitter_manager;
	unk30           = param_8;
	unk2C           = param_9;
	unk28           = param_7;
	unk38           = 3;
	MtxPtr mtx      = mModel->getBaseTRMtx();
	mtx[0][3]       = mPosition.x;
	mtx[1][3]       = mPosition.y;
	mtx[2][3]       = mPosition.z;
	Mtx rot;
	PSMTXRotRad(rot, 'y', 0.017453292f * (f32)unk3A);
	PSMTXConcat(mtx, rot, mtx);
	mKind = kind;
	if (mKind != 2) {
		JGeometry::TVec3<f32> pos = mPosition + unk18;
		if (mKind == 0) {
			mEmitterManager->createEmitter(pos, 0, nullptr, nullptr);
			unk50 = mEmitterManager->unkC8[0][0];
			mEmitterManager->createEmitter(pos, 1, nullptr, nullptr);
			unk54 = mEmitterManager->unkC8[0][0];
			mEmitterManager->createEmitter(pos, 2, nullptr, nullptr);
		} else {
			mEmitterManager->createEmitter(pos, 3, nullptr, nullptr);
		}
		unk58 = mEmitterManager->unkC8[0][0];
		if (mKind != 2) {
			if (mKind == 0) {
				unk54->mStatus |= 1;
				unk50->mStatus |= 1;
			}
			unk58->mStatus |= 1;
		}
	}
}

// fabricated: a quadratic Bezier inline; the target evaluates each segment of
// move() with this shape (p0 * u^2 + p1 * 2ut + p2 * t^2)
static inline f32 bezier(f32 p0, f32 p1, f32 p2, f32 t)
{
	f32 u = 1.0f - t;
	return p0 * (u * u) + p1 * (2.0f * u * t) + p2 * (t * t);
}

void TSelectShine::move()
{
	f32 t = unk28;
	f32 y;
	if (t < 1.0f) {
		y = bezier(0.0f, unk2C * 0.9f, unk2C, t);
	} else if (t < 2.0f) {
		y = bezier(unk2C, unk2C * 0.9f, 0.0f, t - 1.0f);
	} else if (t < 3.0f) {
		y = bezier(0.0f, -unk2C * 0.9f, -unk2C, t - 2.0f);
	} else if (t < 4.0f) {
		y = bezier(-unk2C, -unk2C * 0.9f, 0.0f, t - 3.0f);
	}
	unk18.y = y;

	MtxPtr mtx                = mModel->getBaseTRMtx();
	JGeometry::TVec3<f32> pos = mPosition + unk18;
	mtx[0][3]                 = pos.x;
	mtx[1][3]                 = pos.y;
	mtx[2][3]                 = pos.z;

	s16 minFrame = (s16)(-0.05f * pos.z);
	if (minFrame < 30)
		minFrame = 30;
	if (unk48 != 0) {
		unk3C -= 8;
		if (unk3C < minFrame)
			unk3C = minFrame;
	} else if (unk49 != 0) {
		unk3C += 8;
		s16 maxFrame = mAnmColor->getFrameMax() - 1;
		if (unk3C > maxFrame)
			unk3C = maxFrame;
	} else {
		unk3C = minFrame;
		if (minFrame < 0)
			unk3C = 0;
		s16 maxFrame = mAnmColor->getFrameMax();
		if (minFrame > maxFrame)
			unk3C = maxFrame;
	}

	u8 alpha = (u8)(255.0f * (1.0f - (f32)(unk3C / mAnmColor->getFrameMax())));
	if (mKind != 2) {
		if (mKind == 0) {
			unk50->setGlobalRTMatrix(mtx);
			unk54->setGlobalRTMatrix(mtx);
			unk50->setGlobalAlpha(alpha);
			unk54->setGlobalAlpha(alpha);
		}
		unk58->setGlobalRTMatrix(mtx);
		unk58->setGlobalAlpha(alpha);
	}

	if (mIsSpinning != 0) {
		f32 rate = SMSGetAnmFrameRate();
		Mtx rot;
		PSMTXRotRad(rot, 'y', 0.017453292f * ((f32)unk38 * rate));
		unk34 = (s32)((f32)unk38 * SMSGetAnmFrameRate() + (f32)unk34);
		if (unk34 > 360)
			unk34 = unk34 - 360;
		if (unk34 < 0)
			unk34 = unk34 + 360;
		PSMTXConcat(mtx, rot, mtx);
	} else if (unk34 != 0) {
		s16 delta = (s16)((f32)unk38 * SMSGetAnmFrameRate());
		unk34 += delta;
		if (unk34 > 360) {
			unk34 = 0;
			delta = (delta - unk34) + 360;
		}
		Mtx rot;
		PSMTXRotRad(rot, 'y', 0.017453292f * (f32)delta);
		PSMTXConcat(mtx, rot, mtx);
	}

	unk28 += unk30;
	if (unk28 > 4.0f)
		unk28 = 0.0f;
}

void TSelectShine::makeNewPosition(f32 x, f32 y, f32 z, f32 speed)
{
	// TODO: unknown; only the size (0x2c) is known from the map
}
