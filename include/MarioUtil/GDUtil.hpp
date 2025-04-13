#ifndef MARIO_UTIL_GD_UTIL_HPP
#define MARIO_UTIL_GD_UTIL_HPP

#include <JSystem/JKernel/JKRDisposer.hpp>
#include <dolphin/types.h>
#include <dolphin/gd.h>

class TGDLStatic {
	class TGDLSentinel : public JKRDisposer {
	public:
		/* 0x18 */ TGDLStatic* unk18;

	public:
		TGDLSentinel(TGDLStatic* param_1)
		    : unk18(param_1)
		{
		}
		~TGDLSentinel();
	};

public:
	/* 0x0 */ GDLObj mDispListObj;
	/* 0x10 */ u8 unk10;
	/* 0x11 */ u8 unk11;
	/* 0x14 */ u8* mDispList;
	/* 0x18 */ u32 mDispListSize;
	/* 0x1C */ TGDLSentinel* unk1C;

public:
	void make();
	void alloc(u32);
	virtual ~TGDLStatic();
	virtual void makeDL() = 0;
};

class TGDLDynamic {
public:
	class TGDLStaticAlt {
	public:
		void makeDL();
		~TGDLStaticAlt();
	};
};

#endif
