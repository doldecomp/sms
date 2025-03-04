#ifndef JASCMDSTACK_HPP
#define JASCMDSTACK_HPP

namespace JASystem {

namespace Kernel {

	class TPortArgs;
	class TPortHead;
	class TPortCmd;

	void portCmdInit();
	void portCmdMain(void* data);
	TPortCmd* getPortCmd(TPortHead* head);
	void portCmdProcOnce(TPortHead* head);
	void portCmdProcStay(TPortHead* head);
	void portHeadInit(TPortHead* head);

	class TPortCmd {
	public:
		TPortCmd();
		void addPortCmdOnce();
		void addPortCmdStay();
		void setPortCmd(void (*func)(TPortArgs*), TPortArgs* args);
		void addPortCmd(TPortHead* head);
		void cancelPortCmd(TPortHead* head);
		void cancelPortCmdStay();
	};

} // namespace Kernel

} // namespace JASystem

#endif // JASCMDSTACK_HPP
