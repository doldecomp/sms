#include <string.h>
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
		if (flag < MSF_CARD_BOOL_END) {
			return mCardBools[low >> 3] >> (low & 7) & 1;
		}
		break;
	case 2:
		if (flag < MSF_CARD_INT_END) {
			return mCardInts[low];
		}
		break;
	case 3:
		if (flag < MSF_GAME_BOOL_END) {
			return mGameBools[low >> 3] >> (low & 7) & 1;
		}
		break;
	case 4:
		if (flag < MSF_GAME_INT_END) {
			return mGameInts[low];
		}
		break;
	case 5:
		if (flag < MSF_STAGE_BOOL_END) {
			return mStageBools[low >> 3] >> (low & 7) & 1;
		}
		break;
	case 6:
		if (flag < MSF_STAGE_INT_END) {
			return mStageInts[low];
		}
		break;
	case 7:
		if (flag < MSF_SAVED_OPTION_BOOL_END) {
			return mSavedOptionBools[low >> 3] >> (low & 7) & 1;
		}
		break;
	case 8:
		if (flag < MSF_SAVED_OPTION_INT_END) {
			return mSavedOptionInts[low];
		}
		break;
	case 9:
		if (flag < MSF_OPTION_BOOL_END) {
			return mOptionBools[low >> 3] >> (low & 7) & 1;
		}
		break;
	case 10:
		if (flag < MSF_OPTION_INT_END) {
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
		if (flag < MSF_CARD_BOOL_END) {
			mCardBools[low >> 3] &= ~(1 << (low & 7));
			mCardBools[low >> 3] |= (value & 1) << (low & 7);
		}
		break;
	case 2:
		if (flag < MSF_CARD_INT_END) {
			mCardInts[low] = value;
		}
		break;
	case 3:
		if (flag < MSF_GAME_BOOL_END) {
			mGameBools[low >> 3] &= ~(1 << (low & 7));
			mGameBools[low >> 3] |= (value & 1) << (low & 7);
		}
		break;
	case 4:
		if (flag < MSF_GAME_INT_END) {
			mGameInts[low] = value;
		}
		break;
	case 5:
		if (flag < MSF_STAGE_BOOL_END) {
			mStageBools[low >> 3] &= ~(1 << (low & 7));
			mStageBools[low >> 3] |= (value & 1) << (low & 7);
		}
		break;
	case 6:
		if (flag < MSF_STAGE_INT_END) {
			mStageInts[low] = value;
		}
		break;
	case 7:
		if (flag < MSF_SAVED_OPTION_BOOL_END) {
			mSavedOptionBools[low >> 3] &= ~(1 << (low & 7));
			mSavedOptionBools[low >> 3] |= (value & 1) << (low & 7);
		}
		break;
	case 8:
		if (flag < MSF_SAVED_OPTION_INT_END) {
			mSavedOptionInts[low] = value;
		}
		break;
	case 9:
		if (flag < MSF_OPTION_BOOL_END) {
			mOptionBools[low >> 3] &= ~(1 << (low & 7));
			mOptionBools[low >> 3] |= (value & 1) << (low & 7);
		}
		break;
	case 10:
		if (flag < MSF_OPTION_INT_END) {
			mOptionInts[low] = value;
		}
		break;
	}
}

