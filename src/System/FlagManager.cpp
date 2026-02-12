#include <System/FlagManager.hpp>
#include <System/StageUtil.hpp>
#include <dolphin/os.h>

TFlagManager* TFlagManager::smInstance = 0;

TFlagManager* TFlagManager::start(JKRHeap* heap)
{
	if (smInstance == nullptr)
		smInstance = new (heap, 0) TFlagManager;

	return smInstance;
}

void TFlagManager::end() { }

TFlagManager::TFlagManager()
{
	firstStart();
	resetOpt();
	correctOptFlag();
}

void TFlagManager::resetCard()
{
	for (int i = 0; i < 119; ++i) {
		mCardBools[i] = 0;
	}

	for (int i = 0; i < 21; ++i) {
		mCardInts[i] = 0;
	}

	mLastSaveTime       = 0;
	mLastSaveTimeBackup = 0;

	resetGame();
}

void TFlagManager::resetGame()
{
	for (int i = 0; i < 4; ++i) {
		mGameBools[i] = 0;
	}

	for (int i = 0; i < 5; ++i) {
		mGameInts[i] = 0;
	}

	resetStage();
}

void TFlagManager::resetStage()
{
	for (int i = 0; i < 13; ++i) {
		mStageBools[i] = 0;
	}

	for (int i = 0; i < 100; ++i) {
		mStageInts[i] = 0;
	}
}

s32 TFlagManager::getFlag(u32 flag) const
{
	u32 low = flag & 0xFFFF;
	switch (flag >> 16) {
	case 1:
		if (flag < 0x103B4) {
			return mCardBools[low >> 3] >> (low & 7) & 1;
		}
		break;
	case 2:
		if (flag < 0x20015) {
			return mCardInts[low];
		}
		break;
	case 3:
		if (flag < 0x3001D) {
			return mGameBools[low >> 3] >> (low & 7) & 1;
		}
		break;
	case 4:
		if (flag < 0x40005) {
			return mGameInts[low];
		}
		break;
	case 5:
		if (flag < 0x50064) {
			return mStageBools[low >> 3] >> (low & 7) & 1;
		}
		break;
	case 6:
		if (flag < 0x60064) {
			return mStageInts[low];
		}
		break;
	case 7:
		if (flag < 0x70002) {
			return mSavedOptionBools[low >> 3] >> (low & 7) & 1;
		}
		break;
	case 8:
		if (flag < 0x80001) {
			return mSavedOptionInts[low];
		}
		break;
	case 9:
		if (flag < 0x90001) {
			return mOptionBools[low >> 3] >> (low & 7) & 1;
		}
		break;
	case 10:
		if (flag < 0xA0002) {
			return mOptionInts[low];
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
			mCardBools[low >> 3] &= ~(1 << (low & 7));
			mCardBools[low >> 3] |= (value & 1) << (low & 7);
		}
		break;
	case 2:
		if (flag < 0x20015) {
			mCardInts[low] = value;
		}
		break;
	case 3:
		if (flag < 0x3001D) {
			mGameBools[low >> 3] &= ~(1 << (low & 7));
			mGameBools[low >> 3] |= (value & 1) << (low & 7);
		}
		break;
	case 4:
		if (flag < 0x40005) {
			mGameInts[low] = value;
		}
		break;
	case 5:
		if (flag < 0x50064) {
			mStageBools[low >> 3] &= ~(1 << (low & 7));
			mStageBools[low >> 3] |= (value & 1) << (low & 7);
		}
		break;
	case 6:
		if (flag < 0x60064) {
			mStageInts[low] = value;
		}
		break;
	case 7:
		if (flag < 0x70002) {
			mSavedOptionBools[low >> 3] &= ~(1 << (low & 7));
			mSavedOptionBools[low >> 3] |= (value & 1) << (low & 7);
		}
		break;
	case 8:
		if (flag < 0x80001) {
			mSavedOptionInts[low] = value;
		}
		break;
	case 9:
		if (flag < 0x90001) {
			mOptionBools[low >> 3] &= ~(1 << (low & 7));
			mOptionBools[low >> 3] |= (value & 1) << (low & 7);
		}
		break;
	case 10:
		if (flag < 0xA0002) {
			mOptionInts[low] = value;
		}
		break;
	}
}

