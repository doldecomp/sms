#ifndef STRATEGIC_LIVE_ACTOR_HPP
#define STRATEGIC_LIVE_ACTOR_HPP

#include <version.h>
#include <Strategic/TakeActor.hpp>
#include <Strategic/LiveManager.hpp>
#include <Strategic/Nerve.hpp>

// TODO: where should this live?
struct TLodAnmIndex;
class MActor;
template <class T> class TSpcTypedInterp;
class TMActorKeeper;
class MAnmSound;
class JKRFileLoader;
class TBGCheckData;
class TLodAnm;
class J3DModel;
class TBinder;
class TMapCollisionManager;

enum {
	LIVE_FLAG_BIT_DEAD = 0,
	LIVE_FLAG_BIT_HIDDEN,
	LIVE_FLAG_BIT_CLIPPED_OUT,
	LIVE_FLAG_BIT_NO_SHADOW,
	LIVE_FLAG_BIT_NO_BIND,
	LIVE_FLAG_BIT_UNK20,
	LIVE_FLAG_BIT_UNK40,
	LIVE_FLAG_BIT_AIRBORNE,
	LIVE_FLAG_BIT_ENABLE_CLIPPING,
	LIVE_FLAG_BIT_OUT_OF_AREA,
	LIVE_FLAG_BIT_FORCE_SHADOW,
	LIVE_FLAG_BIT_UNK800,
	LIVE_FLAG_BIT_IGNORE_WATER_IN_GROUND_CHECKS,
	LIVE_FLAG_BIT_ENABLE_FAST_CUBE_CULLING,
#if defined(VERSION_GMSP01) || defined(VERSION_GMSP01_NX)
	LIVE_FLAG_BIT_CALC_INT_FRAME,
#elif defined(VERSION_GMSJ01_01)
	LIVE_FLAG_BIT_UNK4000,
#endif
	LIVE_FLAG_BIT_USER0,
	LIVE_FLAG_BIT_USER1,
	LIVE_FLAG_BIT_USER2,
	LIVE_FLAG_BIT_USER3,
	LIVE_FLAG_BIT_USER4,
	LIVE_FLAG_BIT_USER5,
	LIVE_FLAG_BIT_USER6,
	LIVE_FLAG_BIT_USER7,
	LIVE_FLAG_BIT_USER8,
	LIVE_FLAG_BIT_USER9,
	LIVE_FLAG_BIT_USER10,
	LIVE_FLAG_BIT_USER11,
	LIVE_FLAG_BIT_USER12,
	LIVE_FLAG_BIT_USER13,
	LIVE_FLAG_BIT_USER14,
	LIVE_FLAG_BIT_USER15,
};

enum {
	/// Stops the actor from performing.
	LIVE_FLAG_DEAD = 1u << LIVE_FLAG_BIT_DEAD, // 0x1

	/// Stops the actor from being drawn or animated.
	LIVE_FLAG_HIDDEN = 1u << LIVE_FLAG_BIT_HIDDEN, // 0x2

	/// Set while the actor is outside of the camera's view frustum or inside of
	/// a "fast cube". Stops the actor from being animated or drawn, analogous
	/// to LIVE_FLAG_HIDDEN.
	LIVE_FLAG_CLIPPED_OUT = 1u << LIVE_FLAG_BIT_CLIPPED_OUT, // 0x4

	/// Stops the actor from casting a shadow.
	LIVE_FLAG_NO_SHADOW = 1u << LIVE_FLAG_BIT_NO_SHADOW, // 0x8

	/// Disables "binding", aka physics simulation.
	LIVE_FLAG_NO_BIND = 1u << LIVE_FLAG_BIT_NO_BIND, // 0x10

	LIVE_FLAG_UNK20 = 1u << LIVE_FLAG_BIT_UNK20, // 0x20
	LIVE_FLAG_UNK40 = 1u << LIVE_FLAG_BIT_UNK40, // 0x40

	/// Signifies that the actor is not standing on solid ground.
	LIVE_FLAG_AIRBORNE = 1u << LIVE_FLAG_BIT_AIRBORNE, // 0x80

