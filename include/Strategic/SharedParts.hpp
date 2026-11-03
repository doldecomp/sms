#ifndef STRATEGIC_SHARED_PARTS_HPP
#define STRATEGIC_SHARED_PARTS_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>

class TLiveActor;
class SDLModelData;
class MActor;

class TSharedParts : public JDrama::TViewObj {
public:
	TSharedParts(const TLiveActor* actor, int anmMtxIndex, const char* resName,
	             u32 loadFlags, u32 modelFlags,
	             const char* name = "<TSharedParts>");
	TSharedParts(const TLiveActor*, int, SDLModelData*, u32,
	             const char* name = "<TSharedParts>");
	TSharedParts(const TLiveActor*, int, MActor*,
	             const char* name = "<TSharedParts>");

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	MtxPtr getConnectedMtx() const;
	MActor* getMActor() { return mMActor; }

protected:
	/* 0x10 */ const TLiveActor* mActor;
	/* 0x14 */ int mConnectedAnmMtxIndex;
	/* 0x18 */ MActor* mMActor;
};

#endif
