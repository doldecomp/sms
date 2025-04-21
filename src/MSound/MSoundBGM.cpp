#include <MSound/MSoundBGM.hpp>
#include <JSystem/JAudio/JALibrary/JALModSe.hpp>

JASystem::TTrack* MSBgm::smBgmInTrack[3];
f32 MSBgm::smMainVolume = 0.75f;

void MSBgm::init() { }

void MSBgm::startBGM(u32 param) { }

void MSBgm::stopBGM(u32 param1, u32 param2) { }

void MSBgm::stopTrackBGM(u8 param1, u32 param2) { }

void MSBgm::stopTrackBGMs(u8 param1, u32 param2) { }

void MSBgm::setVolume(u32 param1, f32 param2, u32 param3, u8 param4)
{
	MSBgm* bgm = JALListS<MSBgm, u32>::search(param1 & 0x3FF);
}

void MSBgm::setTrackVolume(u8 param1, f32 param2, u32 param3, u8 param4) {
	JAISound* sound = nullptr;
  
	if (((&smBgmInTrack)[param1] != 0) &&
	   (sound = *(JAISound **)((&smBgmInTrack)[param1] + 0x14), sound != (JAISound *)0x0)) {
		JAISound::setVolume(param1,param2,param3,param4);
	}
	return;
 }

void MSBgm::setSeqTrackVolume(u8 param1, u8 param2, f32 param3, u32 param4) { }

void MSBgm::setPan(u8 param1, f32 param2, u32 param3, u8 param4) { }

void MSBgm::setDolby(u8 param1, f32 param2, u32 param3, u8 param4) { }

void MSBgm::setPause(u8 param1, bool param2) { }

void MSBgm::setSeqTrackMute(u8 param1, u8 param2, bool param3) { }

void MSBgm::setSeqTRACKsMute(u8 param1, bool param2, u16 param3) { }

void MSBgm::setSeqTRACKsMuteH(JAISound* param1, bool param2, u16 param3) { }

void MSBgm::setStageBgmYoshiPercussion(bool param) { }

bool MSBgm::checkPlaying(u32 param) { return false; }

u32 MSBgm::getSceneNo(u32 param) {  
	switch(param) {
	case 0x80010001:
	  return 0x201;
	case 0x80010002:
	  return 0x202;
	case 0x80010003:
	  return 0x203;
	case 0x80010004:
	  return 0x204;
	case 0x80010005:
	  return 0x204;
	case 0x80010006:
	  return 0x205;
	case 0x80010007:
	  return 0x206;
	case 0x80010008:
	  return 0x207;
	case 0x80010009:
	  return 0x208;
	case 0x8001000a:
	  return 0x210;
	case 0x8001000b:
	  return 0x210;
	case 0x8001000c:
	  return 0x210;
	case 0x8001000d:
	  return 0x210;
	case 0x8001000e:
	  return 0x20a;
	case 0x8001000f:
	  return 0x209;
	case 0x80010010:
	  return 0x20a;
	case 0x80010011:
	  return 0xffffffff;
	case 0x80010012:
	  return 0x20c;
	case 0x80010013:
	  return 0x20d;
	case 0x80010014:
	  return 0x20e;
	case 0x80010015:
	  return 0x20f;
	case 0x80010016:
	  return 0x210;
	case 0x80010017:
	  return 0x210;
	case 0x80010018:
	  return 0x206;
	case 0x80010019:
	  return 0x211;
	case 0x8001001a:
	  return 0x212;
	case 0x8001001b:
	  return 0x210;
	case 0x8001001c:
	  return 0x20a;
	case 0x8001001d:
	  return 0x206;
	case 0x8001001e:
	  return 0x20d;
	case 0x8001001f:
	  return 0x210;
	case 0x80010020:
	  return 0x210;
	case 0x80010021:
	  return 0x214;
	case 0x80010022:
	  return 0x206;
	case 0x80010023:
	  return 0x204;
	case 0x80010024:
	  return 0x210;
	case 0x80010025:
	  return 0x20d;
	case 0x80010026:
	  return 0x210;
	case 0x80010027:
	  return 0x210;
	case 0x80010028:
	  return 0x210;
	case 0x80010029:
	  return 0x210;
	case 0x8001002a:
	  return 0x210;
	case 0x8001002b:
	  return 0x210;
	case 0x8001002c:
	  return 0x206;
	case 0x8001002e:
	  return 0x215;
	case 0x8001002f:
	  return 0x210;
	  default:
	  return 0xffffffff;
	 } 
	}

JAISound* MSBgm::getHandle(u8 param) 
{ 
		JASystem::TTrack* track = smBgmInTrack[param];
		if (track) {
			return (JAISound*)(&track[0x14]);
		}
		return nullptr;
	}

JAISound* MSBgm::getJASTrack(JAISound* param1, u8 param2) { return nullptr; }