	/// Controls whether frustum clipping is used for the actor by it's manager.
	LIVE_FLAG_ENABLE_CLIPPING = 1u << LIVE_FLAG_BIT_ENABLE_CLIPPING, // 0x100

	/// Signifies that the actor is outside of the current playable area (e.g.
	/// floors in hotel delphino) and stops it from performing whatsoever.
	LIVE_FLAG_OUT_OF_AREA = 1u << LIVE_FLAG_BIT_OUT_OF_AREA, // 0x200

	/// Forces a shadow to be drawn for the actor even if it's culled out.
	LIVE_FLAG_FORCE_SHADOW = 1u << LIVE_FLAG_BIT_FORCE_SHADOW, // 0x400

	/// Whether the actor "falls" through water during binding.
	LIVE_FLAG_IGNORE_WATER_IN_GROUND_CHECKS
	= 1u << LIVE_FLAG_BIT_IGNORE_WATER_IN_GROUND_CHECKS, // 0x1000

	/// If enabled, the actor will be culled out whenever it is in a different
	/// fast cube from mario.
	LIVE_FLAG_ENABLE_FAST_CUBE_CULLING
	= 1u << LIVE_FLAG_BIT_ENABLE_FAST_CUBE_CULLING, // 0x2000

#if defined(VERSION_GMSP01) || defined(VERSION_GMSP01_NX)
	LIVE_FLAG_CALC_INT_FRAME = 1u << LIVE_FLAG_BIT_CALC_INT_FRAME, // 0x4000
#endif
};

class TLiveActor : public TTakeActor {
public:
	TLiveActor(const char* name = "活動オブジェクト基底型");

	virtual ~TLiveActor();
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual MtxPtr getTakingMtx();
	virtual BOOL belongToGround() const;
	virtual Mtx* getRootJointMtx() const;
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	/**
	 * @brief Updates the collision geometry for the actor.
	 * @note Ground here means "terrain in general", not necessarily the ground
	 * plane. Walls and roofs are included too.
	 */
	virtual void setGroundCollision();
	/**
	 * @brief Runs the AI for the actor via it's spine. Any updates to the
	 * actor's position or rotation should be applied here, within nerves.
	 */
	virtual void control();
	/**
	 * @brief Integrates the velocity, applies it to the current frame's
	 * position delta and binds the movement to a range allowed by the scene's
	 * collision geometry.
	 */
	virtual void bind();
	/**
	 * @brief Does one simulation step for the actor, gathering various sources
	 * of motion and integrating it's velocity, position and rotation.
	 * Triggered by CUE_MOVE.
	 */
	virtual void moveObject();
	/**
	 * @brief Requests a shadow to be drawn for this actor on this frame.
	 * Triggered by CUE_CALC_VIEW
	 */
	virtual void requestShadow();
	/**
	 * @brief Draws the actor's model into it's display lists.
	 * Triggered on CUE_ENTRY.
	 */
	virtual void drawObject(JDrama::TGraphics* graphics);
	virtual void performOnlyDraw(u32 cue, JDrama::TGraphics* graphics);
	virtual u32 getShadowType();
	virtual void kill();
	virtual f32 getGravityY() const;
	virtual BOOL hasMapCollision() const;
	virtual JGeometry::TVec3<f32> getFocalPoint() const;
	virtual void updateAnmSound();
	virtual const char** getBasNameTable() const;

	void stopAnmSound();
	void setCurAnmSound();
	void setAnmSound(const char* path);
	void initAnmSound();
	int getJointTransByIndex(int, JGeometry::TVec3<f32>*) const;
	void getJointTransByName(const char*, JGeometry::TVec3<f32>*) const;
	JGeometry::TVec3<f32> calcVelocityToJumpToY(const JGeometry::TVec3<f32>&,
	                                            f32 speed, f32 gravity) const;
	void calcVelocityToJumpToXZ(const JGeometry::TVec3<f32>&, f32, f32) const;
	void perform(u32 cue, JDrama::TGraphics* graphics);
	void load(JSUMemoryInputStream&);
	void initLodAnm(const TLodAnmIndex*, int, f32);
	J3DModel* getModel() const;
	void calcRideMomentum();
	void calcRidePos();

