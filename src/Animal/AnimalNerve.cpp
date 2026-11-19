#include <Animal/AnimalNerve.hpp>
#include <Animal/AnimalBase.hpp>
#include <Animal/AnimalManager.hpp>
#include <Animal/AnimalSave.hpp>
#include <M3DUtil/MActor.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <Camera/cameralib.hpp>
#include <Strategic/Spine.hpp>

DEFINE_NERVE(TNerveAnimalGraphWander, TLiveActor)
{
	TAnimalBase* actor = (TAnimalBase*)spine->getBody();
	MActor* mActor     = actor->getMActor();

	TAnimalManagerBase* manager = actor->getManager();
	int count                   = manager->mAnimalSave->mSLSharedAnmNum.get();

	if (count != 0 && actor->getInstanceIndex() >= count) {
		TLiveActor* other = manager->getObj(actor->getInstanceIndex() % count);

		int anmIdx = other->getMActor()->getCurAnmIdx(ANM_TYPE_BCK);
		mActor->setBckFromIndex(anmIdx);

		J3DFrameCtrl* myCtrl = mActor->getFrameCtrl(ANM_TYPE_BCK);
		J3DFrameCtrl* otherCtrl
		    = other->getMActor()->getFrameCtrl(ANM_TYPE_BCK);
		myCtrl->setFrame(otherCtrl->getFrame());
	} else if (actor->getActorType() != ACTOR_TYPE_ANIMAL_MEW) {
		// not the right type, skip animation setup
	} else {
		if (spine->getTime() == 0) {
			actor->mFrameTimer->set(CLBPalFrame<int>(150),
			                        CLBPalFrame<int>(500));
		}

		actor->mFrameTimer->update();

		BOOL anmEndsNext = mActor->curAnmEndsNext(ANM_TYPE_BCK, nullptr);

		switch (mActor->getCurAnmIdx(ANM_TYPE_BCK)) {
		case 1:
			if (anmEndsNext) {
				mActor->setBck(0);
				actor->mFrameTimer->set(CLBPalFrame<int>(150),
				                        CLBPalFrame<int>(500));
			}
			break;
		}
	}

	actor->execWalk(true);

	if (!actor->mGoalPath.empty()) {
		if (actor->getCurrentTargetDistance() < 200.0f)
			actor->switchNextGoalPathNode();
	} else {

		if (actor->getGoalDistance() < 100.0f) {
			actor->goToRandomNextGraphNode();
			actor->resetRandomCurPathNode();

			if (actor->mPosition.y <= actor->getCurrentTarget().getPoint().y) {
				mActor->setBck(1);
				actor->mFrameTimer->set(CLBPalFrame<int>(60),
				                        CLBPalFrame<int>(180));
			} else {
				mActor->setBck(0);
				actor->mFrameTimer->set(CLBPalFrame<int>(150),
				                        CLBPalFrame<int>(500));
			}
		}
	}
	return FALSE;
}
