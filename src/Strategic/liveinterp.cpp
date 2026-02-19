#include <Strategic/LiveActor.hpp>
#include <Strategic/spcinterp.hpp>
#include <Strategic/Spine.hpp>
#include <Enemy/SmallEnemy.hpp>
#include <Enemy/WalkerEnemy.hpp>
#include <Enemy/NameKuri.hpp>
#include <Enemy/BossGesso.hpp>
#include <Enemy/BossPakkun.hpp>
#include <Enemy/BossWanwan.hpp>
#include <Enemy/BossEel.hpp>
#include <Enemy/BossHanachan.hpp>
#include <Enemy/TypicalEnemy.hpp>
#include <Animal/AnimalNerve.hpp>
#include <NPC/NpcNerve.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <M3DUtil/MActor.hpp>

#include <M3DUtil/InfectiousStrings.hpp> // TODO: removeme

const TNerveBase<TLiveActor>* NerveGetByIndex(int param_1)
{
	switch (param_1) {
	case 0:
		return &TNerveSmallEnemyFreeze::theNerve();

	case 1:
		return &TNerveSmallEnemyDie::theNerve();

	case 2:
		return &TNerveSmallEnemyJump::theNerve();

	case 3:
		return &TNerveSmallEnemyHitWaterJump::theNerve();

	case 4:
		return &TNerveWalkerGenerate::theNerve();

	case 5:
		return &TNerveWalkerGraphWander::theNerve();

	case 6:
		return &TNerveWalkerEscape::theNerve();

	case 7:
		return &TNerveWalkerAttack::theNerve();

	case 8:
		return &TNerveWalkerPostAttack::theNerve();

	case 9:
		return &TNerveWalkerTraceMario::theNerve();

	case 10:
		return &TNerveNameKuriDrawPollute::theNerve();

	case 0xb:
		return &TNerveNameKuriLand::theNerve();

	case 0xc:
		return &TNerveNameKuriJumpAttack::theNerve();

	case 0xd:
		return &TNerveNameKuriJumpAttackPrepare::theNerve();

	case 0xe:
		return &TNerveNameKuriExplosion::theNerve();

	case 0xf:
		return &TNerveNameKuriDiffuse::theNerve();

	case 0x10:
		return &TNerveNKFollowMario::theNerve();

	case 0x11:
		return &TNerveBGWait::theNerve();

	case 0x12:
		return &TNerveBGTentacleDamage::theNerve();

	case 0x13:
		return &TNerveBGEyeDamage::theNerve();

	case 0x14:
		return &TNerveBGBeakDamage::theNerve();

	case 0x15:
		return &TNerveBGTug::theNerve();

	case 0x16:
		return &TNerveBGDie::theNerve();

	case 0x17:
		return &TNerveBGPollute::theNerve();

	case 0x18:
		return &TNerveBGPolDrop::theNerve();

	case 0x19:
		return &TNerveBGRoll::theNerve();

	case 0x1a:
		return &TNerveBPWait::theNerve();

	case 0x1b:
		return &TNerveBPCannon::theNerve();

	case 0x1c:
		return &TNerveBPVomit::theNerve();

	case 0x1d:
		return &TNerveBPTornado::theNerve();

	case 0x1e:
		return &TNerveBPPivot::theNerve();

	case 0x1f:
		return &TNerveBPSwallow::theNerve();

	case 0x20:
		return &TNerveBPTumbleIn::theNerve();

	case 0x21:
		return &TNerveBPTumble::theNerve();

	case 0x22:
		return &TNerveBPTumbleOut::theNerve();

	case 0x23:
		return &TNerveBPGetUp::theNerve();

	case 0x24:
		return &TNerveBPSwing::theNerve();

	case 0x25:
		return &TNerveBPStompReact::theNerve();

	case 0x26:
		return &TNerveBPJumpReact::theNerve();

	case 0x27:
		return &TNerveBPPreDie::theNerve();

	case 0x28:
		return &TNerveBPDie::theNerve();

	case 0x29:
		return &TNerveBPTakeOff::theNerve();

	case 0x2a:
		return &TNerveBPFly::theNerve();

	case 0x2b:
		return &TNerveBPTouchDown::theNerve();

	case 0x2c:
		return &TNerveBPFlyCannon::theNerve();

	case 0x2d:
		return &TNerveBPFlyPivot::theNerve();

	case 0x2e:
		return &TNerveBPFall::theNerve();

	case 0x2f:
		return &TNerveBPHover::theNerve();

	case 0x30:
		return &TNerveBPSleep::theNerve();

	case 0x31:
		return &TNerveBPBreakSleep::theNerve();

	case 0x32:
		return &TNerveBPWaitL::theNerve();

	case 0x33:
		return &TNerveBPCannonL::theNerve();

	case 0x34:
		return &TNerveBWGraphWander::theNerve();

	case 0x35:
		return &TNerveBWRoll::theNerve();

	case 0x36:
		return &TNerveBWBark::theNerve();

	case 0x37:
		return &TNerveBWJump::theNerve();

	case 0x38:
		return &TNerveBWStun::theNerve();

	case 0x39:
		return &TNerveBWWakeup::theNerve();

	case 0x3a:
		return &TNerveBWJumpToBath::theNerve();

	case 0x3b:
		return &TNerveBWDie::theNerve();

	case 0x3c:
		return &TNerveBWJumpAway::theNerve();

	case 0x3d:
		return &TNerveBWShake::theNerve();

	case 0x3e:
		return &TNerveBWFall::theNerve();

	case 0x3f:
		return &TNerveBEelTearsGenerate::theNerve();

	case 0x40:
		return &TNerveBEelTearsMarioRecover::theNerve();

	case 0x41:
		return &TNerveBEelTearsSplit::theNerve();

	case 0x42:
		return &TNerveBEelTearsWaterHit::theNerve();

	case 0x43:
		return &TNerveBEelTearsMoveUp::theNerve();

	case 0x44:
		return &TNerveOilBallStay::theNerve();

	case 0x45:
		return &TNerveBossEelWaitAppear::theNerve();

	case 0x46:
		return &TNerveBossEelFirstSpin::theNerve();

	case 0x47:
		return &TNerveBossEelSecondSpin::theNerve();

	case 0x48:
		return &TNerveBossEelAppear::theNerve();

	case 0x49:
		return &TNerveBossEelOutWait::theNerve();

	case 0x4a:
		return &TNerveBossEelSlowBack::theNerve();

	case 0x4b:
		return &TNerveBossEelQuickBack::theNerve();

	case 0x4c:
		return &TNerveBossEelEat::theNerve();

	case 0x4d:
		return &TNerveBossEelDie::theNerve();

	case 0x4e:
		return &TNerveBossEelMouthOpenWait::theNerve();

	case 0x4f:
		return &TNerveBossEelSleepOnBottom::theNerve();

	case 0x50:
		return &TNerveBossHanachanGraphWander::theNerve();

	case 0x51:
		return &TNerveBossHanachanTumble::theNerve();

	case 0x52:
		return &TNerveBossHanachanDown::theNerve();

	case 0x53:
		return &TNerveBossHanachanGetUp::theNerve();

	case 0x54:
		return &TNerveBossHanachanDamage::theNerve();

	case 0x55:
		return &TNerveBossHanachanSnort::theNerve();

	case 0x56:
		return &TNerveBossHanachanDead::theNerve();

	case 0x57:
		return &TNerveSBH_SleepContinue::theNerve();

	case 0x58:
		return &TNerveSBH_Fall::theNerve();

	case 0x59:
		return &TNerveTypicalGraphWander::theNerve();

	case 0x5a:
		return &TNerveNPCGraphWander::theNerve();

	case 0x5b:
		return &TNerveNPCUTurn::theNerve();

	case 0x5c:
		return &TNerveNPCGraphWait::theNerve();

	case 0x5d:
		return &TNerveNPCWaitContinue::theNerve();

	case 0x5e:
		return &TNerveNPCWaitMarioApproach::theNerve();

	case 0x5f:
		return &TNerveNPCTurnToMario::theNerve();

	case 0x60:
		return &TNerveNPCWet::theNerve();

	case 0x61:
		return &TNerveNPCSink::theNerve();

	case 0x62:
		return &TNerveNPCRecoverFromSink::theNerve();

	case 99:
		return &TNerveNPCRecoverAfter::theNerve();

	case 100:
		return &TNerveNPCSetPosAfterSinkBottom::theNerve();

	case 0x65:
		return &TNerveNPCTalk::theNerve();

	case 0x66:
		return &TNerveNPCThrow::theNerve();

	case 0x67:
		return &TNerveNPCMad::theNerve();

	case 0x68:
		return &TNerveNPCBlown::theNerve();

	case 0x69:
		return &TNerveNPCMareStand::theNerve();

	case 0x6a:
		return &TNerveAnimalGraphWander::theNerve();

	default:
		return nullptr;
	}
}

