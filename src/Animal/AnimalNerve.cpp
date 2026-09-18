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
// TODO: this is still a reconstruction, and it is what makes
// TNerveAnimalGraphWander::execute's frame 0x118 against the ROM's 0xe8. The
// whole 48 bytes are inline-expansion parameter temporaries in the low region
// -- all 334 instructions and the two `diff` vectors already match, and the
// pool even ends up in the ROM's order. Measured levers, each with identical
// instructions: actor->mMActor, ->mManager, ->mInstanceIndex, ->mActorType and
// (TObjManager*)manager->getObj(i) are -8 each but saturate at -40 together;
// raw manager->unk18[i] is -24; taking `a` by value or spelling diff.sub(b)
// out per component is -8 each. unk18 + mMActor + mManager + mInstanceIndex +
// mActorType + by-value `a` lands 0xe8 exactly with the pool in the ROM's
// order, but leaves every slot 4 bytes high, and no -4 lever was found (a
// split `int count; count = ...`, a split `save`, `mActor` or `hi`/`lo`
// declaration are all worth zero), so that combination is not committed.
//
// Better lead for a header batch: JGeometry::TVec3<f32>::distance() has no
// symbol anywhere in the map, and giving it this body --
//   TVec3<f32> diff = *this; diff.sub(other); return TUtil<f32>::sqrt(diff.squared());
// -- makes `actor->unkF4.getPoint().distance(actor->mPosition)` reproduce all
// 334 instructions with no helper in this TU at all (frame 0x120; the current
// scalar body gives frame 0xe8 exactly but 336 instructions). Retail's second
// site is then literally TSpineEnemy::isReachedToGoal()'s body.
//
// Still open either way: retail holds `count` in r7 and the instance index in
// r6 (ours are swapped) and keeps `other` in r27 where we reuse r28. No
// spelling of the count/index/save locals, of `other`'s type, or of the
// anmIdx/otherCtrl locals moves either.
inline f32 calcDist(const JGeometry::TVec3<f32>& a,
                    const JGeometry::TVec3<f32>& b)
{
	JGeometry::TVec3<f32> diff = a;
	diff.sub(b);
	return JGeometry::TUtil<f32>::sqrt(diff.squared());
}

DEFINE_NERVE(TNerveAnimalGraphWander, TLiveActor)
{
	TAnimalBase* actor          = (TAnimalBase*)spine->getBody();
	MActor* mActor              = actor->getMActor();
	TAnimalManagerBase* manager = (TAnimalManagerBase*)actor->getManager();

	TAnimalSaveIndividual* save = manager->mAnimalSave;
	int count                   = save->mSLSharedAnmNum.get();

	if (count != 0 && actor->getInstanceIndex() >= count) {
		TLiveActor* other = manager->getObj(actor->getInstanceIndex() % count);

		int anmIdx = other->getMActor()->getCurAnmIdx(ANM_TYPE_BCK);
		mActor->setBckFromIndex(anmIdx);

		J3DFrameCtrl* myCtrl = mActor->getFrameCtrl(ANM_TYPE_BCK);
		J3DFrameCtrl* otherCtrl
		    = other->getMActor()->getFrameCtrl(ANM_TYPE_BCK);
		myCtrl->setFrame(otherCtrl->getFrame());
	} else if (actor->getActorType() != 0x800001) {
		// not the right type, skip animation setup
	} else {
		if (spine->getTime() == 0) {
			int hi     = CLBPalFrame<int>(500);
			int lo     = CLBPalFrame<int>(150);
			int* timer = actor->mFrameTimer;
			timer[0]   = 0;
			timer[1]   = MsRandI(lo, hi);
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

				int hi     = CLBPalFrame<int>(500);
				int lo     = CLBPalFrame<int>(150);
				int* timer = actor->mFrameTimer;
				timer[0]   = 0;
				timer[1]   = MsRandI(lo, hi);
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

				int hi     = CLBPalFrame<int>(180);
				int lo     = CLBPalFrame<int>(60);
				int* timer = actor->mFrameTimer;
				timer[0]   = 0;
				timer[1]   = MsRandI(lo, hi);
			} else {
				if (!mActor->checkCurBckFromIndex(0))
					mActor->setBckFromIndex(0);

				int hi     = CLBPalFrame<int>(500);
				int lo     = CLBPalFrame<int>(150);
				int* timer = actor->mFrameTimer;
				timer[0]   = 0;
				timer[1]   = MsRandI(lo, hi);
			}
		}
	}

	return FALSE;
}
