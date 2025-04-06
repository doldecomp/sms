#ifndef ENEMY_SOLID_STACK_HPP
#define ENEMY_SOLID_STACK_HPP

#include <dolphin/types.h>

template <class T> class TSolidStack {
	/* 0x0 */ u32 unk0;
	/* 0x4 */ u32 unk4;
	/* 0x8 */ T* unk8;
	/* 0xC */ // vt
public:
	T pop() { }
	void push(T value)
	{
		if (unk4 < unk0) {
			unk8[unk4] = value;
			++unk4;
		}
	}

	virtual ~TSolidStack() { }
};

#endif
