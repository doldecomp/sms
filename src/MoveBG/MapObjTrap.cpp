#include <MoveBG/MapObjTrap.hpp>
#include <Map/MapData.hpp>
#include <M3DUtil/MActor.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <Player/MarioAccess.hpp>
#include <System/Application.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/Particles.hpp>
#include <Strategic/Strategy.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

static void dummy(Vec* v)
{
	*v = (Vec) { 0.0f, 0.0f, 0.0f };
	*v = (Vec) { 1.0f, 1.0f, 1.0f };
}

TLampTrapSpikeHit::TLampTrapSpikeHit(TLampTrapSpike* trap, const char* name)
    : THitActor(name)
    , unk68(trap)
{
	initHitActor(0x4000001E, 3, -0x80000000, 500.0f, 300.0f, 500.0f, 300.0f);
	JDrama::TNameRefGen::search<TIdxGroupObj>("アイテムグループ")
	    ->getChildren()
	    .push_back(this);
	offHitFlag(HIT_FLAG_NO_COLLISION);
}

BOOL TLampTrapSpikeHit::receiveMessage(THitActor* sender, u32 message)
{
	return unk68->receiveMessage(sender, message);
}

void TLampTrapSpikeHit::perform(u32 cue, JDrama::TGraphics* graphics)
{
	THitActor::perform(cue, graphics);
	if (cue & CUE_MOVE) {
		mPosition = unk68->getPosition();
		mPosition.y += 2300.0f;
		int state = unk68->getSpikeState();
		if (state == 2 || state == 0 || state == 1) {
			for (int i = 0; i < getColNum(); ++i)
				if (getCollision(i)->isActorType(-0x7fffffff))
					SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
		}
	}
}

TLampTrapSpike::TLampTrapSpike(const char* name)
    : TMapObjBase(name)
    , unk138(3)
    , unk13C(0)
    , unk140(nullptr)
{
}

void TLampTrapSpike::initMapObj()
{
	TMapObjBase::initMapObj();
	onHitFlag(HIT_FLAG_NO_COLLISION);
}

// TODO (closure batch 152): 99.9%, instruction-exact, frame exact (0x78).
// The whole residue is the known-open JGadget iterator temp-pool word: the
// three `TList::iterator` temporaries around `TList_pointer_void::insert` sit
// at 0x38/0x3c/0x40 in retail and 0x3c/0x40/0x44 here, i.e. our pool base is
// one word high while the frame total is right. TLampTrapIron::loadAfter is
// byte-for-byte the same residue, so both close together or not at all; see
// frame-gaps.md's "batch 133" entry (the grouping is not the conversion
// lever).
void TLampTrapSpike::loadAfter()
{
	TMapObjBase::loadAfter();
	unk140 = new TLampTrapSpikeHit(this, "トゲあたり");
}

BOOL TLampTrapSpike::receiveMessage(THitActor* sender, u32 message)
{
	TMapObjBase::receiveMessage(sender, message);
}

