#include <Camera/CameraMapTool.hpp>

TNameRefAryT<TCameraMapTool>* gpCamMapToolTable;

void TCameraMapTool::load(JSUMemoryInputStream&) { }

void TCameraMapTool::calcPosAndAt(JGeometry::TVec3<float>*,
                                  JGeometry::TVec3<float>*) const
{
}
