#ifndef NPC_NPC_EVENT_HPP
#define NPC_NPC_EVENT_HPP

#include <System/EventWatcher.hpp>

class TNpcEvent {
public:
	static void reviveOneSunflower();
	void initDownSunflowerNum();
	static void initNpcBuiltin(TSpcTypedBinary<TEventWatcher>*);
};

#endif
