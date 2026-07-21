
// This file uses reverse MAP definition order because Enemy is built with
// -inline deferred.
#include <Enemy/EnemyMario.hpp>

void TEnemyMario::changeEMDoing(u16 doing)
{
	mEMDoingTimer = 0;
	mEMDoing      = doing;
}

void TEnemyMario::startRunAway()
{
	changeEMDoing(EM_DOING_RUN_AWAY);
}

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

void TEnemyMario::damageExec(THitActor*, int, int, int, f32, int, f32, s16)
{
}
