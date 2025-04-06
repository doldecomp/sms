#ifndef NPC_NPC_NERVE_HPP
#define NPC_NPC_NERVE_HPP

#include <Strategic/Nerve.hpp>

class TLiveActor;

class TNerveNPCGraphWander : public TNerveBase<TLiveActor> {
public:
	virtual void execute(TSpineBase<TLiveActor>*) const;
	static const TNerveNPCGraphWander& theNerve();
};

class TNerveNPCUTurn : public TNerveBase<TLiveActor> {
public:
	virtual void execute(TSpineBase<TLiveActor>*) const;
	static const TNerveNPCUTurn& theNerve();
};

class TNerveNPCGraphWait : public TNerveBase<TLiveActor> {
public:
	virtual void execute(TSpineBase<TLiveActor>*) const;
	static const TNerveNPCGraphWait& theNerve();
};

class TNerveNPCWaitContinue : public TNerveBase<TLiveActor> {
public:
	virtual void execute(TSpineBase<TLiveActor>*) const;
	static const TNerveNPCWaitContinue& theNerve();
};

class TNerveNPCWaitMarioApproach : public TNerveBase<TLiveActor> {
public:
	virtual void execute(TSpineBase<TLiveActor>*) const;
	static const TNerveNPCWaitMarioApproach& theNerve();
};

class TNerveNPCTurnToMario : public TNerveBase<TLiveActor> {
public:
	virtual void execute(TSpineBase<TLiveActor>*) const;
	static const TNerveNPCTurnToMario& theNerve();
};

class TNerveNPCWet : public TNerveBase<TLiveActor> {
public:
	virtual void execute(TSpineBase<TLiveActor>*) const;
	static const TNerveNPCWet& theNerve();
};

class TNerveNPCSink : public TNerveBase<TLiveActor> {
public:
	virtual void execute(TSpineBase<TLiveActor>*) const;
	static const TNerveNPCSink& theNerve();
};

class TNerveNPCRecoverFromSink : public TNerveBase<TLiveActor> {
public:
	virtual void execute(TSpineBase<TLiveActor>*) const;
	static const TNerveNPCRecoverFromSink& theNerve();
};

class TNerveNPCRecoverAfter : public TNerveBase<TLiveActor> {
public:
	virtual void execute(TSpineBase<TLiveActor>*) const;
	static const TNerveNPCRecoverAfter& theNerve();
};

class TNerveNPCSetPosAfterSinkBottom : public TNerveBase<TLiveActor> {
public:
	virtual void execute(TSpineBase<TLiveActor>*) const;
	static const TNerveNPCSetPosAfterSinkBottom& theNerve();
};

class TNerveNPCTalk : public TNerveBase<TLiveActor> {
public:
	virtual void execute(TSpineBase<TLiveActor>*) const;
	static const TNerveNPCTalk& theNerve();
};

class TNerveNPCThrow : public TNerveBase<TLiveActor> {
public:
	virtual void execute(TSpineBase<TLiveActor>*) const;
	static const TNerveNPCThrow& theNerve();
};

class TNerveNPCMad : public TNerveBase<TLiveActor> {
public:
	virtual void execute(TSpineBase<TLiveActor>*) const;
	static const TNerveNPCMad& theNerve();
};

class TNerveNPCBlown : public TNerveBase<TLiveActor> {
public:
	virtual void execute(TSpineBase<TLiveActor>*) const;
	static const TNerveNPCBlown& theNerve();
};

class TNerveNPCMareStand : public TNerveBase<TLiveActor> {
public:
	virtual void execute(TSpineBase<TLiveActor>*) const;
	static const TNerveNPCMareStand& theNerve();
};

#endif
