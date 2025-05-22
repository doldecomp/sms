#ifndef STRATEGIC_TAKE_ACTOR_HPP
#define STRATEGIC_TAKE_ACTOR_HPP

#include <Strategic/HitActor.hpp>

class TTakeActor : public THitActor {
public:
	TTakeActor(const char* name)
	    : THitActor(name)
	    , unk68(nullptr)
	    , unk6C(nullptr)
	{
	}
	virtual ~TTakeActor() { }
	virtual MtxPtr getTakingMtx() = 0;
	virtual void ensureTakeSituation();
	virtual void moveRequest(const JGeometry::TVec3<f32>&);
	virtual void getRadiusAtY(float) const;

	bool isTaken() const { return unk68 != nullptr ? true : false; }

public:
	/* 0x68 */ TTakeActor* unk68;
	/* 0x6C */ TTakeActor* unk6C;
};

#endif
