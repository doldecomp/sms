#include <NPC/NpcNerve.hpp>
#include <Strategic/Spine.hpp>
#include <System/MarDirector.hpp>
#include <M3DUtil/MActor.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <Enemy/Graph.hpp>
#include <Camera/cameralib.hpp>
#include <Player/MarioAccess.hpp>
#include <NPC/NpcBase.hpp>

DEFINE_NERVE(TNerveNPCGraphWander, TLiveActor)
{
	TBaseNPC* self = (TBaseNPC*)spine->getBody();
	if (spine->getTime() == 0) {
		self->unk22C->doThing();
	}

	self->execWalk(true);

	JGeometry::TVec3<f32> local_582 = self->unkF4.getPoint();

	JGeometry::TVec3<f32> local_58(local_582.x - self->mPosition.x, 0.0f,
	                               local_582.z - self->mPosition.z);

	(void)&local_58;

	f32 fVar2 = local_58.squared();

	if (!self->unk114.empty()) {
		if (fVar2 < CLBSquared(100.0f) && !self->unk114.empty()) {
			self->unkF4 = self->unk114.pop();
		}
	} else {
		bool bVar6 = false;
		bool bVar5 = false;
		if (self->getTracer()->hasOnlyOneNext()) {
			bVar6 = true;
			if (self->getTracer()->currPitchIsZero())
				bVar5 = true;
		}
		bool bVar4 = self->unk22C->doThing2();
		if ((bVar6 == true && fVar2 < CLBSquared(50.0f))
		    || (bVar6 == false && fVar2 < CLBSquared(100.0f))) {
			if (!self->checkActionFlag(TBaseNPC::NPC_ACTION_UNK80) && bVar4) {
				spine->pushAfterCurrent(&TNerveNPCGraphWait::theNerve());
				return true;
			}

			self->goToShortestNextGraphNode();
			if (bVar6) {
				spine->pushAfterCurrent(&TNerveNPCUTurn::theNerve());
				if (bVar5)
					self->onLiveFlag(LIVE_FLAG_UNK200000);
				return true;
			}
		}
	}

	return false;
}

DEFINE_NERVE(TNerveNPCUTurn, TLiveActor)
{
	TBaseNPC* self = (TBaseNPC*)spine->getBody();

	if (self->execUTurn()) {
		self->mMarchSpeed = 0.0f;
		self->offLiveFlag(LIVE_FLAG_UNK200000);
		spine->pushAfterCurrent(&TNerveNPCGraphWander::theNerve());
		return true;
	}
	return false;
}

DEFINE_NERVE(TNerveNPCGraphWait, TLiveActor)
{
	TBaseNPC* self = (TBaseNPC*)spine->getBody();

	if (spine->getTime() == 0)
		self->unk22C->doThing();

	if (self->getMarchSpeed() < 0.001f) {
		if (self->unk22C->doThing2()) {
			spine->pushAfterCurrent(&TNerveNPCGraphWander::theNerve());
			return true;
		}
	} else {
		self->execWalk(false);
	}
	return false;
}

DEFINE_NERVE(TNerveNPCWaitContinue, TLiveActor)
{
	TBaseNPC* self = (TBaseNPC*)spine->getBody();

	if (spine->getTime() == 0) {
		if (self->getHolder() != nullptr) {
			self->npcTakenIn();
		} else {
			self->npcWaitIn();
		}
	}

	return false;
}

DEFINE_NERVE(TNerveNPCWaitMarioApproach, TLiveActor)
{
	TBaseNPC* self = (TBaseNPC*)spine->getBody();

	if (self->isInBodyTurnSearchRange() && self->isClean()) {
		spine->pushAfterCurrent(&TNerveNPCTurnToMario::theNerve());
		return true;
	}

	if (self->getActorType() - 0x400001C > 1) {

		if (!self->isPeachTired()) {

			if (self->isSunflowerReviving()) {
				self->sunflowerReviving();
			} else if (self->getActorType() == 0x4000006) {
				self->monteMESetAnmWhenFar();
				self->execTurnToFirstState();
			} else {
				if (self->isNeedTurnToFirstState()) {
					if (self->execTurnToFirstState())
						self->npcWaitIn();
					else
						self->npcStepIn();
				} else {
					if (spine->getTime() == 0)
						self->npcWaitIn();
				}
			}
		}
	}

	return false;
}

