#include <Enemy/EnemyTable.hpp>
#include <Enemy/Conductor.hpp>
#include <stdlib.h>

void TStageEnemyInfo::load(JSUMemoryInputStream& stream)
{
	JDrama::TNameRef::load(stream);
	char buffer[256];
	stream.readString(buffer, 256);
	mManagerName = new char[1 + strlen(buffer)];
	strcpy(mManagerName, buffer);
	stream.read(&mFlags, 4);
	stream.read(&mWeight, 4);
}

TStageEnemyInfoTable::TStageEnemyInfoTable(const char* name)
    : TNameRefPtrAryT<TStageEnemyInfo>(name)
{
	gpConductor->registerEnemyInfoTable(this);
}

TStageEnemyInfo* TStageEnemyInfoTable::getMatchedInfo(s32 param_1)
{
	s32 weightSum = 0;
	for (TStageEnemyInfo** it = begin(); it != end(); ++it) {
		TStageEnemyInfo* info = *it;
		if (info->isMatching(param_1))
			weightSum += info->getWeight();
	}

	if (weightSum == 0)
		return nullptr;

	s32 x = rand() * (1.f / (RAND_MAX + 1)) * weightSum;
	for (TStageEnemyInfo** it = begin(); it != end(); ++it) {
		TStageEnemyInfo* info = *it;
		if (info->isMatching(param_1)) {
			x -= info->getWeight();
			if (x <= 0)
				return info;
		}
	}

	return nullptr;
}
