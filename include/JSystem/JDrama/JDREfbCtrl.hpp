#ifndef JDR_EFB_CTRL_HPP
#define JDR_EFB_CTRL_HPP

#include <JSystem/JDrama/JDRFlag.hpp>
#include <JSystem/JDrama/JDRViewObj.hpp>
#include <dolphin/gx.h>

namespace JDrama {

class TEfbCtrlTex : public TViewObj {
public:
	TEfbCtrlTex(const char*, TFlagT<u16>);

	virtual ~TEfbCtrlTex();
	virtual void perform(u32, TGraphics*);

	void setTexAttb(const GXTexObj&);
};

}; // namespace JDrama

#endif
