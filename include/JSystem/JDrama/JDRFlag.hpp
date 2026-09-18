#ifndef JDR_FLAG_HPP
#define JDR_FLAG_HPP

namespace JDrama {

template <class T> class TFlagT {
public:
	TFlagT(T v = T()) { mValue = v; }

	// TODO: retail *calls* this copy constructor (weak 0xc from
	// MarDirectorDirect.o) twice in TMarDirector::decideNextStage, for
	// TGameSequence::set's by-value TFlagT<u16> parameter, and copies the
	// argument into a second stack temporary first (`lhz`/`sth` into one slot,
	// then `bl` the copy ctor into the parameter slot) -- the two-stack-object
	// shape of docs/AGENT_MATCHING_TIPS.md "A by-value class parameter forces
	// the copy through memory". Our build elides both copies.
	//
	// Measured in a scratch TU with the game flags (probe*.cpp, a TGameSequence
	// local plus `*gSeq = s`):
	//   as written (everything in class):      no temporaries, no calls at all.
	//   + `~TFlagT() { }`:                     TGameSequence::set becomes a
	//     call with the parameter passed by address in r6, which is exactly
	//     retail's ABI in updateGameMode/changeState -- but it also emits
	//     __dt__13TGameSequenceFv, and the map has no TFlagT or TGameSequence
	//     destructor, so the class is trivially destructible.
	//   + every TFlagT member moved out of class: ctors, `set` and `operator=`
	//     all become calls; retail inlines `operator=` (its `lhz` + `bl set`
	//     is visible) and inlines the converting ctor in decideNextStage.
	//   + only the ctors and `set` out of class: reproduces
	//     decideNextStage's second block almost exactly (copy ctor call, then
	//     `lhz` + `bl set`) but still lacks the intermediate temporary, and
	//     the converting ctor comes out as a call where retail inlines it.
	//   + only this copy ctor out of class (tried in the real tree): breaks
	//     the DOL -- JDRDStage 100 -> 42 matched_code, JDREfbCtrl 100 -> 88,
	//     JDRViewConnecter 100 -> 50, TAirportEventSink::watch 99.96 -> 47,
	//     TApplication::proc 99.93 -> 94, and MarDirectorDirect itself drops
	//     0.6. The three JDrama constructors that take a TFlagT<u16> by value
	//     pin the in-class spelling.
	// So the lever is not the declaration form of this constructor. The
	// remaining suspect is the *argument* at the call sites in
	// MarDirectorDirect.cpp (what makes MWCC materialise the extra temporary),
	// not this header.
	TFlagT(const TFlagT<T>& other)
	    : mValue(other.mValue)
	{
	}

	// fabricated
	TFlagT& operator=(const TFlagT<T>& other)
	{
		set(other.mValue);
		return *this;
	}

	void set(T v) { mValue = v; }
	T get() const { return mValue; }

	void setBit(T bit, bool on)
	{
		if (on)
			mValue |= bit;
		else
			mValue &= ~bit;
	}

	bool check(T bit) const { return (mValue & bit) != 0; }
	void on(T bit) { mValue |= bit; }
	void off(T bit) { mValue &= ~bit; }

public:
	T mValue;
};

}; // namespace JDrama

#endif
