#ifndef STRATEGIC_SPINE_HPP
#define STRATEGIC_SPINE_HPP

#include <Strategic/SolidStack.hpp>
#include <types.h>

class TSpineEnemy;
template <class T> class TNerveBase;

template <class T> class TSpineBase {
public:
	/* 0x0 */ TSpineEnemy* unk0;
	/* 0x4 */ TSolidStack<const TNerveBase<T>*> unk4;
	/* 0x14 */ const TNerveBase<T>* unk14;
	/* 0x18 */ const TNerveBase<T>* unk18;
	/* 0x1C */ const TNerveBase<T>* unk1C;
	/* 0x20 */ u32 unk20;
	/* 0x24 */ // vt

public:
	TSpineBase(TSpineEnemy* owner)
	    : unk0(owner)
	    , unk4(8)
	    , unk14(nullptr)
	    , unk18(0)
	    , unk20(0)
	{
	}

	TNerveBase<T>* getLatestNerve() const;
	void pushNerve(const TNerveBase<T>* nerve)
	{
		if (!nerve)
			return;

		if (unk14 != nullptr) {
			unk1C = unk14;
			unk4.push(unk14);
		}
		unk20 = 0;
		unk14 = nerve;
	}

	virtual ~TSpineBase() { }
	virtual void update();

	// fabricated
	void reset() { unk4.clear(); }

	// fabricated
	void pushDefault()
	{
		if (const TNerveBase<T>* def = unk18)
			unk4.push(def);
	}
};

#endif
