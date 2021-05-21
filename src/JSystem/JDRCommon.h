#ifndef __JDR_COMMON_H__
#define __JDR_COMMON_H__

#include "types.h"

extern "C"
{
	void *memcpy(void *, const void *, u32);

#define VI_DISPLAY_PIX_SZ 2

#define VI_INTERLACE 0
#define VI_NON_INTERLACE 1
#define VI_PROGRESSIVE 2
	// 3 is reserved

#define VI_NTSC 0
#define VI_PAL 1
#define VI_MPAL 2
#define VI_DEBUG 3
#define VI_DEBUG_PAL 4
#define VI_EURGB60 5
	// 6 and 7 are reserved

#define VI_TVMODE(FMT, INT) (((FMT) << 2) + (INT))

	typedef enum
	{
		VI_TVMODE_NTSC_INT = VI_TVMODE(VI_NTSC, VI_INTERLACE),
		VI_TVMODE_NTSC_DS = VI_TVMODE(VI_NTSC, VI_NON_INTERLACE),
		VI_TVMODE_NTSC_PROG = VI_TVMODE(VI_NTSC, VI_PROGRESSIVE),

		VI_TVMODE_PAL_INT = VI_TVMODE(VI_PAL, VI_INTERLACE),
		VI_TVMODE_PAL_DS = VI_TVMODE(VI_PAL, VI_NON_INTERLACE),

		VI_TVMODE_EURGB60_INT = VI_TVMODE(VI_EURGB60, VI_INTERLACE),
		VI_TVMODE_EURGB60_DS = VI_TVMODE(VI_EURGB60, VI_NON_INTERLACE),

		VI_TVMODE_MPAL_INT = VI_TVMODE(VI_MPAL, VI_INTERLACE),
		VI_TVMODE_MPAL_DS = VI_TVMODE(VI_MPAL, VI_NON_INTERLACE),

		VI_TVMODE_DEBUG_INT = VI_TVMODE(VI_DEBUG, VI_INTERLACE),

		VI_TVMODE_DEBUG_PAL_INT = VI_TVMODE(VI_DEBUG_PAL, VI_INTERLACE),
		VI_TVMODE_DEBUG_PAL_DS = VI_TVMODE(VI_DEBUG_PAL, VI_NON_INTERLACE)
	} VITVMode;

	typedef enum
	{
		VI_XFBMODE_SF = 0,
		VI_XFBMODE_DF
	} VIXFBMode;

	typedef struct _GXRenderModeObj
	{
		VITVMode viTVmode;
		u16 fbWidth;   // no xscale from efb to xfb
		u16 efbHeight; // embedded frame buffer
		u16 xfbHeight; // external frame buffer, may yscale efb
		u16 viXOrigin;
		u16 viYOrigin;
		u16 viWidth;
		u16 viHeight;
		VIXFBMode xFBmode;		  // whether single-field or double-field in
								  // XFB.
		u8 field_rendering;		  // rendering fields or frames?
		u8 aa;					  // antialiasing on?
		u8 sample_pattern[12][2]; // aa sample pattern
		u8 vfilter[7];			  // vertical filter coefficients
	} GXRenderModeObj;

} /*
extern "C" */

#endif /* __JDR_COMMON_H__ */
