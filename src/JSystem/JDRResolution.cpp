#include "JSystem/JDrama.hpp"


// #ifdef _EU
#if 1
s32 JDrama::GetVIWidthMax(VITVMode tvm)
{
	switch (tvm)
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

s32 JDrama::GetVIHeightMax(VITVMode tvm)
{
	switch (tvm)
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
#else

extern "C"
{
s32 GetVIWidthMax(void)
{
	VITVMode tvm = VIGetTvFormat();
	switch (tvm)
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

s32 GetVIHeightMax(void)
{
	VITVMode tvm = VIGetTvFormat();
	switch (tvm)
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
} // extern "C"
#endif