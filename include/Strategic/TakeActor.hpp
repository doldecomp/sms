#ifndef STRATEGIC_TAKE_ACTOR_HPP
#define STRATEGIC_TAKE_ACTOR_HPP

#include <Strategic/HitActor.hpp>

class TTakeActor : public THitActor {
public:
	// The map has this ctor as UNUSED 0x50 in MapWireManager.cpp, so retail
	// emitted exactly one dead copy of it. Header round 16 ruled out both
	// header spellings and located the cause in that TU instead:
	//   * out-of-class in this header without `inline` does emit a size-exact
	//     0x50 copy, but as a *global* in all 205 objects that include the
	//     header, where the map wants one; the map does list duplicate UNUSED
	//     functions (SMS_getNormalStage__FUl, IsMember), so one entry means
	//     one emission. DOL stayed exact, nothing moved.
	//   * in-class (this form) emits no copy at all, because MWCC only emits
	//     the out-of-line body of a non-virtual in-class inline when some
	//     site fails to inline it. TTakeActor's virtuals do get weak copies.
	// The single emission belongs to MapWireManager.cpp: `loadAfter` inlines
	// `entry` (UNUSED 0x144) which inlines `TMapWireActorManager`'s ctor
	// (UNUSED 0x154), and the emitted-but-stripped copies of those two are
	// what reference a TTakeActor ctor at depth >= 2. Dropping
	// `TMapWireActor::TMapWireActor`'s `#pragma dont_inline` reproduces all of
	// it -- weak `__ct__10TTakeActorFPCc` at exactly 0x50 in that one object
	// and the manager ctor at exactly 0x154 (from 284) -- but costs the exact
	// `TMapWireManager::loadAfter` (100 -> 82.2) and pushes `entry` 320 -> 352
	// against 0x144, because retail's split is per site and the pragma is not.
	// So it stays a MapWireManager.cpp depth problem, not a header one.
	TTakeActor(const char* name)
	    : THitActor(name)
	    , mHolder(nullptr)
	    , mHeldObject(nullptr)
	{
	}
	virtual ~TTakeActor() { }
	virtual MtxPtr getTakingMtx() = 0;
	virtual void ensureTakeSituation()
	{
		if (mHeldObject != nullptr && mHeldObject->mHolder != this)
			mHeldObject = nullptr;

		if (mHolder != nullptr && mHolder->mHeldObject != this)
			mHolder = nullptr;
	}
	virtual BOOL moveRequest(const JGeometry::TVec3<f32>& where_to)
	{
		mPosition = where_to;
		return true;
	}
	virtual f32 getRadiusAtY(f32) const;

	// A two-`return` body is refused expansion on the right of a `||`/`&&`
	// chain at every depth, which is why retail `bl`s this from
	// TFireWanwan::updateRumble (the map's only weak isTaken, 0x1c from
	// fireWanwan.cpp). The equivalent ternary always expands.
	BOOL isTaken() const
	{
		if (mHolder != nullptr)
			return TRUE;
		return FALSE;
	}
	bool isHolding() const { return mHeldObject != nullptr ? true : false; }
	TTakeActor* getHolder() { return mHolder; }
	TTakeActor* getHeldObject() { return mHeldObject; }

public:
	/* 0x68 */ TTakeActor* mHolder;
	/* 0x6C */ TTakeActor* mHeldObject;
};

#endif
