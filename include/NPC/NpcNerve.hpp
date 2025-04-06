#ifndef NPC_NPC_NERVE_HPP
#define NPC_NPC_NERVE_HPP

#include <Enemy/SolidStack.hpp>

// TODO: these base classes should probably live somewhere else
class TSpineEnemy;
template <class T> class TNerveBase;

template <class T> class TSpineBase {
public:
	/* 0x0 */ TSpineEnemy* unk0;
	/* 0x4 */ TSolidStack<TNerveBase<T>*> unk4;
	/* 0x14 */ TNerveBase<T>* unk14;
	/* 0x18 */ u32 unk18;
	/* 0x1C */ TNerveBase<T>* unk1C;
	/* 0x20 */ u32 unk20;
	/* 0x24 */ // vt

public:
	TNerveBase<T>* getLatestNerve() const;
	void pushNerve(const TNerveBase<T>*);

	virtual ~TSpineBase();
	virtual void update();
};

template <class T> class TNerveBase {
public:
	TNerveBase() { }
	virtual ~TNerveBase() { }
	virtual void execute(TSpineBase<T>*) const = 0;
};

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
