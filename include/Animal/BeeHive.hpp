#ifndef ANIMAL_BEE_HIVE_HPP
#define ANIMAL_BEE_HIVE_HPP

#include <Animal/fishoid.hpp>
#include <Enemy/EnemyManager.hpp>

class TBeeHive : public TRealoid {
public:
	TBeeHive(const char* name = "ハチの巣とハチ");

	virtual TRealoidActor* createRealoidActor(MActor*);

	/* 0x158 */ char unk158[0x1C0 - 0x158];
};

class TBeeHiveManager : public TEnemyManager {
public:
	TBeeHiveManager(const char* name = "ハチの巣マネージャー");
};

#endif
