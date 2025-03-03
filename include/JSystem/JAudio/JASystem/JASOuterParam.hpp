#ifndef JASOUTERPARAM_HPP
#define JASOUTERPARAM_HPP

namespace JASystem {

namespace TTrack {
	class TOuterParam {
	public:
		TOuterParam();
		void initExtBuffer();
		void setParam(unsigned char, float);
		void onSwitch(unsigned short);
		void checkOuterSwitch(unsigned short);
		unsigned short getIntFirFilter(unsigned char);
		unsigned short getOuterUpdate();
		void setOuterUpdate(unsigned short);
		void setOuterSwitch(unsigned short);
		void setFirFilter(short*);
		unsigned short getSwitch();
		void setIntFirFilter(short, unsigned char);
	};
} // namespace TTrack

} // namespace JASystem

#endif // JASOUTERPARAM_HPP
