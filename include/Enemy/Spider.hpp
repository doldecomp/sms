#ifndef ENEMY_SPIDER_HPP
#define ENEMY_SPIDER_HPP

#include <dolphin/types.h>
#include <Strategic/Binder.hpp>

class TBGCheckData;

class TSpider : public TBinder {
public:
	TSpider();
	~TSpider();
	virtual void bind(TLiveActor*);

public:
	/* 0x4 */ int unk4;
	/* 0x8 */ int unk8;
	/* 0xC */ TBGCheckData* unkC;
	/* 0x10 */ f32 unk10;
};

#endif
