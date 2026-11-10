#include <GC2D/ChangeValue.hpp>
#include <Camera/cameralib.hpp>

void TChangeValue::calcChangeValue()
{
	s32 ratio = CLBCalcRatio(unk0, unk1, unk2);
#if defined(VERSION_GMSJ01_01)
	f32 delta = CLBLinearInbetween<f32>(0.0f, 1.0f, unk5);
#else
#endif
}
