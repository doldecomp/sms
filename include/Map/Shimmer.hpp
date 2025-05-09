#ifndef MAP_SHIMMER_HPP
#define MAP_SHIMMER_HPP

#include <JSystem/JDrama/JDRActor.hpp>

class J3DModelData;
class J3DModel;
class J3DAnmTextureSRTKey;
class J3DFrameCtrl;

class TShimmer : public JDrama::TActor {
public:
	TShimmer(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);

	void far();
	void near();

public:
	/* 0x44 */ J3DModelData* unk44;
	/* 0x48 */ J3DModel* unk48;
	/* 0x4C */ char unk4C[0x8];
	/* 0x54 */ J3DAnmTextureSRTKey* unk54;
	/* 0x58 */ J3DFrameCtrl* unk58;
	/* 0x5C */ char unk5C[0x7C - 0x5C];
	/* 0x7C */ u16 unk7C;
};

#endif
