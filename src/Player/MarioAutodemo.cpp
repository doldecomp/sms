#include <Player/MarioMain.hpp>
#include <Enemy/Conductor.hpp>
#include <System/MarDirector.hpp>

BOOL TMario::winDemo()
{
	switch (mActionState) {
	case 0:
		if (mHeldObject != nullptr) {
			mHeldObject->receiveMessage(mHeldObject, 0xD);
			mHeldObject = nullptr;
		}
		gpConductor->killEnemiesWithin(mPosition, 2000.0f);
		if (jumpProcess(0) == TRUE) {
			gpMarDirector->fireGetStar((TShine*)unk384);
			unk384->receiveMessage(this, 0x4);
			mActionState = 1;
		}
		break;
	case 1:
		setAnimation(0xCD, 1.0f);
		stopProcess();
		break;
	}

	return FALSE;
}
