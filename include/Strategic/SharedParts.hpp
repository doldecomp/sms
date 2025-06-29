#ifndef STRATEGIC_SHARED_PARTS_HPP
#define STRATEGIC_SHARED_PARTS_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>

class TLiveActor;
class SDLModelData;
class MActor;

class TSharedParts : public JDrama::TViewObj {
public:
	TSharedParts(const TLiveActor*, int, const char*, u32, u32,
	             const char* name = "<TSharedParts>");
	TSharedParts(const TLiveActor*, int, SDLModelData*, u32,
	             const char* name = "<TSharedParts>");
	TSharedParts(const TLiveActor*, int, MActor*,
	             const char* name = "<TSharedParts>");

	virtual void perform(u32, JDrama::TGraphics*);
	MtxPtr getConnectedMtx() const;

public:
	/* 0x10 */ const TLiveActor* unk10;
	/* 0x14 */ int mConnectedAnmMtxIndex;
	/* 0x18 */ MActor* unk18;
};

#endif
