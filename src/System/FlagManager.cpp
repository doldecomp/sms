//#include "os/OSRtc.h"
//#include "os/OSTime.h"
#include "Runtime.PPCEABI.H/__mem.h"
#include "System/FlagManager.hpp"
#include "System/StageUtil.hpp"

TFlagManager* TFlagManager::smInstance = 0;

TFlagManager::TFlagManager() { }

TFlagManager* TFlagManager::start(JKRHeap* heap)
{
	u8 _padding[8];
	if (smInstance == NULL) {
		TFlagManager* fm = new (heap, 0) TFlagManager();

		if (fm != NULL) {
			fm->firstStart();
			fm->initOpt();
		}

		smInstance = fm;
	}

	return smInstance;
}

void TFlagManager::resetCard()
{
	for (int i = 0; i < 119; ++i) {
		this->mCardBools[i] = 0;
	}

	for (int i = 0; i < 21; ++i) {
		this->mCardInts[i] = 0;
	}

	this->mLastSaveTime       = 0;
	this->mLastSaveTimeBackup = 0;

	this->resetGame();
}

void TFlagManager::resetGame()
{
	for (int i = 0; i < 4; ++i) {
		this->mGameBools[i] = 0;
	}

	for (int i = 0; i < 5; ++i) {
		this->mGameInts[i] = 0;
	}

	this->resetStage();
}

void TFlagManager::resetStage()
{
	for (int i = 0; i < 13; ++i) {
		this->mStageBools[i] = 0;
	}

	for (int i = 0; i < 100; ++i) {
		this->mStageInts[i] = 0;
	}
}

s32 TFlagManager::getFlag(u32 flag) const
{
	u32 low = flag & 0xFFFF;
	switch (flag >> 16) {
	case 1:
		if (flag < 0x103B4) {
			return this->mCardBools[low >> 3] >> (low & 7) & 1;
		}
		break;
	case 2:
		if (flag < 0x20015) {
			return this->mCardInts[low];
		}
		break;
	case 3:
		if (flag < 0x3001D) {
			return this->mGameBools[low >> 3] >> (low & 7) & 1;
		}
		break;
	case 4:
		if (flag < 0x40005) {
			return this->mGameInts[low];
		}
		break;
	case 5:
		if (flag < 0x50064) {
			return this->mStageBools[low >> 3] >> (low & 7) & 1;
		}
		break;
	case 6:
		if (flag < 0x60064) {
			return this->mStageInts[low];
		}
		break;
	case 7:
		if (flag < 0x70002) {
			return this->mSavedOptionBools[low >> 3] >> (low & 7) & 1;
		}
		break;
	case 8:
		if (flag < 0x80001) {
			return this->mSavedOptionInts[low];
		}
		break;
	case 9:
		if (flag < 0x90001) {
			return this->mOptionBools[low >> 3] >> (low & 7) & 1;
		}
		break;
	case 10:
		if (flag < 0xA0002) {
			return this->mOptionInts[low];
		}
		break;
	}
	return 0;
}

void TFlagManager::setFlag(u32 flag, s32 value)
{
	u32 low = flag & 0xFFFF;
	switch (flag >> 16) {
	case 1:
		if (flag < 0x103B4) {
			this->mCardBools[low >> 3] &= ~(1 << (low & 7));
			this->mCardBools[low >> 3] |= (value & 1) << (low & 7);
		}
		break;
	case 2:
		if (flag < 0x20015) {
			this->mCardInts[low] = value;
		}
		break;
	case 3:
		if (flag < 0x3001D) {
			this->mGameBools[low >> 3] &= ~(1 << (low & 7));
			this->mGameBools[low >> 3] |= (value & 1) << (low & 7);
		}
		break;
	case 4:
		if (flag < 0x40005) {
			this->mGameInts[low] = value;
		}
		break;
	case 5:
		if (flag < 0x50064) {
			this->mStageBools[low >> 3] &= ~(1 << (low & 7));
			this->mStageBools[low >> 3] |= (value & 1) << (low & 7);
		}
		break;
	case 6:
		if (flag < 0x60064) {
			this->mStageInts[low] = value;
		}
		break;
	case 7:
		if (flag < 0x70002) {
			this->mSavedOptionBools[low >> 3] &= ~(1 << (low & 7));
			this->mSavedOptionBools[low >> 3] |= (value & 1) << (low & 7);
		}
		break;
	case 8:
		if (flag < 0x80001) {
			this->mSavedOptionInts[low] = value;
		}
		break;
	case 9:
		if (flag < 0x90001) {
			this->mOptionBools[low >> 3] &= ~(1 << (low & 7));
			this->mOptionBools[low >> 3] |= (value & 1) << (low & 7);
		}
		break;
	case 10:
		if (flag < 0xA0002) {
			this->mOptionInts[low] = value;
		}
		break;
	}
}

