#include <Enemy/BathtubPeach.hpp>
#include <M3DUtil/MActor.hpp>
#include <Map/BathWaterManager.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MoveBG/MapObjCorona.hpp>
#include <Player/MarioAccess.hpp>
#include <Strategic/Nerve.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Spine.hpp>
#include <System/Application.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoaderFlags.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <math.h>

// rogue includes needed for matching sinit, bss & rodata
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/MActorAnm.hpp>
#include <Map/MapCollisionEntry.hpp>

static const char* bathtubpeach_bastable[] = {
	nullptr, nullptr,
	nullptr, nullptr,
	nullptr, nullptr,
	nullptr, nullptr,
	nullptr, nullptr,
	nullptr, nullptr,
	nullptr, nullptr,
	nullptr, nullptr,
	nullptr, nullptr,
	nullptr, nullptr,
	nullptr, "/scene/bathtubpeach/bas/peach_wait.bas",
	nullptr,
};

class TNervePeachStagger : public TNerveBase<TLiveActor> {
public:
	virtual BOOL execute(TSpineBase<TLiveActor>* spine) const
	{
		TBathtubPeach* self = (TBathtubPeach*)spine->getBody();
		self->changeAnm(0, 0, 0.5f);
		if (self->getMActor()->curAnmEndsNext(ANM_TYPE_BCK, nullptr))
			return true;
		return false;
	}

	static const TNervePeachStagger& theNerve()
	{
		static TNervePeachStagger nerve;
		return nerve;
	}
};

class TNervePeachEscape : public TNerveBase<TLiveActor> {
public:
	virtual BOOL execute(TSpineBase<TLiveActor>* spine) const
	{
		TBathtubPeach* self = (TBathtubPeach*)spine->getBody();
		TBathtub* bathtub
		    = static_cast<TBathtub*>(JDrama::TNameRefGen::search("バスタブ"));
		if (bathtub->unk29A)
			return false;

		if (!(spine->getTime() & 4)) {
			if (bathtub->getBathtubData().unk64)
				spine->pushNerve(&TNervePeachStagger::theNerve());
			return false;
		}

		self->changeAnm(1, 1, 0.5f);

		Mtx* mtx = bathtub->getRootJointMtx();
		f32 cx   = (*mtx)[0][3];
		f32 cz   = (*mtx)[2][3];

		JGeometry::TVec3<f32> marioPos = *gpMarioPos;
		f32 marioAngle
		    = (360.0f / 65536.0f) * matan(marioPos.z - cz, marioPos.x - cx);
		f32 dz        = self->mPosition.z - cz;
		f32 dx        = self->mPosition.x - cx;
		f32 selfAngle = (360.0f / 65536.0f) * matan(dz, dx);
		f32 diff
		    = -180.0f
		      + std::fmodf(360.0f + (selfAngle - marioAngle - -180.0f), 360.0f);

		f32 angle;
		if (diff < 0.0f)
			angle
			    = -180.0f
			      + std::fmodf(360.0f
			                       + (marioAngle - self->getParam()->angle.get()
			                          - -180.0f),
			                   360.0f);
		else
			angle
			    = -180.0f
			      + std::fmodf(360.0f
			                       + (marioAngle + self->getParam()->angle.get()
			                          - -180.0f),
			                   360.0f);

		f32 radius = self->getParam()->radius.get();
		JGeometry::TVec3<f32> target;
		target.x = cx + radius * MsSin(angle);
		target.z = cz + radius * MsCos(angle);
		self->goTo(target);
		self->faceTo(*gpMarioPos, self->getParam()->turnSpeed2.get());
		return false;
	}

	static const TNervePeachEscape& theNerve()
	{
		static TNervePeachEscape nerve;
		return nerve;
	}
};

TBathtubPeach::TBathtubPeach(const char* name)
    : TSpineEnemy(name)
{
	onLiveFlag(LIVE_FLAG_AIRBORNE);
	offLiveFlag(LIVE_FLAG_ENABLE_CLIPPING);
	offLiveFlag(LIVE_FLAG_UNK10);
}

// fabricated
inline void clampMove(TBathtubPeach* s, JGeometry::TVec2<f32>& d)
{
	if (d.squared() >= s->getParam()->speed.get() * s->getParam()->speed.get())
		d.setLength(s->getParam()->speed.get());
}

