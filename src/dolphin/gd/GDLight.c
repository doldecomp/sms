#include <dolphin/gd/GDLight.h>
#include "__gd.h"

void GDSetLightAttn(GXLightID light, float a0, float a1, float a2, float k0,
                    float k1, float k2)
{
	GDWriteXFCmdHdr(__GDLightID2Offset(k2) + 0x604, 6);
	GDWrite_f32(light);
	GDWrite_f32(a0);
	GDWrite_f32(a1);
	GDWrite_f32(a2);
	GDWrite_f32(k0);
	GDWrite_f32(k1);
}

void GDSetLightColor(GXLightID light, GXColor* color)
{
	GDWriteXFCmd(__GDLightID2Offset(light) + 0x603,
	             color->r << 24 | color->g << 16 | color->b << 8 | color->a);
}

void GDSetLightPos(GXLightID light, float x, float y, float z)
{
	GDWriteXFCmdHdr(__GDLightID2Offset(light) + 0x60a, 2);
	GDWrite_f32(x);
	GDWrite_f32(y);
	GDWrite_f32(z);
}

void GDSetLightDir(GXLightID light, float nx, float ny, float nz)
{
	GDWriteXFCmdHdr(__GDLightID2Offset(light) + 0x60d, 2);
	GDWrite_f32(nx);
	GDWrite_f32(ny);
	GDWrite_f32(nz);
}

void GDSetChanMatColor(GXChannelID chan, GXColor* color)
{
	GDWriteXFCmd((chan & 1) + 0x100c,
	             color->r << 24 | color->g << 16 | color->b << 8 | color->a);
}

void GDSetChanCtrl(GXChannelID chan, u8 enable, GXColorSrc amb_src,
                   GXColorSrc mat_src, u32 light_mask, GXDiffuseFn diff_fn,
                   GXAttnFn attn_fn)
{
	u32 reg;

	reg = 0;
	reg |= mat_src;
	reg |= enable << 1;
	reg |= ((attn_fn == 0 ? 0 : diff_fn) << 7);
	reg |= (light_mask & 0xf) << 2;
	reg |= (light_mask & 0xf0) << 7;
	reg |= (attn_fn != 0) << 10;
	reg |= (attn_fn != 2) << 9;
	reg |= amb_src << 6;
	GDWriteXFCmd((chan & 3) + 0x100e, reg);
	if ((chan == 4) || (chan == 5)) {
		GDWriteXFCmd(chan + 0x100c, reg);
	}
}