DEFINE_NERVE(TNerveNPCTurnToMario, TLiveActor)
{
	TBaseNPC* self = (TBaseNPC*)spine->getBody();

	if (!self->isInBodyTurnSearchRange() || self->mPollutionAmount != 0.0f) {
		spine->pushAfterCurrent(&TNerveNPCWaitMarioApproach::theNerve());
		return true;
	}

	if (self->getActorType() - 0x400001C > 1) {
		if (!self->isPeachTired()) {
			if (self->isSunflowerReviving()) {
				self->sunflowerReviving();
			} else if (self->getActorType() == 0x4000006) {
				self->monteMESetAnmWhenNear();
				self->execTurnToFirstState();
			} else if (self->isTurnToMarioWhenApproach()) {
				SMS_GoRotate(self->mPosition, SMS_GetMarioPos(),
				             self->getTurnSpeed(), &self->mRotation.y);

				// TODO: wtf?
				JGeometry::TVec3<f32> axis = SMS_GetMarioPos();
				axis -= self->mPosition;
				JGeometry::TVec3<f32> copy  = axis;
				JGeometry::TVec3<f32> copy2 = copy;
				JGeometry::TVec3<f32> copy3;
				copy3.set(copy2);

				f32 angle
				    = MsWrap(abs(self->mRotation.y - MsGetRotFromZaxisY(copy3)),
				             0.0f, 360.0f);
				if (angle < 0.001f)
					self->npcWaitIn();
				else
					self->npcStepIn();
			} else {
				if (self->isNeedTurnToFirstState()) {
					if (self->execTurnToFirstState())
						self->npcWaitIn();
					else
						self->npcStepIn();
				} else {
					if (spine->getTime() == 0)
						self->npcWaitIn();
				}
			}
		}
	}

	return false;
}

DEFINE_NERVE(TNerveNPCWet, TLiveActor)
{
	TBaseNPC* self = (TBaseNPC*)spine->getBody();

	if (spine->getTime() == 0)
		self->npcWetIn();

	if (self->npcWetting())
		return true;

	return false;
}

DEFINE_NERVE(TNerveNPCSink, TLiveActor)
{
	TBaseNPC* self = (TBaseNPC*)spine->getBody();
	self->npcSinking();
	return false;
}

DEFINE_NERVE(TNerveNPCRecoverFromSink, TLiveActor)
{
	TBaseNPC* self = (TBaseNPC*)spine->getBody();

	if (self->npcRecoverFromSinking()) {
		spine->pushAfterCurrent(&TNerveNPCRecoverAfter::theNerve());
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveNPCRecoverAfter, TLiveActor)
{
	TBaseNPC* self = (TBaseNPC*)spine->getBody();

	if (spine->getTime() == 0)
		self->npcRecoverAfterIn();

	if (self->unkD0->getCurrentAnmKind() == NPC_ANM_KIND_UNK3
	    && self->getMActor()->isCurAnmAlreadyEnd(0)) {
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveNPCSetPosAfterSinkBottom, TLiveActor)
{
	TBaseNPC* self = (TBaseNPC*)spine->getBody();

	self->setPosAndInitAfterSinkBottom();

	return true;
}

DEFINE_NERVE(TNerveNPCTalk, TLiveActor)
{
	TBaseNPC* self = (TBaseNPC*)spine->getBody();

	if (gpMarDirector->isThing()) {
		if (spine->getTime() == 0)
			self->npcTalkIn();
		self->npcTalking();
	} else {
		if (self->getActorType() == 0x400001C)
			return true;
		self->npcTalkOut();
	}

	return false;
}

DEFINE_NERVE(TNerveNPCThrow, TLiveActor)
{
	TBaseNPC* self = (TBaseNPC*)spine->getBody();

	if (spine->getTime() == 0)
		self->npcThrowIn();

	if (self->npcThrowing())
		return true;

	return false;
}

DEFINE_NERVE(TNerveNPCMad, TLiveActor)
{
	TBaseNPC* self = (TBaseNPC*)spine->getBody();

	if (spine->getTime() == 0)
		self->npcMadIn();

	if (self->npcMadding())
		return true;

	return false;
}

DEFINE_NERVE(TNerveNPCBlown, TLiveActor)
{
	TBaseNPC* self = (TBaseNPC*)spine->getBody();

	if (spine->getTime() == 0)
		self->npcBlownIn();

	if (self->npcBlowning()) {
		if (self->isNormalMare())
			spine->pushAfterCurrent(&TNerveNPCMareStand::theNerve());
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveNPCMareStand, TLiveActor)
{
	TBaseNPC* self = (TBaseNPC*)spine->getBody();

	if (spine->getTime() == 0)
		self->npcMareStandIn();

	if (self->npcMareStanding())
		return true;

	return false;
}
