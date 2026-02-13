#ifndef ANIMAL_ANIMAL_MANAGER_HPP
#define ANIMAL_ANIMAL_MANAGER_HPP

#include <Enemy/EnemyManager.hpp>

class TAnimalSaveIndividual;

class TAnimalManagerBase : public TEnemyManager {
public:
	TAnimalManagerBase(const char* name);

	virtual void clipEnemies(JDrama::TGraphics*);

public:
	/* 0x54 */ f32 mViewClipNear;
	/* 0x58 */ f32* mViewClipFarPtr;
	/* 0x5C */ TAnimalSaveIndividual* mAnimalSave;
};

class TMewManager : public TAnimalManagerBase {
public:
	TMewManager(const char* name);

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void createModelData();
};

#endif
