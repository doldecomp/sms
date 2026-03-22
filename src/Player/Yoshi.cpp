
#include <Player/Yoshi.hpp>
#include <Player/MarioMain.hpp>
#include <Map/Map.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/MSoundBGM.hpp>
#include <System/Application.hpp>
#include <System/MarDirector.hpp>
#include <MoveBG/Item.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/MActorAnm.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>

// Free function - J3DNode callback for Yoshi head control
static int YoshiHeadCtrl(J3DNode* node, int param) {
	if (param == 0) {
		// TODO: implement head rotation based on Mario's facing angle
	}
	return 1;
}

// Reverse address order for -inline deferred

// getFrameCtrl - 0x80150920
J3DFrameCtrl* TYoshi::getFrameCtrl() const {
	return mActor->getFrameCtrl(0);
}

// getMtxPtrFootL - 0x80150904
MtxPtr TYoshi::getMtxPtrFootL() const {
	return mActor->unk4->getAnmMtx(_3e);
}

// getMtxPtrFootR - 0x801508E8
MtxPtr TYoshi::getMtxPtrFootR() const {
	u16 footRJoint = *(u16*)((u8*)this + 0x40);
	return mActor->unk4->getAnmMtx(footRJoint);
}

// init - 0x8014FE5C (675 instructions - extremely complex)
void TYoshi::init(TMario* mario) {
	mMario = mario;
	mState = EGG;
	*(u8*)((u8*)this + 0x01) = 0;
	s16 unk06 = *(s16*)((u8*)this + 0x06);
	mSubState = unk06;
	*(s16*)((u8*)this + 0x04) = 7200;
	*(s16*)((u8*)this + 0x06) = 7200;
	mMaxJuice = 21300;
	mCurJuice = mMaxJuice;
	mTranslation.x = 0.0f;
	mTranslation.y = 0.0f;
	*(f32*)((u8*)this + 0x28) = 0.0f;
	*(f32*)((u8*)this + 0x2C) = 0.0f;
	*(s16*)((u8*)this + 0x70) = 0;
	*(s16*)((u8*)this + 0x72) = 384;
	// The rest of init is extremely complex (allocations, model setup, etc.)
	// TODO: implement remaining ~600 instructions
}

// initInLoadAfter - 0x8014FD88
void TYoshi::initInLoadAfter() {
	// TODO: implement - 53 instructions
	// Allocates and sets up sub-animation data
}

// thinkBtp - 0x8014FCD8
void TYoshi::thinkBtp(int animIdx) {
	int btpIdx = 4;
	switch (animIdx) {
	case 0:
		btpIdx = 0;
		break;
	case 1:
		btpIdx = 1;
		break;
	case 9:
		btpIdx = 2;
		break;
	case 25:
		btpIdx = 3;
		break;
	default:
		break;
	}

	if (mCurBtpIdx != (u16)btpIdx) {
		mActor->setBtpFromIndex(btpIdx);
		J3DFrameCtrl* ctrl = mActor->getFrameCtrl(3);
		ctrl->setRate(0.0f);
		mCurBtpIdx = btpIdx;
	}
}

// changeAnimation - 0x8014FC3C
void TYoshi::changeAnimation(int id) {
	int curAnm = mActor->getCurAnmIdx(0);
	if (id == curAnm)
		return;

	if (!mActor->checkCurAnmFromIndex(id, 0)) {
		mActor->setBckFromIndex(id);
	}
	thinkBtp(id);
	u32* rates = mAnimFrameRates;
	u32 frameRate = rates[id];
	// TODO: call play function on mBckPlayer with frameRate
}

// getEmitPosDir - 0x8014FBF0
void TYoshi::getEmitPosDir(JGeometry::TVec3<f32>* pos, JGeometry::TVec3<f32>* dir) const {
	J3DModel* model = mActor->unk4;
	MtxPtr mtx = model->getAnmMtx(mJoint);
	dir->x = mtx[0][0];
	dir->y = mtx[1][0];
	dir->z = mtx[2][0];
	pos->x = mtx[0][3];
	pos->y = mtx[1][3];
	pos->z = mtx[2][3];
}

