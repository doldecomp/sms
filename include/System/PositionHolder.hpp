#ifndef SYSTEM_POSITION_HOLDER_HPP
#define SYSTEM_POSITION_HOLDER_HPP

#include <dolphin/mtx.h>
#include <JSystem/JDrama/JDRNameRef.hpp>
#include <Strategic/NameRefAry.hpp>

class TStagePositionInfo;

extern TNameRefAryT<TStagePositionInfo>* gpPositionHolder;

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
