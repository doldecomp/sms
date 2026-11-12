#include <JSystem/JAudio/JASystem/JASCmdStack.hpp>
#include <JSystem/JAudio/JASystem/JASCallback.hpp>
#include <dolphin/os.h>
#include <dolphin/types.h>

static JASystem::Kernel::TPortHead cmd_once;
static JASystem::Kernel::TPortHead cmd_stay;

namespace JASystem {

namespace Kernel {

	static s32 portCmdMain(void* data);
	static TPortCmd* getPortCmd(TPortHead* queue);

	TPortCmd::TPortCmd()
	{
		mQueue    = nullptr;
		mNext     = nullptr;
		mCallback = nullptr;
		mArgs     = nullptr;
	}

	BOOL TPortCmd::addPortCmdOnce() { return addPortCmd(&cmd_once); }

	BOOL TPortCmd::addPortCmdStay() { return addPortCmd(&cmd_stay); }

	BOOL TPortCmd::setPortCmd(PortCallback callback, TPortArgs* args)
	{
		mCallback = callback;
		mArgs     = args;
		mQueue    = nullptr;
		return true;
	}

	BOOL TPortCmd::addPortCmd(TPortHead* queue)
	{
		BOOL interruptsWereEnabled = OSDisableInterrupts();
		if (mQueue) {
			OSRestoreInterrupts(interruptsWereEnabled);
			return false;
		}

		if (queue->mLast)
			queue->mLast->mNext = this;
		else
			queue->mFirst = this;

		queue->mLast = this;
		mNext        = nullptr;
		mQueue       = queue;
		OSRestoreInterrupts(interruptsWereEnabled);
		return true;
	}

	void TPortCmd::cancelPortCmd(TPortHead* queue) { }

	void TPortCmd::cancelPortCmdStay() { }

	void portCmdProcOnce(TPortHead* queue)
	{
		for (;;) {
			TPortCmd* cmd = getPortCmd(queue);
			if (!cmd)
				break;

			cmd->getFunc()(cmd->getArgs());
		}
	}

	void portCmdProcStay(TPortHead* queue)
	{
		TPortCmd* cmd = queue->mFirst;
		for (;;) {
			if (!cmd)
				break;

			cmd->getFunc()(cmd->getArgs());

			cmd = cmd->getNext();
		}
	}

	void portHeadInit(TPortHead* queue)
	{
		queue->mFirst = nullptr;
		queue->mLast  = nullptr;
	}

	void portCmdInit()
	{
		portHeadInit(&cmd_once);
		portHeadInit(&cmd_stay);
		registerAiCallback(portCmdMain, nullptr);
	}

	static TPortCmd* getPortCmd(TPortHead* queue)
	{
		TPortCmd* result = nullptr;

		if (queue->mFirst) {
			TPortCmd* first = queue->mFirst;
			result          = first;
			queue->mFirst   = first->mNext;
			if (!queue->mFirst)
				queue->mLast = nullptr;

			first->mQueue = nullptr;
		}
		return result;
	}

	static s32 portCmdMain(void* data)
	{
		portCmdProcOnce(&cmd_once);
		portCmdProcStay(&cmd_stay);
		return 0;
	}

} // namespace Kernel

} // namespace JASystem
