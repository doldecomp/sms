#ifndef SYSTEM_STAGE_EVENT_INFO_HPP
#define SYSTEM_STAGE_EVENT_INFO_HPP

#include <JSystem/JDrama/JDRNameRef.hpp>

class TStageEventInfo : public JDrama::TNameRef {
public:
	TStageEventInfo(const char* name = "<StageEventInfo>")
	    : JDrama::TNameRef(name)
	{
	}

	virtual void load(JSUMemoryInputStream&);

public:
	/* 0xC */ u32 unkC;
	/* 0x10 */ const char* unk10;
	/* 0x14 */ const char* unk14;
	/* 0x18 */ const char* unk18;
	/* 0x1C */ const char* unk1C;
	/* 0x20 */ const char* unk20;
	/* 0x24 */ const char* unk24;
	/* 0x28 */ JDrama::TNameRef* unk28;
};

#endif
