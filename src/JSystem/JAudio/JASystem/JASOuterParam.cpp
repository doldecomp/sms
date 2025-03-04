#include <JSystem/JAudio/JASystem/JASOuterParam.hpp>

namespace JASystem {

namespace TTrack {

	TOuterParam::TOuterParam() { }

	void TOuterParam::initExtBuffer() { }

	void TOuterParam::setParam(u8 param1, f32 param2) { }

	void TOuterParam::onSwitch(u16 param) { }

	void TOuterParam::checkOuterSwitch(u16 param) { }

	u16 TOuterParam::getIntFirFilter(u8 param) { return 0; }

	u16 TOuterParam::getOuterUpdate() { return 0; }

	void TOuterParam::setOuterUpdate(u16 param) { }

	void TOuterParam::setOuterSwitch(u16 param) { }

	void TOuterParam::setFirFilter(s16* param) { }

	u16 TOuterParam::getSwitch() { return 0; }

	void TOuterParam::setIntFirFilter(s16 param1, u8 param2) { }

} // namespace TTrack

} // namespace JASystem
