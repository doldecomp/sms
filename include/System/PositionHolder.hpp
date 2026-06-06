#ifndef SYSTEM_POSITION_HOLDER_HPP
#define SYSTEM_POSITION_HOLDER_HPP

#include <JSystem/JGeometry/JGVec3.hpp>
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
	/* 0xC */ JGeometry::TVec3<f32> unkC;
};

#endif