bool TFlagManager::getBool(u32 flag) const
{
	switch (flag >> 16) {
	case 1:
		if (flag < MSF_CARD_BOOL_END) {
			return getFlag(flag) != 0;
		}
		break;
	case 3:
		if (flag < MSF_GAME_BOOL_END) {
			return getFlag(flag) != 0;
		}
		break;
	case 5:
		if (flag < MSF_STAGE_BOOL_END) {
			return getFlag(flag) != 0;
		}
		break;
	case 7:
		if (flag < MSF_SAVED_OPTION_BOOL_END) {
			return getFlag(flag) != 0;
		}
		break;
	case 9:
		if (flag < MSF_OPTION_BOOL_END) {
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
		if (flag < MSF_CARD_BOOL_END) {
			setFlag(flag, value ? 1 : 0);
		}
		break;
	case 2:
		break;
	case 3:
		if (flag < MSF_GAME_BOOL_END) {
			setFlag(flag, value ? 1 : 0);
		}
		break;
	case 4:
		break;
	case 5:
		if (flag < MSF_STAGE_BOOL_END) {
			setFlag(flag, value ? 1 : 0);
		}
		break;
	case 6:
		break;
	case 7:
		if (flag < MSF_SAVED_OPTION_BOOL_END) {
			setFlag(flag, value ? 1 : 0);
		}
		break;
	case 8:
		break;
	case 9:
		if (flag < MSF_OPTION_BOOL_END) {
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
		if (flag < MSF_CARD_INT_END) {
			mCardInts[low] += amount;
		}
		break;
	case 4:
		if (flag < MSF_GAME_INT_END) {
			mGameInts[low] += amount;
		}
		break;
	case 6:
		if (flag < MSF_STAGE_INT_END) {
			mStageInts[low] += amount;
		}
		break;
	case 8:
		if (flag < MSF_SAVED_OPTION_INT_END) {
			mSavedOptionInts[low] += amount;
		}
		break;
	case 10:
		if (flag < MSF_OPTION_INT_END) {
			mOptionInts[low] += amount;
		}
		break;
	}
}

void TFlagManager::decFlag(u32 flag, s32 amount) { incFlag(flag, -amount); }

void TFlagManager::incMario(s32 amount)
{
	incFlag(MSF_LIFE_COUNT, amount);
	if (99 < getFlag(MSF_LIFE_COUNT)) {
		setFlag(MSF_LIFE_COUNT, 99);
	}
}

bool TFlagManager::getShineFlag(u8 shine) const
{
	if (shine >= 120) {
		shine = 0;
	}
	return getFlag(shine + MSF_SHINE_BASE) != 0;
}

void TFlagManager::setShineFlag(u8 shine)
{
	if (shine >= 120) {
		shine = 0;
	}
	u32 flag = MSF_SHINE_BASE + shine;
	if (getFlag(flag) == 0) {
		incFlag(MSF_SHINE_COUNT, 1);
		setFlag(flag, 1);
	}
}

void TFlagManager::incGoldCoinFlag(u8 area, s32 amount)
{
	incFlag(MSF_GOLD_COIN_COUNT, amount);
	if (getFlag(MSF_GOLD_COIN_COUNT) > getFlag(MSF_COIN_RECORD_BASE + area)) {
		setFlag(MSF_COIN_RECORD_BASE + area, getFlag(MSF_GOLD_COIN_COUNT));
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
	u32 flag = MSF_BLUE_COIN_BASE + (stage - 1) * 50 + blueCoin;
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
	u32 flag = MSF_BLUE_COIN_BASE + (stage - 1) * 50 + blueCoin;
	if (getFlag(flag) == 0) {
		incFlag(MSF_BLUE_COIN_COUNT, 1);
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
	u32 flag = MSF_NOZZLE_BASE + (stage - 1) * 2 + nozzle;
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
	u32 flag = MSF_NOZZLE_BASE + (stage - 1) * 2 + nozzle;
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
	saveSuccess();
	correctFlag();
}

void TFlagManager::correctFlag()
{
	if (getFlag(MSF_LIFE_COUNT) < 3)
		setFlag(MSF_LIFE_COUNT, 3);

	if (getFlag(MSF_RACE_RECORD_GELATO) == 0)
		setFlag(MSF_RACE_RECORD_GELATO, 3500);

	if (getFlag(MSF_RACE_RECORD_PIANTA) == 0)
		setFlag(MSF_RACE_RECORD_PIANTA, 3000);

	if (getFlag(MSF_RACE_RECORD_NOKI) == 0)
		setFlag(MSF_RACE_RECORD_NOKI, 4000);

	if (getFlag(MSF_BOX_GAME_RECORD) == 0)
		setFlag(MSF_BOX_GAME_RECORD, 3000);

	setBool(true, MSF_FMV_OPENING_WATCHED);
	setBool(true, MSF_AUTO_DEMO_WATCHED);

	int shines = 0;
	for (u32 flag = MSF_SHINE_BASE; flag <= MSF_SHINE_LAST; ++flag) {
		if (getFlag(flag) != 0) {
			++shines;
		}
	}
	setFlag(MSF_SHINE_COUNT, shines);

	int blues = 0;
	for (u32 flag = MSF_BLUE_COIN_BASE; flag <= MSF_BLUE_COIN_LAST; ++flag) {
		if (getFlag(flag) != 0) {
			++blues;
		}
	}
	setFlag(MSF_BLUE_COIN_COUNT, blues);
}

void TFlagManager::save(JSUMemoryOutputStream& out)
{
	mLastSaveTimeBackup = mLastSaveTime;
	mLastSaveTime       = OSGetTime();

	incFlag(MSF_SAVE_COUNT, 1);

	u32 magic = 4;
	out.write(&magic, sizeof(magic));

	s64 time = mLastSaveTime;
	out.write(&time, sizeof(time));

	u64 padding = 0;
	out.write(&padding, sizeof(padding));

	s32 saveCount = getFlag(MSF_SAVE_COUNT);
	out.write(&saveCount, sizeof(saveCount));

	u16 shineCount = getFlag(MSF_SHINE_COUNT);
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
	incFlag(MSF_SAVE_COUNT, -1);
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
	setBool(!getBool(MSF_SAVED_RUMBLE_OFF), MSF_RUMBLE);

	if (OSGetSoundMode() == 0) {
		setFlag(MSF_SOUND_MODE, 0);
	} else {
		setFlag(MSF_SOUND_MODE, getBool(MSF_SAVED_SURROUND) ? 2 : 1);
	}

#ifdef VERSION_GMSP01
	s32 language = getFlag(MSF_SAVED_LANGUAGE);
	if (language == 0) {
		u8 osLanguage = OSGetLanguage();
		if (osLanguage >= 5)
			osLanguage = 0;
		setFlag(MSF_LANGUAGE, osLanguage);
	} else {
		setFlag(MSF_LANGUAGE, language - 1);
	}

	setBool(!getBool(MSF_SAVED_SUBTITLES_OFF), MSF_SUBTITLES);
#else
	setFlag(MSF_LANGUAGE, 0x100);
#endif
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
	setBool(!getBool(MSF_RUMBLE), MSF_SAVED_RUMBLE_OFF);
	switch (getFlag(MSF_SOUND_MODE)) {
	case 0:
		OSSetSoundMode(0);
		setBool(false, MSF_SAVED_SURROUND);
		break;
	case 1:
		OSSetSoundMode(1);
		setBool(false, MSF_SAVED_SURROUND);
		break;
	case 2:
		OSSetSoundMode(1);
		setBool(true, MSF_SAVED_SURROUND);
	}
#ifdef VERSION_GMSP01
	setFlag(MSF_SAVED_LANGUAGE, getFlag(MSF_LANGUAGE) + 1);
	setBool(!getBool(MSF_SUBTITLES), MSF_SAVED_SUBTITLES_OFF);
#else
	setFlag(MSF_SAVED_LANGUAGE, 0);
#endif
	out.write(mSavedOptionBools, sizeof(mSavedOptionBools));
	out.write(mSavedOptionInts, sizeof(mSavedOptionInts));
}