bool TFlagManager::getBool(u32 flag) const
{
	switch (flag >> 16) {
	case 1:
		if (flag < 0x103B4) {
			return this->getFlag(flag) != 0;
		}
		break;
	case 3:
		if (flag < 0x3001D) {
			return this->getFlag(flag) != 0;
		}
		break;
	case 5:
		if (flag < 0x50064) {
			return this->getFlag(flag) != 0;
		}
		break;
	case 7:
		if (flag < 0x70002) {
			return this->getFlag(flag) != 0;
		}
		break;
	case 9:
		if (flag < 0x90001) {
			return this->getFlag(flag) != 0;
		}
		break;
	}
	return false;
}

void TFlagManager::setBool(bool value, u32 flag)
{
	switch (flag >> 16) {
	case 1:
		if (flag < 0x103B4) {
			this->setFlag(flag, value ? 1 : 0);
		}
		break;
	case 2:
		break;
	case 3:
		if (flag < 0x3001D) {
			this->setFlag(flag, value ? 1 : 0);
		}
		break;
	case 4:
		break;
	case 5:
		if (flag < 0x50064) {
			this->setFlag(flag, value ? 1 : 0);
		}
		break;
	case 6:
		break;
	case 7:
		if (flag < 0x70002) {
			this->setFlag(flag, value ? 1 : 0);
		}
		break;
	case 8:
		break;
	case 9:
		if (flag < 0x90001) {
			this->setFlag(flag, value ? 1 : 0);
		}
		break;
	case 10:
		break;
	}
}

void TFlagManager::incFlag(u32 flag, s32 amount)
{
	u32 low = flag & 0xFFFF;
	switch (flag >> 16) {
	case 2:
		if (flag < 0x20015) {
			this->mCardInts[low] += amount;
		}
		break;
	case 4:
		if (flag < 0x40005) {
			this->mGameInts[low] += amount;
		}
		break;
	case 6:
		if (flag < 0x60064) {
			this->mStageInts[low] += amount;
		}
		break;
	case 8:
		if (flag < 0x80001) {
			this->mSavedOptionInts[low] += amount;
		}
		break;
	case 10:
		if (flag < 0xA0002) {
			this->mOptionInts[low] += amount;
		}
		break;
	}
}

void TFlagManager::decFlag(u32 flag, s32 amount)
{
	this->incFlag(flag, -amount);
}

void TFlagManager::incMario(s32 amount)
{
	this->incFlag(0x20001, amount);
	if (99 < this->getFlag(0x20001)) {
		this->setFlag(0x20001, 99);
	}
}

bool TFlagManager::getShineFlag(u8 shine) const
{
	if (shine >= 120) {
		shine = 0;
	}
	return this->getFlag(shine + 0x10000) != 0;
}

void TFlagManager::setShineFlag(u8 shine)
{
	if (shine >= 120) {
		shine = 0;
	}
	u32 flag = 0x10000 + shine;
	if (this->getFlag(flag) == 0) {
		this->incFlag(0x40000, 1);
		this->setFlag(flag, 1);
	}
}

void TFlagManager::incGoldCoinFlag(u8 area, s32 amount)
{
	this->incFlag(0x40002, amount);
	if (this->getFlag(0x40002) > this->getFlag(0x20005 + area)) {
		this->setFlag(0x20005 + area, this->getFlag(0x40002));
	}
}

bool TFlagManager::getBlueCoinFlag(u8 area, u8 blueCoin) const
{
	u8 stage = SMS_getShineStage(area);
	if (stage == 0 || stage >= 10) {
		stage = 1;
	}
	if (blueCoin >= 50) {
		blueCoin = 0;
	}
	u32 flag = 0x10078 + (stage - 1) * 50 + blueCoin;
	return this->getFlag(flag) != 0;
}

