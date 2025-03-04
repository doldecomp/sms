#include <JSystem/JAudio/JASystem/JASCmdStack.hpp>

namespace JASystem {

namespace Kernel {

	void portCmdInit() { }

	void portCmdMain(void* data) { }

	TPortCmd* getPortCmd(TPortHead* head) { return 0; }

	void portCmdProcOnce(TPortHead* head) { }

	void portCmdProcStay(TPortHead* head) { }

	void portHeadInit(TPortHead* head) { }

	TPortCmd::TPortCmd() { }

	void TPortCmd::addPortCmdOnce() { }

	void TPortCmd::addPortCmdStay() { }

	void TPortCmd::setPortCmd(void (*func)(TPortArgs*), TPortArgs* args) { }

	void TPortCmd::addPortCmd(TPortHead* head) { }

	void TPortCmd::cancelPortCmd(TPortHead* head) { }

	void TPortCmd::cancelPortCmdStay() { }

} // namespace Kernel

} // namespace JASystem
