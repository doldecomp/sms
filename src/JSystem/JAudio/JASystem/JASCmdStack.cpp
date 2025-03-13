#include <JSystem/JAudio/JASystem/JASCmdStack.hpp>
#include <JSystem/JAudio/JASystem/JASCallback.hpp>
#include <dolphin/os.h>
#include <types.h>

namespace JASystem {

namespace Kernel {

	JASystem::Kernel::TPortHead cmd_once;
	JASystem::Kernel::TPortHead cmd_stay;

	static s32 portCmdMain(void* data);
	static TPortCmd* getPortCmd(TPortHead* head);

	TPortCmd::TPortCmd()
	{
		unk0 = nullptr;
		unk4 = nullptr;
		unk8 = nullptr;
		unkC = nullptr;
	}

	BOOL TPortCmd::addPortCmdOnce() { return addPortCmd(&cmd_once); }

	BOOL TPortCmd::addPortCmdStay() { return addPortCmd(&cmd_stay); }

	BOOL TPortCmd::setPortCmd(PortCallback func, TPortArgs* args)
	{
		unk8 = func;
		unkC = args;
		unk0 = nullptr;
		return true;
	}

	BOOL TPortCmd::addPortCmd(TPortHead* head)
	{
		BOOL enable = OSDisableInterrupts();
		if (unk0) {
			OSRestoreInterrupts(enable);
			return false;
		}

		if (head->unk4)
			head->unk4->unk4 = this;
		else
			head->unk0 = this;

		head->unk4 = this;
		unk4       = nullptr;
		unk0       = head;
		OSRestoreInterrupts(enable);
		return true;
	}

	void TPortCmd::cancelPortCmd(TPortHead* head) { }

	void TPortCmd::cancelPortCmdStay() { }

	void portCmdProcOnce(TPortHead* head)
	{
		for (;;) {
			TPortCmd* cmd = getPortCmd(head);
			if (!cmd)
				break;

			cmd->unk8(cmd->unkC);
		}
	}

	void portCmdProcStay(TPortHead* head)
	{
		TPortCmd* cmd = head->unk0;
		for (;;) {
			if (!cmd)
				break;

			cmd->unk8(cmd->unkC);

			cmd = cmd->unk4;
		}
	}

	void portHeadInit(TPortHead* head)
	{
		head->unk0 = nullptr;
		head->unk4 = nullptr;
	}

	void portCmdInit()
	{
		portHeadInit(&cmd_once);
		portHeadInit(&cmd_stay);
		registerAiCallback(portCmdMain, nullptr);
	}

	static TPortCmd* getPortCmd(TPortHead* head)
	{
		TPortCmd* r30 = nullptr;

		if (head->unk0) {
			TPortCmd* r31 = head->unk0;
			r30           = r31;
			head->unk0    = r31->unk4;
			if (!head->unk0)
				head->unk4 = nullptr;

			r31->unk0 = nullptr;
		}
		return r30;
	}

	static s32 portCmdMain(void* data)
	{
		// TODO: inlines inside of portCmdProc*
		// (but does it matter?)
		char trash[0x30];

		portCmdProcOnce(&cmd_once);
		portCmdProcStay(&cmd_stay);
		return 0;
	}

} // namespace Kernel

} // namespace JASystem
