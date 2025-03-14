#include <JSystem/JAudio/JASystem/JASOuterParam.hpp>

namespace JASystem {

namespace TTrack {

	TOuterParam::TOuterParam()
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

	void TOuterParam::initExtBuffer()
	{
		unk0 = 0;
		unk2 = 0;
	}

	void TOuterParam::setOuterSwitch(u16 param) { unk0 = param; }

	u16 TOuterParam::getSwitch() { return unk0; }

	bool TOuterParam::checkOuterSwitch(u16 param) { return unk0 & param; }

	void TOuterParam::setOuterUpdate(u16 param) { unk2 = param; }

	u16 TOuterParam::getOuterUpdate() { return unk2; }

	void TOuterParam::setIntFirFilter(s16 param1, u8 param2)
	{
		unk1C[param2] = param1;
	}

	s16 TOuterParam::getIntFirFilter(u8 param) { return unk1C[param]; }

	void TOuterParam::setParam(u8 param1, f32 param2)
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

	void TOuterParam::onSwitch(u16 param)
	{
		unk0 |= param;
		unk2 |= param;
	}

	void TOuterParam::setFirFilter(s16* param)
	{
		unk2 |= 0x80;
		unk0 |= 0x80;
		for (u8 i = 0; i < 8; i++)
			unk1C[i] = param[i];
	}

} // namespace TTrack

} // namespace JASystem