// TODO (closure batch 152): 99.5%, instruction-exact, frame exact (0x60). The
// residue is one callee-saved GPR too many: retail allocates r27-r31 and
// reuses r27 for two disjoint pointer live ranges (the `addi r27, r3, 0` at
// the state check and the `mr r27, r3` at the MActor keeper), while we spend
// r26 and r28 on them and keep the `.rodata` base in r28 as well, so the
// whole set is rotated one step and the extra register eats four bytes of the
// low pool. Filed under frame-gaps.md's known-open zero-frame rotations.
void TLampTrapSpike::control()
{
	BOOL bVar1 = false;
	int thing  = unk138;

	switch (thing) {
	case 0: {
		J3DFrameCtrl* ctrl = getMActor()->getFrameCtrl(ANM_TYPE_BCK);
		if (unk13C == 0) {
			getMActor()->setBck("lamptrapspike_up");
			J3DFrameCtrl* ctrl = getMActor()->getFrameCtrl(ANM_TYPE_BCK);
			ctrl->setFrame(6.0f);
			ctrl->setRate(SMSGetAnmFrameRate());
		}
		if (getMActor()->curAnmEndsNext(ANM_TYPE_BCK, nullptr)) {
			unk13C = 0;
			unk138 = 2;
		}
		if (ctrl->getFrame() > ctrl->getEnd() * 0.5f)
			bVar1 = true;
	} break;

	case 1: {
		J3DFrameCtrl* ctrl = getMActor()->getFrameCtrl(ANM_TYPE_BCK);
		if (unk13C == 0) {
			getMActor()->setBck("lamptrapspike_down");
			J3DFrameCtrl* ctrl = getMActor()->getFrameCtrl(ANM_TYPE_BCK);
			ctrl->setFrame(0.0f);
			ctrl->setRate(SMSGetAnmFrameRate() * 0.8f);
		}
		if (getMActor()->curAnmEndsNext(ANM_TYPE_BCK, nullptr)) {
			unk13C = 0;
			unk138 = 3;
		}
		if (ctrl->getFrame() > ctrl->getEnd() * 0.5f)
			bVar1 = true;
	} break;

	case 2:
		if (unk13C == 0) {
			getMActor()->setBck("lamptrapspike_up");
			J3DFrameCtrl* ctrl = getMActor()->getFrameCtrl(ANM_TYPE_BCK);
			ctrl->setFrame(ctrl->getEnd());
			ctrl->setRate(0.0f);
			SMSGetMSound()->startSoundActor(MSD_SE_OBJ_MVING_FENCT_SET,
			                                &mPosition);
		}
		if (unk13C >= 360) {
			unk13C = 0;
			unk138 = 1;
		}
		bVar1 = true;
		break;

	case 3:
		if (unk13C == 0) {
			getMActor()->setBck("lamptrapspike_down");
			if (J3DFrameCtrl* ctrl = getMActor()->getFrameCtrl(ANM_TYPE_BCK)) {
				ctrl->setFrame(ctrl->getEnd());
				ctrl->setRate(0.0f);
			}
		}
		if (unk13C >= 120) {
			unk13C = 0;
			unk138 = 4;
		}
		bVar1 = false;
		break;

	case 4: {
		J3DFrameCtrl* ctrl = getMActor()->getFrameCtrl(ANM_TYPE_BCK);
		if (unk13C == 0) {
			getMActor()->setBck("lamptrapspike_up");
			J3DFrameCtrl* ctrl = getMActor()->getFrameCtrl(ANM_TYPE_BCK);
			ctrl->setFrame(0.0f);
			ctrl->setRate(SMSGetAnmFrameRate() * 0.1f);
		}
		if (ctrl->checkPass(6.0f))
			ctrl->setRate(0.0f);
		if (unk13C >= 240) {
			unk13C = 0;
			unk138 = 0;
		}
	} break;

	default:
	case 5:
		J3DFrameCtrl* ctrl = getMActor()->getFrameCtrl(ANM_TYPE_BCK);
		if (unk13C == 0) {
			getMActor()->setBck("lamptrapspike_up");
			J3DFrameCtrl* ctrl = getMActor()->getFrameCtrl(ANM_TYPE_BCK);
			ctrl->setFrame(6.0f);
			ctrl->setRate(-SMSGetAnmFrameRate());
		}
		if (ctrl->checkPass(0.0f)) {
			unk13C = 0;
			unk138 = 0;
		}
		break;
	}

	if (unk138 == thing) {
		++unk13C;
		if (unk13C == 0)
			unk13C = 1;
	}

	TMapObjBase::control();
	const TBGCheckData* plane = SMS_GetMarioGrPlane();
	if (bVar1 && plane && plane->getActor() == this
	    && SMS_IsMarioTouchGround4cm())
		SMS_SendMessageToMario(this, 0xA);
}

void TLampTrapSpike::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TMapObjBase::perform(cue, graphics);
	unk140->perform(cue, graphics);
}

