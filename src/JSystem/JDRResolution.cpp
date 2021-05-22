#if !(defined(SMS_REGION) && (SMS_REGION == EU))
#define VIGetTvFormat	_dummy
#include "JSystem/JDrama.hpp"
#undef VIGetTvFormat
extern "C"						// TODO	this is a very hacky workaround
	VITVMode VIGetTvFormat(GXRenderModeObj *);
#else
#include "JSystem/JDrama.hpp"
#endif

#if defined(SMS_REGION) && (SMS_REGION == EU)
s32 JDrama::GetVIWidthMax(VITVMode tvm)
{
	switch (tvm)
#else
s32 JDrama::GetVIWidthMax(GXRenderModeObj *rmo)
{
	switch (VIGetTvFormat(rmo))
#endif
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

#if defined(SMS_REGION) && (SMS_REGION == EU)
s32 JDrama::GetVIHeightMax(VITVMode tvm)
{
	switch (tvm)
#else
s32 JDrama::GetVIHeightMax(GXRenderModeObj *rmo)
{
	switch (VIGetTvFormat(rmo))
#endif
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
