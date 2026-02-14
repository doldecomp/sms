#include <Animal/AnimalNerve.hpp>
#include <Animal/AnimalBase.hpp>
#include <Animal/AnimalManager.hpp>
#include <Animal/AnimalSave.hpp>
#include <M3DUtil/MActor.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <Camera/cameralib.hpp>
#include <Strategic/Spine.hpp>

f32 calcDist(const JGeometry::TVec3<f32>& a, const JGeometry::TVec3<f32>& b)
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

		int anmIdx = other->getMActor()->getCurAnmIdx(0);
		mActor->setBckFromIndex(anmIdx);

		J3DFrameCtrl* myCtrl    = mActor->getFrameCtrl(0);
		J3DFrameCtrl* otherCtrl = other->getMActor()->getFrameCtrl(0);
		myCtrl->mCurrentFrame   = otherCtrl->mCurrentFrame;
	} else if (actor->getActorType() != 0x800001) {
		// not the right type, skip animation setup
	} else {
		if (spine->getTime() == 0) {
			int hi     = CLBPalFrame<int>(500);
			int lo     = CLBPalFrame<int>(150);
			int* timer = actor->mFrameTimer;
			timer[0]   = 0;
			timer[1]   = lo + (int)((f32)(hi - lo) * (MsRandF())) + 1;
		}

		int* timer = actor->mFrameTimer;
		timer[0]   = timer[0] + 1;
		if (timer[0] >= timer[1]) {
			timer[0] = timer[1];
		}

		BOOL anmEndsNext = mActor->curAnmEndsNext(0, 0);

		if (mActor->getCurAnmIdx(0) == 1 && anmEndsNext) {
			if (!mActor->checkCurBckFromIndex(0))
				mActor->setBckFromIndex(0);

			int hi     = CLBPalFrame<int>(500);
			int lo     = CLBPalFrame<int>(150);
			int* timer = actor->mFrameTimer;
			timer[0]   = 0;
			timer[1]   = lo + (int)((f32)(hi - lo) * (MsRandF())) + 1;
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

			const JGeometry::TVec3<f32>& fwdPos = actor->unkF4.getPoint();

			if (actor->mPosition.y <= fwdPos.y) {
				if (!mActor->checkCurBckFromIndex(1))
					mActor->setBckFromIndex(1);

				int hi     = CLBPalFrame<int>(180);
				int lo     = CLBPalFrame<int>(60);
				int* timer = actor->mFrameTimer;
				timer[0]   = 0;
				timer[1]   = lo + (int)((f32)(hi - lo) * (MsRandF())) + 1;
			} else {
				if (!mActor->checkCurBckFromIndex(0))
					mActor->setBckFromIndex(0);

				int hi     = CLBPalFrame<int>(500);
				int lo     = CLBPalFrame<int>(150);
				int* timer = actor->mFrameTimer;
				timer[0]   = 0;
				timer[1]   = lo + (int)((f32)(hi - lo) * (MsRandF())) + 1;
			}
		}
	}

	return FALSE;
}
