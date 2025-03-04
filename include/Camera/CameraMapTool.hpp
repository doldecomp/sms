#ifndef CAMERA_CAMERA_MAP_TOOL_HPP
#define CAMERA_CAMERA_MAP_TOOL_HPP

#include <JSystem/JDrama/JDRNameRef.hpp>
#include <JSystem/JGeometry.hpp>

class TCameraMapTool : public JDrama::TNameRef {
public:
	TCameraMapTool();

	void calcPosAndAt(JGeometry::TVec3<float>*, JGeometry::TVec3<float>*) const;
	void load(JSUMemoryInputStream&);
};

extern TCameraMapTool* gpCamMapToolTable;

#endif
