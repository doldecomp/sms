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
bool gateCheckFunc(u32, f32);
void processModFunc(JAISound*, f32, u32, u8);
f32 processModDistVolume(u32, f32);
f32 processModDistPitch(u32, f32);
f32 processModDistFx(u32, f32);
void append(ModType, const char*, u32, f32, f32, f32, f32, f32,
            JALCalc::CurveSign, f32, f32, u8);
void appendGrpMember(JALSystem::ModType, u32, u32);

extern class TFlagManager* spFManager;

class TFlagManager {
public:
	TFlagManager();
	void addUseFlag(u32, u16);
	void getUseFlag(u32);
	bool isRegistered(u32, u16);

public:
	void* unk0[16];
};

}; // namespace JALSystem

#endif
