#ifndef SYSTEM_POSITION_HOLDER_HPP
#define SYSTEM_POSITION_HOLDER_HPP

#include <JSystem/JDrama/JDRNameRef.hpp>
#include <dolphin/mtx.h>

class TStagePositionInfo;

extern TStagePositionInfo* gpPositionHolder;

class TStagePositionInfo : public JDrama::TNameRef {
public:
	TStagePositionInfo(const char* name = "<StagePositionInfo>")
	    : JDrama::TNameRef(name)
	{
	}

	virtual void load(JSUMemoryInputStream&);

public:
	/* 0xC */ Vec unkC;
};

#endif
