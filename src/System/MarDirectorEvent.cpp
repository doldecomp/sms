#include <System/MarDirector.hpp>
#include <System/TalkCursor.hpp>
#include <System/MarioGamePad.hpp>
#include <System/FlagManager.hpp>
#include <System/Application.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MoveBG/Item.hpp>
#include <NPC/NpcBase.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/Mario.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// Defined in cameragc.cpp, which owns them in the map.
extern const char* cCameraBckNameShineGetInside;
extern const char* cCameraBckNameShineGetOutside;

void TMarDirector::entryNPC(TBaseNPC* npc) { unk88.push_back(npc); }

void isNearMapObj(const JDrama::TActor&, const char*, f32) { }

void is3BarrelNear(const TBaseNPC&) { }

void TMarDirector::getTalkMsgID(TBaseNPC*) { }

void TMarDirector::updateFlag(TBaseNPC*, u32, u32) { }

// TODO: every instruction and slot exact; the frame is 0x50 against retail's
// 0x58. The getStatus()/getPosition() accessors (+8 and +0x18) put `marioPos`
// on retail's 0x44, so retail has 8 more bytes *above* it, not in the low
// region. SMS_GetMarioPos() reaches 0x58 but only by adding 4 of low region
// (marioPos 0x48). Inert (l5): marioPos declared before bestDist, copy-ctor
// or assignment spellings, `it` at function scope, `it++`, a named `npc` or
// `dist` split, raw mPosition at one or two sites.
TBaseNPC* TMarDirector::findNearestTalkNPC()
{
	TBaseNPC* result = nullptr;
	if (gpMarioOriginal->getStatus() == MARIO_STATUS_WAIT) {
		f32 bestDist                   = 5000000.0f;
		JGeometry::TVec3<f32> marioPos = *gpMarioPos;
		JGadget::TVector_pointer<TBaseNPC*>::iterator it;

		for (it = unk88.begin(); it != unk88.end(); ++it) {
			TBaseNPC* npc = *it;
			if (npc->checkLiveFlag(LIVE_FLAG_UNK100000)
			    || !npc->checkLiveFlag(LIVE_FLAG_UNK20000))
				continue;

			f32 dist = (npc->getPosition().x - marioPos.x)
			               * (npc->getPosition().x - marioPos.x)
			           + (npc->getPosition().y - marioPos.y)
			                 * (npc->getPosition().y - marioPos.y)
			           + (npc->getPosition().z - marioPos.z)
			                 * (npc->getPosition().z - marioPos.z);
			if (dist < bestDist) {
				bestDist = dist;
				result   = npc;
			}
		}
	}
	return result;
}

TBaseNPC* TMarDirector::findNearestTakeNPC()
{
	TBaseNPC* result = nullptr;
	JGadget::TVector_pointer<TBaseNPC*>::iterator it;

	for (it = unk88.begin(); it != unk88.end(); ++it) {
		TBaseNPC* npc = *it;

		if (npc->isNowCanTaken() && gpMarioOriginal->isTakeSituation(npc))
			result = npc;
	}
	return result;
}

// Binders over the player, the pad and the talk cursor: with SMSGetCamera()
// at the L-button test they are retail's 0x30 of pool in movement_game (one
// of fourteen equally sized lever combinations; weakly evidenced).
static inline TMario* MDEMario() { TMario* m = gpMarioOriginal; return m; }
static inline TMarioGamePad* MDEPad(TMarDirector* d) { TMarioGamePad* p = d->unk18[0]; return p; }
static inline TTalkCursor* MDECursor(TMarDirector* d) { TTalkCursor* c = d->unk84; return c; }

void TMarDirector::movement_game()
{
	MDECursor(this)->associateNPC(nullptr);

	switch (unk124) {
	case 0:
		unk18[0]->offFlag(0x4);
		if (MDEMario()->isHolding())
			return;
		if (SMSGetCamera()->isLButtonCamera())
			return;

		if (!gpCamera->isDemoCamera()) {
			if (TBaseNPC* takeNpc = findNearestTakeNPC()) {
				MDECursor(this)->associateNPC(takeNpc);
			} else {
				TBaseNPC* talkNpc = findNearestTalkNPC();
				if (talkNpc != nullptr) {
					unkA0 = talkNpc;
					unk84->associateNPC(talkNpc);
					MDEPad(this)->onFlag(4);
					unk128 |= 0x1;
					if ((unk128 & 2)
					    && (unk18[0]->mEnabledFrameMeaning & 0x800))
						unk126 = 1;
				}
			}
		}
		break;
	}
}

