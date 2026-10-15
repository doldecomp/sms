#include <JSystem/JDrama/JDRResolution.hpp>
#include <dolphin/vi.h>

#if defined(VERSION_GMSP01) || defined(VERSION_GMSP01_NX)
u16 JDrama::GetVIWidthMax(u32 tvFormat)
#else
u16 JDrama::GetVIWidthMax()
#endif
{
#if !(defined(VERSION_GMSP01) || defined(VERSION_GMSP01_NX))
	const u32 tvFormat = VIGetTvFormat();
#endif
	switch (tvFormat) {
	case VI_TVMODE_NTSC_DS: // 1
		return 720;
	case VI_TVMODE_NTSC_PROG: // 2
		return 720;
	case VI_TVMODE_PAL_DS: // 5
		return 720;
	default:
	case VI_TVMODE_NTSC_INT: // 0
		return 720;
	}
}

#if defined(VERSION_GMSP01) || defined(VERSION_GMSP01_NX)
u16 JDrama::GetVIHeightMax(u32 tvFormat)
#else
u16 JDrama::GetVIHeightMax()
#endif
{
#if !(defined(VERSION_GMSP01) || defined(VERSION_GMSP01_NX))
	const u32 tvFormat = VIGetTvFormat();
#endif
	switch (tvFormat) {
	case VI_TVMODE_NTSC_DS: // 1
		return 574;
	case VI_TVMODE_NTSC_PROG: // 2
		return 480;
	case VI_TVMODE_PAL_DS: // 5
		return 480;
	default:
	case VI_TVMODE_NTSC_INT: // 0
		return 480;
	}
}
