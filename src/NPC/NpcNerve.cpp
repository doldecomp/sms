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
		self->unk22C->resetGraphWanderTimer();
	}

	self->execWalk(true);

	JGeometry::TVec3<f32> goal = self->unkF4.getPoint();

	JGeometry::TVec3<f32> toGoal(goal.x - self->getPosition().x, 0.0f,
	                             goal.z - self->getPosition().z);

	// TODO: fabricated. Retail gives `toGoal` a stack home and reloads all
	// three components for the unfused squared(); a plain named local is
	// scalar-replaced and fuses. Taking its address is the only spelling
	// found that keeps the frame at 0x128; writing the whole expression as an
	// unnamed TVec3 temporary reproduces the instruction stream exactly but
	// grows the frame to 0x138 and puts the temp above `goal` instead of
	// below it (retail: goal 0xd0, difference 0xdc, 8 free bytes at 0xe8).
	(void)&toGoal;

	f32 fVar2 = toGoal.squared();

	if (!self->unk114.empty()) {
		if (fVar2 < CLBSquared(100.0f) && !self->unk114.empty()) {
			self->unkF4 = self->unk114.pop();
		}
	} else {
		bool bVar6 = false;
		bool bVar5 = false;
		// TODO: retail `bl`s TGraphTracer::getCurGraphIndex and ::getGraph
		// inside this first site (and reloads `0x124(r31)` between them
		// because of the calls) while expanding both of them at the
		// currPitchIsZero site just below, where we expand them at both.
		// Measured this batch by probing extra accessor levels in
		// Graph.hpp: the two accessors currently sit at inline depth 3
		// (hasOnlyOneNext -> getCurrent/getRailNode -> accessors), one
		// added level leaves them at depth 4 (still expanded, budget 2 vs
		// cost 1) and two added levels put them at depth 5, where both do
		// become `bl`s -- so retail's chain has four enclosing levels here.
		// Two further constraints: `getGraphNode` must NOT share the body
		// that calls them (at depth 5 it refuses too, and the map has no
		// out-of-line TGraphWeb::getGraphNode anywhere, so retail indexed
		// `unk0[i]` directly at that level), and the retail index is
		// fetched before the graph, which only `getGraphNode(getCurGraphIndex())`
		// right-to-left gives. Enemy.a enemyMario.cpp `consider()` is the
		// other refusing caller of the const `getGraph` and is a matching
		// function, so its two `bl getGraph`s are further evidence.
		// Rejected here because the only spellings that reach depth 5 need
		// two pure forwarding accessors invented on top of the already
		// fabricated hasOnlyOneNext/currPitchIsZero (neither name is in the
		// map). Note `getCurrent() const` reading `mCurrIdx` instead of
		// `getCurGraphIndex()` is codegen-neutral today but cannot be right:
		// the map emits getCurGraphIndex weak out of line *in this TU*.
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

// TODO: 99.6% and frame-exact since resetGraphWaitTimer's two reads became
// `.value` in min-then-max order. What is left is a three-register rotation:
// retail keeps `self->unk22C` in r30 with minFrame/maxFrame in r29/r28, i.e.
// it ranks the timer pointer as a pool/base temp above the two locals, while we
// rank it as the inlined callee's `this` and put it in r28. That is the
// known-open `this`-versus-pool-base callee-saved swap (frame-gaps.md,
// "batch 145"); the scratch register for mPtrSaveNormal (r4 against our r3)
// follows it.
DEFINE_NERVE(TNerveNPCGraphWait, TLiveActor)
{
	TBaseNPC* self = (TBaseNPC*)spine->getBody();

	if (spine->getTime() == 0)
		self->unk22C->resetGraphWaitTimer();

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

// Binding level worth +8 of low region (frame-gaps.md, "batch 110").
static inline u32 NpcActorType(const TBaseNPC* p)
{
	u32 actorType = p->getActorType();
	return actorType;
}

DEFINE_NERVE(TNerveNPCWaitMarioApproach, TLiveActor)
{
	TBaseNPC* self = (TBaseNPC*)spine->getBody();

	if (self->isInBodyTurnSearchRange() && self->isClean()) {
		spine->pushAfterCurrent(&TNerveNPCTurnToMario::theNerve());
		return true;
	}

	u32 actorType = NpcActorType(self);
	if (actorType - 0x400001C > 1) {

		if (!self->isPeachTired()) {

			if (self->isSunflowerReviving()) {
				self->sunflowerReviving();
			} else if (actorType == 0x4000006) {
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

// TODO: adding the NpcActorType binding level here as well moves the frame
// 0x88 -> 0x90 (retail 0xb0) without changing the three-slot layout, so it is
// not the missing construct and is left off.
DEFINE_NERVE(TNerveNPCTurnToMario, TLiveActor)
{
	TBaseNPC* self = (TBaseNPC*)spine->getBody();

	if (!self->isInBodyTurnSearchRange() || self->mPollutionAmount != 0.0f) {
		spine->pushAfterCurrent(&TNerveNPCWaitMarioApproach::theNerve());
		return true;
	}

	u32 actorType = self->getActorType();
	if (actorType - 0x400001C > 1) {
		if (!self->isPeachTired()) {
			if (self->isSunflowerReviving()) {
				self->sunflowerReviving();
			} else if (actorType == 0x4000006) {
				self->monteMESetAnmWhenNear();
				self->execTurnToFirstState();
			} else if (self->isTurnToMarioWhenApproach()) {
				SMS_GoRotate(self->mPosition, SMS_GetMarioPos(),
				             self->getTurnSpeed(), &self->mRotation.y);

				// TODO: instruction-exact, but the frame is 40 bytes
				// short and the three 12-byte slots are laid out the
				// other way round: retail has `axis` lowest (0x5c)
				// with the two by-value copies ascending above it
				// (0x78, 0x88) and 16 dead bytes between axis and the
				// first copy, while every named-local spelling puts
				// the copies below `axis` in declaration order. The
				// two integer word copies themselves are real (retail
				// copies 0x5c -> 0x78 -> 0x88 before reading x/z), so
				// the level structure is right and only the placement
				// is wrong -- most likely the copies belong to an
				// inlined callee that takes the direction by value.
				// `toMario3` itself is folded away, but removing it
				// costs the memory home of `toMario2` (99.9 -> 97.5),
				// so some further level really does bind a copy here.
				// Measured: the three slots are contiguous and
				// *descending* here (0x44 copy2, 0x50 copy1, 0x5c
				// axis, 12-byte stride) because they are named locals,
				// while retail's are *ascending* with padding (0x5c
				// axis, 16 dead, 0x78 copy1, 4 pad, 0x88 copy2, 4 pad,
				// 0x98 the int->double temp) -- i.e. retail has no
				// named TVec3 local at all in this block and all four
				// slots are inline temporaries in expansion order.
				// Closing it therefore needs the whole angle to be one
				// expression whose callee copies the direction twice
				// and holds one uninitialised TVec3 (the 16 dead
				// bytes); every by-value TU-local helper spelling of
				// that is artificial, so it is left open.
				JGeometry::TVec3<f32> axis = SMS_GetMarioPos();
				axis -= self->mPosition;
				JGeometry::TVec3<f32> toMario  = axis;
				JGeometry::TVec3<f32> toMario2 = toMario;
				JGeometry::TVec3<f32> toMario3;
				toMario3.set(toMario2);

				f32 angle = MsWrap(
				    abs(self->mRotation.y - MsGetRotFromZaxisY(toMario3)),
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
	    && self->getMActor()->isCurAnmAlreadyEnd(ANM_TYPE_BCK)) {
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

	if (SMSGetMarDirector()->isThing()) {
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
