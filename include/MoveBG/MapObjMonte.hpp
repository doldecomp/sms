#ifndef MOVE_BG_MAP_OBJ_MONTE_HPP
#define MOVE_BG_MAP_OBJ_MONTE_HPP

#include <JSystem/JGeometry.hpp>
#include <MoveBG/MapObjBase.hpp>
#include <MoveBG/MapObjBlock.hpp>

class JAISound;

class TMapObjMonteRoot : public TMapObjBase {
public:
	virtual void initMapObj();
	TMapObjMonteRoot(const char* name = "根っこ")
	    : TMapObjBase(name)
	{
	}
};

class TJumpMushroom : public TMapObjBase {
public:
	virtual void load(JSUMemoryInputStream&);
	virtual BOOL receiveMessage(THitActor*, u32);
	TJumpMushroom(const char* name = "ジャンプきのこ")
	    : TMapObjBase(name)
	{
	}
};

class THangingBridge;

class THangingBridgeBoard : public TLeanBlock {
public:
	void drawOneRope(const JGeometry::TVec3<f32>&) const;
	void drawRopes() const;
	void push(f32);
	void pushNeighbor(f32);
	virtual void setGroundCollision();
	virtual void control();
	virtual void initMapObj();
	virtual void calcDefaultMtx();
	THangingBridgeBoard(const char*);

	static f32 mMarioAccelY;
	static f32 mMarioHipDropAccelY;
	static f32 mReturnAccelRate;
	static f32 mSpeedDownRate;
	static f32 mRopeWidthX;
	static f32 mRopeWidthZ;
	static f32 mTexPosRate;

public:
	/* 0x194 */ THangingBridgeBoard* unk194;
	/* 0x198 */ THangingBridgeBoard* unk198;
	/* 0x19C */ THangingBridgeBoard* unk19C;
	/* 0x1A0 */ THangingBridgeBoard* unk1A0;
	/* 0x1A4 */ JGeometry::TVec3<f32> unk1A4[2];
	/* 0x1BC */ THangingBridge* unk1BC;
};

class THangingBridge : public JDrama::TViewObj {
public:
	void drawLowerMinus(const JGeometry::TVec3<f32>&,
	                    const JGeometry::TVec3<f32>&,
	                    const JGeometry::TVec2<f32>&, int) const;
	void drawLowerPlus(const JGeometry::TVec3<f32>&,
	                   const JGeometry::TVec3<f32>&,
	                   const JGeometry::TVec2<f32>&, int) const;
	void drawUpper(const JGeometry::TVec3<f32>&, const JGeometry::TVec3<f32>&,
	               const JGeometry::TVec2<f32>&, int) const;
	void setDrawPos(int, f32, JGeometry::TVec3<f32>*) const;
	void drawRopeBetweenBoards(f32, int) const;
	void initDraw() const;
	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);
	void initMonte();
	THangingBridge(const char* name = "つり橋");

	static f32 mRopeWidthBetweenBoards;
	static f32 mRopeWidthBetweenBoardsY;
	static int mPointNumBetweenBoards;
	static f32 mBetweenBoardsTexPosRate;
	static f32 mRopeHeight;

public:
	/* 0x10 */ s32 unk10;
	/* 0x14 */ THangingBridgeBoard** unk14;
	/* 0x18 */ JGeometry::TVec3<f32> unk18;
	/* 0x24 */ JGeometry::TVec3<f32> unk24;
	/* 0x30 */ JGeometry::TVec2<f32> unk30;
	/* 0x38 */ f32* unk38;
	/* 0x3C */ f32 unk3C;
	/* 0x40 */ f32 unk40;
	/* 0x44 */ f32 unk44;
};

class TSwingBoard : public TMapObjBase {
public:
	void drawOneRope(const JGeometry::TVec3<f32>&,
	                 const JGeometry::TVec3<f32>&) const;
	void initDraw() const;
	void swing();
	virtual void load(JSUMemoryInputStream&);
	virtual void control();
	virtual void draw() const;
	TSwingBoard(const char* name = "つり橋");

	static f32 mBoardWidth;
	static f32 mRopeWidthX;
	static f32 mRopeWidthZ;
	static f32 mTexPosRate;
	static f32 mReturnAccelRate;
	static f32 mSpeedDownRate;

public:
	/* 0x138 */ f32 unk138;
	/* 0x13C */ f32 unk13C;
	/* 0x140 */ f32 unk140;
	/* 0x144 */ f32 unk144;
	/* 0x148 */ f32 unk148;
	/* 0x14C */ TMtx34f unk14C;
	/* 0x17C */ JGeometry::TVec3<f32> unk17C;
	/* 0x188 */ JAISound* unk188;
};

class TGoalFlag : public TMapObjBase {
public:
	virtual f32 getRadiusAtY(f32) const { return 20.0f; }
	virtual void initMapObj();
	virtual void touchActor(THitActor*);
	TGoalFlag(const char* name = "ゴールフラグ")
	    : TMapObjBase(name)
	{
	}
};

class TFluffManager;

class TFluff : public TMapObjBase {
public:
	virtual f32 getRadiusAtY(f32) const { return 20.0f; }
	virtual void control();
	virtual void kill();
	virtual void appear();
	virtual void initMapObj();
	virtual u32 touchWater(THitActor*);
	void move();
	TFluff(const char*);

	static f32 mScaleUpSpeed;
	static f32 mScaleDownSpeed;

	enum {
		STATE_UNK2 = 0x2,
		STATE_UNK3 = 0x3,
		STATE_UNK4 = 0x4,
	};

public:
	/* 0x138 */ f32 unk138;
	/* 0x13C */ f32 unk13C;
	/* 0x140 */ f32 unk140;
	/* 0x144 */ f32 unk144;
	/* 0x148 */ f32 unk148;
	/* 0x14C */ f32 unk14C;
	/* 0x150 */ f32 unk150;
	/* 0x154 */ f32 unk154;
	/* 0x158 */ f32 unk158;
	/* 0x15C */ f32 unk15C;
	/* 0x160 */ f32 unk160;
	/* 0x164 */ f32 unk164;
	/* 0x168 */ TFluffManager* unk168;
	/* 0x16C */ u8 unk16C;
};

class TFluffManager : public TMapObjBase {
public:
	void findNextFluff();
	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void control();
	void registerNextFluff(TFluff*);
	void setUpNextFluff();
	TFluff* newFluff(const char*);
	f32 getRandomX() const;
	f32 getRandomZ() const;
	TFluffManager(const char* name = "特別な綿毛");

	static f32 mWindMin;

	enum {
		STATE_UNK2 = 0x2,
		STATE_UNK3 = 0x3,
	};

public:
	/* 0x138 */ f32 unk138;
	/* 0x13C */ f32 unk13C;
	/* 0x140 */ f32 unk140;
	/* 0x144 */ u32 unk144;
	/* 0x148 */ JGeometry::TVec3<f32> unk148;
	/* 0x154 */ f32 unk154;
	/* 0x158 */ TFluff* unk158;
	/* 0x15C */ TFluff* unk15C;
	/* 0x160 */ s32 unk160;
	/* 0x164 */ s32 unk164;
	/* 0x168 */ TFluff** unk168;
};

#endif
