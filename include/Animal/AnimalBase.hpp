#ifndef ANIMAL_ANIMAL_BASE_HPP
#define ANIMAL_ANIMAL_BASE_HPP

#include <Enemy/Enemy.hpp>

struct TAnimalBaseUnk150 {
	TAnimalBaseUnk150()
	{
		unk0 = 0;
		unk4 = 1;
	}

	int unk0;
	int unk4;
};

class TAnimalBase : public TSpineEnemy {
public:
	TAnimalBase(u32, const char* name = "?");

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();

	void execWalk(bool);
	static void getRotationFlyToDir(JGeometry::TVec3<f32>*,
	                                const JGeometry::TVec3<f32>&, f32, f32);
	void resetRandomCurPathNode();
	void loadAfter();
	void initNoLoad_(TAnimalBase*);

	// UNUSED (fully inlined in the original; present in mario.MAP)
	void animalWalkIn();
	void flyToCurPathNode(f32, f32);

public:
	/* 0x150 */ int* mFrameTimer;
};
#endif
