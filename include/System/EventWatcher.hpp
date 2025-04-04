#ifndef SYSTEM_EVENT_WATCHER_HPP
#define SYSTEM_EVENT_WATCHER_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>

class TEventWatcher : JDrama::TViewObj {
public:
	TEventWatcher(const char*, const char*);
	TEventWatcher(const char*);

	virtual ~TEventWatcher() { }
	virtual void perform(u32, JDrama::TGraphics*);
	virtual void launchScript(const char*);
};

#endif