void linSetBck(TSpcTypedInterp<TLiveActor>* interp, u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	TSpcSlice arg = interp->pop();

	MActor* actor = interp->getOwner()->getMActor();
	if (actor)
		actor->setBck(arg.getDataString());

	interp->push();
}

void linSetSubBck(TSpcTypedInterp<TLiveActor>* interp, u32 arg_num) { }

void linSetBpk(TSpcTypedInterp<TLiveActor>* interp, u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	TSpcSlice arg = interp->pop();

	MActor* actor = interp->getOwner()->getMActor();
	if (actor)
		actor->setBpk(arg.getDataString());

	interp->push();
}

void linSetBtp(TSpcTypedInterp<TLiveActor>* interp, u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	TSpcSlice arg = interp->pop();

	MActor* actor = interp->getOwner()->getMActor();
	if (actor)
		actor->setBtp(arg.getDataString());

	interp->push();
}

void linSetBtk(TSpcTypedInterp<TLiveActor>* interp, u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	TSpcSlice arg = interp->pop();

	MActor* actor = interp->getOwner()->getMActor();
	if (actor)
		actor->setBtk(arg.getDataString());

	interp->push();
}

void linSetBlk(TSpcTypedInterp<TLiveActor>* interp, u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	TSpcSlice arg = interp->pop();

	MActor* actor = interp->getOwner()->getMActor();
	if (actor)
		actor->setBlk(arg.getDataString());

	interp->push();
}

