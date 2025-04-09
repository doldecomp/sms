#ifndef ENEMY_SPIDER_HPP
#define ENEMY_SPIDER_HPP

#include <dolphin/types.h>
#include <Strategic/Binder.hpp>

class TSpider : public TBinder {
public:
	TSpider();
	~TSpider();
	virtual void bind(TLiveActor*);

public:
	/* 0x4 */ u32 unk4;
	/* 0x8 */ u32 unk8;
	/* 0xC */ u32 unkC;
	/* 0x10 */ f32 unk10;
};

#endif
