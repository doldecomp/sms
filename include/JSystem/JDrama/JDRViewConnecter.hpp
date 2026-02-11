#ifndef JDR_VIEW_CONNECTER_HPP
#define JDR_VIEW_CONNECTER_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>

namespace JDrama {

class TViewConnecter : public TViewObj {
public:
	TViewConnecter(TViewObj*, TViewObj*, TFlagT<u16>, const char*);

	virtual void perform(u32, JDrama::TGraphics*);

public:
	/* 0x10 */ TViewObj* unk10;
	/* 0x14 */ TViewObj* unk14;
	/* 0x18 */ TFlagT<u16> unk18;
};

}; // namespace JDrama

#endif
