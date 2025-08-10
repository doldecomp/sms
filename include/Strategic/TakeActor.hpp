#ifndef STRATEGIC_TAKE_ACTOR_HPP
#define STRATEGIC_TAKE_ACTOR_HPP

#include <Strategic/HitActor.hpp>

class TTakeActor : public THitActor {
public:
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
	virtual bool moveRequest(const JGeometry::TVec3<f32>& where_to)
	{
		mPosition = where_to;
		return true;
	}
	virtual f32 getRadiusAtY(f32) const;

	BOOL isTaken() const { return mHolder != nullptr ? TRUE : FALSE; }
	bool isHolding() const { return mHeldObject != nullptr ? true : false; }
	TTakeActor* getHolder() { return mHolder; }
	TTakeActor* getHeldObject() { return mHeldObject; }

public:
	/* 0x68 */ TTakeActor* mHolder;
	/* 0x6C */ TTakeActor* mHeldObject;
};

#endif
