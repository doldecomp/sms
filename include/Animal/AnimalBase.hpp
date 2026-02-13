#ifndef ANIMAL_ANIMAL_BASE_HPP
#define ANIMAL_ANIMAL_BASE_HPP

#include <Enemy/Enemy.hpp>

class TAnimalBase : public TSpineEnemy {
public:
	TAnimalBase(u32, const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();

	void execWalk(bool);
	void getRotationFlyToDir(JGeometry::TVec3<f32>*,
	                         const JGeometry::TVec3<f32>&, f32, f32);
	void resetRandomCurPathNode();
	void loadAfter();

public:
	/* 0x150 */ int* mFrameTimer;
};

#endif