// setEggYoshiPtr - 0x8014FBE8
void TYoshi::setEggYoshiPtr(TEggYoshi* egg) {
	mEgg = egg;
}

// appearFromEgg - 0x8014FA60
bool TYoshi::appearFromEgg(const JGeometry::TVec3<f32>& pos, f32 angle, TEggYoshi* egg) {
	// TODO: implement fully - 98 instructions
	return true;
}

// disappear - 0x8014F94C
bool TYoshi::disappear() {
	u8 state = (u8)mState;
	u8 active;
	if (state != 0) {
		active = 1;
	} else {
		active = 0;
	}
	if (!active)
		return false;

	if (state == MOUNTED) {
		mMario->getOffYoshi(true);
	}
	u32 marioState = mMario->mState;
	u8 inWater;
	if (marioState & 0x00030000) {
		inWater = 1;
	} else {
		inWater = 0;
	}
	if (inWater) {
		mState = DROWNING;
		// inline changeAnimation(25)
		int curAnm = mActor->getCurAnmIdx(0);
		if (curAnm != 25) {
			if (!mActor->checkCurAnmFromIndex(25, 0)) {
				mActor->setBckFromIndex(25);
			}
			thinkBtp(25);
			u32 frameRate = mAnimFrameRates[25];
			// TODO: set frame rate
		}
	} else {
		mState = DYING;
	}
	mType = 0;
	mSubState = 30;
	return true;
}

// kill - 0x8014F834
void TYoshi::kill() {
	u8 state = (u8)mState;
	u8 active;
	if (state != 0) {
		active = 1;
	} else {
		active = 0;
	}
	if (active) {
		if (state == MOUNTED) {
			mMario->getOffYoshi(true);
		}
		u32 marioState = mMario->mState;
		u8 inWater;
		if (marioState & 0x00030000) {
			inWater = 1;
		} else {
			inWater = 0;
		}
		if (inWater) {
			mState = DROWNING;
			changeAnimation(25);
		} else {
			mState = DYING;
		}
		mType = 0;
		mSubState = 30;
	}
	// TODO: stop sounds on mBckPlayer and mBckPlayer2
}

// ride - 0x8014F744
void TYoshi::ride() {
	mState = MOUNTED;
	changeAnimation(22);
	// TODO: set yoshi color, start sound, fire ride yoshi on director
}

// getOff - 0x8014F594
void TYoshi::getOff(bool knockedOff) {
	if ((u8)mState != MOUNTED)
		return;

	*(f32*)((u8*)this + 0x2C) = 0.0f;
	mState = UNMOUNTED;
	s16 subState = *(s16*)((u8*)this + 0x04);
	mSubState = subState;

	if (knockedOff) {
		changeAnimation(1);
	} else {
		changeAnimation(23);
	}
	// TODO: sound, warp, etc
}

// thinkAnimation - 0x8014F1A0
void TYoshi::thinkAnimation() {
	// TODO: implement - 253 instructions
}

// thinkUpper - 0x8014EF78
void TYoshi::thinkUpper() {
	if ((u8)mState != MOUNTED)
		return;
	// TODO: implement - 138 instructions
}

// doSearch - 0x8014EA18
void TYoshi::doSearch() {
	// TODO: implement - 344 instructions
}

// doEat - 0x8014E8F4
void TYoshi::doEat(u32 fruitID) {
	// TODO: implement - 73 instructions
}

// thinkHoldOut - 0x8014E794
void TYoshi::thinkHoldOut() {
	// TODO: implement - 88 instructions
}

// movement - 0x8014DAF4
void TYoshi::movement() {
	// TODO: implement - 800+ instructions
}

// calcAnim - 0x8014D6B8
void TYoshi::calcAnim() {
	// TODO: implement - 271 instructions
}

// viewCalc - 0x8014D638
void TYoshi::viewCalc() {
	u8 state = (u8)mState;
	u8 active;
	if (state != 0) {
		active = 1;
	} else {
		active = 0;
	}
	if (!active)
		return;

	mActor->viewCalc();
	// TODO: virtual viewCalc on sub-actors at 0x44, 0x48
	// TODO: non-virtual call on tongue object at 0x38
}

// entry - 0x8014D37C
void TYoshi::entry() {
	// TODO: implement - 175 instructions
}
