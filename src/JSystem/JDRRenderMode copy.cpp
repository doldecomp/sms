#ifndef __JDR_COMMON_H__
#define __JDR_COMMON_H__

#ifndef _DOL_TYPES_H_
#define _DOL_TYPES_H_

#ifndef NULL
#define NULL (void *)0
#endif

typedef signed char s8;
typedef unsigned char u8;
typedef signed short int s16;
typedef unsigned short int u16;
typedef signed long s32;
typedef unsigned long u32;
typedef signed long long int s64;
typedef unsigned long long int u64;

typedef volatile u8 vu8;
typedef volatile u16 vu16;
typedef volatile u32 vu32;
typedef volatile u64 vu64;
typedef volatile s8 vs8;
typedef volatile s16 vs16;
typedef volatile s32 vs32;
typedef volatile s64 vs64;

typedef float f32;
typedef double f64;

#endif

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

namespace JDrama
{

	/* JDRRenderMode: */
	bool IsEqualRenderModeVIParams(const GXRenderModeObj &, const GXRenderModeObj &);
	bool IsRenderModeHalfAspectRatio(const GXRenderModeObj &);
	f32 GetRenderModeYScale(const GXRenderModeObj &);
	void CalcRenderModeXFBHeight(GXRenderModeObj *, u16);
	void CalcRenderModeVIXOrigin(GXRenderModeObj *rmo);
	void CalcRenderModeVIYOrigin(GXRenderModeObj *rmo);
	void CopyRenderModeSamplePattern(GXRenderModeObj *rmo, const u8 (*s)[2]);
	void CopyRenderModeVFilter(GXRenderModeObj *rmo, const u8 *s);

	/* JDRResolution: */
	s32 GetVIWidthMax();
	s32 GetVIHeightMax();

}

extern "C"
{
#define OSHalt(msg) OSPanic(__FILE__, __LINE__, msg)
	void OSPanic(const char *file, int line, const char *msg, ...);

	f32 GXGetYScaleFactor(u16 efbHeight, u16 xfbHeight);
	u16 GXGetNumXfbLines(u16 efbHeight, f32 yScale);

	VITVMode VIGetTvFormat();
}

s32 JDrama::GetVIWidthMax()
{
	switch (VIGetTvFormat())
	{
	case VI_TVMODE_NTSC_DS:			// 1
		return 720;
	case VI_TVMODE_NTSC_PROG:		// 2
		return 720;
	case VI_TVMODE_PAL_DS:			// 5
		return 720;
	default:
	case VI_TVMODE_NTSC_INT:		// 0
		return 720;
	}
}


s32 JDrama::GetVIHeightMax()
{
	switch (VIGetTvFormat())
	{
	case VI_TVMODE_NTSC_DS:			// 1
		return 574;
	case VI_TVMODE_NTSC_PROG:		// 2
		return 480;
	case VI_TVMODE_PAL_DS:			// 5
		return 480;
	default:
	case VI_TVMODE_NTSC_INT:		// 0
		return 480;
	}
}
