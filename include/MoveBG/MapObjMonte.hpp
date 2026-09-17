#ifndef MOVE_BG_MAP_OBJ_MONTE_HPP
#define MOVE_BG_MAP_OBJ_MONTE_HPP

#include <MoveBG/MapObjBase.hpp>
#include <MoveBG/MapObjBlock.hpp>

// TODO: mark virtual methods as such

class TMapObjMonteRoot : public TMapObjBase {
public:
	void initMapObj();
	TMapObjMonteRoot(const char* name = "根っこ");
};

class TJumpMushroom : public TMapObjBase {
public:
	BOOL receiveMessage(THitActor*, unsigned long);
	void load(JSUMemoryInputStream&);
	TJumpMushroom(const char* name = "ジャンプきのこ");
};

class THangingBridgeBoard : public TLeanBlock {
public:
	void drawOneRope(const JGeometry::TVec3<f32>&) const;
	void drawRopes() const;
	void push(f32);
	void pushNeighbor(f32);
	void control();
	void calcDefaultMtx();
	void setGroundCollision();
	void initMapObj();
	THangingBridgeBoard(const char*);

	static f32 mMarioAccelY;
	static f32 mMarioHipDropAccelY;
	static f32 mReturnAccelRate;
	static f32 mSpeedDownRate;
	static f32 mRopeWidthX;
	static f32 mRopeWidthZ;
	static f32 mTexPosRate;
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
	void perform(unsigned long, JDrama::TGraphics*);
	void initMonte();
	void loadAfter();
	THangingBridge(const char* name = "つり橋");

	static f32 mRopeWidthBetweenBoards;
	static f32 mRopeWidthBetweenBoardsY;
	static int mPointNumBetweenBoards;
	static f32 mBetweenBoardsTexPosRate;
};

class TSwingBoard : public TMapObjBase {
public:
	void drawOneRope(const JGeometry::TVec3<f32>&,
	                 const JGeometry::TVec3<f32>&) const;
	void initDraw() const;
	void draw() const;
	void swing();
	void control();
	void load(JSUMemoryInputStream&);
	TSwingBoard(const char* name = "つり橋");

	static f32 mBoardWidth;
	static f32 mRopeWidthX;
	static f32 mRopeWidthZ;
	static f32 mTexPosRate;
	static f32 mReturnAccelRate;
	static f32 mSpeedDownRate;
};

class TGoalFlag : public TMapObjBase {
public:
	f32 getRadiusAtY(f32) const;
	void touchActor(THitActor*);
	void initMapObj();
	TGoalFlag(const char* name = "ゴールフラグ");
};

class TFluff : public TMapObjBase {
public:
	f32 getRadiusAtY(f32) const;
	u32 touchWater(THitActor*);
	void move();
	void kill();
	void control();
	void appear();
	void initMapObj();
	TFluff(const char*);

	static f32 mScaleUpSpeed;
	static f32 mScaleDownSpeed;
};

class TFluffManager : public TMapObjBase {
public:
	void findNextFluff();
	void control();
	void registerNextFluff(TFluff*);
	void setUpNextFluff();
	void newFluff(const char*);
	void getRandomX() const;
	void getRandomZ() const;
	void loadAfter();
	void load(JSUMemoryInputStream&);
	TFluffManager(const char* name = "特別な綿毛");

	static f32 mWindMin;
};

#endif