void linSetBls(TSpcTypedInterp<TLiveActor>* interp, u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	TSpcSlice arg = interp->pop();

	MActor* actor = interp->getOwner()->getMActor();
	if (actor)
		arg.getDataString(); // NOTE: there's no BLS

	interp->push();
}

void linSetAnmRate(TSpcTypedInterp<TLiveActor>* interp, u32 arg_num)
{
	interp->verifyArgNum(2, &arg_num);
	TLiveActor* owner = interp->getOwner();
	TSpcSlice arg1    = interp->pop();
	TSpcSlice arg2    = interp->pop();

	switch (arg2.getDataInt()) {
	case 0:
		owner->getMActor()->setFrameRate(arg1.getDataFloat(), 0);
		break;
	case 1:
		owner->getMActor()->setFrameRate(arg1.getDataFloat(), 3);
		break;
	}

	interp->push();
}

void linGetSRT(TSpcTypedInterp<TLiveActor>* interp, u32 arg_num)
{
	interp->verifyArgNum(2, &arg_num);
	TLiveActor* owner = interp->getOwner();
	TSpcSlice arg1    = interp->pop();
	TSpcSlice arg2    = interp->pop();

	switch (arg2.getDataInt()) {
	case 0:
		switch (arg1.getDataInt()) {
		case 0: {
			TSpcSlice slice;
			slice.setDataFloat(owner->mPosition.x);
			interp->push(slice);
		} break;
		case 1: {
			TSpcSlice slice;
			slice.setDataFloat(owner->mPosition.y);
			interp->push(slice);
		} break;
		case 2: {
			TSpcSlice slice;
			slice.setDataFloat(owner->mPosition.z);
			interp->push(slice);
		} break;
		default:
			interp->push();
			break;
		}
		break;
	case 1:
		switch (arg1.getDataInt()) {
		case 0: {
			TSpcSlice slice;
			slice.setDataFloat(owner->mRotation.x);
			interp->push(slice);
		} break;
		case 1: {
			TSpcSlice slice;
			slice.setDataFloat(owner->mRotation.y);
			interp->push(slice);
		} break;
		case 2: {
			TSpcSlice slice;
			slice.setDataFloat(owner->mRotation.z);
			interp->push(slice);
		} break;
		default:
			interp->push();
			break;
		}
		break;
	case 2:
		switch (arg1.getDataInt()) {
		case 0: {
			TSpcSlice slice;
			slice.setDataFloat(owner->mScaling.x);
			interp->push(slice);
		} break;
		case 1: {
			TSpcSlice slice;
			slice.setDataFloat(owner->mScaling.y);
			interp->push(slice);
		} break;
		case 2: {
			TSpcSlice slice;
			slice.setDataFloat(owner->mScaling.z);
			interp->push(slice);
		} break;
		default:
			interp->push();
			break;
		}
		break;
	default:
		interp->push();
		break;
	}
}

