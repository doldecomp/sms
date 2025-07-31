#ifndef ENEMY_WALKER_HPP
#define ENEMY_WALKER_HPP

#include <Strategic/SolidStack.hpp>
#include <Strategic/Binder.hpp>

class TBGCheckData;
class TSpider;

class TWalker : public TBinder {
public:
	TWalker();
	~TWalker();
	void reset();
	virtual void bind(TLiveActor*);
	void setMode(int);

public:
	/* 0x4 */ TSolidStack<const TBGCheckData*> unk4;
	/* 0x14 */ u32 unk14;
	/* 0x18 */ f32 unk18;
	/* 0x1C */ int unk1C;
	/* 0x20 */ u32 unk20;
	/* 0x24 */ u32 unk24;
	/* 0x28 */ s16 unk28;
	/* 0x2C */ TSpider* unk2C;
	/* 0x30 */ u8 unk30;
};

#endif
