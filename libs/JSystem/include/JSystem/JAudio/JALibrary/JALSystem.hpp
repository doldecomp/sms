#ifndef JALSYSTEM_HPP
#define JALSYSTEM_HPP

#include <JSystem/JAudio/JALibrary/JALCalc.hpp>

class JAISound;

namespace JALSystem {

enum ModType {
	ModType_JALSeModVolFunk = 0x1,
	ModType_JALSeModPitFunk = 0x2,
	ModType_JALSeModEffFunk = 0x4,
	ModType_JALSeModVolDist = 0x8,
	ModType_JALSeModPitDist = 0x10,
	ModType_JALSeModEffDist = 0x20,

	ModType_JALSeModVolFGrp = 0x40,
	ModType_JALSeModPitFGrp = 0x80,
	ModType_JALSeModEffFGrp = 0x100,
	ModType_JALSeModVolDGrp = 0x200,
	ModType_JALSeModEffDGrp = 0x400,
	ModType_JALSeModPitDGrp = 0x800,
};

void init();
bool gateCheckFunc(u32 soundID, f32 value);
void processModFunc(JAISound* sound, f32 value, u32 fadeTime, u8 slot);
f32 processModDistVolume(u32 soundID, f32 distance);
f32 processModDistPitch(u32 soundID, f32 distance);
f32 processModDistFx(u32 soundID, f32 distance);
void append(JALSystem::ModType type, const char* name, u32 soundID, f32 inMin,
            f32 inMax, f32 defaultValue, f32 outMin, f32 outMax,
            JALCalc::CurveSign curveSign, f32 unk1, f32 unk2, u8 flags);
void appendGrpMember(JALSystem::ModType groupType, u32 groupID,
                     u32 memberSoundID);

extern class TFlagManager* spFManager;

class TFlagManager {
public:
	TFlagManager();
	void addUseFlag(u32 soundID, u16 modType);
	u16 getUseFlag(u32 soundID);
	bool isRegistered(u32 soundID, u16 modType);

	static TFlagManager* get() { return spFManager; }

public:
	u16* unk0[16];
};

}; // namespace JALSystem

#endif
