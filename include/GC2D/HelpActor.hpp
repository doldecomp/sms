#ifndef GC2D_HELP_ACTOR_HPP
#define GC2D_HELP_ACTOR_HPP

#include <Strategic/HitActor.hpp>

class TLiveActor;

class THelpActor : public THitActor {
public:
	THelpActor(const char* = "バルーンヘルプ");
	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);
	virtual bool check();

	int getHelpID();

public:
	/* 0x68 */ u32 unk68;
	/* 0x6C */ const char* unk6C;
	/* 0x70 */ TLiveActor* unk70; // type is a guess
	/* 0x74 */ bool unk74;
};

class TSwitchHelpActor : public THelpActor {
public:
	TSwitchHelpActor(const char* name = "スイッチバルーンヘルプ");

	virtual bool check();
};

#endif
