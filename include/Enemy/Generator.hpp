#ifndef ENEMY_GENERATOR_HPP
#define ENEMY_GENERATOR_HPP

#include <Strategic/HitActor.hpp>

class TEnemyManager;
class TGraphWeb;

class TGenerator : public JDrama::TViewObj {
public:
	TGenerator(const char* name = "<TGenerator>");

	virtual void load(JSUMemoryInputStream& stream);
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

public:
	/* 0x10 */ JGeometry::TVec3<f32> mPos;
	/* 0x1C */ JGeometry::TVec3<f32> mRot;
	/* 0x28 */ const char* mManagerName;
	/* 0x2C */ TEnemyManager* mManager;
	/* 0x30 */ const char* mGraphName;
	/* 0x34 */ TGraphWeb* mGraph;
	/* 0x38 */ s32 mInterval;
	/* 0x3C */ s32 mTimer;
};

class TOneShotGenerator : public THitActor {
public:
	TOneShotGenerator(const char* name = "<TOneShotGenerator>");

	virtual void load(JSUMemoryInputStream& stream);
	virtual void loadAfter();
	virtual BOOL receiveMessage(THitActor* sender, u32 message);

public:
	/* 0x68 */ const char* mManagerName;
	/* 0x6C */ TEnemyManager* mManager;
	/* 0x70 */ const char* mGraphName;
	/* 0x74 */ TGraphWeb* mGraph;
	/* 0x78 */ s32 mCount;
};

#endif
