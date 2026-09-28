#include <dolphin/gd/GDLight.h>
#include <dolphin/gd/GDBase.h>

void GDSetLightAttn(GXLightID light, float a0, float a1, float a2, float k0,
                    float k1, float k2)
{
	GDWriteXFCmdHdr(__GDLightID2Offset(light) + XF_LIGHT_ATTN_ID, 6);
	GDWrite_f32(a0);
	GDWrite_f32(a1);
	GDWrite_f32(a2);
	GDWrite_f32(k0);
	GDWrite_f32(k1);
	GDWrite_f32(k2);
}

void GDSetLightColor(GXLightID light, GXColor color)
{
	GDWriteXFCmd(__GDLightID2Offset(light) + XF_LIGHT_COLOR_ID,
	             color.r << 24 | color.g << 16 | color.b << 8 | color.a);
}

void GDSetLightPos(GXLightID light, float x, float y, float z)
{
	GDWriteXFCmdHdr(__GDLightID2Offset(light) + XF_LIGHT_POS_ID, 3);
	GDWrite_f32(x);
	GDWrite_f32(y);
	GDWrite_f32(z);
}

void GDSetLightDir(GXLightID light, float nx, float ny, float nz)
{
	GDWriteXFCmdHdr(__GDLightID2Offset(light) + XF_LIGHT_DIR_ID, 3);
	GDWrite_f32(nx);
	GDWrite_f32(ny);
	GDWrite_f32(nz);
}

void GDSetChanMatColor(GXChannelID chan, GXColor color)
{
	GDWriteXFCmd((chan & 1) + XF_REG_MATERIAL0_ID,
	             color.r << 24 | color.g << 16 | color.b << 8 | color.a);
}

void GDSetChanCtrl(GXChannelID chan, u8 enable, GXColorSrc amb_src,
                   GXColorSrc mat_src, u32 light_mask, GXDiffuseFn diff_fn,
                   GXAttnFn attn_fn)
{
	u32 reg;

	reg = XF_REG_CHAN_CTRL(mat_src, enable, light_mask & 0xF, amb_src,
	                       attn_fn == GX_AF_SPEC ? GX_DF_NONE : diff_fn,
	                       attn_fn != GX_AF_NONE, attn_fn != GX_AF_SPEC,
	                       (light_mask >> 4) & 0xF);
	GDWriteXFCmd((chan & 3) + XF_REG_COLOR0CNTRL_ID, reg);
	if ((chan == 4) || (chan == 5)) {
		GDWriteXFCmd(chan + XF_REG_MATERIAL0_ID, reg);
	}
}
