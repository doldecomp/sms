#ifndef STRATEGIC_SPINE_HPP
#define STRATEGIC_SPINE_HPP

#include <Strategic/SolidStack.hpp>
#include <types.h>

class TSpineEnemy;
template <class T> class TNerveBase;

template <class T> class TSpineBase {
private:
	// fabricated but they probably had a global typedef for it
	typedef const TNerveBase<T>* Nerve;

	/* 0x0 */ TSpineEnemy* mBody; // whoever we are the spine of
	/* 0x4 */ TSolidStack<Nerve> mVertebrae;
	/* 0x14 */ Nerve unk14;
	/* 0x18 */ Nerve unk18;
	/* 0x1C */ Nerve unk1C;
	/* 0x20 */ int mTime;
	/* 0x24 */ // vt

public:
	TSpineBase(TSpineEnemy* owner)
	    : mBody(owner)
	    , mVertebrae(8)
	    , unk14(nullptr)
	    , unk18(0)
	    , mTime(0)
	{
	}

	TNerveBase<T>* getLatestNerve() const
	{
		if (unk14)
			return unk14;
		return unk1C;
	}

	// matching
	void pushNerve(Nerve nerve)
	{
		if (!nerve)
			return;

		if (unk14 != nullptr) {
			unk1C = unk14;
			mVertebrae.push(unk14);
		}
		mTime = 0;
		unk14 = nerve;
	}

	virtual ~TSpineBase() { }
	virtual void update();

	/////////////////////////////////////////////////
	// Everything fabricated based on enemy code, names made up,
	// I still don't completely understand how this abstraction
	// is supposed to actually work

	// fabricated
	void setNext(Nerve nerve)
	{
		if (unk14 != nullptr)
			unk1C = unk14;

		mTime = 0;
		unk14 = nerve;
	}

	// fabricated
	void initWith(Nerve nerve)
	{
		mVertebrae.clear();
		mTime = 0;
		unk14 = nerve;
		unk18 = nerve;
		unk1C = nullptr;
	}

	// fabricated
	void pushRaw(Nerve nerve)
	{
		if (nerve)
			mVertebrae.push(nerve);
	}

	// fabricated
	void reset() { mVertebrae.clear(); }

	// fabricated
	void pushDefault() { pushRaw(unk18); }

	// fabricated
	Nerve getCurrentNerve() const { return unk14; }

	// fabricated
	int getTime() const { return mTime; }
	TSpineEnemy* getBody() const { return mBody; }
};

#endif
