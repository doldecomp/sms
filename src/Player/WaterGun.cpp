#include "Player/Watergun.hpp"
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <Player/MarioMain.hpp>
#include <Player/Yoshi.hpp>

#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>

// Define the global variable in .data section
const TNozzleBmdData nozzleBmdData
    = { { { 0,                                          // _00
	        0,                                          // _04
	        nullptr,                                    // mHelmetPath
	        "/mario/watergun2/normal_wg",               // mPath
	        "/mario/watergun2/normal_wg/normal_wg.bmd", // mBmdPath
	        1,                                          // mNumEmitters
	        2,                                          // _15
	        { { 1, 0 }, { 4, 0 }, { 4, 0 } } },
	      { 0,                                          // _00
	        0,                                          // _04
	        nullptr,                                    // mHelmetPath
	        "/mario/watergun2/rocket_wg",               // mPath
	        "/mario/watergun2/rocket_wg/rocket_wg.bmd", // mBmdPath
	        1,                                          // mNumEmitters
	        2,                                          // _15
	        { { 2, 1 }, { 4, 0 }, { 4, 0 } } },
	      { 0,                                        // _00
	        0,                                        // _04
	        "/mario/bmd/wg_hel_diver.bmd",            // mHelmetPath
	        "/mario/watergun2/hover_wg",              // mPath
	        "/mario/watergun2/hover_wg/hover_wg.bmd", // mBmdPath
	        2,                                        // mNumEmitters
	        0xc,                                      // _15
	        { { 1, 0 }, { 1, 0 }, { 4, 0 } } },
	      { 0,                                        // _00
	        0,                                        // _04
	        nullptr,                                  // mHelmetPath
	        "/mario/watergun2/dummy_wg",              // mPath
	        "/mario/watergun2/dummy_wg/dummy_wg.bmd", // mBmdPath
	        1,                                        // mNumEmitters
	        2,                                        // _15
	        { { 3, 0 }, { 4, 0 }, { 4, 0 } } },
	      { 0,                                        // _00
	        0,                                        // _04
	        nullptr,                                  // mHelmetPath
	        "/mario/watergun2/hover_wg",              // mPath
	        "/mario/watergun2/hover_wg/hover_wg.bmd", // mBmdPath
	        2,                                        // mNumEmitters
	        0xc,                                      // _15
	        { { 1, 0 }, { 1, 0 }, { 4, 0 } } },
	      { 0,                                      // _00
	        0,                                      // _04
	        nullptr,                                // mHelmetPath
	        "/mario/watergun2/back_wg",             // mPath
	        "/mario/watergun2/back_wg/back_wg.bmd", // mBmdPath
	        1,                                      // mNumEmitters
	        2,                                      // _15
	        { { 1, 0 }, { 4, 0 }, { 4, 0 } } } } };

TWaterGun::TWaterGun(TMario* mario)
    : mMario(mario)
    , mNozzleDeform("spray", "spray.prm", this)
    , mNozzleDeformBomb("bomb", "bomb.prm", this)
    , mNozzleRocket("rocket", "rocket.prm", this)
    , mNozzleUnderWater("underwater", "underwater.prm", this)
    , mNozzleYoshiDeform("yoshi", "yoshi.prm", this)
    , mNozzleYoshiDeformBomb("yoshibomb", "yoshibomb.prm", this)
    , mNozzleHover("hover", "hover.prm", this)
    , mNozzleTurbo("turbo", "turbo.prm", this)
{
	mNozzleList[0] = &mNozzleDeform;
	mNozzleList[1] = &mNozzleDeformBomb;
	mNozzleList[2] = &mNozzleRocket;
	mNozzleList[3] = &mNozzleUnderWater;
	mNozzleList[4] = &mNozzleYoshiDeform;
	mNozzleList[5] = &mNozzleYoshiDeformBomb;
	mNozzleList[6] = &mNozzleHover;
	mNozzleList[7] = &mNozzleTurbo;
}

void TWaterGun::init() { }

inline void TWaterGun::setEmitPt()
{
	int index;
	int offset;
	// TODO: Register r27 and r28 are swapped on some instructions. I cannot
	// figure out how to swap them correctly. Very likely some inline setter.
	// Below looks ugly, but closest i could get.
	for (index = 0, offset = 0;
	     index < nozzleBmdData.getEmitterCount(this->mCurrentNozzle);
	     index += 1, offset += 0xc) {
		MtxPtr p1 = this->getEmitMtx(index);
		if (p1 != nullptr) {
			*(float*)((int)&this->mEmitPos[index].x + offset) = (p1)[0][3];
			*(float*)((int)&this->mEmitPos[index].y + offset) = (p1)[1][3];
			*(float*)((int)&this->mEmitPos[index].z + offset) = (p1)[2][3];
		}
	}
}

