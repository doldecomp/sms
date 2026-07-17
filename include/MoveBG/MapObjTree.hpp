#ifndef MOVE_BG_MAP_OBJ_TREE_HPP
#define MOVE_BG_MAP_OBJ_TREE_HPP

#include <MoveBG/MapObjGeneral.hpp>

class TMapObjLeaf {
public:
	TMapObjLeaf();

public:
	/* 0x0 */ f32 unk0;                // Leaf angle
	/* 0x4 */ f32 unk4;                // Leaf angular velocity
	/* 0x8 */ TMapCollisionMove* unk8; // Leaf collision
	/* 0xC */ JGeometry::TMatrix34<JGeometry::SMatrix34C<f32> >
	    unkC; // Base matrix
};

class TMapObjTree : public TMapObjGeneral {
public:
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual f32 getRadiusAtY(f32 param_1) const
	{
		return unk148
		       + (unk14C - unk148) * (mPosition.y + mDamageHeight - param_1)
		             / mDamageHeight;
	}
	virtual void initMapObj();
	virtual void touchPlayer(THitActor*);

	int controlLeaf(int);
	void initEach();
	TMapObjTree(const char* name = "木");

	static f32 mNearMiddle;
	static f32 mMiddleFar;
	static f32 mBananaTreeJumpPower;

public:
	/* 0x148 */ f32 unk148;          // Upper canopy radius
	/* 0x14C */ f32 unk14C;          // Lower canopy radius
	/* 0x150 */ s32 unk150;          // Leaf count
	/* 0x154 */ TMapObjLeaf* unk154; // Leaf data
	/* 0x158 */ bool unk158; // Probably true when all leaf motion has settled
	/* 0x15C */ f32 unk15C;  // Probably standard leaf movement speed
	/* 0x160 */ f32 unk160;  // Hip-drop leaf movement speed
	/* 0x164 */ f32 unk164;  // Leaf return strength
	/* 0x168 */ f32 unk168;  // Leaf damping
	/* 0x16C */ u32 unk16C;
};

class TMapEventSink;

class TMapObjTreeScale : public TMapObjTree {
public:
	enum {
		STATE_UNKB = 0xB, // Fully shrunken
		STATE_UNKC = 0xC, // Growing vertically
		STATE_UNKD = 0xD, // Growing horizontally
	};

	virtual void loadAfter();
	virtual void control();
	virtual u32 touchWater(THitActor*);

	void startScaleUp()
	{
		awake();
		mActorType = 0x40000039;
		removeMapCollision();
		mState = STATE_UNKC;
	}
	void beSmall();
	TMapObjTreeScale(const char* name = "スケールの木");

	static f32 mScaleSpeedY;
	static f32 mStatusChangeScaleY;
	static f32 mScaleSpeedXZ;
	static f32 mScaleMin;

public:
	/* 0x170 */ JGeometry::TVec3<f32> unk170[30]; // Particle positions
	/* 0x2D8 */ s32 unk2D8;                       // Index into unk170
	/* 0x2DC */ s32 unk2DC;                       // Particle timer
	/* 0x2E0 */ TMapEventSink* unk2E0; // Bianco terrain-sinking event
};

#endif