void TBathtubPeach::goTo(const JGeometry::TVec3<f32>& target)
{
	JGeometry::TVec2<f32> d(target.x - mPosition.x, target.z - mPosition.z);
	clampMove(this, d);
	mPosition.x += d.x;
	mPosition.z += d.y;
}

void TBathtubPeach::faceTo(const JGeometry::TVec3<f32>& target, f32 speed)
{
	f32 dx = target.x - mPosition.x;
	f32 dz = target.z - mPosition.z;
	if (dx * dx + dz * dz <= 0.0000038146973f)
		return;

	f32 yaw = MsAtan2(dz, dx) - 90.0f;
	f32 diff
	    = -180.0f + std::fmodf(360.0f + (yaw - mRotation.y - -180.0f), 360.0f);
	if (diff < -speed)
		mRotation.y
		    = -180.0f
		      + std::fmodf(360.0f + (mRotation.y - speed - -180.0f), 360.0f);
	else if (diff > speed)
		mRotation.y
		    = -180.0f
		      + std::fmodf(360.0f + (mRotation.y + speed - -180.0f), 360.0f);
	else
		mRotation.y = yaw;
}

void TBathtubPeach::changeAnm(int bck, int btp, f32 rate)
{
	if (!getMActor()->checkCurBckFromIndex(bck)) {
		getMActor()->setBckFromIndex(bck);
		setAnmSound(getBas(bck));
	}

	if (getMActor()->getCurAnmIdx(ANM_TYPE_BTP) != btp)
		getMActor()->setBtpFromIndex(btp);

	J3DFrameCtrl* ctrl = getMActor()->getFrameCtrl(ANM_TYPE_BCK);
	ctrl->setRate(rate * (2.0f * SMSGetAnmFrameRate()));
}

const char** TBathtubPeach::getBasNameTable() const
{
	return bathtubpeach_bastable;
}

void TBathtubPeach::init(TLiveManager* manager)
{
	TSpineEnemy::init(manager);
	mSpine->initWith(&TNervePeachEscape::theNerve());
	initAnmSound();
	reset();
	mScaling.set(2.0f, 2.0f, 2.0f);
}

void TBathtubPeach::reset()
{
	mPosition.scale(0.21f);
	unk150.init(50.0f, 50.0f, 50.0f, 50.0f, 0.0f);
	unk130 = 0;
	mScaling.set(1.5f, 1.5f, 1.5f);
	mBinder = &unk150;
	TSpineEnemy::reset();
	changeAnm(1, 1, 0.5f);
}

void TBathtubPeach::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TSpineEnemy::perform(cue, graphics);
}

Mtx* TBathtubPeach::getRootJointMtx() const
{
	return (Mtx*)getModel()->getBaseTRMtx();
}

BOOL TBathtubPeach::receiveMessage(THitActor* sender, u32 message)
{
	return TSpineEnemy::receiveMessage(sender, message);
}

void TBathtubPeach::calcRootMatrix()
{
	TBathtub* bathtub
	    = static_cast<TBathtub*>(JDrama::TNameRefGen::search("バスタブ"));
	if (bathtub && bathtub->unk29A)
		PSMTXCopy(bathtub->getPeachMtxInDemo(), getModel()->getBaseTRMtx());
	else
		TLiveActor::calcRootMatrix();
}

TBathtubPeachParams* TBathtubPeach::getParam() const
{
	return (TBathtubPeachParams*)((TEnemyManager*)mManager)->getSaveParam();
}

TBathtubPeachManager::TBathtubPeachManager(const char* name)
    : TEnemyManager(name)
{
}

TSpineEnemy* TBathtubPeachManager::createEnemyInstance() { return nullptr; }

void TBathtubPeachManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "ahiru_peach.bmd",
		  J3DMLF_MaterialPEFull | J3DMLF_MaterialTexGenFull
		      | J3DMLF_UseUniqueMaterials | (4 << J3DMLF_TevStageNumShift),
		  0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

void TBathtubPeachManager::load(JSUMemoryInputStream& stream)
{
	TEnemyManager::load(stream);
	unk38 = new TBathtubPeachParams("/enemy/bathtubpeach.prm");
}
