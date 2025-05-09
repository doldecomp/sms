#ifndef M_SOUND_MAIN_SIDE_HPP
#define M_SOUND_MAIN_SIDE_HPP

#include <dolphin/mtx.h>

class MSStage {
public:
	void setPosPtr(Vec*);
	void stageLoop();
	static MSStage* init(u8, u8);
};

class MSSTageSimpleEnvironmentMonte {
public:
	void proc();
};

class MSSTageSimpleEnvironment {
public:
	void proc();
};

class MSStageCubeSwitch {
public:
	void toBossBgm();
	void toStageBgm();
	void proc();
};

class MSStageCubeFade {
public:
	void calcParamRatioInCube(long);
	void proc();
	MSStageCubeFade();
};

class MSStageCubeFadeMonte {
public:
	void proc();
	MSStageCubeFadeMonte();
};

class MSStageDistFadeMonte {
public:
	void proc();
	MSStageDistFadeMonte(const Vec*, float, float, u32, bool);
};

class MSStageDistFade {
public:
	void proc();
	MSStageDistFade(const Vec*, float, float, u32, bool);
};

class MSStageProc {
public:
	void setBgmPosition(const Vec&, float, bool, u32, u32);
};

class MSMainProc {
public:
	void startStageBGM(u8, u8);
	void endStageEntranceDemo(u8, u8);
	static void entranceDemoLoop(u32);
	void startStageEntranceDemo(u8, u8);
	void setMSoundEnterStage(u8, u8);
	void setBossNotDamagedFlag(bool);
	void getBossLivesFlag2();
	void getBossLivesFlag();
	static void setBossLivesFlag2(bool);
	void setBossLivesFlagOnlyFlag(bool);
	static void setBossLivesFlag(bool);
	void fromTalkingCameraDemo(bool);
	void toTalkingCameraDemo();
	void fromTHPDemo();
	void toTHPDemo();
	void fromInnerCameraDemo();
	void toInnerCameraDemo();
	void entranceDemoWipeInEnd();
	void getMonteVillageActorArea(const Vec&);
};

#endif
