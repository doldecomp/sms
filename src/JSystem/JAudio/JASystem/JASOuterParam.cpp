#include <JSystem/JAudio/JASystem/JASTrack.hpp>

namespace JASystem {

TTrack::TOuterParam::TOuterParam()
{
	unk0  = 0;
	unk2  = 0;
	unk4  = 0.0f;
	unk8  = 0.0f;
	unkC  = 0.0f;
	unk10 = 0.0f;
	unk14 = 0.0f;
	unk18 = 0.0f;
	for (int i = 0; i < 8; i++)
		unk1C[i] = 0;
}

void TTrack::TOuterParam::initExtBuffer()
{
	unk0 = 0;
	unk2 = 0;
}

void TTrack::TOuterParam::setOuterSwitch(u16 param) { unk0 = param; }

u16 TTrack::TOuterParam::getSwitch() { return unk0; }

bool TTrack::TOuterParam::checkOuterSwitch(u16 param) { return unk0 & param; }

void TTrack::TOuterParam::setOuterUpdate(u16 param) { unk2 = param; }

u16 TTrack::TOuterParam::getOuterUpdate() { return unk2; }

void TTrack::TOuterParam::setIntFirFilter(s16 param1, u8 param2)
{
	unk1C[param2] = param1;
}

s16 TTrack::TOuterParam::getIntFirFilter(u8 param) { return unk1C[param]; }

void TTrack::TOuterParam::setParam(u8 param1, f32 param2)
{
	f32* var1;
	switch (param1) {
	case 1:
		var1 = &unk4;
		break;
	case 2:
		var1 = &unk8;
		break;
	case 4:
		var1 = &unkC;
		break;
	case 16:
		var1 = &unk10;
		break;
	case 8:
		var1 = &unk14;
		break;
	case 64:
		var1 = &unk18;
		break;
	default:
		return;
	}
	*var1 = param2;
	unk2 |= param1;
}

void TTrack::TOuterParam::onSwitch(u16 param)
{
	unk0 |= param;
	unk2 |= param;
}

void TTrack::TOuterParam::setFirFilter(s16* param)
{
	unk2 |= 0x80;
	unk0 |= 0x80;
	for (u8 i = 0; i < 8; i++)
		unk1C[i] = param[i];
}

} // namespace JASystem