// Binding level over the sound singleton, worth +8 of low region per site.
static inline MSound* MarDirectorEventGetMSound()
{
	MSound* sound = SMSGetMSound();
	return sound;
}

void TMarDirector::fireGetBlueCoin(TCoin* coin)
{
	if (!coin)
		return;

	TFlagManager::smInstance->setBlueCoinFlag(gpApplication.mCurrArea.unk0,
	                                          coin->getEventId());
	unk4C |= 0x200;
	unk261 = 1;
	MarDirectorEventGetMSound()->startSoundActor(
	    MSD_SE_SY_BLUE_COIN_GET, &coin->mPosition, 0, nullptr, 0, 4);
}

void TMarDirector::fireGetNozzle(TItemNozzle* nozzle)
{
	if (!nozzle)
		return;

	u8 area = gpApplication.mCurrArea.getStage();
	if (nozzle->isActorType(0x20000022)
	    && !TFlagManager::smInstance->getNozzleRight(area, 0)) {
		TFlagManager::smInstance->setNozzleRight(area, 0);
		unk4C |= 0x200;
		unk261 = 3;
	} else if (nozzle->isActorType(0x2000002A)
	           && !TFlagManager::smInstance->getNozzleRight(area, 1)) {
		TFlagManager::smInstance->setNozzleRight(area, 1);
		unk4C |= 0x200;
		unk261 = 4;
	}
}

void TMarDirector::fireGetStar(TShine* shine)
{
	unk25C = shine;
	unk4C |= 1;
	JGeometry::TVec3<f32>& v = shine->mInitialRotation;
	// The polarity is read off the branch: retail's `bne` leaves the Outside
	// name in the fallthrough arm, so the test is spelled `!unk190` and a
	// non-zero unk190 still selects Inside.
	// The default-constructed flag (its ctor's defaulted argument is one more
	// inline level) puts the temporary at the top of the frame as retail's.
	fireStartDemoCamera(!shine->unk190 ? cCameraBckNameShineGetOutside
	                                   : cCameraBckNameShineGetInside,
	                    &gpMarioOriginal->mPosition, -1, v.y, false, nullptr, 0,
	                    nullptr, JDrama::TFlagT<u16>());
}

// TODO: 99.8%, instruction-exact; the only residue is a 16-byte frame gap
// (0x28 vs 0x18), so retail has two 4-byte temporaries we are missing.
static inline TFlagManager* TMarDirectorGetFlagManager()
{
	TFlagManager* flagManager = TFlagManager::smInstance;
	return flagManager;
}

void TMarDirector::fireRideYoshi(TYoshi* yoshi)
{
	if (!yoshi)
		return;

	if (gpApplication.mCurrArea.unk0 != 1)
		return;

	if (TMarDirectorGetFlagManager()->getBool(0x1038F))
		return;

	TMarDirectorGetFlagManager()->setBool(true, 0x1038F);
	unk4C |= 0x200;
	unk261 = 5;
}

void TMarDirector::fireDefeatEnemy(TSpineEnemy*) { }

void TMarDirector::fireDemoMovie(u32, TLiveActor*) { }

void TMarDirector::movement()
{
	switch (mState) {
	case STATE_UNK4:
		movement_game();
		break;
	}
}

// TODO: 99.7%, instruction-exact since TFlagT::operator= returns void; the
// frame is 0x40 against retail's 0x50 (`next` 0xc low, the four flag
// temporaries 4 low). MDEApp().setMovie(6) is +8 and an identity fork over
// param_2 the other +8 (100% together, refused as a fabricated level);
// `curr` declared inside the else arm lands `next` on 0x38 but renumbers
// r29/r30. Inert or worse: `curr` or a split `cur` declared at the top, an
// int/u16 copy of param_1, a conversion helper (by reference, pointer or
// value), direct `mNextArea = next`/`set`, a pointer-returning application
// accessor, by-value setNextArea/operator=, const-reference set() flag.
// A reference binder (`TApplication& app = gpApplication; return app;`) at
// the setNextArea site reaches 0x50 at 99.9 but puts the extra 8 bytes
// among the flag temporaries (0x28/0x34) rather than retail's hole at
// 0x30-0x37 under `next`; refused as an alias (decideNextStage: same
// frame, same misplacement, no gain).
// Reference-returning accessor: retail folds the TGameSequence stores onto
// the &gpApplication base (0x12/0x13/0x14) instead of binding &mNextArea.
static inline TApplication& MDEApp() { return gpApplication; }

