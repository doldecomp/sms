#ifndef MOVE_BG_MAP_OBJ_POLE_HPP
#define MOVE_BG_MAP_OBJ_POLE_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>
#include <Strategic/HitActor.hpp>

struct ResTIMG;

class TMapObjPole : public THitActor {
public:
	void draw();
	void load(JSUMemoryInputStream&);
	TMapObjPole(const char*);

public:
	/* 0x68 */ f32 unk68;
	/* 0x6C */ f32 unk6C;
};

class TMapObjPoleManager;

extern TMapObjPoleManager* gpMapObjPoleManager;

class TMapObjPoleManager : public JDrama::TViewObj {
public:
	class TMapObjPoleInfo {
	public:
		TMapObjPoleInfo()
		    : unk0(0)
		    , unk54(0)
		{
		}

	public:
		/* 0x0 */ int unk0;
		/* 0x4 */ TMapObjPole* unk4[20];
		/* 0x54 */ ResTIMG* unk54;
	};

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);

	void initDraw();
	void loadPole(TMapObjPoleManager::TMapObjPoleInfo*, TMapObjPole*,
	              const char*);
	void registerObj(TMapObjPole*);
	TMapObjPoleManager(const char*);

public:
	/* 0x10 */ TMapObjPoleInfo unk10[1];
};

#endif