void TFlagManager::setBlueCoinFlag(u8 area, u8 blueCoin)
{
	u8 unused[8];
	u8 stage = SMS_getShineStage(area);
	if (stage == 0 || stage >= 10) {
		stage = 1;
	}
	if (blueCoin >= 50) {
		blueCoin = 0;
	}
	u32 flag = 0x10078 + (stage - 1) * 50 + blueCoin;
	if (this->getFlag(flag) == 0) {
		this->incFlag(0x40001, 1);
		this->setFlag(flag, 1);
	}
}

bool TFlagManager::getNozzleRight(u8 area, u8 nozzle) const
{
	u8 stage = SMS_getShineStage(area);
	if (stage == 0 || stage >= 10) {
		stage = 1;
	}
	if (nozzle >= 2) {
		nozzle = 0;
	}
	u32 flag = 0x10366 + (stage - 1) * 2 + nozzle;
	return this->getFlag(flag) != 0;
}

void TFlagManager::setNozzleRight(u8 area, u8 nozzle)
{
	char unused[8];
	unsigned char stage = SMS_getShineStage(area);
	if (stage == 0 || stage >= 10) {
		stage = 1;
	}
	if (nozzle >= 2) {
		nozzle = 0;
	}
	u32 flag = 0x10366 + (stage - 1) * 2 + nozzle;
	if (this->getFlag(flag) == 0) {
		this->setFlag(flag, 1);
	}
}

void TFlagManager::load(JSUMemoryInputStream& in)
{
	this->resetCard();

	u32 magic;
	in.read(&magic, sizeof(magic));

	if (magic == 4) {
		in.read(&this->mLastSaveTime, sizeof(this->mLastSaveTime));

		u64 padding1 = 0;
		in.read(&padding1, sizeof(padding1));

		u32 padding2;
		in.read(&padding2, sizeof(padding2));

		u16 padding3;
		in.read(&padding3, sizeof(padding3));

		u16 padding4;
		in.read(&padding4, sizeof(padding4));

		u32 padding5;
		for (int i = 0; i < 16; ++i) {
			in.read(&padding5, sizeof(padding5));
		}

		in.read(&this->mCardBools, sizeof(this->mCardBools));
		in.skip(0x200 - sizeof(this->mCardBools));
		in.read(&this->mCardInts, sizeof(this->mCardInts));
		in.skip(0x200 - sizeof(this->mCardInts));
	}

	memcpy(this->mSavedCardBools, this->mCardBools, sizeof(this->mCardBools));
	memcpy(this->mSavedCardInts, this->mCardInts, sizeof(this->mCardInts));
	this->mSavedLastSaveTime = this->mLastSaveTime;
	this->correctFlag();
}

void TFlagManager::restore()
{
	this->resetCard();
	this->mLastSaveTime = this->mSavedLastSaveTime;
	memcpy(this->mCardBools, this->mSavedCardBools, sizeof(this->mCardBools));
	memcpy(this->mCardInts, this->mSavedCardInts, sizeof(this->mCardInts));
	this->correctFlag();
}

void TFlagManager::firstStart()
{
	this->resetCard();
	memcpy(this->mSavedCardBools, this->mCardBools, sizeof(this->mCardBools));
	memcpy(this->mSavedCardInts, this->mCardInts, sizeof(this->mCardInts));
	this->mSavedLastSaveTime = this->mLastSaveTime;
	this->correctFlag();
}

void TFlagManager::correctFlag()
{
	if (this->getFlag(0x20001) < 3) {
		this->setFlag(0x20001, 3);
	}
	if (this->getFlag(0x20002) == 0) {
		this->setFlag(0x20002, 3500);
	}
	if (this->getFlag(0x20003) == 0) {
		this->setFlag(0x20003, 3000);
	}
	if (this->getFlag(0x20014) == 0) {
		this->setFlag(0x20014, 4000);
	}
	if (this->getFlag(0x20004) == 0) {
		this->setFlag(0x20004, 3000);
	}
	this->setBool(true, 0x1039A);
	this->setBool(true, 0x1039D);

	int shines = 0;
	for (u32 flag = 0x10000; flag <= 0x10077; ++flag) {
		if (this->getFlag(flag) != 0) {
			++shines;
		}
	}
	this->setFlag(0x40000, shines);

	int blues = 0;
	for (u32 flag = 0x10078; flag <= 0x10365; ++flag) {
		if (this->getFlag(flag) != 0) {
			++blues;
		}
	}
	this->setFlag(0x40001, blues);
}

