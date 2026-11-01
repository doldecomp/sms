#include <Enemy/BathtubPeach.hpp>
#include <MoveBG/MapObjCorona.hpp>
#include <Player/MarioAccess.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/MActorAnm.hpp>
#include <Map/MapCollisionManager.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/ObjModel.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JGeometry.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <JSystem/JMath.hpp>
#include <math.h>
#include <macros.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/MActorAnm.hpp>

// Peach's two nerves keep their theNerve() inline in the class (the map has
// weak nerve$localstatic0$theNerve__... objects for them), and their
// execute()s are weak too, so the classes are defined right here.

// Peach losing her balance while the bathtub shakes.
class TNervePeachStagger : public TNerveBase<TLiveActor> {
public:
	virtual BOOL execute(TSpineBase<TLiveActor>* spine) const
	{
		TBathtubPeach* peach = (TBathtubPeach*)spine->getBody();
		peach->changeAnm(0, 0, 2.0f);
		if (peach->getMActor()->curAnmEndsNext(ANM_TYPE_BCK, nullptr))
			return true;
		return false;
	}
	static const TNervePeachStagger& theNerve()
	{
		static TNervePeachStagger nerve;
		return nerve;
	}
};

// Swim around the bathtub so as to keep away from Mario, stopping when the
// tub rocks.
// TODO: nonmatching. The target keeps every fmodf, plus getSaveParam and
// TVec2::dot inside goTo, out of line, so its inlining runs deeper than this
// version's; goTo is also 0x14 bytes short of the map's size.
class TNervePeachEscape : public TNerveBase<TLiveActor> {
public:
	virtual BOOL execute(TSpineBase<TLiveActor>* spine) const
	{
		TBathtubPeach* peach = (TBathtubPeach*)spine->getBody();
		TBathtub* bathtub = (TBathtub*)JDrama::TNameRefGen::search("バスタブ");
		if (bathtub->unk29A)
			return false;

		if (!(spine->getTime() & 4)) {
			if (bathtub->getBathtubData().unk64)
				spine->pushNerve(&TNervePeachStagger::theNerve());
			return false;
		}

		peach->changeAnm(1, 1, 2.0f);

		// Aim for the point on the circle around the tub that is `angle` degrees
		// away from Mario, on whichever side Peach already is.
		MtxPtr tub = *bathtub->getRootJointMtx();
		f32 centerX = tub[0][3];
		f32 centerZ = tub[2][3];

		JGeometry::TVec3<f32> marioPos = SMS_GetMarioPos();
		f32 marioDir = (360.0f / 65536.0f)
		               * matan(marioPos.z - centerZ, marioPos.x - centerX);
		f32 peachDir
		    = (360.0f / 65536.0f)
		      * matan(peach->mPosition.z - centerZ, peach->mPosition.x - centerX);

		f32 dir;
		if (MsWrapFmodf(peachDir - marioDir, -180.0f, 180.0f) < 0.0f)
			dir = MsWrapFmodf(marioDir - peach->getParam()->angle.get(), -180.0f,
			                  180.0f);
		else
			dir = MsWrapFmodf(marioDir + peach->getParam()->angle.get(), -180.0f,
			                  180.0f);

		f32 radius = peach->getParam()->radius.get();
		peach->goTo(JGeometry::TVec3<f32>(centerX + radius * MsSin(dir), 0.0f,
		                                  centerZ + radius * MsCos(dir)));
		peach->faceTo(SMS_GetMarioPos(), peach->getParam()->turnSpeed2.get());
		return false;
	}
	static const TNervePeachEscape& theNerve()
	{
		static TNervePeachEscape nerve;
		return nerve;
	}
};

static const char* bathtubpeach_bastable[] = {
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	"/scene/bathtubpeach/bas/peach_wait.bas",
	nullptr,
};

