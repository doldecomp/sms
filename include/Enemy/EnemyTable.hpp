#ifndef ENEMY_ENEMY_TABLE_HPP
#define ENEMY_ENEMY_TABLE_HPP

#include <Strategic/NameRefPtrAry.hpp>

class TStageEnemyInfo : public JDrama::TNameRef {
public:
	TStageEnemyInfo();
	void load(JSUMemoryInputStream&);

	// fabricated
	BOOL isMatching(s32 flags) const { return flags & mFlags; }
	s32 getWeight() const { return mWeight; }

public:
	/* 0xC */ char* mManagerName;
	/* 0x10 */ s32 mFlags;
	/* 0x14 */ u32 mWeight;
};

class TStageEnemyInfoTable : public TNameRefPtrAryT<TStageEnemyInfo> {
public:
	TStageEnemyInfoTable(const char* name = "<TStageEnemyInfoTable>");

	TStageEnemyInfo* getMatchedInfo(s32);
};

#endif
