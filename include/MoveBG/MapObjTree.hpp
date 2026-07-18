#ifndef MOVE_BG_MAP_OBJ_TREE_HPP
#define MOVE_BG_MAP_OBJ_TREE_HPP

#include <MoveBG/MapObjGeneral.hpp>

class TMapObjLeaf {
public:
	TMapObjLeaf();

public:
	/* 0x0 */ f32 mAngle;
	/* 0x4 */ f32 mAngularVelocity;
	/* 0x8 */ TMapCollisionMove* mCollision;
	/* 0xC */ TMtx34f mTransform;
};

class TMapObjTree : public TMapObjGeneral {
public:
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual f32 getRadiusAtY(f32 param_1) const
	{
		return mMinCanopyRadius
		       + (mMaxCanopyRadius - mMinCanopyRadius)
		             * (mPosition.y + mDamageHeight - param_1) / mDamageHeight;
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
	/* 0x148 */ f32 mMinCanopyRadius;
	/* 0x14C */ f32 mMaxCanopyRadius;
	/* 0x150 */ s32 mLeafNum;
	/* 0x154 */ TMapObjLeaf* mLeaves;
	/* 0x158 */ bool mFreezeLeaves;
	/* 0x15C */ f32 mLeafTouchImpulse;
	/* 0x160 */ f32 mLeafHipDropImpulse; // Hip-drop leaf movement speed
	/* 0x164 */ f32 mLeafStiffness;
	/* 0x168 */ f32 mLeafDamping;
	/* 0x16C */ u32 unk16C;
};

class TMapEventSink;

class TMapObjTreeScale : public TMapObjTree {
public:
	enum {
		STATE_SMALL             = 0xB,
		STATE_SCALING_UP_Y_ONLY = 0xC,
		STATE_SCALING_UP        = 0xD,
	};

	virtual void loadAfter();
	virtual void control();
	virtual u32 touchWater(THitActor*);

	void startScaleUp();
	void beSmall();
	TMapObjTreeScale(const char* name = "スケールの木");

	static f32 mScaleSpeedY;
	static f32 mStatusChangeScaleY;
	static f32 mScaleSpeedXZ;
	static f32 mScaleMin;

public:
	/* 0x170 */ JGeometry::TVec3<f32> mParticlePositions[30];
	/* 0x2D8 */ s32 mNextFreeParticlePos;
	/* 0x2DC */ s32 mParticleEmitTimer;
	/* 0x2E0 */ TMapEventSink* unk2E0; // Bianco terrain-sinking event
};

#endif
