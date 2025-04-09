#ifndef STRATEGIC_BINDER_HPP
#define STRATEGIC_BINDER_HPP

class TLiveActor;

class TBinder {
public:
	TBinder();
	virtual ~TBinder();
	virtual void bind(TLiveActor*) = 0;
};

#endif