bool TFlagManager::getBool(u32 flag) const
{
	switch (flag >> 16) {
	case 1:
		if (flag < 0x103B4) {
			return getFlag(flag) != 0;
		}
		break;
	case 3:
		if (flag < 0x3001D) {
			return getFlag(flag) != 0;
		}
		break;
	case 5:
		if (flag < 0x50064) {
			return getFlag(flag) != 0;
		}
		break;
	case 7:
		if (flag < 0x70002) {
			return getFlag(flag) != 0;
		}
		break;
	case 9:
		if (flag < 0x90001) {
			return getFlag(flag) != 0;
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
			setFlag(flag, value ? 1 : 0);
		}
		break;
	case 2:
		break;
	case 3:
		if (flag < 0x3001D) {
			setFlag(flag, value ? 1 : 0);
		}
		break;
	case 4:
		break;
	case 5:
		if (flag < 0x50064) {
			setFlag(flag, value ? 1 : 0);
		}
		break;
	case 6:
		break;
	case 7:
		if (flag < 0x70002) {
			setFlag(flag, value ? 1 : 0);
		}
		break;
	case 8:
		break;
	case 9:
		if (flag < 0x90001) {
			setFlag(flag, value ? 1 : 0);
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
			mCardInts[low] += amount;
		}
		break;
	case 4:
		if (flag < 0x40005) {
			mGameInts[low] += amount;
		}
		break;
	case 6:
		if (flag < 0x60064) {
			mStageInts[low] += amount;
		}
		break;
	case 8:
		if (flag < 0x80001) {
			mSavedOptionInts[low] += amount;
		}
		break;
	case 10:
		if (flag < 0xA0002) {
			mOptionInts[low] += amount;
		}
		break;
	}
}

void TFlagManager::decFlag(u32 flag, s32 amount) { incFlag(flag, -amount); }

void TFlagManager::incMario(s32 amount)
{
	incFlag(0x20001, amount);
	if (99 < getFlag(0x20001)) {
		setFlag(0x20001, 99);
	}
}

bool TFlagManager::getShineFlag(u8 shine) const
{
	if (shine >= 120) {
		shine = 0;
	}
	return getFlag(shine + 0x10000) != 0;
}

void TFlagManager::setShineFlag(u8 shine)
{
	if (shine >= 120) {
		shine = 0;
	}
	u32 flag = 0x10000 + shine;
	if (getFlag(flag) == 0) {
		incFlag(0x40000, 1);
		setFlag(flag, 1);
	}
}

