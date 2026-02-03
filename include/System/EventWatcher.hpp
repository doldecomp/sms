#ifndef SYSTEM_EVENT_WATCHER_HPP
#define SYSTEM_EVENT_WATCHER_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>
#include <Strategic/spcinterp.hpp>

class TEventWatcher : public JDrama::TViewObj {
public:
	TEventWatcher(const char* name, const char* script);
	TEventWatcher(const char* name = "<EventWatcher>");

	virtual ~TEventWatcher() { }
	virtual void perform(u32, JDrama::TGraphics*);
	virtual void launchScript(const char*);

public:
	/* 0x10 */ TSpcTypedBinary<TEventWatcher>* mBinary;
	/* 0x10 */ TSpcTypedInterp<TEventWatcher>* mInterp;
};

#endif
