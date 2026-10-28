#ifndef SYSTEM_GAME_SEQUENCE_HPP
#define SYSTEM_GAME_SEQUENCE_HPP

#include <JSystem/JDrama/JDRFlag.hpp>
#include <dolphin/types.h>

class TGameSequence {
public:
	TGameSequence(u8 stage = 0, u8 scenario = 0, JDrama::TFlagT<u16> flag = 0)
	{
		set(stage, scenario, flag);
	}

	TGameSequence& operator=(const TGameSequence& other)
	{
		set(other.mStage, other.mScenario, other.unk2);
		return *this;
	}

	void set(u8 param_1, u8 param_2, JDrama::TFlagT<u16> param_3)
	{
		mStage    = param_1;
		mScenario = param_2;
		unk2      = param_3;
	}

	u8 getStage() const { return mStage; }
	u8 getScenario() const { return mScenario; }

public:
	/* 0x0 */ u8 mStage;
	/* 0x1 */ u8 mScenario;
	/* 0x2 */ JDrama::TFlagT<u16> unk2;
};

#endif