void TFlagManager::incGoldCoinFlag(u8 area, s32 amount)
{
	incFlag(0x40002, amount);
	if (getFlag(0x40002) > getFlag(0x20005 + area)) {
		setFlag(0x20005 + area, getFlag(0x40002));
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
	return getFlag(flag) != 0;
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
	if (getFlag(flag) == 0) {
		incFlag(0x40001, 1);
		setFlag(flag, 1);
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
	return getFlag(flag) != 0;
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
	if (getFlag(flag) == 0) {
		setFlag(flag, 1);
	}
}

void TFlagManager::load(JSUMemoryInputStream& in)
{
	resetCard();

	u32 magic;
	in.read(&magic, sizeof(magic));

	if (magic == 4) {
		in.read(&mLastSaveTime, sizeof(mLastSaveTime));

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

		in.read(&mCardBools, sizeof(mCardBools));
		in.skip(0x200 - sizeof(mCardBools));
		in.read(&mCardInts, sizeof(mCardInts));
		in.skip(0x200 - sizeof(mCardInts));
	}

	memcpy(mSavedCardBools, mCardBools, sizeof(mCardBools));
	memcpy(mSavedCardInts, mCardInts, sizeof(mCardInts));
	mSavedLastSaveTime = mLastSaveTime;
	correctFlag();
}

void TFlagManager::restore()
{
	resetCard();
	mLastSaveTime = mSavedLastSaveTime;
	memcpy(mCardBools, mSavedCardBools, sizeof(mCardBools));
	memcpy(mCardInts, mSavedCardInts, sizeof(mCardInts));
	correctFlag();
}

void TFlagManager::firstStart()
{
	resetCard();
	memcpy(mSavedCardBools, mCardBools, sizeof(mCardBools));
	memcpy(mSavedCardInts, mCardInts, sizeof(mCardInts));
	mSavedLastSaveTime = mLastSaveTime;
	correctFlag();
}

void TFlagManager::correctFlag()
{
	if (getFlag(0x20001) < 3)
		setFlag(0x20001, 3);

	if (getFlag(0x20002) == 0)
		setFlag(0x20002, 3500);

	if (getFlag(0x20003) == 0)
		setFlag(0x20003, 3000);

	if (getFlag(0x20014) == 0)
		setFlag(0x20014, 4000);

	if (getFlag(0x20004) == 0)
		setFlag(0x20004, 3000);

	setBool(true, 0x1039A);
	setBool(true, 0x1039D);

	int shines = 0;
	for (u32 flag = 0x10000; flag <= 0x10077; ++flag) {
		if (getFlag(flag) != 0) {
			++shines;
		}
	}
	setFlag(0x40000, shines);

	int blues = 0;
	for (u32 flag = 0x10078; flag <= 0x10365; ++flag) {
		if (getFlag(flag) != 0) {
			++blues;
		}
	}
	setFlag(0x40001, blues);
}

void TFlagManager::save(JSUMemoryOutputStream& out)
{
	mLastSaveTimeBackup = mLastSaveTime;
	mLastSaveTime       = OSGetTime();

	incFlag(0x20000, 1);

	u32 magic = 4;
	out.write(&magic, sizeof(magic));

	s64 time = mLastSaveTime;
	out.write(&time, sizeof(time));

	u64 padding = 0;
	out.write(&padding, sizeof(padding));

	s32 saveCount = getFlag(0x20000);
	out.write(&saveCount, sizeof(saveCount));

	u16 shineCount = getFlag(0x40000);
	out.write(&shineCount, sizeof(shineCount));

	u16 padding2 = 0;
	out.write(&padding2, sizeof(padding2));

	for (s32 i = 0; i < 16; ++i) {
		u32 padding3 = 0;
		out.write(&padding3, sizeof(padding3));
	}

	out.write(mCardBools, sizeof(mCardBools));
	out.skip(0x200 - sizeof(mCardBools), 0);
	out.write(mCardInts, sizeof(mCardInts));
	out.skip(0x200 - sizeof(mCardInts), 0);
}

void TFlagManager::saveSuccess()
{
	memcpy(mSavedCardBools, mCardBools, sizeof(mCardBools));
	memcpy(mSavedCardInts, mCardInts, sizeof(mCardInts));
	mSavedLastSaveTime = mLastSaveTime;
}

void TFlagManager::saveFail()
{
	mLastSaveTime = mLastSaveTimeBackup;
	incFlag(0x20000, -1);
}

void TFlagManager::resetOpt()
{
	for (int i = 0; i < 1; ++i)
		mSavedOptionBools[i] = 0;
	for (int i = 0; i < 1; ++i)
		mSavedOptionInts[i] = 0;
}

void TFlagManager::correctOptFlag()
{
	setBool(!getBool(0x70000), 0x90000);

	if (OSGetSoundMode() == 0) {
		setFlag(0xA0000, 0);
	} else {
		setFlag(0xA0000, getBool(0x70001) ? 2 : 1);
	}

	setFlag(0xA0001, 0x100);
}

void TFlagManager::loadOption(JSUMemoryInputStream& in)
{
	resetOpt();

	u32 magic;
	in.read(&magic, sizeof(magic));
	if (magic == 2) {
		in.read(mSavedOptionBools, sizeof(mSavedOptionBools));
		in.read(mSavedOptionInts, sizeof(mSavedOptionInts));
	}

	correctOptFlag();
}

void TFlagManager::saveOption(JSUMemoryOutputStream& out)
{
	u32 magic = 2;
	out.write(&magic, sizeof(magic));
	setBool(!getBool(0x90000), 0x70000);
	switch (getFlag(0xA0000)) {
	case 0:
		OSSetSoundMode(0);
		setBool(false, 0x70001);
		break;
	case 1:
		OSSetSoundMode(1);
		setBool(false, 0x70001);
		break;
	case 2:
		OSSetSoundMode(1);
		setBool(true, 0x70001);
	}
	setFlag(0x80000, 0);
	out.write(mSavedOptionBools, sizeof(mSavedOptionBools));
	out.write(mSavedOptionInts, sizeof(mSavedOptionInts));
}
