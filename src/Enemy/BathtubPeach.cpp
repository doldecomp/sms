#include <Enemy/BathtubPeach.hpp>
#include <M3DUtil/MActor.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <Map/Map.hpp>
// Nothing here names a collision type, but TMapCollisionBase::setUpTrans emits
// the {0,0,0} and {1,1,1} vector literals that sit between the
// InfectiousStrings block and this unit's own strings in .rodata (map @2852 and
// @2854). Without it every string offset in the unit is 0x18 too low.
#include <Map/MapCollisionManager.hpp>
#include <MoveBG/MapObjCorona.hpp>
#include <Player/MarioAccess.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Spine.hpp>
#include <JSystem/JMath.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// 23 slots, of which only index 21 names a .bas -- the rest of ahiru_peach's
// .bck animations are silent. The two the nerves actually select (0 for the
// stagger, 1 for the paddle) are among them, so changeAnm() ends up calling
// setAnmSound(nullptr) both times. The map gives the table's size (0x5c) but
// no names for the silent slots.
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

// Both nerve classes are TU-local and define every member in the class body:
// the map lists their execute() and their compiler-generated destructor as
// weak, and theNerve()'s singleton shows up only as the
// nerve$localstatic0$/init$localstatic1$ pair, i.e. it was inlined away
// everywhere. That rules out DECLARE_NERVE/DEFINE_NERVE, whose theNerve() is a
// global symbol with a static called `instance`.

// Plays the "knocked about" animation once while the tub is tumbling, then
// hands control back to the escape nerve.
class TNervePeachStagger : public TNerveBase<TLiveActor> {
public:
	static const TNervePeachStagger& theNerve()
	{
		static TNervePeachStagger nerve;
		return nerve;
	}

	virtual BOOL execute(TSpineBase<TLiveActor>* spine) const
	{
		TBathtubPeach* peach = (TBathtubPeach*)spine->getBody();

		peach->changeAnm(0, 0, 0.5f);

		return peach->getMActor()->curAnmEndsNext(ANM_TYPE_BCK,
		                                          nullptr)
		           ? TRUE
		           : FALSE;
	}
};

// Fabricated, but the ROM's shape: every degree wrap in this TU computes
// `l + std::fmodf((r - l) + (t - l), r - l)` with l = -180, r = 180, and the
// two inline levels above std::fmodf are what make MWCC call the weak 0x5c
// copy the map records for this TU instead of expanding it. Same pair as
// koopajr.cpp's WrapDirectionF / WrapRadianF, in degrees.
static inline f32 WrapDirectionF(f32 t, f32 l, f32 r)
{
	return l + std::fmodf((r - l) + (t - l), r - l);
}

static inline f32 WrapDegreesF(f32 angle)
{
	return WrapDirectionF(angle, -180.0f, 180.0f);
}

// Paddles around the rim of the bathtub so as to stay `angle` degrees away
// from Mario, always facing him.
//
// TODO: 94.2%. The remaining instruction differences all come from one shared
// header, and they were measured rather than guessed:
// include/PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/math.h -- the ROM calls
// fmodf__3stdFff (0x5c, emitted weak from wireTrap.cpp) at all six sites in
// this function. Our std::fmodf is an `inline` wrapper around ::fmod, so every
// site expands to `bl fmod` plus an `frsp` and an extra `lfd` of the double
// 360.0. That accounts for the whole residual, including the float-register
// renumbering and the frame gap.
//
// The spelling of std::fmodf is not the blocker -- the header's TODO has the
// measured table. MWCC refuses a 0x5c body only from inline depth four down,
// and this nerve reaches it at depth two (nerve -> faceTo -> std::fmodf), so
// two inline wrappers are missing above it. The ROM computes
// `l + std::fmodf((r - l) + (t - l), r - l)` at each of the three sites below,
// so the pair is a wrap-into-[l,r) helper plus whatever faceTo called to get
// the [-180,180) range; neither is MathUtil.hpp's loop-based MsWrap<f>.
//
// The TVec2::setLength forwarders this nerve also needed are in place now (see
// JGVec2.hpp): that level of nesting is what puts TVec2::dot at inline depth
// five inside goTo() and makes MWCC emit the weak
// dot__Q29JGeometry8TVec2<f>CFRCQ29JGeometry8TVec2<f> the map lists for this
// TU, which in turn keeps `dir` on the stack the way the ROM has it.
class TNervePeachEscape : public TNerveBase<TLiveActor> {
public:
	static const TNervePeachEscape& theNerve()
	{
		static TNervePeachEscape nerve;
		return nerve;
	}

