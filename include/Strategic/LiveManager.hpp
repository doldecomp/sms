#ifndef STRATEGIC_LIVE_MANAGER_HPP
#define STRATEGIC_LIVE_MANAGER_HPP

#include <Strategic/ObjManager.hpp>

class TLiveActor;
class TSpcBinary;

class TLiveManager : public TObjManager {
public:
	TLiveManager(const char* = "ライブアクターマネージャ");

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);
	virtual void clipActors(JDrama::TGraphics*);
	virtual void setFlagOutOfCube();
	virtual void createSpcBinary();
	virtual BOOL hasMapCollision() const;

	const TLiveActor* getActorByFlag(u32) const;
	void clipActorsAux(JDrama::TGraphics*, float, float);
	void manageActor(TLiveActor*);

	// fabricated
	TLiveActor* getObj(int i) { return (TLiveActor*)TObjManager::getObj(i); }
	const TLiveActor* getObj(int i) const
	{
		return (TLiveActor*)TObjManager::getObj(i);
	}

public:
	/* 0x34 */ TSpcBinary* unk34;
};

#endif
