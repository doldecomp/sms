#ifndef M_SOUND_MAIN_SIDE_HPP
#define M_SOUND_MAIN_SIDE_HPP

#include <dolphin/mtx.h>

class MSStage {
public:
	MSStage() { }

	virtual void setPosPtr(Vec*) { }
	virtual void stageLoop();
	virtual void proc() = 0;

	static MSStage* init(u8, u8);

	static MSStage* smMSStage;
};

class MSSTageSimpleEnvironmentMonte {
public:
	void proc();
};

class MSSTageSimpleEnvironment : public MSStage {
public:
	MSSTageSimpleEnvironment()
	    : unk4(0x5012)
	{
	}

	virtual void proc();

public:
	/* 0x4 */ u32 unk4;
};

class MSStageCubeFade : public MSStage {
public:
	MSStageCubeFade();

	virtual void proc();

	f32 calcParamRatioInCube(s32);

public:
	/* 0x4 */ int unk4;
	/* 0x8 */ int unk8;
	/* 0xC */ f32 unkC;
};

class MSStageCubeSwitch : public MSStageCubeFade {
public:
	MSStageCubeSwitch(u8 param_1)
	    : unk10(param_1)
	{
	}

	virtual void proc();

	void toStageBgm();
	void toBossBgm();

public:
	/* 0x10 */ u8 unk10;
	/* 0x11 */ u8 unk11;
};

class MSStageCubeFadeMonte : public MSStageCubeFade {
public:
	MSStageCubeFadeMonte();

	virtual void proc();

public:
	/* 0x10 */ int unk10;
	/* 0x14 */ int unk14;
};

class MSStageDistFade : public MSStage {
public:
	MSStageDistFade(const Vec*, f32, f32, u32, bool);

	virtual void proc();

public:
	/* 0x4 */ u32 unk4;
	/* 0x8 */ f32 unk8;
	/* 0xC */ f32 unkC;
	/* 0x10 */ const Vec* unk10;
	/* 0x14 */ u32 unk14;
	/* 0x18 */ u8 unk18;
};

class MSStageDistFadeMonte : public MSStageDistFade {
public:
	MSStageDistFadeMonte(const Vec*, f32, f32, u32, bool);

	virtual void proc();

public:
	/* 0x1C */ int unk1C;
	/* 0x20 */ int unk20;
};

class MSStageProc {
public:
	void setBgmPosition(const Vec&, f32, bool, u32, u32);
};

namespace MSMainProc {

int getMonteVillageActorArea(const Vec&);
void entranceDemoWipeInEnd();
void toInnerCameraDemo();
void fromInnerCameraDemo();
void toTHPDemo();
void fromTHPDemo();
void toTalkingCameraDemo();
void fromTalkingCameraDemo(bool);
void setBossLivesFlag(bool);
void setBossLivesFlagOnlyFlag(bool);
void setBossLivesFlag2(bool);
bool getBossLivesFlag();
bool getBossLivesFlag2();
void setBossNotDamagedFlag(bool);
void setMSoundEnterStage(u8, u8);
void startStageEntranceDemo(u8, u8);
void entranceDemoLoop(u32);
void endStageEntranceDemo(u8, u8);
void startStageBGM(u8, u8);

} // namespace MSMainProc

#endif