	virtual BOOL execute(TSpineBase<TLiveActor>* spine) const
	{
		TBathtubPeach* peach = (TBathtubPeach*)spine->getBody();
		// search2 rather than search<TBathtub>: the template's internal
		// static_cast leaves the result as a compiler temporary, so
		// later blocks re-read it out of r3, while the ROM reads the
		// bathtub through the local's own register (r28 here, r31 in
		// calcRootMatrix). The cast at the call site is also why
		// JDRNameRefGen.hpp doubts the template ever existed.
		TBathtub* bathtub
		    = (TBathtub*)JDrama::TNameRefGen::search2("バスタブ");

		if (bathtub->unk29A)
			return FALSE;

		if (!(spine->getTime() & 4)) {
			if (bathtub->getBathtubData().unk64)
				spine->pushNerve(&TNervePeachStagger::theNerve());
			return FALSE;
		}

		peach->changeAnm(1, 1, 0.5f);

		Mtx* tubMtx = bathtub->getRootJointMtx();
		f32 tubX    = (*tubMtx)[0][3];
		f32 tubZ    = (*tubMtx)[2][3];

		JGeometry::TVec3<f32> marioPos = *gpMarioPos;
		f32 marioAngle                 = (360.0f / 65536.0f)
		                 * matan(marioPos.z - tubZ, marioPos.x - tubX);
		f32 peachAngle = (360.0f / 65536.0f)
		                 * matan(peach->mPosition.z - tubZ,
		                         peach->mPosition.x - tubX);

		// These three sites sit one inline level higher than faceTo's, so
		// MWCC still expands std::fmodf here where the ROM calls it; the
		// helper pair is nevertheless the better spelling (93.9% against
		// 88.9% with the wrap written out).
		f32 goalAngle;
		if (WrapDegreesF(peachAngle - marioAngle) < 0.0f)
			goalAngle
			    = WrapDegreesF(marioAngle - peach->getParam()->angle.get());
		else
			goalAngle
			    = WrapDegreesF(marioAngle + peach->getParam()->angle.get());

		f32 radius = peach->getParam()->radius.get();

		peach->goTo(JGeometry::TVec3<f32>(
		    radius * JMASSin(DEG2SHORTANGLE(goalAngle)) + tubX, 0.0f,
		    radius * JMASCos(DEG2SHORTANGLE(goalAngle)) + tubZ));

		peach->faceTo(*gpMarioPos, peach->getParam()->turnSpeed2.get());

		return FALSE;
	}
};

TBathtubPeach::TBathtubPeach(const char* name)
    : TSpineEnemy(name)
{
	onLiveFlag(LIVE_FLAG_AIRBORNE);
	offLiveFlag(LIVE_FLAG_UNK100);
	offLiveFlag(LIVE_FLAG_UNK10);
}

// Steps at most `speed` units along the horizontal line towards `goal`. UNUSED
// in the map (0xc4) because the escape nerve is its only caller and inlines it;
// that one inline level is also what pushes TVec2::dot out of line there, which
// is how we know the nerve calls goTo rather than spelling the body out.
//
// The ROM reads speed twice: inlined (lwz 0x38 / lfs 0xe0) for the comparison
// and through a real `bl TEnemyManager::getSaveParam()` for the setLength
// argument. Routing only the second one through getSpeed() -- one inline level
// above getParam() -- reproduces both reads and the weak
// getSaveParam__13TEnemyManagerCFv the map lists as a duplicate in this TU
// (93.6 -> 94.2 on the nerve). getSpeed() itself leaves no symbol, as the map
// requires. A getSpeed() on TBathtubPeachParams instead is worth nothing: the
// level has to sit above getParam(), not below it.
//
// TODO: 0xc0 here against the map's 0xc4. In the standalone copy goTo is the
// emitted function, so getSaveParam lands two levels shallower and MWCC merges
// the two reads again; the missing 4 bytes are that second read.
void TBathtubPeach::goTo(const JGeometry::TVec3<f32>& goal)
{
	JGeometry::TVec2<f32> dir(goal.x - mPosition.x, goal.z - mPosition.z);

	f32 speed = getParam()->speed.get();
	if (dir.squared() >= speed * speed)
		dir.setLength(getSpeed());

	mPosition.x += dir.x;
	mPosition.z += dir.y;
}