	MActor* getMActor() const { return mMActor; }

	static f32 mVelocityMinY;

	// fabricated
	const TBGCheckData* getGroundPlane() const { return mGroundPlane; }
	f32 getGroundHeight() const { return mGroundHeight; }
	bool checkLiveFlag(u32 flag) const { return mLiveFlag & flag; }
	bool isAirborne() const
	{
		return checkLiveFlag(LIVE_FLAG_AIRBORNE) ? 1 : 0;
	}
	void offLiveFlag(u32 flag) { mLiveFlag &= ~flag; }
	void onLiveFlag(u32 flag) { mLiveFlag |= flag; }
	const TMActorKeeper* getActorKeeper() const { return mMActorKeeper; }
	TMActorKeeper* getActorKeeper() { return mMActorKeeper; }
	TLiveManager* getManager() { return mManager; }
	s16 getInstanceIndex() const { return mInstanceIndex; }
	MAnmSound* getAnmSound() { return mAnmSound; }
	TMapCollisionManager* getMapCollisionManager()
	{
		return mMapCollisionManager;
	}
	void getNextFramePosition(JGeometry::TVec3<f32>& result)
	{
		result = mPosition;
		result.add(mPositionDelta);
		// It's not clear why we copy mVelocity to a local variable first,
		// but it matches what TWireBinder::bind does.
		// We can revisit later if needed.
		JGeometry::TVec3<f32> velocity = mVelocity;
		result.add(velocity);
	}
	JGeometry::TVec3<f32> getVelocity() const { return mVelocity; }
	void setVelocity(const JGeometry::TVec3<f32>& v) { mVelocity = v; }
	void setVelocityAndFlag10(f32 x, f32 y, f32 z)
	{
		mVelocity.set(x, y, z);
		offLiveFlag(LIVE_FLAG_NO_BIND);
	}
	void setPositionDelta(const JGeometry::TVec3<f32>& v)
	{
		mPositionDelta = v;
	}
	TLodAnm* getLodAnm() { return mLodAnm; }
	const char* getBas(int idx) const
	{
		const char** basTable = getBasNameTable();
		if (!basTable)
			return nullptr;
		return basTable[idx];
	}

	enum {
		RIDE_MODE_NONE             = 0,
		RIDE_MODE_POSITION         = 1,
		RIDE_MODE_POSITION_AND_YAW = 2,
	};

public:
	/* 0x70 */ TLiveManager* mManager;
	/* 0x74 */ MActor* mMActor;
	/* 0x78 */ TMActorKeeper* mMActorKeeper;
	/* 0x7C */ s16 mInstanceIndex;
	/* 0x80 */ MAnmSound* mAnmSound;
	/* 0x84 */ const char* mAnmSoundPath;
	/* 0x88 */ TBinder* mBinder;
	/* 0x8C */ TSpineBase<TLiveActor>* mSpine;
	/* 0x90 */ TSpcTypedInterp<TLiveActor>* unk90;
	/* 0x94 */ JGeometry::TVec3<f32> mPositionDelta;
	/* 0xA0 */ JGeometry::TVec3<f32> mRotationDelta;
	/* 0xAC */ JGeometry::TVec3<f32> mVelocity;
	/* 0xB8 */ f32 mScaledBodyRadius; // TODO: imprecise name
	/* 0xBC */ f32 mBodyRadius;
	/* 0xC0 */ f32 mHeadHeight;
	/* 0xC4 */ const TBGCheckData* mGroundPlane;
	/* 0xC8 */ f32 mGroundHeight;
	/* 0xCC */ f32 mGravity;
	/* 0xD0 */ TLodAnm* mLodAnm;
	/* 0xD4 */ const TLiveActor* mGroundActor;
	/* 0xD8 */ JGeometry::TVec3<f32> mRidePos;
	/* 0xE4 */ f32 mGroundActorYaw;
	/* 0xE8 */ s8 mRideMode;
	/* 0xEC */ TMapCollisionManager* mMapCollisionManager;
	/* 0xF0 */ u32 mLiveFlag;
};

#endif
