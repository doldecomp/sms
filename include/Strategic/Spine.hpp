#ifndef STRATEGIC_SPINE_HPP
#define STRATEGIC_SPINE_HPP

#include <Strategic/SolidStack.hpp>
#include <types.h>

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

	virtual ~TSpineBase();
	virtual void update();
};

#endif