// Turns mRotation.y towards `target` by at most `turn_speed` degrees. UNUSED,
// 0x12c in the map, and inlined into the escape nerve.
//
// The product is named because the ROM keeps `fmuls` and `fsubs` apart here;
// writing (360.0f / 65536.0f) * matan(dz, dx) - 90.0f contracts them into one
// fmsubs (see docs/catalog/codegen-tells.md, fp_contract).
//
// TODO: 0x144 here against the map's 0x12c. The 0x18 of surplus is exactly the
// six instructions our three inlined std::fmodf expansions add (an `frsp` plus
// an extra `lfd` of the double 360.0 each); see the note on the escape nerve.
void TBathtubPeach::faceTo(const JGeometry::TVec3<f32>& target, f32 turn_speed)
{
	f32 dz = target.z - mPosition.z;
	f32 dx = target.x - mPosition.x;

	if (dx * dx + dz * dz <= JGeometry::TUtil<f32>::epsilon())
		return;

	f32 angle = (360.0f / 65536.0f) * matan(dz, dx);
	f32 goal  = angle - 90.0f;
	f32 diff  = WrapDegreesF(goal - mRotation.y);

	if (diff < -turn_speed)
		mRotation.y = WrapDegreesF(mRotation.y - turn_speed);
	else if (diff > turn_speed)
		mRotation.y = WrapDegreesF(mRotation.y + turn_speed);
	else
		mRotation.y = goal;
}

// Switches the model onto bck/btp `bck`/`btp` at `rate` times the game's
// animation speed, re-pointing the animation sound at the new .bas. UNUSED,
// 0xe0 in the map, inlined into reset() and both nerves.
void TBathtubPeach::changeAnm(int bck, int btp, f32 rate)
{
	if (!getMActor()->checkCurBckFromIndex(bck)) {
		getMActor()->setBckFromIndex(bck);

		const char** table = getBasNameTable();
		setAnmSound(table == nullptr ? nullptr : table[bck]);
	}

	if (getMActor()->getCurAnmIdx(ANM_TYPE_BTP) != btp)
		getMActor()->setBtpFromIndex(btp);

	J3DFrameCtrl* frameCtrl = getMActor()->getFrameCtrl(ANM_TYPE_BCK);
	frameCtrl->setRate(rate * (2.0f * SMSGetAnmFrameRate()));
}

const char** TBathtubPeach::getBasNameTable() const
{
	return bathtubpeach_bastable;
}

void TBathtubPeach::init(TLiveManager* live_manager)
{
	TSpineEnemy::init(live_manager);

	mSpine->initWith(&TNervePeachEscape::theNerve());

	initAnmSound();
	reset();

	mScaling.setAll(2.0f);
}

void TBathtubPeach::reset()
{
	// TODO: this really does scale the placed position rather than the
	// scaling; the stage file must store her position in a larger unit than
	// the tub's own space.
	mPosition.scale(0.21f);

	mBathtubBinder.init(50.0f, 50.0f, 50.0f, 50.0f, 0.0f);

	unk130 = 0;
	mScaling.setAll(1.5f);
	mBinder = &mBathtubBinder;

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
	    = (TBathtub*)JDrama::TNameRefGen::search2("バスタブ");

	if (bathtub && bathtub->unk29A)
		MTXCopy(bathtub->getPeachMtxInDemo(), getModel()->getBaseTRMtx());
	else
		TLiveActor::calcRootMatrix();
}

// UNUSED, 0xc in the map.
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
