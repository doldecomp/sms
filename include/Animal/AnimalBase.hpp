#ifndef ANIMAL_ANIMAL_BASE_HPP
#define ANIMAL_ANIMAL_BASE_HPP

#include <Enemy/Enemy.hpp>
#include <MarioUtil/RandomUtil.hpp>

class TAnimalManagerBase;

// fabricated
struct TRandomIntervalTimer {
	TRandomIntervalTimer()
	{
		unk0 = 0;
		unk4 = 1;
	}

	void set(int lo, int hi)
	{
		unk0 = 0;
		unk4 = MsRandI(lo, hi);
	}

	void update()
	{
		unk0 += 1;
		if (unk0 >= unk4) {
			unk0 = unk4;
		}
	}

	int unk0;
	int unk4;
};

class TAnimalBase : public TSpineEnemy {
public:
	TAnimalBase(u32, const char* name = "?");

	virtual void load(JSUMemoryInputStream& stream);
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void init(TLiveManager* manager);
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

	// fabricated
	TAnimalManagerBase* getManager() { return (TAnimalManagerBase*)mManager; }

public:
	/* 0x150 */ TRandomIntervalTimer* mFrameTimer;
};
#endif
