#ifndef JDR_SM_J3D_ACT_HPP
#define JDR_SM_J3D_ACT_HPP

#include <JSystem/JDrama/JDRActor.hpp>

class J3DModelData;
class J3DModel;
class J3DAnmBase;
class J3DFrameCtrl;
class J3DMtxCalc;

namespace JDrama {

class TSmJ3DAct : public TActor {
public:
	TSmJ3DAct()
	    : TActor("<SmJ3DAct>")
	    , unk44(nullptr)
	    , unk48(nullptr)
	    , unk4C(nullptr)
	    , unk50(nullptr)
	    , unk54(nullptr)
	{
	}

	void initModDat();
	void load(JSUMemoryInputStream&);
	void perform(u32, JDrama::TGraphics*);
	~TSmJ3DAct() { }

public:
	/* 0x44 */ J3DModelData* unk44;
	/* 0x48 */ J3DModel* unk48;
	/* 0x4C */ J3DAnmBase* unk4C;
	/* 0x50 */ J3DFrameCtrl* unk50;
	/* 0x54 */ J3DMtxCalc* unk54;
};

}; // namespace JDrama

#endif