void linSetSRT(TSpcTypedInterp<TLiveActor>* interp, u32 arg_num)
{
	interp->verifyArgNum(3, &arg_num);
	TLiveActor* owner = interp->getOwner();
	TSpcSlice arg1    = interp->pop();
	TSpcSlice arg2    = interp->pop();
	TSpcSlice arg3    = interp->pop();

	f32 value = arg1.getDataFloat();

	switch (arg3.getDataInt()) {
	case 1:
		value = MsWrap(value, 0.0f, 360.0f);
		switch (arg2.getDataInt()) {
		case 0:
			owner->mRotation.x = value;
			break;
		case 1:
			owner->mRotation.y = value;
			break;
		case 2:
			owner->mRotation.z = value;
			break;
		}
		break;

	case 0:
		switch (arg2.getDataInt()) {
		case 0:
			owner->mPosition.x = value;
			break;
		case 1:
			owner->mPosition.y = value;
			break;
		case 2:
			owner->mPosition.z = value;
			break;
		}
		break;

	case 2:
		switch (arg2.getDataInt()) {
		case 0:
			owner->mScaling.x = value;
			break;
		case 1:
			owner->mScaling.y = value;
			break;
		case 2:
			owner->mScaling.z = value;
			break;
		}
		break;
	}

	interp->push();
}

void linPushNerve(TSpcTypedInterp<TLiveActor>* interp, u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	TLiveActor* owner = interp->getOwner();
	TSpcSlice arg     = interp->pop();

	const TNerveBase<TLiveActor>* nerve = NerveGetByIndex(arg.getDataInt());

	if (!nerve) {
		interp->push();
	} else {
		owner->mSpine->pushNerve(nerve);
		interp->push();
	}
}

template <> void TSpcTypedBinary<TLiveActor>::initUserBuiltin()
{
	bindSystemDataToSymbol("pushNerve", (u32)&linPushNerve);
	bindSystemDataToSymbol("setBck", (u32)&linSetBck);
	bindSystemDataToSymbol("setBpk", (u32)&linSetBpk);
	bindSystemDataToSymbol("setBtp", (u32)&linSetBtp);
	bindSystemDataToSymbol("setBtk", (u32)&linSetBtk);
	bindSystemDataToSymbol("setBlk", (u32)&linSetBlk);
	bindSystemDataToSymbol("setBls", (u32)&linSetBls);
	bindSystemDataToSymbol("setSRT", (u32)&linSetSRT);
	bindSystemDataToSymbol("getSRT", (u32)&linGetSRT);
	bindSystemDataToSymbol("setAnmRate", (u32)&linSetAnmRate);
}
