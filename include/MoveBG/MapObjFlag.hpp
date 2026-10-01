#ifndef MOVE_BG_MAP_OBJ_FLAG_HPP
#define MOVE_BG_MAP_OBJ_FLAG_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/JGeometry.hpp>
#include <Strategic/HitActor.hpp>
#include <dolphin/types.h>

class JSUMemoryInputStream;
struct ResTIMG;

class TMapObjFlag : public THitActor {
public:
	TMapObjFlag(const char* name = "旗");

	virtual void load(JSUMemoryInputStream&);
	virtual void updateVertex();

	void init(const char*);
	void update();
	void draw();
	static f32 mFlutterSpeed;

public:
	/* 0x68 */ f32 unk68;
	/* 0x6C */ f32 unk6C;
	/* 0x70 */ s32 unk70;
	/* 0x74 */ s32 unk74;
	/* 0x78 */ JGeometry::TVec3<f32>** unk78;
	/* 0x7C */ f32 unk7C;
	/* 0x80 */ f32 unk80;
	/* 0x84 */ f32 unk84;
	/* 0x88 */ f32 unk88;
	/* 0x8C */ TMtx34f unk8C;
	/* 0xBC */ s32 unkBC;
};

class TMapObjFlagManager : public JDrama::TViewObj {
public:
	class TMapObjFlagInfo {
	public:
		TMapObjFlagInfo()
		    : unk0(0)
		    , unk54(nullptr)
		{
		}

		/* 0x00 */ s32 unk0;
		/* 0x04 */ TMapObjFlag* unk4[0x14];
		/* 0x54 */ ResTIMG* unk54;
	};

	TMapObjFlagManager(const char* name = "旗管理");

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);

	void registerObj(TMapObjFlag*, const char*);
	void loadFlag(TMapObjFlagInfo*, TMapObjFlag*, const char*);
	void initDraw();

	/* 0x10 */ TMapObjFlagInfo unk10[15];
};

extern TMapObjFlagManager* gpMapObjFlagManager;

class TMapObjFlagLower : public TMapObjFlag {
public:
	virtual void updateVertex();
};

class TMapObjFlagSail : public TMapObjFlag {
public:
	virtual void updateVertex();
};

#endif // MOVE_BG_MAP_OBJ_FLAG_HPP
