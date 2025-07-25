#ifndef STRATEGIC_SPINE_HPP
#define STRATEGIC_SPINE_HPP

#include <Strategic/SolidStack.hpp>
#include <types.h>

class TSpineEnemy;
template <class T> class TNerveBase;

template <class T> class TSpineBase {
private:
	/* 0x0 */ TSpineEnemy* unk0;
	/* 0x4 */ TSolidStack<const TNerveBase<T>*> unk4;
	/* 0x14 */ const TNerveBase<T>* unk14;
	/* 0x18 */ const TNerveBase<T>* unk18;
	/* 0x1C */ const TNerveBase<T>* unk1C;
	/* 0x20 */ int mTime;
	/* 0x24 */ // vt

public:
	TSpineBase(TSpineEnemy* owner)
	    : unk0(owner)
	    , unk4(8)
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
	void pushNerve(const TNerveBase<T>* nerve)
	{
		if (!nerve)
			return;

		if (unk14 != nullptr) {
			unk1C = unk14;
			unk4.push(unk14);
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
	void setNext(const TNerveBase<T>* nerve)
	{
		if (unk14 != nullptr)
			unk1C = unk14;

		mTime = 0;
		unk14 = nerve;
	}

	// fabricated
	void initWith(const TNerveBase<T>* nerve)
	{
		unk4.clear();
		mTime = 0;
		unk14 = nerve;
		unk18 = nerve;
		unk1C = nullptr;
	}

	// fabricated
	void pushRaw(const TNerveBase<T>* nerve)
	{
		if (nerve)
			unk4.push(nerve);
	}

	// fabricated
	void reset() { unk4.clear(); }

	// fabricated
	void pushDefault()
	{
		if (const TNerveBase<T>* def = unk18)
			unk4.push(def);
	}

	// fabricated
	const TNerveBase<T>* getCurrentNerve() const { return unk14; }

	// fabricated
	int getTime() const { return mTime; }
	TSpineEnemy* getBody() const { return unk0; }
};

#endif
