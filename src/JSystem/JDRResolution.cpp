#include "JSystem/JDrama.hpp"


s32 JDrama::GetVIWidthMax(GXRenderModeObj *rmo)
{
	switch (VIGetTvFormat(rmo))
	{
	case VI_TVMODE_NTSC_INT: // 0
		return 720;
	case VI_TVMODE_NTSC_PROG: // 2
		return 720;
	case VI_TVMODE_PAL_DS: // 5
		return 720;
	}
	//	default:
	return 720;
}


s32 JDrama::GetVIHeightMax(GXRenderModeObj *rmo)
{
	switch (VIGetTvFormat(rmo))
	{
	case VI_TVMODE_NTSC_INT: // 0
		return 574;
	case VI_TVMODE_NTSC_PROG: // 2
		return 480;
	case VI_TVMODE_PAL_DS: // 5
		return 480;
	}
	//	default:
	return 480;
}
