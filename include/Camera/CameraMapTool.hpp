#ifndef CAMERA_CAMERA_MAP_TOOL_HPP
#define CAMERA_CAMERA_MAP_TOOL_HPP

#include <JSystem/JDrama/JDRNameRef.hpp>
#include <JSystem/JGeometry.hpp>
#include <Strategic/NameRefAry.hpp>

class TCameraMapTool : public JDrama::TNameRef {
public:
	TCameraMapTool(const char* name = "<TCameraMapTool>")
	    : JDrama::TNameRef(name)
	{
	}

	void calcPosAndAt(JGeometry::TVec3<f32>*, JGeometry::TVec3<f32>*) const;
	void load(JSUMemoryInputStream&);

public:
	/* 0xC */ JGeometry::TVec3<f32> unkC;
	/* 0x18 */ f32 unk18; // TODO: these 2 floats are a struct I think?
	/* 0x1C */ f32 unk1C;
	/* 0x20 */ u32 unk20;
	/* 0x24 */ int unk24;
	/* 0x28 */ int unk28;
	/* 0x2C */ u32 unk2C;
};

extern TNameRefAryT<TCameraMapTool>* gpCamMapToolTable;

#endif
