#include <Strategic/LiveActor.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/question.hpp>
#include <System/MarDirector.hpp>
#include <MarioUtil/MtxUtil.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/LodAnm.hpp>
#include <MarioUtil/ShadowUtil.hpp>
#include <Map/MapData.hpp>
#include <Map/Map.hpp>
#include <Map/MapCollisionManager.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <Enemy/Conductor.hpp>
#include <MSound/MAnmSound.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

f32 TLiveActor::mVelocityMinY = -40.0f;

TLiveActor::TLiveActor(const char* name)
    : TTakeActor(name)
{
	unk70 = nullptr;
	unk74 = nullptr;
	unk78 = nullptr;
	unk7C = 0;
	unk80 = nullptr;
	unk84 = nullptr;
	unk88 = nullptr;
	unk8C = nullptr;
	unk90 = nullptr;
	unk94.zero();
	unkA0.zero();
	unkAC.x   = 0.0f;
	unkAC.y   = 0.0f;
	unkAC.z   = 0.0f;
	unkB8     = 10.0f;
	unkBC     = 25.0f;
	unkC0     = 50.0f;
	unkC4     = nullptr;
	unkC8     = 0.0;
	unkCC     = 0.15;
	unkD0     = nullptr;
	unkD4     = nullptr;
	unkE4     = 0.0;
	unkE8     = 1;
	unkEC     = nullptr;
	mLiveFlag = 0x100;

	unkD8.x = unkD8.y = unkD8.z = 0.0;

	unkC4 = TMap::getIllegalCheckData();
	if (gpMarDirector->map != 8)
		mLiveFlag |= 0x2000;
}

TLiveActor::~TLiveActor() { }

void TLiveActor::calcRidePos()
{
	if (!unkD4)
		return;

	Mtx mtx;
	if (!unkD4->getRootJointMtx())
		SMS_GetActorMtx(*unkD4, mtx);
	else
		MTXCopy(*unkD4->getRootJointMtx(), mtx);
	MTXInverse(mtx, mtx);
	MTXMultVec(mtx, &mPosition, &unkD8);
}

bool TLiveActor::belongToGround() const
{
	if (unkC4 && ((unkC4->unk4 & 0x10 ? true : false) == true ? false : true)
	    && unkC4->unk44 != nullptr && !(mLiveFlag & 0x80 ? TRUE : FALSE))
		return true;

	return false;
}

#pragma dont_inline on
void TLiveActor::calcRideMomentum() { }
#pragma dont_inline off

J3DModel* TLiveActor::getModel() const { return unk74->unk4; }

Mtx* TLiveActor::getRootJointMtx() const { return nullptr; }

void TLiveActor::initLodAnm(const TLodAnmIndex* param_1, int param_2,
                            f32 param_3)
{
	if (!unkD0)
		unkD0 = new TLodAnm(this, param_1, param_2, param_3);
}

void TLiveActor::init(TLiveManager* manager)
{
	if (!manager) {
		if (unk3C) {
			unk78 = new TMActorKeeper(nullptr, 0);
			unk74 = unk78->createMActorFromDefaultBmd(
			    *(const char**)((u8*)unk3C + 0xC), 0);
		}
		gpConductor->registerAloneActor(this);
	} else {
		unk70 = manager;
		unk78 = new TMActorKeeper(unk70, 1);
		unk70->manageActor(this);
		unk74 = unk78->createMActorFromNthData(0, 0);
	}

	initHitActor(0, 1, 0, unkBC, unkC0, unkBC, unkC0);

	unk64 |= 1;
	mLiveFlag &= ~0x400;

	if (unk80)
		initAnmSound();
}

void TLiveActor::load(JSUMemoryInputStream& stream)
{
	char str[256];

	JDrama::TActor::load(stream);
	stream.readString(str, 256);
	JDrama::TNameRef* ref
	    = JDrama::TNameRefGen::getInstance()->getRootNameRef()->search(str);
	unkC4 = TMap::getIllegalCheckData();

	TLiveManager* casted = (TLiveManager*)ref;
	init(casted);
}

void TLiveActor::bind() { }

void TLiveActor::control() { }

void TLiveActor::calcRootMatrix()
{
	J3DModel* model = unk74->getUnk4();
	// TODO: DEG2SHORT?
	MsMtxSetXYZRPH(model->unk20, mPosition.x, mPosition.y, mPosition.z,
	               mRotation.x * (65536.0f / 360.0f),
	               mRotation.y * (65536.0f / 360.0f),
	               mRotation.z * (65536.0f / 360.0f));
	model->unk14 = mScaling;
}

void TLiveActor::kill()
{
	mLiveFlag |= 0x1;
	mLiveFlag |= 0x40;
}

u32 TLiveActor::receiveMessage(THitActor*, u32) { return 0; }

u32 TLiveActor::getShadowType() { return 0; }

void TLiveActor::setGroundCollision()
{
	if (!unkEC)
		return;
	if (!unkEC->unk8)
		return;

	unkEC->unk8->moveSRT(mPosition, mRotation, mScaling);
}

void TLiveActor::moveObject()
{
	ensureTakeSituation();
	unk94.zero();
	unkA0.zero();
	control();
	calcRideMomentum();
	bind();
	mPosition.add(unk94);
	mRotation.add(unkA0);
	setGroundCollision();

	if (unkD4) {
		Mtx mtx;
		if (getRootJointMtx() == nullptr) {
			SMS_GetActorMtx(*unkD4, mtx);
		} else {
			MTXCopy(*unkD4->getRootJointMtx(), mtx);
		}
		MTXInverse(mtx, mtx);
		MTXMultVec(mtx, &mPosition, &unkD8);
	}
}

