
// This file uses reverse MAP definition order because Enemy is built with
// -inline deferred.
#include <Enemy/EnemyMario.hpp>
#include <Enemy/Emario.hpp>
#include <Enemy/Graph.hpp>
#include <Map/MapData.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/MarDirector.hpp>
#include <System/MSoundMainSide.hpp>

void TEnemyMario::changeEMDoing(u16 doing)
{
	mEMDoingTimer = 0;
	mEMDoing      = doing;
}

void TEnemyMario::startDisappear(u16 doing)
{
	mDisappearPosition = mPosition;

	u8 currentMap      = gpMarDirector->getCurrentMap();
	u8 currentStage    = gpMarDirector->getCurrentStage();
	bool keepBossLives = false;
	if (currentMap == 1 && currentStage == 1) {
		keepBossLives = true;
	} else if (currentMap == 1 && currentStage == 9) {
		keepBossLives = true;
	}

	if (!keepBossLives) {
		MSMainProc::setBossLivesFlag(false);
	}

	gpMarioParticleManager->emitAndBindToPosPtr(0xED, &mDisappearPosition, 0,
	                                            nullptr);
	changeEMDoing(doing);
}

void TEnemyMario::startRunAway() { changeEMDoing(EM_DOING_RUN_AWAY); }

u8 TEnemyMario::thinkTrample()
{
	if (mEMFlags != 0) {
		return FALSE;
	}

	if (mEMDoing == EM_DOING_DOWN_WAIT_TO_TALK) {
		--mTrampleCount;
		if (mTrampleCount > 0) {
			changeEMDoing(EM_DOING_TRAMPLED);
		}

		return TRUE;
	}

	return FALSE;
}

void TEnemyMario::reachGoal()
{
	mGoalFlags |= 1;
	changeEMDoing(EM_DOING_GOAL);
}

void TEnemyMario::checkReturn()
{
	if (!mGroundPlane->checkFlag(BG_CHECK_FLAG_ILLEGAL)) {
		return;
	}

	TGraphWeb* graph = mEMario->unk124->getGraph();
	int nodeIndex    = graph->findNearestNodeIndex(mPosition, -1);
	bool searching   = true;
	while (searching) {
		Vec point;
		graph->getGraphNode(nodeIndex).getPoint(&point);

		if (JGeometry::TVec3<f32>(point - *gpMarioPos).length() > 1000.0f) {
			searching = false;
			mPosition = point;
		}

		nodeIndex = (nodeIndex + 1) % graph->getNodeNum();
	}
}

void TEnemyMario::damageExec(THitActor*, int, int, int, f32, int, f32, s16) { }