void TFlagManager::save(JSUMemoryOutputStream& out)
{
	u8 _padding[8];

	this->mLastSaveTimeBackup = this->mLastSaveTime;
	this->mLastSaveTime       = OSGetTime();

	this->incFlag(0x20000, 1);

	u32 magic = 4;
	out.write(&magic, sizeof(magic));

	s64 time = this->mLastSaveTime;
	out.write(&time, sizeof(time));

	u64 padding = 0;
	out.write(&padding, sizeof(padding));

	s32 saveCount = this->getFlag(0x20000);
	out.write(&saveCount, sizeof(saveCount));

	u16 shineCount = this->getFlag(0x40000);
	out.write(&shineCount, sizeof(shineCount));

	u16 padding2 = 0;
	out.write(&padding2, sizeof(padding2));

	for (s32 i = 0; i < 16; ++i) {
		u32 padding3 = 0;
		out.write(&padding3, sizeof(padding3));
	}

	out.write(this->mCardBools, sizeof(this->mCardBools));
	out.skip(0x200 - sizeof(this->mCardBools), 0);
	out.write(this->mCardInts, sizeof(this->mCardInts));
	out.skip(0x200 - sizeof(this->mCardInts), 0);
}

void TFlagManager::saveSuccess()
{
	memcpy(this->mSavedCardBools, this->mCardBools, sizeof(this->mCardBools));
	memcpy(this->mSavedCardInts, this->mCardInts, sizeof(this->mCardInts));
	this->mSavedLastSaveTime = this->mLastSaveTime;
}

void TFlagManager::saveFail()
{
	this->mLastSaveTime = this->mLastSaveTimeBackup;
	this->incFlag(0x20000, -1);
}

void TFlagManager::resetOpt()
{
	this->mSavedOptionBools[0] = 0;
	this->mSavedOptionInts[0]  = 0;
}

void TFlagManager::correctOptFlag()
{
	this->setBool(!this->getBool(0x70000), 0x90000);

	if (OSGetSoundMode() == 0) {
		this->setFlag(0xA0000, 0);
	} else {
		this->setFlag(0xA0000, this->getBool(0x70001) ? 2 : 1);
	}

	this->setFlag(0xA0001, 0x100);
}

void TFlagManager::loadOption(JSUMemoryInputStream& in)
{
	this->mSavedOptionBools[0] = 0;
	this->mSavedOptionInts[0]  = 0;

	u32 magic;
	u8 _padding[24];
	in.read(&magic, sizeof(magic));
	if (magic == 2) {
		in.read(this->mSavedOptionBools, sizeof(this->mSavedOptionBools));
		in.read(this->mSavedOptionInts, sizeof(this->mSavedOptionInts));
	}

	this->setBool(!this->getBool(0x70000), 0x90000);

	if (OSGetSoundMode() == 0) {
		this->setFlag(0xA0000, 0);
	} else {
		this->setFlag(0xA0000, this->getBool(0x70001) ? 2 : 1);
	}

	this->setFlag(0xA0001, 0x100);
}

void TFlagManager::saveOption(JSUMemoryOutputStream& out)
{
	u32 magic = 2;
	out.write(&magic, sizeof(magic));
	this->setBool(!this->getBool(0x90000), 0x70000);
	switch (this->getFlag(0xA0000)) {
	case 0:
		OSSetSoundMode(0);
		this->setBool(false, 0x70001);
		break;
	case 1:
		OSSetSoundMode(1);
		this->setBool(false, 0x70001);
		break;
	case 2:
		OSSetSoundMode(1);
		this->setBool(true, 0x70001);
	}
	this->setFlag(0x80000, 0);
	out.write(this->mSavedOptionBools, sizeof(this->mSavedOptionBools));
	out.write(this->mSavedOptionInts, sizeof(this->mSavedOptionInts));
}
