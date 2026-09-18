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
// Batch 131 committed the combination the note below had measured but held
// back: raw `manager->unk18[i]`, `actor->mMActor`, `actor->mManager`,
// `actor->mInstanceIndex`, `actor->mActorType` and a by-value `a` here. Raw
// member reads are the evidence-backed spelling for this nerve (each accessor
// level is worth 8 bytes of low region and the frame only closes without
// them), and the frame is now the ROM's 0xe8 with all 334 instructions and the
// pool in the ROM's order: 99.6 -> 99.7%, 76 differing rows -> 39.
//
// TODO: the 39 that remain are two clusters, and neither is a frame gap.
//   1. Every inline-temporary slot is uniformly **4 bytes high** (the two
//      calcDist copies at 0x98/0x7c against 0x94/0x78, the getPoint buffer at
//      0x88 against 0x84), so one 4-byte pool item is still missing at the
//      *bottom* -- i.e. in an expansion after the second calcDist site. The
//      only knob found there is reading `actor->unkF4` raw instead of
//      `getUnkF4()` at the height test, which lands every slot (39 rows -> 15)
//      but is demonstrably not retail's spelling: retail materialises
//      `&unkF4` in its own `addi r3, r31, 0xf4` and then adds 4 inside
//      TPathNode::getPoint(), which only the accessor produces. So the
//      accessor is kept and the 4 bytes are owed elsewhere. Worth zero or
//      worse: a named `f32` for either side of the height test, the `int*
//      timer` locals spelled out, a copy instead of a reference for `curPos`,
//      a named 100.0f, folding `save` into `count`, and `getObj(i)` in place
//      of `unk18[i]` (+0x18).
//   2. retail holds `count` in r7 and the instance index in r6 (ours are
//      swapped) and keeps `other` in r27 where we use r28. No spelling of the
//      count/index/save locals, of `other`'s type, or of the anmIdx/otherCtrl
//      locals moves either.
//
// Better lead for a header batch: JGeometry::TVec3<f32>::distance() has no
// symbol anywhere in the map, and giving it this body --
//   TVec3<f32> diff = *this; diff.sub(other); return TUtil<f32>::sqrt(diff.squared());
// -- makes `actor->unkF4.getPoint().distance(actor->mPosition)` reproduce all
// 334 instructions with no helper in this TU at all (frame 0x120; the current
// scalar body gives frame 0xe8 exactly but 336 instructions). Retail's second
// site is then literally TSpineEnemy::isReachedToGoal()'s body.

inline f32 calcDist(JGeometry::TVec3<f32> a,
                    const JGeometry::TVec3<f32>& b)
{
	a.sub(b);
	return JGeometry::TUtil<f32>::sqrt(a.squared());
}

DEFINE_NERVE(TNerveAnimalGraphWander, TLiveActor)
{
	TAnimalBase* actor          = (TAnimalBase*)spine->getBody();
	MActor* mActor              = actor->mMActor;
	TAnimalManagerBase* manager = (TAnimalManagerBase*)actor->mManager;

	TAnimalSaveIndividual* save = manager->mAnimalSave;
	int count                   = save->mSLSharedAnmNum.get();

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
