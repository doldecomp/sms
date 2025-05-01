#ifndef FLAG_MANAGER_HPP
#define FLAG_MANAGER_HPP

#include <JSystem/JKernel/JKRHeap.hpp>
#include <JSystem/JSupport/JSUMemoryInputStream.hpp>
#include <JSystem/JSupport/JSUMemoryOutputStream.hpp>

class TFlagManager {
public:
	void saveOption(JSUMemoryOutputStream& out);
	void loadOption(JSUMemoryInputStream& in);
	void correctOptFlag();
	void resetOpt();
	void saveFail();
	void saveSuccess();
	void save(JSUMemoryOutputStream& out);
	void correctFlag();
	void firstStart();
	void restore();
	void load(JSUMemoryInputStream& in);
	void setNozzleRight(u8 area, u8 nozzle);
	bool getNozzleRight(u8 area, u8 nozzle) const;
	void setBlueCoinFlag(u8 area, u8 blueCoin);
	bool getBlueCoinFlag(u8 area, u8 blueCoin) const;
	void incGoldCoinFlag(u8 area, s32 amount);
	void setShineFlag(u8 shine);
	bool getShineFlag(u8 shine) const;
	void incMario(s32 amount);
	void decFlag(u32 flag, s32 amount);
	void incFlag(u32 flag, s32 amount);
	void setBool(bool value, u32 flag);
	bool getBool(u32 flag) const;
	void setFlag(u32 flag, s32 value);
	long getFlag(u32 flag) const;
	void resetStage();
	void resetGame();
	void resetCard();
	static TFlagManager* start(JKRHeap* heap);

	static TFlagManager* getInstance() { return smInstance; }

	static TFlagManager* smInstance;

private:
	TFlagManager();
	void initOpt()
	{
		resetOpt();
		correctOptFlag();
	}

	u8 mCardBools[119];
	s32 mCardInts[21];
	u8 mGameBools[4];
	s32 mGameInts[5];
	u8 mStageBools[13];
	s32 mStageInts[100];
	u8 mSavedOptionBools[1];
	s32 mSavedOptionInts[1];
	u8 mOptionBools[1];
	s32 mOptionInts[2];
	s64 mLastSaveTime;
	s64 mLastSaveTimeBackup;
	u8 mSavedCardBools[119];
	s32 mSavedCardInts[21];
	s64 mSavedLastSaveTime;
};

#endif