TLampTrapIronHit::TLampTrapIronHit(TLampTrapIron* trap, const char* name)
    : THitActor(name)
    , unk68(trap)
{
	initHitActor(0x4000001D, 3, -0x80000000, 500.0f, 300.0f, 500.0f, 300.0f);
	JDrama::TNameRefGen::search<TIdxGroupObj>("アイテムグループ")
	    ->getChildren()
	    .push_back(this);
	offHitFlag(HIT_FLAG_NO_COLLISION);
}

BOOL TLampTrapIronHit::receiveMessage(THitActor* sender, u32 message)
{
	return unk68->receiveMessage(sender, message);
}

void TLampTrapIronHit::perform(u32 cue, JDrama::TGraphics* graphics)
{
	THitActor::perform(cue, graphics);
	if (cue & CUE_MOVE) {
		mPosition = unk68->mPosition;
		mPosition.y += 2300.0f;
		if (unk68->unk13C > 0)
			for (int i = 0; i < getColNum(); ++i)
				if (getCollision(i)->isActorType(-0x7fffffff))
					SMS_SendMessageToMario(this, 0xA);
	}
}

u32 TLampTrapIron::mHitPointMax  = 60;
u32 TLampTrapIron::mFireTimerMax = 1200;

TLampTrapIron::TLampTrapIron(const char* name)
    : TMapObjBase(name)
    , unk138(nullptr)
    , unk13C(0)
    , unk140(0)
{
}

void TLampTrapIron::initMapObj()
{
	TMapObjBase::initMapObj();
	onHitFlag(HIT_FLAG_NO_COLLISION);
	unk140 = 0;
	unk13C = mHitPointMax;
}

// TODO (closure batch 152): the same JGadget iterator temp-pool word as
// TLampTrapSpike::loadAfter above; see the note there.
void TLampTrapIron::loadAfter()
{
	TMapObjBase::loadAfter();
	unk138 = new TLampTrapIronHit(this, "鉄板あたり");
}

// TODO (closure batch 152): 99.9%, instruction-exact; frame 0x28 vs our 0x20,
// so eight bytes of dead low region. The only inlined callees are
// `THitActor::isActorType` (bool), `MActor::getBaseTRMtx` (pointer, already
// worth +4 here) and the static `mFireTimerMax` read -- `TLiveActor::getModel`
// is a real `bl`, so a level over it is worth nothing. No honest +8 carrier
// found: the natural candidates are a `gpMarioParticleManager` fork (+4 by the
// global rule, and Yoshi.cpp's `YoshiGetMarioParticleManager` is the precedent
// for the shape) and an accessor pair on `unk13C`/`unk140`, neither of which
// reaches 8 on its own.
BOOL TLampTrapIron::receiveMessage(THitActor* sender, u32 message)
{
	if (sender->isActorType(0x1000001)) {
		if (unk13C > 0) {
			--unk13C;
			if (unk13C == 0) {
				unk140 = mFireTimerMax;
				gpMarioParticleManager->emitAndBindToMtxPtr(
				    100, getModel()->getBaseTRMtx(), 0, this);
			}
		}
		return true;
	}

	return TMapObjBase::receiveMessage(sender, message);
}

void TLampTrapIron::control()
{
	TMapObjBase::control();
	if (unk140 <= 0) {
		const TBGCheckData* plane = SMS_GetMarioGrPlane();
		if (plane && plane->getActor() == this && SMS_IsMarioTouchGround4cm())
			SMS_SendMessageToMario(this, 10);
	} else {
		--unk140;
		if (unk140 == 0)
			unk13C = mHitPointMax;
	}
}

void TLampTrapIron::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TMapObjBase::perform(cue, graphics);
	unk138->perform(cue, graphics);
	if ((cue & CUE_CALC_ANIM) && unk13C > 0) {
		gpMarioParticleManager->emit(PARTICLE_MS_TEPPANFIRE_B, &mPosition, 3,
		                             this);
		gpMarioParticleManager->emit(PARTICLE_MS_TEPPANFIRE_A, &mPosition, 1,
		                             this);
	}
}

static void dummy2(f32* f) { *f = 0.0f; }
