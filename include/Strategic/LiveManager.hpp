#ifndef STRATEGIC_LIVE_MANAGER_HPP
#define STRATEGIC_LIVE_MANAGER_HPP

#include <Strategic/ObjManager.hpp>

class TLiveActor;
class TSpcBinary;

class TLiveManager : public TObjManager {
public:
	TLiveManager(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);
	virtual void clipActors(JDrama::TGraphics*);
	virtual void setFlagOutOfCube();
	virtual void createSpcBinary();
	virtual bool hasMapCollision() const;

	void getActorByFlag(u32) const;
	void clipActorsAux(JDrama::TGraphics*, float, float);
	void manageActor(TLiveActor*);

public:
	/* 0x34 */ TSpcBinary* unk34;
};

#endif
