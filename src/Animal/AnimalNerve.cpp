#include <Animal/AnimalNerve.hpp>
#include <Animal/AnimalBase.hpp>
#include <Animal/AnimalManager.hpp>
#include <Animal/AnimalSave.hpp>
#include <M3DUtil/MActor.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <Camera/cameralib.hpp>
#include <Strategic/Spine.hpp>

// `inline`, not a plain function: the map's closure for AnimalNerve.o lists
// exactly four weak duplicates (JGeometry::TUtil<f32>::sqrt, TPathNode::~TPathNode,
// TNerveBase<TLiveActor>'s vtable and destructor) and nothing else, so whatever
// computed these two distances left no symbol of its own. Plain and `static`
// both emit a 0xac global the map has no room for; `inline` removes it with no
// codegen change.
//
// Matching levers that closed execute (frame already 0xe8 from batch 131's raw
// member reads + by-value calcDist `a`):
//   * `mSLSharedAnmNum.value` not `.get()` — the const-ref temporary from
//     `.get()` was the uniform +4 on every low-pool slot (getUnkF4 at the
//     height test is kept; raw unkF4 lands the pool but drops retail's
//     `addi r3, r31, 0xf4` getPoint receiver).
//   * `s32 count` not `int` — restores retail's r7/r6 count/index colouring.
//   * `resetAnmTimer` around the shared MsRandI setup — restores `other` in
//     r27 (open-coded sites leave it in r28).

inline f32 calcDist(JGeometry::TVec3<f32> a,
                    const JGeometry::TVec3<f32>& b)
{
	a.sub(b);
	return JGeometry::TUtil<f32>::sqrt(a.squared());
}

inline void resetAnmTimer(TAnimalBase* actor, int lo, int hi)
{
	int* timer = actor->mFrameTimer;
	timer[0]   = 0;
	timer[1]   = MsRandI(lo, hi);
}

DEFINE_NERVE(TNerveAnimalGraphWander, TLiveActor)
{
	TAnimalBase* actor          = (TAnimalBase*)spine->getBody();
	MActor* mActor              = actor->mMActor;
	TAnimalManagerBase* manager = (TAnimalManagerBase*)actor->mManager;

	TAnimalSaveIndividual* save = manager->mAnimalSave;
	s32 count                   = save->mSLSharedAnmNum.value;

	if (count != 0 && actor->mInstanceIndex >= count) {
		TLiveActor* other = (TLiveActor*)manager->unk18[actor->mInstanceIndex % count];

		int anmIdx = other->getMActor()->getCurAnmIdx(ANM_TYPE_BCK);
		mActor->setBckFromIndex(anmIdx);

		J3DFrameCtrl* myCtrl = mActor->getFrameCtrl(ANM_TYPE_BCK);
		J3DFrameCtrl* otherCtrl
		    = other->getMActor()->getFrameCtrl(ANM_TYPE_BCK);
		myCtrl->setFrame(otherCtrl->getFrame());
	} else if (actor->mActorType != 0x800001) {
		// not the right type, skip animation setup
	} else {
		if (spine->getTime() == 0) {
			resetAnmTimer(actor, CLBPalFrame<int>(150), CLBPalFrame<int>(500));
		}

		int* timer = actor->mFrameTimer;
		timer[0]   = timer[0] + 1;
		if (timer[0] >= timer[1]) {
			timer[0] = timer[1];
		}

		BOOL anmEndsNext = mActor->curAnmEndsNext(ANM_TYPE_BCK, nullptr);

		switch (mActor->getCurAnmIdx(ANM_TYPE_BCK)) {
		case 1:
			if (anmEndsNext) {
				if (!mActor->checkCurBckFromIndex(0))
					mActor->setBckFromIndex(0);

				resetAnmTimer(actor, CLBPalFrame<int>(150), CLBPalFrame<int>(500));
			}
			break;
		}
	}

	actor->execWalk(true);

	if (!actor->unk114.empty()) {
		const JGeometry::TVec3<f32>& goalPos = actor->unkF4.getPoint();
		f32 dist = calcDist(goalPos, actor->mPosition);

		if (dist < 200.0f && !actor->unk114.empty()) {
			actor->unkF4 = actor->unk114.pop();
		}
	} else {
		const JGeometry::TVec3<f32>& curPos = actor->unk104.getPoint();
		f32 dist = calcDist(curPos, actor->mPosition);

		if (dist < 100.0f) {
			actor->goToRandomNextGraphNode();
			actor->resetRandomCurPathNode();

			if (actor->mPosition.y <= actor->getUnkF4().getPoint().y) {
				if (!mActor->checkCurBckFromIndex(1))
					mActor->setBckFromIndex(1);

				resetAnmTimer(actor, CLBPalFrame<int>(60), CLBPalFrame<int>(180));
			} else {
				if (!mActor->checkCurBckFromIndex(0))
					mActor->setBckFromIndex(0);

				resetAnmTimer(actor, CLBPalFrame<int>(150), CLBPalFrame<int>(500));
			}
		}
	}

	return FALSE;
}