TBathtubPeach::TBathtubPeach(const char* name)
    : TSpineEnemy(name)
{
	onLiveFlag(LIVE_FLAG_AIRBORNE);
	offLiveFlag(LIVE_FLAG_ENABLE_CLIPPING);
	offLiveFlag(LIVE_FLAG_UNK10);
}

// Move straight towards @p pos, by at most `speed` units per frame.
// TODO: incorrect size (0xB0, the map says 0xC4)
void TBathtubPeach::goTo(const JGeometry::TVec3<f32>& pos)
{
	JGeometry::TVec2<f32> delta(pos.x - mPosition.x, pos.z - mPosition.z);
	if (delta.x * delta.x + delta.y * delta.y
	    > getParam()->speed.get() * getParam()->speed.get())
		delta.setLength(getParam()->speed.get());
	mPosition.x += delta.x;
	mPosition.z += delta.y;
}

// Turn towards @p pos by at most @p speed degrees.
// TODO: incorrect size (0x128, the map says 0x12C)
void TBathtubPeach::faceTo(const JGeometry::TVec3<f32>& pos, f32 speed)
{
	f32 dx = pos.x - mPosition.x;
	f32 dz = pos.z - mPosition.z;
	if (dx * dx + dz * dz <= JGeometry::TUtil<f32>::epsilon())
		return;

	f32 target = (360.0f / 65536.0f) * matan(dz, dx) - 90.0f;
	f32 diff   = MsWrapFmodf(target - mRotation.y, -180.0f, 180.0f);
	if (diff < -speed)
		mRotation.y = MsWrapFmodf(mRotation.y - speed, -180.0f, 180.0f);
	else if (diff > speed)
		mRotation.y = MsWrapFmodf(mRotation.y + speed, -180.0f, 180.0f);
	else
		mRotation.y = target;
}

void TBathtubPeach::changeAnm(int bck, int btp, f32 speed)
{
	if (!getMActor()->checkCurBckFromIndex(bck)) {
		getMActor()->setBckFromIndex(bck);
		setAnmSound(getBas(bck));
	}

	if (btp != getMActor()->getCurAnmIdx(ANM_TYPE_BTP))
		getMActor()->setBtpFromIndex(btp);

	J3DFrameCtrl* ctrl = getMActor()->getFrameCtrl(ANM_TYPE_BCK);
	f32 rate           = 0.5f * (speed * SMSGetAnmFrameRate());
	ctrl->setRate(rate);
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
	// TODO: why the scene position is scaled down
	mPosition.scale(0.21f);
	mBathtubBinder.init(50.0f, 50.0f, 50.0f, 50.0f, 0.0f);
	unk130 = 0;
	mScaling.set(1.5f, 1.5f, 1.5f);
	mBinder = &mBathtubBinder;
	TSpineEnemy::reset();
	changeAnm(1, 1, 2.0f);
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

// During the bathtub's demo, Peach is carried along by its joint.
void TBathtubPeach::calcRootMatrix()
{
	TBathtub* bathtub = (TBathtub*)JDrama::TNameRefGen::search("バスタブ");
	if (bathtub != nullptr && bathtub->unk29A) {
		MTXCopy(bathtub->getPeachMtxInDemo(), getModel()->getBaseTRMtx());
		return;
	}
	TLiveActor::calcRootMatrix();
}

const TBathtubPeachParams* TBathtubPeach::getParam() const
{
	return (const TBathtubPeachParams*)((TEnemyManager*)mManager)
	    ->getSaveParam();
}

TBathtubPeachManager::TBathtubPeachManager(const char* name)
    : TEnemyManager(name)
{
}

TSpineEnemy* TBathtubPeachManager::createEnemyInstance() { return nullptr; }

void TBathtubPeachManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "ahiru_peach.bmd", 0x14240000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

void TBathtubPeachManager::load(JSUMemoryInputStream& stream)
{
	TEnemyManager::load(stream);
	unk38 = new TBathtubPeachParams("/enemy/bathtubpeach.prm");
}
