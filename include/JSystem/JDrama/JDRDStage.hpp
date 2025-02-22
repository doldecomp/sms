#ifndef JDR_DSTAGE_HPP
#define JDR_DSTAGE_HPP

#include <JSystem/JDrama/JDRViewConnecter.hpp>
#include <JSystem/JDrama/JDRFlag.hpp>

namespace JDrama {

class TDStageDisp : public TViewConnecter {
public:
	TDStageDisp(const char*, TFlagT<unsigned short>);

	virtual ~TDStageDisp() { }

	void getEfbCtrlDisp() const;
	TViewObj* getEfbCtrlDisp();
};

} // namespace JDrama

#endif
