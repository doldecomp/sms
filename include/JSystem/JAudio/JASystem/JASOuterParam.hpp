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
		void checkOuterSwitch(u16);
		u16 getIntFirFilter(u8);
		u16 getOuterUpdate();
		void setOuterUpdate(u16);
		void setOuterSwitch(u16);
		void setFirFilter(s16*);
		u16 getSwitch();
		void setIntFirFilter(s16, u8);
	};
} // namespace TTrack

} // namespace JASystem

#endif // JASOUTERPARAM_HPP
