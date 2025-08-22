#ifndef STRATEGIC_SPINE_HPP
#define STRATEGIC_SPINE_HPP

#include <Strategic/SolidStack.hpp>
#include <dolphin/os.h>
#include <types.h>

class TSpineEnemy;
template <class T> class TNerveBase;

template <class T> class TSpineBase {
private:
	// fabricated but they probably had a global typedef for it
	typedef const TNerveBase<T>* Nerve;

	/* 0x0 */ TSpineEnemy* mBody; // whoever we are the spine of
	/* 0x4 */ TSolidStack<Nerve> mVertebrae;
	/* 0x14 */ Nerve mCurrent;
	/* 0x18 */ Nerve unk18;
	/* 0x1C */ Nerve mPrevious;
	/* 0x20 */ int mTime;
	/* 0x24 */ // vt

public:
	TSpineBase(TSpineEnemy* owner)
	    : mBody(owner)
	    , mVertebrae(8)
	    , mCurrent(nullptr)
	    , unk18(0)
	    , mTime(0)
	{
	}

	TNerveBase<T>* getLatestNerve() const
	{
		if (mCurrent)
			return mCurrent;
		return mPrevious;
	}

	// matching
	void pushNerve(Nerve nerve)
	{
		if (!nerve)
			return;

		if (mCurrent != nullptr) {
			mPrevious = mCurrent;
			mVertebrae.push(mCurrent);
		}
		mTime    = 0;
		mCurrent = nerve;
	}

	virtual ~TSpineBase() { }
	virtual void update()
	{
		if (mCurrent == nullptr) {
			Nerve nerve = popNerve();

			if (nerve) {
				if (mCurrent != nullptr)
					mPrevious = mCurrent;

				mCurrent = nerve;
				mTime    = 0;
			}
		}

		if (mCurrent != nullptr) {
			BOOL cont = mCurrent->execute(this);
			mTime += 1;
			if (mTime < 0)
				mTime = 1;

			if (cont) {
				mPrevious = mCurrent;
				mCurrent  = nullptr;
			}
		} else {
			OSReport("TSpineBase : broken nerve chain\n");
		}
	}

	/////////////////////////////////////////////////
	// Everything fabricated based on enemy code, names made up,
	// I still don't completely understand how this abstraction
	// is supposed to actually work

	// fabricated
	Nerve popNerve()
	{
		if (mVertebrae.size() > 0)
			return mVertebrae.pop();
		else
			return nullptr;
	}

	// fabricated
	void setNext(Nerve nerve)
	{
		if (mCurrent != nullptr)
			mPrevious = mCurrent;

		mTime    = 0;
		mCurrent = nerve;
	}

	// fabricated
	void initWith(Nerve nerve)
	{
		mVertebrae.clear();
		mTime     = 0;
		mCurrent  = nerve;
		unk18     = nerve;
		mPrevious = nullptr;
	}

	// fabricated
	void pushAfterCurrent(Nerve nerve)
	{
		if (nerve)
			mVertebrae.push(nerve);
	}

	// fabricated
	void reset() { mVertebrae.clear(); }

	// fabricated
	void setDefaultNext() { setNext(unk18); }

	// fabricated
	Nerve getCurrentNerve() const { return mCurrent; }

	// fabricated
	Nerve getDefault() const { return unk18; }

	// fabricated
	int getTime() const { return mTime; }
	TSpineEnemy* getBody() const { return mBody; }

	// fabricated
	bool isIdle() const { return mCurrent == nullptr && mVertebrae.empty(); }
};

#endif