void TWaterGun::perform(u32 flags, JDrama::TGraphics* graphics)
{
	volatile u32 unused2[22]; // Huge stack for some reason...
	Mtx* p1;
	MActor* p2;
	int iVar2;
	int iVar3;

	if ((flags & 0x1) != 0) {
		if ((this->mFlags & 0x10) != 0) {
			this->mCurrentWater = 0;
		}
		this->movement();
	}

	if ((flags & 0x2) != 0) {
		this->calcAnimation(graphics);
	}

	this->mFluddModel->perform(flags, graphics);

	if ((flags & 0x2) != 0) {
		p2 = this->getCurrentNozzle()->mActor;
		if (p2 != nullptr) {
			PSMTXCopy(this->getModel()->getAnmMtx(this->mCurFluddTransformIdx),
			          p2->unk4->mBaseMtx);
		}

		setEmitPt();
	}

	p2 = this->getCurrentNozzle()->mActor;
	if (p2 != nullptr) {
		p2->perform(flags, graphics);
	}
}

void TWaterGun::calcAnimation(JDrama::TGraphics* graphics) { }

void TWaterGun::changeBackup() { }

void TWaterGun::changeNozzle(TNozzleType type, bool immediate) { }

bool TWaterGun::damage()
{
	if (this->hasWater()) {
		TNozzleBase* nozzle = this->getCurrentNozzle();

		this->mCurrentWater -= nozzle->mEmitParams.mDamageLoss.value;

		if (this->mCurrentWater < 0) {
			this->mCurrentWater = 0;
		}
		return true;
	}
	return false;
}

void TWaterGun::emit() { }

MtxPtr TWaterGun::getEmitMtx(int jointIndex)
{
	volatile u32 unused2[24]; // TODO: A lot of stack space, possibly a lot of
	                          // inlined functions.
	MtxPtr result;
	if (!this->mMario->onYoshi()) {
		result = this->getYoshiMtx();
	} else {
		result = this->getWillBeEmitted(jointIndex);
	}
	return result;
}

void TWaterGun::getEmitPosDirSpeed(int index, JGeometry::TVec3<f32>* pos,
                                   JGeometry::TVec3<f32>* dir,
                                   JGeometry::TVec3<f32>* speed)
{
}

MtxPtr TWaterGun::getNozzleMtx()
{
	return this->mFluddModel->unk4->getAnmMtx(this->mCurFluddTransformIdx);
}

f32 TWaterGun::getPressure() { return 0.0f; }

f32 TWaterGun::getPressureMax()
{
	volatile u32 unused2[6]; // TODO: A lot of stack space, possibly a lot of
	                         // inlined functions.

	if (this->getCurrentNozzle()->getNozzleKind() == 1) {
		return this->getCurrentNozzle()->mEmitParams.mInsidePressureMax.get();
	}

	return 0.0f;
}

void TWaterGun::initInLoadAfter() { }

bool TWaterGun::isEmitting() { return false; }

bool TWaterGun::isPressureOn() { return false; }

void TWaterGun::movement() { }

void TWaterGun::rotateProp(f32 rotation) { }

void TWaterGun::setAmountToRate(f32 rate)
{
	volatile u32 unused2[7]; // TODO: possibly inlined function
	if (this->mCurrentNozzle == 3) {
		TNozzleBase* currentNozzle = this->getCurrentNozzle();
		s32 amountMax       = currentNozzle->mEmitParams.mAmountMax.get();
		this->mCurrentWater = amountMax;
	} else {
		TNozzleBase* currentNozzle = this->getCurrentNozzle();
		this->mCurrentWater
		    = rate * currentNozzle->mEmitParams.mAmountMax.get();
	}
}

void TWaterGun::setBaseTRMtx(Mtx matrix)
{
	// TODO: Needs MSMtxSetRotRPH header
}

bool TWaterGun::suck()
{
	volatile u32 unused1[7]; // TODO: A lot of stack space, possibly a lot of
	if (this->isYoshiNozzle()) {
		return false;
	} else {
		s32 suckRate = this->getSuckRate();
		if (suckRate > 0) {
			this->incWater(suckRate);
			return true;
		}
	}
	return false;
}
