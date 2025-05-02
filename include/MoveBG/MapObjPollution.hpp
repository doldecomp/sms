#ifndef MOVE_BG_MAP_OBJ_POLLUTION_HPP
#define MOVE_BG_MAP_OBJ_POLLUTION_HPP

#include <MoveBG/MapObjBase.hpp>

struct ResTIMG;

class TPolluterBase : public TMapObjBase {
public:
	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);

	TPolluterBase(const char*);

public:
	/* 0x138 */ MActor* unk138;
};

class TRevivalPolluter {
public:
	void pollute();
	void registerPolluteTex();
	void loadInfo(JSUMemoryInputStream&);

	TRevivalPolluter();

public:
	/* 0x0 */ u32 unk0;
	/* 0x4 */ ResTIMG* unk4;
	/* 0x8 */ u16 unk8;
	/* 0xC */ f32 unkC;
	/* 0x10 */ f32 unk10;
	/* 0x14 */ f32 unk14;
	/* 0x18 */ u32 unk18;
};

class TMapObjRevivalPollution : public JDrama::TViewObj {
public:
	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);

	TMapObjRevivalPollution(const char*);

public:
	/* 0x10 */ s32 unk10;
	/* 0x14 */ TRevivalPolluter* unk14;
};

#endif