void TLiveActor::requestShadow()
{
	if (mLiveFlag & 0xB)
		return;

	if (!(mLiveFlag & 0x204) || (mLiveFlag & 0x400)) {
		TCircleShadowRequest local_2c;

		local_2c.unk0 = mPosition;

		if (!(mLiveFlag & 0x80 ? 0 : 1)) {
			local_2c.unk0.y = unkC8;
			local_2c.unk1D  = 0;
		}

		local_2c.unk10 = local_2c.unkC = unkB8;

		local_2c.unk1C = getShadowType();
		local_2c.unk14 = mRotation.y;

		if (mLiveFlag & 0x400) {
			gpBindShadowManager->forceRequest(local_2c, getActorType());
		} else {
			gpBindShadowManager->request(local_2c, getActorType());
		}
	}

	if (!(mLiveFlag & 0x204) && !checkActorType(0x40000000)) {
		gpQuestionManager->request(mPosition, unkB8);
	}
}

void TLiveActor::drawObject(JDrama::TGraphics*)
{
	if (mLiveFlag & 3 || !unk74)
		return;

	unk74->setLightData(unkC4, mPosition);
	unk74->entry();
}

void TLiveActor::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (mLiveFlag & 0x201)
		return;

	if (param_1 & 1)
		moveObject();

	if (param_1 & 2)
		updateAnmSound();

	if (unk74) {
		if (param_1 & 2)
			unk74->frameUpdate();

		if (param_1 & 4)
			requestShadow();

		if (!(mLiveFlag & 6)) {
			if (param_1 & 2) {
				calcRootMatrix();
				unk74->calc();
			}

			if (param_1 & 4)
				unk74->viewCalc();

			if (param_1 & 0x200)
				drawObject(param_2);
		}
	}
}

void TLiveActor::performOnlyDraw(u32 param_1, JDrama::TGraphics* param_2)
{
	if (mLiveFlag & 0x201)
		return;
	if (!unk74)
		return;

	if (param_1 & 4)
		requestShadow();

	if (!(mLiveFlag & 6)) {
		if (param_1 & 2) {
			calcRootMatrix();
			unk74->calc();
		}

		if (param_1 & 4)
			unk74->viewCalc();

		if (param_1 & 0x200)
			drawObject(param_2);
	}
}

JGeometry::TVec3<f32>
TLiveActor::calcVelocityToJumpToY(const JGeometry::TVec3<f32>& param_1,
                                  f32 param_2, f32 param_3) const
{
	JGeometry::TVec3<f32> vec;
	SMSCalcJumpVelocityY(param_1, mPosition, param_2, param_3, -3.0625f, &vec);
	return vec;
}

f32 TLiveActor::getGravityY() const { return unkCC; }

BOOL TLiveActor::hasMapCollision() const { return unkEC ? 1 : 0; }

int TLiveActor::getJointTransByIndex(int param_1,
                                     JGeometry::TVec3<f32>* param_2) const
{
	if (unk74 == nullptr) {
		*param_2 = mPosition;
		return -1;
	}

	if (mLiveFlag & 4) {
		*param_2 = mPosition;
		return param_1;
	}

	MtxPtr mtx = unk74->unk4->getAnmMtx(param_1);
	param_2->set(mtx[0][3], mtx[1][3], mtx[2][3]);
	return param_1;
}

Vec TLiveActor::getFocalPoint() const { return mPosition; }

MtxPtr TLiveActor::getTakingMtx()
{
	if (!unk74)
		return nullptr;

	return unk74->unk4->unk20;
}

void TLiveActor::initAnmSound()
{
	if (unk80)
		return;

	if (mActorType & 0x4000000 ? true : false)
		unk80 = new MAnmSoundNPC(gpMSound);
	else
		unk80 = new MAnmSound(gpMSound);

	unk80->initAnmSound(nullptr, 1, 0.0f);
}

void TLiveActor::updateAnmSound()
{
	if (!unk80)
		return;
	if (!unk84)
		return;

	J3DFrameCtrl* ctrl = unk74->getFrameCtrl(0);
	unk80->animeLoop(&mPosition, ctrl->getCurrentFrame(), ctrl->getRate(), 0,
	                 4);
}

void TLiveActor::setAnmSound(const char* param_1)
{
	if (!unk80)
		OSPanic(__FILE__, 0x385, "TLiveActor[%s] : mAnmSound == NULL\n", mName);

	unk84 = param_1;

	if (unk84 != nullptr) {
		void* res = JKRFileLoader::getGlbResource(unk84);
		unk80->initAnmSound(res, 1, 0.0f);
	} else {
		unk80->initAnmSound(nullptr, 1, 0.0f);
	}
}

void TLiveActor::setCurAnmSound()
{
	const char* name = nullptr;

	if (unk74) {
		int idx = unk74->getCurAnmIdx(0);
		if (idx >= 0) {
			const char** table = getBasNameTable();

			name = !table ? nullptr : table[idx];
		}
	}

	setAnmSound(name);
}

const char** TLiveActor::getBasNameTable() const { return nullptr; }

void TLiveActor::stopAnmSound()
{
	if (unk80 && unk84)
		unk80->stop();
}
