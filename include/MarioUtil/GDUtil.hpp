#ifndef MARIO_UTIL_GD_UTIL_HPP
#define MARIO_UTIL_GD_UTIL_HPP

#include <JSystem/JKernel/JKRDisposer.hpp>
#include <dolphin/types.h>
#include <dolphin/gd.h>
#include <dolphin/gx.h>

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
	TGDLStatic(u32 size)
	    : unk10(0)
	    , unk11(0)
	    , mDispList(nullptr)
	    , mDispListSize(0)
	    , unk1C(nullptr)
	{
		alloc(size);
	}

	// fabricated name; the body is proven, the name is not
	void callDL()
	{
		if (!unk10)
			make();
		GXCallDisplayList(mDispListObj.start,
		                  mDispListObj.ptr - mDispListObj.start);
	}

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