void TMarDirector::setNextStage(u16 param_1, JDrama::TActor* param_2)
{
	if (checkUnk4CFlag(0x2))
		return;

	TGameSequence next;
	if (param_1 >= 0x100) {
		next.unk0 = (param_1 >> 8) - 1;
		next.unk1 = param_1 & 0xff;
	} else {
		next.unk0 = param_1;
		next.unk1 = 0xFF;
	}

	MDEApp().setNextArea(next);

	const TGameSequence& curr = gpApplication.mCurrArea;
	if (param_2 != nullptr) {
		onUnk4CFlag(0x4);
		unk250 = param_2;
	} else {
		u8 cur = curr.getStage();
		if ((cur == 1 && next.getStage() == 5)
		    || (cur == 1 && next.getStage() == 6)
		    || (cur == 1 && next.getStage() == 8)) {
			onUnk4CFlag(0x8);
		} else {
			onUnk4CFlag(0x2);
		}
	}

	switch (next.getStage()) {
	case 0x37:
		onUnk4CFlag(0x100);
		gpApplication.setMovie(6);
		break;
	}
}

void TMarDirector::fireStageEvent(TMapObjBase*) { }

void TMarDirector::fireStartDemoCamera(const char* param_1,
                                       const JGeometry::TVec3<f32>* param_2,
                                       s32 param_3, f32 param_4, bool param_5,
                                       s32 (*param_6)(u32, u32), u32 param_7,
                                       JDrama::TActor* param_8,
                                       JDrama::TFlagT<u16> param_9)
{
	// The named difference is the 15th statement: retail calls this body
	// out of line from fireGetStar where 14 statements would expand it.
	u8 diff = unk24C - unk24D;
	if ((diff & 7) >= 7)
		return;

	unk4C |= 0x40;
	unk12C[unk24C].unk0  = param_1;
	unk12C[unk24C].unk4  = param_2;
	unk12C[unk24C].unk8  = param_3;
	unk12C[unk24C].unkC  = param_4;
	unk12C[unk24C].unk10 = param_5;
	unk12C[unk24C].unk14 = param_6;
	unk12C[unk24C].unk18 = param_7;
	unk12C[unk24C].unk1C = param_8;
	unk12C[unk24C].unk20 = param_9;

	unk24C += 1;
	unk24C &= 7;
}

void TMarDirector::fireEndDemoCamera() { unk4C |= 0x80; }

// The 0x28 of pool is the setter level through MDEApp() at the seven movie
// stores plus the flag-manager binder at the first setBool.
void TMarDirector::fireStreamingMovie(u8 param_1)
{
	switch (param_1) {
	case 0:
		if (!checkUnk4CFlag(0x100)) {
			onUnk4CFlag(0x100);
			setNextStage(0x1, nullptr);
			TMarDirectorGetFlagManager()->setBool(true, 0x10389);
			TFlagManager::smInstance->setBool(true, 0x30004);
			MDEApp().setMovie(param_1);
		}
		break;

	case 10:
		if (!checkUnk4CFlag(0x100)) {
			onUnk4CFlag(0x100);
			setNextStage(0x3B, nullptr);
			MDEApp().setMovie(param_1);
		}
		break;

	case 7:
		if (!checkUnk4CFlag(0x100)) {
			onUnk4CFlag(0x100);
			setNextStage(0xE06, nullptr);
			MDEApp().setMovie(param_1);
		}
		break;

	case 8:
		if (!checkUnk4CFlag(0x100)) {
			onUnk4CFlag(0x100);
			setNextStage(0xE07, nullptr);
			MDEApp().setMovie(param_1);
		}
		break;

	case 11:
		if (!checkUnk4CFlag(0x100)) {
			onUnk4CFlag(0x100);
			setNextStage(0x3C, nullptr);
			MDEApp().setMovie(param_1);
		}
		break;

	case 2:
		if (!checkUnk4CFlag(0x100)) {
			onUnk4CFlag(0x100);
			setNextStage(0x101, nullptr);
			MDEApp().setMovie(param_1);
		}
		break;

	// The jump table spans 0..12, so 12 is a real case label sharing the
	// default handling; without it MWCC emits a 12-entry table.
	case 12:
	default:
		if (!checkUnk4CFlag(0x100)) {
			onUnk4CFlag(0x100);
			setNextStage(0xF, nullptr);
			MDEApp().setMovie(param_1);
		}
		break;
	}
}
