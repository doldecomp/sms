#ifndef JASOUTERPARAM_HPP
#define JASOUTERPARAM_HPP

#include <dolphin/types.h>

namespace JASystem {

namespace TTrack {
	class TOuterParam {
	public:
		TOuterParam();
		void initExtBuffer();
		void setParam(u8, f32);
		void onSwitch(u16);
		bool checkOuterSwitch(u16);
		s16 getIntFirFilter(u8);
		u16 getOuterUpdate();
		void setOuterUpdate(u16);
		void setOuterSwitch(u16);
		void setFirFilter(s16*);
		u16 getSwitch();
		void setIntFirFilter(s16, u8);

	public:
		/* 0x00 */ u16 unk0;
		/* 0x02 */ u16 unk2;
		/* 0x04 */ f32 unk4;
		/* 0x08 */ f32 unk8;
		/* 0x0C */ f32 unkC;
		/* 0x10 */ f32 unk10;
		/* 0x14 */ f32 unk14;
		/* 0x18 */ f32 unk18;
		/* 0x1C */ s16 unk1C[8];
	};
} // namespace TTrack

} // namespace JASystem

#endif // JASOUTERPARAM_HPP
