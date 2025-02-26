#include "Player/Watergun.hpp"

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

void TWaterGun::perform(u32 flags, JDrama::TGraphics* graphics) {
    if ((flags & 0x1) != 0) {
        if ((this->mFlags & 0x10) != 0) {
            this->mCurrentWater = 0;
        }
        this->movement();
    } 

    if ((flags & 0x2) != 0) {
        this->calcAnimation(graphics);
    }

    /* Requires MActor
        
    TNozzleBase *currentNozzle = TWaterGun_getCurrentNozzle_override(that);
    MActor *nozzleMactor       = currentNozzle->mModel;
    that->mFluddModel->perform(flags, graphics);
    if ((flags & 0x2) != 0) {
        if (nozzleMactor != nullptr) {
            PSMTXCopy(that->mFluddModel->mModel->mJointArray[that->mCurFluddTransformIdx],
                      nozzleMactor->mModel->mBaseMtx);
        }

        // Set emit positions for each stream
        // Note: I am unsure if it supports more than 2 natively, might have to expand this if more
        // than 2 emitters
        for (int i = 0; i < gNozzleNumberOfStreams[that->mCurrentNozzle]; ++i) {
            Mtx *mtx = that->getEmitMtx(i);
            if (mtx != nullptr) {
                that->mEmitPos[i].x = (*mtx)[0][3];
                that->mEmitPos[i].y = (*mtx)[1][3];
                that->mEmitPos[i].z = (*mtx)[2][3];
            }
        }
    }

    if (nozzleMactor != nullptr) {
        nozzleMactor->perform(flags, graphics);
    }
    */
}

void TWaterGun::calcAnimation(JDrama::TGraphics* graphics) { }

void TWaterGun::changeBackup() { }

void TWaterGun::changeNozzle(TNozzleType type, bool immediate) { }

bool TWaterGun::damage() { return false; }

void TWaterGun::emit() { }

Mtx* TWaterGun::getEmitMtx(int index) { return nullptr; }

void TWaterGun::getEmitPosDirSpeed(int index, JGeometry::TVec3<f32>* pos,
                                   JGeometry::TVec3<f32>* dir,
                                   JGeometry::TVec3<f32>* speed)
{
}

Mtx* TWaterGun::getNozzleMtx() { return nullptr; }

f32 TWaterGun::getPressure() { return 0.0f; }

f32 TWaterGun::getPressureMax() { return 0.0f; }

void TWaterGun::initInLoadAfter() { }

bool TWaterGun::isEmitting() { return false; }

bool TWaterGun::isPressureOn() { return false; }

void TWaterGun::movement() { }

void TWaterGun::rotateProp(f32 rotation) { }

void TWaterGun::setAmountToRate(f32 rate) {
    if (this->mCurrentNozzle == 3) {
        TNozzleBase* currentNozzle = this->mNozzleList[this->mCurrentNozzle];
        s32 amountMax = currentNozzle->mEmitParams.mAmountMax.value;
        this->mCurrentWater = amountMax;
    } else {
        TNozzleBase* currentNozzle = this->mNozzleList[this->mCurrentNozzle];
        this->mCurrentWater = rate * currentNozzle->mEmitParams.mAmountMax.value;
    }
}

void TWaterGun::setBaseTRMtx(Mtx matrix) { }

bool TWaterGun::suck(void)
{
	if (this->mCurrentNozzle == 3) {
		return false;
	}

	TNozzleBase* currentNozzle = this->mNozzleList[this->mCurrentNozzle];

	s32 suckRate               = (s32)((f32)this->mCurrentPressure
                         * currentNozzle->mEmitParams.mSuckRate.value);

	if (suckRate == 0) {
		return false;
	}

	this->mCurrentWater += suckRate;

	s32 maxWater = currentNozzle->mEmitParams.mAmountMax.value;
	if (this->mCurrentWater > maxWater) {
		this->mCurrentWater = maxWater;
	}

	if (this->mCurrentWater < maxWater) {
		// TODO: Play sucking sound
        /*
        if (gpMSound->gateCheck(0xf)) {
            MSoundSE::startSoundActor(0xf, that->mEmitPos[0], 0, nullptr, 0, 4);
        }
        */
	}

	return true;
}
