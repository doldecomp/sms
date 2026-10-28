
#include <GC2D/hx_wiper.h>
#include <JSystem/ResTIMG.hpp>
#include <MarioUtil/ReinitGX.hpp>
#include <dolphin/dvd.h>
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <dolphin/os/OSCache.h>
#include <dolphin/vi.h>
#include <fake_tgmath.h>
#include <version.h>

#if defined(VERSION_GMSP01) || defined(VERSION_GMSJ01_01)                      \
    || defined(VERSION_GMSP01_NX)
#define HX_TV_FORMAT_TIMING
#endif

f32 sinf(f32);
f32 cosf(f32);

typedef struct HxMotion {
	f32 unk0;
	f32 unk4;
	f32 unk8;
	f32 unkC;
	f32 unk10;
	f32 unk14;
	f32 unk18;
	f32 unk1C;
	f32 unk20;
} HxMotion;

typedef struct HxWiper {
	u32 width;
	u32 height;
	u32 halfWidth;
	u32 halfHeight;
	u8 state;
	u8 type;
	u8 direction;
	u8 _padding13;
	f32 elapsed;
	f32 deltaTime;
	s32 resourceArg;
	void (*handler)(void);
	s32 hasResource;
	s32 hasResourceEx;
	void* resource;
	void* resourceEx;
	u32 resourceSize;
	s32 animationState;
	u32 timer;
	HxMotion motion;
#ifdef HX_TV_FORMAT_TIMING
	u8 isPal;
#endif
} HxWiper;

typedef struct HxDrawPath {
	f32 x;
	f32 y;
	s32 type;
} HxDrawPath;

static void Hx_SetVFilter(f32);
static void __Hx_FrBufferMorf(u32, u32);
static void Hx_CameraInit(void);
#ifdef VERSION_GMSP01_NX
static void Hx_CameraInitWide(void);
static void Hx_CameraInitWide2(void);
#endif
static void Hx_GxInit(int, int);
static void Hx_Circle(void);
static void Hx_Test1(void);
static void Hx_Test5(void);
static void Hx_Test4(void);
static void Hx_Test2R(void);
static void Hx_Test2(void);
static void Hx_Door(void);
static void Hx_Logo(void);
static void Hx_GameOver(void);
static void Hxs1_Test1(f32, f32, f32);
static void Hxs1_Test2(u32, u32, f32, f32, f32, f32);
static void Hxs1_Circle(f32);
static void Hxs2_Circle(u8, f32, f32);
static void Hxs_FrBufferMorf2(f32);
static void Hxs_FrBufferMorf2B(f32);
static void Hxs_Logo_TexDraw(f32, f32, f32, f32, f32, f32);
static void dummy_handler(void);

static HxWiper hx;
static u8 hx_buffer[0x3300] __attribute__((aligned(32)));
static u16 img_wx;
static u16 img_wy;
static u8 vtable[7];

#ifdef VERSION_GMSP01_NX
static void Hx_CameraInitParam(int wide, int wide2)
{
	static Vec camLoc;
	static Vec objPt;
	static Vec up = { 0.0f, -10.0f, 0.0f };
	Mtx44 projection;
	Mtx view;
	f32 extraWidth;
	f32 halfWidth  = (f32)(hx.width >> 1);
	f32 halfHeight = (f32)(hx.height >> 1);
	f32 near       = 0.0f;
	f32 far        = 100.0f;

	camLoc.x = halfWidth;
	camLoc.y = halfHeight;
	camLoc.z = -30.0f;
	objPt.x  = halfWidth;
	objPt.y  = halfHeight;
	objPt.z  = 0.0f;
	if (wide != 0)
		extraWidth = 0.0f;
	else
		extraWidth = 110.0f;
	C_MTXOrtho(projection, halfHeight, -halfHeight, -halfWidth - extraWidth,
	           halfWidth + extraWidth, near, far);
	GXSetProjection(projection, GX_ORTHOGRAPHIC);
	GXSetViewport(0.0f, 0.0f, 640.0f, wide2 != 0 ? 480.0f : 385.71429f, 0.0f,
	              1.0f);
	C_MTXLookAt(view, &camLoc, &up, &objPt);
	GXSetCullMode(GX_CULL_NONE);
	GXSetCoPlanar(GX_DISABLE);
	GXSetZMode(GX_DISABLE, GX_ALWAYS, GX_DISABLE);
	GXSetNumTexGens(0);
	GXSetNumTevStages(1);
	GXSetNumIndStages(0);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
	GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
	GXSetLineWidth(6, GX_TO_ZERO);
	GXLoadPosMtxImm(view, GX_PNMTX0);
	GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_VTX, GX_SRC_VTX,
	              GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
	GXSetChanCtrl(GX_COLOR1A1, GX_DISABLE, GX_SRC_REG, GX_SRC_REG,
	              GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
	GXSetNumChans(1);
}

static void Hx_CameraInitWide2(void) { Hx_CameraInitParam(1, 1); }

static void Hx_CameraInitWide(void) { Hx_CameraInitParam(1, 0); }

static void Hx_CameraInit(void) { Hx_CameraInitParam(0, 0); }
#else
static void Hx_CameraInit(void)
{
	static Vec camLoc = { 320.0f, 240.0f, -30.0f };
	static Vec objPt  = { 320.0f, 240.0f, 0.0f };
	static Vec up     = { 0.0f, -10.0f, 0.0f };
	Mtx44 projection;
	Mtx view;
	f32 halfWidth  = (f32)(hx.width >> 1);
	f32 halfHeight = (f32)(hx.height >> 1);
	f32 near       = 0.0f;
	f32 far        = 100.0f;

	camLoc.x = halfWidth;
	camLoc.y = halfHeight;
	objPt.x  = halfWidth;
	objPt.y  = halfHeight;
	C_MTXOrtho(projection, halfHeight, -halfHeight, -halfWidth, halfWidth, near,
	           far);
	GXSetProjection(projection, GX_ORTHOGRAPHIC);
	GXSetViewport(0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
	C_MTXLookAt(view, &camLoc, &up, &objPt);
	GXSetCullMode(GX_CULL_NONE);
	GXSetCoPlanar(GX_DISABLE);
	GXSetZMode(GX_DISABLE, GX_ALWAYS, GX_DISABLE);
	GXSetNumTexGens(0);
	GXSetNumTevStages(1);
	GXSetNumIndStages(0);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
	GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
	GXSetLineWidth(6, GX_TO_ZERO);
	GXLoadPosMtxImm(view, GX_PNMTX0);
	GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_VTX, GX_SRC_VTX,
	              GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
	GXSetChanCtrl(GX_COLOR1A1, GX_DISABLE, GX_SRC_REG, GX_SRC_REG,
	              GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
	GXSetNumChans(1);
}
#endif

static void (*handle_table[15])(void) = {
	dummy_handler, Hx_Circle, Hx_Circle, Hx_Test1,    Hx_Test1,
	Hx_Test5,      Hx_Test5,  Hx_Test4,  Hx_Test4,    Hx_Test2R,
	Hx_Test2,      Hx_Door,   Hx_Logo,   Hx_GameOver, dummy_handler,
};
static u8 handle_type[15] = {
	0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0,
};
static void* fbuf              = hx_buffer;
static u8 vtable_org[7]        = { 0x10, 0x10, 0, 0, 0, 0x10, 0x10 };
static u8 dec_step[4]          = { 0, 1, 5, 6 };
static u8 inc_step[3]          = { 2, 3, 4 };
static void* fbuf2             = hx_buffer;
static void* gmover_tex_buffer = hx_buffer;

static void Hx_GxInit(int texture, int blend)
{
	switch (texture) {
	case 0:
		GXSetNumTexGens(0);
		GXSetNumTevStages(1);
		GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
		              GX_COLOR0A0);
		GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
		break;
	case 1:
		GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY,
		                  GX_FALSE, GX_PTIDENTITY);
		GXSetNumTexGens(1);
		GXSetNumTevStages(1);
		GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
		GXClearVtxDesc();
		GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
		GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
		GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
		GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
		GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
		GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
		break;
	}

	switch (blend) {
	case 1:
		GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA,
		               GX_LO_CLEAR);
		return;
	case 0:
		GXSetBlendMode(GX_BM_NONE, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_CLEAR);
		return;
	}
}

static void Hgx_DrawCircle(void) { }

static void Hgx_init_tobj_resource(GXTexObj* texObj, struct ResTIMG* resource)
{
	u8* imageData = (u8*)resource;
	u8 format     = resource->format;
	u8 wrapS      = resource->wrapS;
	u8 wrapT      = resource->wrapT;
	u8 minFilter  = resource->minFilter;
	u8 magFilter  = resource->magFilter;

	imageData += resource->imageDataOffset;

	img_wx = resource->width;
	img_wy = resource->height;
	GXInitTexObj(texObj, imageData, img_wx, img_wy, (GXTexFmt)format,
	             (GXTexWrapMode)wrapS, (GXTexWrapMode)wrapT, GX_FALSE);
	GXInitTexObjLOD(texObj, (GXTexFilter)minFilter, (GXTexFilter)magFilter,
	                0.0f, 0.0f, 0.0f, GX_DISABLE, GX_DISABLE, GX_ANISO_1);
}

static void Hgx_ReadTexture(const char* path, void* destination)
{
	DVDFileInfo fileInfo;
	s32 readSize;

#ifdef VERSION_GMSP01_NX
	if (hx.hasResource == 0) {
#else
	switch (hx.hasResource) {
	case 0:
#endif
		if (DVDOpen(path, &fileInfo) != 0) {
			readSize
			    = DVDReadPrio(&fileInfo, destination, fileInfo.length, 0, 2);
			DVDClose(&fileInfo);
			DCStoreRange(destination, readSize);
		}
	}
}

static void Hx_GetFrBuffer(void* buffer, u32 left, u32 top, u32 width,
                           u32 height)
{
	GXColor clear = { 0, 0, 0, 0 };

	GXSetTexCopySrc((u16)left, (u16)top, (u16)width, (u16)height);
	GXSetTexCopyDst((u16)width, (u16)height, GX_TF_RGB565, GX_FALSE);
	GXGetTexBufferSize((u16)width, (u16)height, GX_TF_RGB565, GX_FALSE, 0);
	GXSetCopyClear(clear, 0xFFFFFF);
	GXCopyTex(buffer, GX_TRUE);
	GXPixModeSync();
}

static void Hx_SetVFilter(f32 strength)
{
	u8 count;
	u32 i;

	count = 64.0f * strength;
	for (i = 0; i < 7; ++i)
		vtable[i] = vtable_org[i];
	for (i = 0; i < count; ++i) {
		vtable[dec_step[i % 4]]--;
		vtable[inc_step[i % 3]]++;
	}
	GXSetCopyFilter(GX_FALSE, NULL, GX_TRUE, vtable);
}

void Hx_SetVFilterFade(void) { }

static void __Hx_FrBufferMorf(u32 left, u32 top)
{
	GXTexObj texObj;

	Hx_CameraInit();
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
	Hx_GetFrBuffer(fbuf, left, top, 48, 48);
	GXInvalidateTexAll();
	GXSetNumTexGens(1);
	GXSetNumTevStages(1);
	GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
	GXSetBlendMode(GX_BM_NONE, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_CLEAR);
	GXInitTexObj(&texObj, fbuf, 48, 48, GX_TF_RGB565, GX_CLAMP, GX_CLAMP,
	             GX_FALSE);
	GXInitTexObjLOD(&texObj, GX_LINEAR, GX_LINEAR, 0.0f, 10.0f, 0.0f,
	                GX_DISABLE, GX_ENABLE, GX_ANISO_1);
	GXLoadTexObj(&texObj, GX_TEXMAP0);
	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition3f32(left, top, 0.0f);
	GXColor1u32(0);
	GXTexCoord2f32(0.0f, 0.0f);
	GXPosition3f32(left + 48, top, 0.0f);
	GXColor1u32(0);
	GXTexCoord2f32(1.0f, 0.0f);
	GXPosition3f32(left + 48, top + 48, 0.0f);
	GXColor1u32(0);
	GXTexCoord2f32(1.0f, 1.0f);
	GXPosition3f32(left, top + 48, 0.0f);
	GXColor1u32(0);
	GXTexCoord2f32(0.0f, 1.0f);
	GXEnd();
}

static void Hx_FrBufferMorf(f32 strength)
{
	Hx_SetVFilter(strength);
	__Hx_FrBufferMorf(hx.halfWidth - 24, hx.halfHeight - 24);
}

static void Frb2_InitGx(GXTexObj* texObj)
{
#ifdef VERSION_GMSP01_NX
	Hx_CameraInitWide2();
#else
	Hx_CameraInit();
#endif
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
	Hx_SetVFilter(1.0f);
	GXSetNumTexGens(1);
	GXSetNumTevStages(1);
	GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
	GXSetBlendMode(GX_BM_NONE, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_CLEAR);
	GXInitTexObj(texObj, fbuf2, 160, 16, GX_TF_RGB565, GX_CLAMP, GX_CLAMP,
	             GX_FALSE);
	GXInitTexObjLOD(texObj, GX_LINEAR, GX_LINEAR, 0.0f, 10.0f, 0.0f, GX_DISABLE,
	                GX_ENABLE, GX_ANISO_1);
	GXLoadTexObj(texObj, GX_TEXMAP0);
}

static void Frb2_InitBlackBox(void)
{
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
	GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEX_DISABLE, GX_COLOR0A0);
}

static void Frb2_RendBox(u32 color, f32 left, f32 top, f32 right, f32 bottom)
{
	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition3f32(left, top, 0.0f);
	GXColor1u32(color);
	GXPosition3f32(right, top, 0.0f);
	GXColor1u32(color);
	GXPosition3f32(right, bottom, 0.0f);
	GXColor1u32(color);
	GXPosition3f32(left, bottom, 0.0f);
	GXColor1u32(color);
	GXEnd();
}

static void Hx_Warning(int warning) { }

void SetDisplaySize(void) { }

void Hx_ResetWipe(u32 width, u32 height)
{
	hx.state         = 0;
	hx.width         = width;
	hx.height        = height;
	hx.halfWidth     = hx.width >> 1;
	hx.halfHeight    = hx.height >> 1;
	hx.hasResource   = 0;
	hx.hasResourceEx = 0;
}

void Hx_ProvideResource(void* resource, int size)
{
	if (hx.state == 2)
		Hx_Warning(1);
	if (hx.hasResource != 0)
		Hx_Warning(3);
	hx.hasResource  = 1;
	hx.resource     = resource;
	hx.resourceSize = size;
}

void Hx_ProvideResourceEx(void* resource)
{
	if (hx.state == 2)
		Hx_Warning(1);
	hx.hasResourceEx = 1;
	hx.resourceEx    = resource;
}

void Hx_RemoveResource(void)
{
	if (hx.state == 2)
		Hx_Warning(1);
	if (hx.hasResource == 0)
		Hx_Warning(2);
	hx.hasResource   = 0;
	hx.hasResourceEx = 0;
}

void Hx_StartWipe(int type, int resourceArg)
{
#ifdef HX_TV_FORMAT_TIMING
	if (VIGetTvFormat() == VI_PAL)
		hx.isPal = 1;
	else
		hx.isPal = 0;
#endif
	if (hx.hasResource == 0) {
		hx.resource     = hx_buffer;
		hx.resourceSize = sizeof(hx_buffer);
	}
	switch (hx.state) {
	case 2:
		Hx_Warning(1);
	}
	hx.state       = 1;
	hx.type        = type;
	hx.elapsed     = 0.0f;
	hx.resourceArg = resourceArg;
}

static void dummy_handler(void) { }

int Hx_GetWipeType(int type) { return handle_type[type]; }

u32 Hx_UpdateWipe(f32 deltaTime)
{
	ReInitializeGX();
	switch (hx.state) {
	case 0:
		break;
	case 3:
		if (hx.direction != 1) {
			Hx_CameraInit();
			Hx_GxInit(0, 0);
			Frb2_InitBlackBox();
			Frb2_RendBox(0xFF, 0.0f, 0.0f, (f32)hx.width, (f32)hx.height);
		}
		break;
	case 1:
		hx.handler        = handle_table[hx.type];
		hx.direction      = handle_type[hx.type];
		hx.state          = 2;
		hx.animationState = 0;
		/* fall through */
	case 2:
		hx.deltaTime = deltaTime;
		GXDrawDone();
		hx.handler();
		GXDrawDone();
		hx.elapsed += deltaTime;
		break;
	}
	return hx.state;
}

static u32 Hx_TimerCountDown(void)
{
	if (hx.timer != 0)
		hx.timer--;
	return hx.timer;
}

static void Hx_MotionSet(HxMotion* motion, f32 distance, f32 time1, f32 time2,
                         f32 time3)
{
	f32 velocity;

	motion->unk0 = time1;
	motion->unk4 = motion->unk0 + time2;
	motion->unk8 = motion->unk4 + time3;
	velocity     = (2.0f * distance) / (time3 + (time2 + (time1 + time2)));
	if (time1 != 0.0f)
		motion->unkC = velocity / time1;
	if (time3 != 0.0f)
		motion->unk14 = -velocity / time3;
	motion->unk10 = 0.0f;
	motion->unk18 = 0.0f;
	motion->unk20 = 0.0f;
	motion->unk1C = 0.0f;
}

static f32 Hx_MotionUpdate(HxMotion* motion)
{
	if (motion->unk0 > motion->unk1C)
		motion->unk18 += motion->unkC;
	else if (motion->unk4 <= motion->unk1C)
		motion->unk18 += motion->unk14;
	motion->unk1C += 1.0f;
	motion->unk20 += motion->unk18;
	return motion->unk20;
}

#ifdef HX_TV_FORMAT_TIMING
static u32 HX_CIRCLE_STATE1_TIME;
static u32 HX_CIRCLE_STATE1_TIME2;
#endif

static void Hx_Circle(void)
{
	static f32 r;
	static f32 p1;
	static f32 p2;
	static f32 p3;
	static u16 a1;
	static u16 a2;
	static u16 a3;
	static f32 boke;

#ifdef HX_TV_FORMAT_TIMING
	switch (hx.isPal) {
	case 1:
		HX_CIRCLE_STATE1_TIME  = 25;
		HX_CIRCLE_STATE1_TIME2 = 20;
		break;
	default:
		HX_CIRCLE_STATE1_TIME  = 30;
		HX_CIRCLE_STATE1_TIME2 = 25;
		break;
	}
#endif

	switch (hx.animationState) {
	case 0:
		p3   = 0.0f;
		p2   = 0.0f;
		p1   = 0.0f;
		a3   = 0;
		a2   = 0;
		a1   = 0;
		r    = 1.0f;
		boke = 0.0f;
		switch (hx.direction) {
		case 0:
#ifdef HX_TV_FORMAT_TIMING
			Hx_MotionSet(&hx.motion, VERSION_SELECT(400.0f, GMSP01_NX(533.0f)),
			             2.0f, HX_CIRCLE_STATE1_TIME2 - 12, 10.0f);
			hx.timer = HX_CIRCLE_STATE1_TIME2;
#else
			Hx_MotionSet(&hx.motion, 400.0f, 2.0f, 13.0f, 10.0f);
			hx.timer = 25;
#endif
			break;
		case 1:
#ifdef HX_TV_FORMAT_TIMING
			Hx_MotionSet(&hx.motion, VERSION_SELECT(400.0f, GMSP01_NX(533.0f)),
			             5.0f, HX_CIRCLE_STATE1_TIME - 20, 15.0f);
			hx.timer = HX_CIRCLE_STATE1_TIME;
#else
			Hx_MotionSet(&hx.motion, 400.0f, 5.0f, 10.0f, 15.0f);
			hx.timer = 30;
#endif
			break;
		}
		hx.animationState++;
		/* fall through */
	case 1:
		r = Hx_MotionUpdate(&hx.motion);
		switch (hx.direction) {
		case 1:
#ifdef HX_TV_FORMAT_TIMING
			boke += 2.0f / HX_CIRCLE_STATE1_TIME;
#else
			boke += 0.06666667f;
#endif
			if (boke > 1.0f)
				boke = 1.0f;
#ifndef VERSION_GMSP01_NX
			Hx_FrBufferMorf(boke);
#endif
			Hx_SetVFilter(1.0f);
			break;
		case 0:
			r = 400.0f - r;
			if (r < 0.0f)
				r = 0.0f;
			break;
		}
		if (Hx_TimerCountDown() == 0) {
			hx.animationState++;
			hx.state = 3;
		}
		break;
	default:
		hx.state = 3;
		break;
	}

	Hxs1_Circle(r);
	if (r > 22.0f) {
		Hxs2_Circle((u8)((s32)a1 >> 8), (r - 20.0f) + p1, r);
		p1 += 0.05f;
		if (a1 < (u16)-0x100)
			a1 += 0x180;
	}
	if (r > 42.0f) {
		f32 radius = r - 40.0f;
		Hxs2_Circle((u8)((s32)a2 >> 8), radius + p2, (r - 20.0f) + p1);
		p2 += 0.12f;
		if (a2 < (u16)-0x100)
			a2 += 0xC0;
	}
	if (r > 62.0f) {
		f32 radius = r - 60.0f;
		Hxs2_Circle((u8)((s32)a3 >> 8), radius + p3, (r - 40.0f) + p2);
		p3 += 0.25f;
		if (a3 < (u16)-0x100)
			a3 += 0x80;
	}
}

static void Hxs1_Circle(f32 radius)
{
	f32 x[2];
	f32 y[2];
	f32 radiusSquared;
	f32 distance;
	u32 i;

	Hx_CameraInit();
	Hx_GxInit(0, 1);
	radiusSquared = radius * radius;
	for (i = 0; i <= hx.halfHeight; ++i) {
		distance = hx.halfHeight - i;
		y[0]     = i;
		y[1]     = i;
		if (hx.halfHeight - i >= radius) {
			GXBegin(GX_LINES, GX_VTXFMT0, 4);
			x[0] = VERSION_SELECT(0.0f, GMSP01_NX(-110.0f));
			x[1] = hx.width + VERSION_SELECT(0, GMSP01_NX(110));
			GXPosition3f32(x[0], y[0], 1.0f);
			GXColor1u32(0xFF);
			GXPosition3f32(x[1], y[1], 1.0f);
			GXColor1u32(0xFF);
			y[0] = hx.height - i;
			y[1] = hx.height - i;
			GXPosition3f32(x[0], y[0], 1.0f);
			GXColor1u32(0xFF);
			GXPosition3f32(x[1], y[1], 1.0f);
			GXColor1u32(0xFF);
			GXEnd();
		} else {
			f32 offset = sqrtf(radiusSquared - distance * distance);

			GXBegin(GX_LINES, GX_VTXFMT0, 8);
			x[0] = VERSION_SELECT(0.0f, GMSP01_NX(-110.0f));
			x[1] = hx.halfWidth - offset;
			GXPosition3f32(x[0], y[0], 1.0f);
			GXColor1u32(0xFF);
			GXPosition3f32(x[1], y[1], 1.0f);
			GXColor1u32(0xFF);
			x[0] = hx.halfWidth + offset;
			x[1] = hx.width + VERSION_SELECT(0, GMSP01_NX(110));
			GXPosition3f32(x[0], y[0], 1.0f);
			GXColor1u32(0xFF);
			GXPosition3f32(x[1], y[1], 1.0f);
			GXColor1u32(0xFF);
			y[0] = hx.height - i;
			y[1] = hx.height - i;
			GXPosition3f32(x[0], y[0], 1.0f);
			GXColor1u32(0xFF);
			GXPosition3f32(x[1], y[1], 1.0f);
			GXColor1u32(0xFF);
			x[0] = VERSION_SELECT(0.0f, GMSP01_NX(-110.0f));
			x[1] = hx.halfWidth - offset;
			GXPosition3f32(x[0], y[0], 1.0f);
			GXColor1u32(0xFF);
			GXPosition3f32(x[1], y[1], 1.0f);
			GXColor1u32(0xFF);
			GXEnd();
		}
	}
}

static void Hxs2_Circle(u8 color, f32 innerRadius, f32 outerRadius)
{
	f32 x[2];
	f32 y[2];
	f32 outerOffset;
	f32 innerOffset;
	f32 distance;
	f32 innerRadiusSquared;
	f32 outerRadiusSquared;
	u32 i;
	u32 rgba[2];

	Hx_CameraInit();
	Hx_GxInit(0, 1);
	rgba[0]            = color;
	rgba[1]            = color;
	innerRadiusSquared = innerRadius * innerRadius;
	outerRadiusSquared = outerRadius * outerRadius;
	for (i = hx.halfHeight - outerRadius; i <= hx.halfHeight; ++i) {
		distance    = hx.halfHeight - i;
		outerOffset = sqrtf(outerRadiusSquared - distance * distance);
		y[0]        = i;
		y[1]        = i;
		if (distance >= innerRadius) {
			GXBegin(GX_LINES, GX_VTXFMT0, 4);
			x[0] = hx.halfWidth - outerOffset;
			x[1] = hx.halfWidth + outerOffset;
			GXPosition3f32(x[0], y[0], 1.0f);
			GXColor1u32(rgba[0]);
			GXPosition3f32(x[1], y[1], 1.0f);
			GXColor1u32(rgba[1]);
			y[0] = hx.height - i;
			y[1] = hx.height - i;
			GXPosition3f32(x[0], y[0], 1.0f);
			GXColor1u32(rgba[0]);
			GXPosition3f32(x[1], y[1], 1.0f);
			GXColor1u32(rgba[1]);
			GXEnd();
		} else {
			innerOffset = sqrtf(innerRadiusSquared - distance * distance);
			GXBegin(GX_LINES, GX_VTXFMT0, 8);
			x[0] = hx.halfWidth - outerOffset;
			x[1] = hx.halfWidth - innerOffset;
			GXPosition3f32(x[0], y[0], 1.0f);
			GXColor1u32(rgba[0]);
			GXPosition3f32(x[1], y[1], 1.0f);
			GXColor1u32(rgba[1]);
			x[0] = hx.halfWidth + innerOffset;
			x[1] = hx.halfWidth + outerOffset;
			GXPosition3f32(x[0], y[0], 1.0f);
			GXColor1u32(rgba[0]);
			GXPosition3f32(x[1], y[1], 1.0f);
			GXColor1u32(rgba[1]);
			y[0] = hx.height - i;
			y[1] = hx.height - i;
			GXPosition3f32(x[0], y[0], 1.0f);
			GXColor1u32(rgba[0]);
			GXPosition3f32(x[1], y[1], 1.0f);
			GXColor1u32(rgba[1]);
			x[0] = hx.halfWidth - outerOffset;
			x[1] = hx.halfWidth - innerOffset;
			GXPosition3f32(x[0], y[0], 1.0f);
			GXColor1u32(rgba[0]);
			GXPosition3f32(x[1], y[1], 1.0f);
			GXColor1u32(rgba[1]);
			GXEnd();
		}
	}
}

static void Hxs_FrBufferMorf2(f32 amount)
{
	GXTexObj texObj;
	f32 y;

	Frb2_InitGx(&texObj);
	if (amount < (f32)(hx.width >> 2)) {
		for (y = 0.0f; y < (f32)hx.height; y += 16.0f) {
			Hx_GetFrBuffer(fbuf2, 0, (u32)y, 160, 16);
			GXInvalidateTexAll();
			GXLoadTexObj(&texObj, GX_TEXMAP0);
			GXBegin(GX_QUADS, GX_VTXFMT0, 4);
			GXPosition3f32(amount, y, 0.0f);
			GXColor1u32(0);
			GXTexCoord2f32(0.0f, 0.0f);
			GXPosition3f32((f32)(hx.width >> 2), y, 0.0f);
			GXColor1u32(0);
			GXTexCoord2f32(1.0f, 0.0f);
			GXPosition3f32((f32)(hx.width >> 2), y + 16.0f, 0.0f);
			GXColor1u32(0);
			GXTexCoord2f32(1.0f, 1.0f);
			GXPosition3f32(amount, y + 16.0f, 0.0f);
			GXColor1u32(0);
			GXTexCoord2f32(0.0f, 1.0f);
			GXEnd();
			GXDrawDone();
		}
	}
	Frb2_InitBlackBox();
	Frb2_RendBox(0xFF, 0.0f, 0.0f, amount, (f32)hx.height);
	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition3f32(0.0f, 0.0f, 0.0f);
	GXColor1u32(0xFF);
	GXPosition3f32(amount, 0.0f, 0.0f);
	GXColor1u32(0xFF);
	GXPosition3f32(amount, (f32)hx.height, 0.0f);
	GXColor1u32(0xFF);
	GXPosition3f32(0.0f, (f32)hx.height, 0.0f);
	GXColor1u32(0xFF);
	GXEnd();
}

static void Hxs_FrBufferMorf2B(f32 amount)
{
	GXTexObj texObj;
	s32 captureX = (hx.width >> 1) + (hx.width >> 2);
	f32 right;
	f32 y;

	Frb2_InitGx(&texObj);
	right = (f32)hx.width - amount;
	if (amount < (f32)(hx.width >> 2)) {
		for (y = 0.0f; y < (f32)hx.height; y += 16.0f) {
			Hx_GetFrBuffer(fbuf2, captureX, (u32)y, 160, 16);
			GXInvalidateTexAll();
			GXLoadTexObj(&texObj, GX_TEXMAP0);
			GXBegin(GX_QUADS, GX_VTXFMT0, 4);
			GXPosition3f32((f32)captureX, y, 0.0f);
			GXColor1u32(0);
			GXTexCoord2f32(0.0f, 0.0f);
			GXPosition3f32(right, y, 0.0f);
			GXColor1u32(0);
			GXTexCoord2f32(1.0f, 0.0f);
			GXPosition3f32(right, y + 16.0f, 0.0f);
			GXColor1u32(0);
			GXTexCoord2f32(1.0f, 1.0f);
			GXPosition3f32((f32)captureX, y + 16.0f, 0.0f);
			GXColor1u32(0);
			GXTexCoord2f32(0.0f, 1.0f);
			GXEnd();
			GXDrawDone();
		}
	}
	Frb2_InitBlackBox();
	Frb2_RendBox(0xFF, right, 0.0f, (f32)hx.width, (f32)hx.height);
}

static void Hx_Door(void)
{
	s32 position;

	switch (hx.animationState) {
	case 0:
		hx.animationState++;
		Hx_MotionSet(&hx.motion, (f32)(hx.width >> 1), 5.0f, 6.0f, 5.0f);
		break;
	case 1:
		position = (s32)Hx_MotionUpdate(&hx.motion);
		Hxs_FrBufferMorf2((f32)position);
		if ((u32)position >= (hx.width >> 1)) {
			hx.animationState++;
			Hx_MotionSet(&hx.motion, (f32)(hx.width >> 1), 5.0f, 6.0f, 5.0f);
		}
		break;
	case 2:
		Hxs_FrBufferMorf2((f32)(hx.width >> 1));
		position = (s32)Hx_MotionUpdate(&hx.motion);
		Hxs_FrBufferMorf2B((f32)position);
		if ((u32)position >= (hx.width >> 1))
			hx.animationState++;
		break;
	case 3:
		Hxs_FrBufferMorf2((f32)(hx.width >> 1));
		Hxs_FrBufferMorf2B((f32)(hx.width >> 1));
		hx.state = 3;
		break;
	}
}

static void Hxs_GameOver(u8 alpha, f32 mag, f32 rot)
{
	GXTexObj texObj;
	Vec dir;
	Mtx rotation;
	f32 cx;
	f32 cy;
	f32 len;
	f32 u0;
	f32 v0;
	f32 u1;
	f32 v1;
	f32 u2;
	f32 v2;
	f32 u3;
	f32 v3;
	f32 aspect;

#ifdef VERSION_GMSP01_NX
	Hx_CameraInitWide();
	mag *= 4.0f / 3.0f;
#else
	Hx_CameraInit();
#endif
	gmover_tex_buffer = hx.resource;
	Hgx_init_tobj_resource(&texObj, (struct ResTIMG*)gmover_tex_buffer);
	GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY,
	                  GX_FALSE, GX_PTIDENTITY);
	GXSetNumTexGens(1);
	GXSetNumTevStages(2);
	GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO,
	                GX_CC_ZERO);
	GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_TEXA, GX_CA_ZERO, GX_CA_ZERO,
	                GX_CA_ZERO);
	GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
	                GX_TRUE, GX_TEVPREV);
	GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
	                GX_TRUE, GX_TEVPREV);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
	GXSetTevColor(GX_TEVREG0, (GXColor) { 0, 0, 0, alpha });
	GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO,
	                GX_CC_ZERO);
	GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_A0, GX_CA_APREV,
	                GX_CA_ZERO);
	GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
	                GX_TRUE, GX_TEVPREV);
	GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
	                GX_TRUE, GX_TEVPREV);
	GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
	              GX_COLOR_NULL);
	GXLoadTexObj(&texObj, GX_TEXMAP0);
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
	GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
	aspect = (f32)(hx.width / hx.height) / ((f32)img_wx / (f32)img_wy);
	dir.x  = 0.5f;
	dir.y  = 0.5f;
	dir.z  = 0.0f;
	VECNormalize(&dir, &dir);
	MTXRotRad(rotation, 'Z', rot);
	MTXMultVec(rotation, &dir, &dir);
	cx  = 0.5f;
	cy  = 0.5f;
	len = mag * sqrtf(cx * cx + cy * cy);
	u0  = aspect * (-dir.x * len) + cx;
	v0  = -dir.y * len + cy;
	cx  = 0.5f;
	cy  = 0.5f;
	len = mag * sqrtf(cx * cx + cy * cy);
	u1  = aspect * (dir.y * len) + cx;
	v1  = -dir.x * len + cy;
	cx  = 0.5f;
	cy  = 0.5f;
	len = mag * sqrtf(cx * cx + cy * cy);
	u2  = aspect * (dir.x * len) + cx;
	v2  = dir.y * len + cy;
	cx  = 0.5f;
	cy  = 0.5f;
	len = mag * sqrtf(cx * cx + cy * cy);
	u3  = aspect * (-dir.y * len) + cx;
	v3  = dir.x * len + cy;
	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
#ifdef VERSION_GMSP01_NX
	GXPosition3f32(0.0f, -80.0f, 0.0f);
	GXColor1u32(0);
	GXTexCoord2f32(u0, v0);
	GXPosition3f32((f32)hx.width, -80.0f, 0.0f);
	GXColor1u32(0);
	GXTexCoord2f32(u1, v1);
	GXPosition3f32((f32)hx.width, (f32)(hx.height + 80), 0.0f);
	GXColor1u32(0);
	GXTexCoord2f32(u2, v2);
	GXPosition3f32(0.0f, (f32)(hx.height + 80), 0.0f);
#else
	GXPosition3f32(0.0f, 0.0f, 0.0f);
	GXColor1u32(0);
	GXTexCoord2f32(u0, v0);
	GXPosition3f32((f32)hx.width, 0.0f, 0.0f);
	GXColor1u32(0);
	GXTexCoord2f32(u1, v1);
	GXPosition3f32((f32)hx.width, (f32)hx.height, 0.0f);
	GXColor1u32(0);
	GXTexCoord2f32(u2, v2);
	GXPosition3f32(0.0f, (f32)hx.height, 0.0f);
#endif
	GXColor1u32(0);
	GXTexCoord2f32(u3, v3);
	GXEnd();
	GXSetBlendMode(GX_BM_NONE, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_CLEAR);
}

void InitWipe(void) { }

#ifdef HX_TV_FORMAT_TIMING
static u32 STATE1_TIME;
static u32 STATE1_TIME1;
static u32 STATE1_TIME2;
static u32 STATE1_TIME3;
static u32 STATE2_TIME;
static u32 STATE3_TIME;
#endif

static void Hx_GameOver(void)
{
	static f32 mag = 1.0f;
	static f32 rot;
	static f32 fade;
	static f32 boundtable[] = {
		-0.13f, 6.0f,   0.13f, 6.0f,  -0.12f, 8.0f, 0.12f,
		-8.0f,  -0.11f, 12.0f, 0.11f, 12.0f,  0.0f, 0.0f,
	};
	static s32 boundstate;
	static u32 boundtimer;
	static f32 bounddelta;
	static u8 alpha;

#ifdef HX_TV_FORMAT_TIMING
	switch (hx.isPal) {
	case 1:
		STATE1_TIME  = 50;
		STATE1_TIME1 = 10;
		STATE1_TIME2 = 15;
		STATE1_TIME3 = 25;
		STATE2_TIME  = 10;
		STATE3_TIME  = 32;
		break;
	default:
		STATE1_TIME  = 50;
		STATE1_TIME1 = 10;
		STATE1_TIME2 = 15;
		STATE1_TIME3 = 25;
		STATE2_TIME  = 10;
		STATE3_TIME  = 32;
		break;
	}
#endif

	switch (hx.animationState) {
	case 0:
		Hgx_ReadTexture("/data/wipe_gameover.bti", gmover_tex_buffer);
		mag  = 0.3f;
		rot  = 0.0f;
		fade = 0.0f;
		hx.animationState++;
#ifdef HX_TV_FORMAT_TIMING
		hx.timer = STATE1_TIME;
		Hx_MotionSet(&hx.motion, -12.566371f, STATE1_TIME1, STATE1_TIME2,
		             STATE1_TIME3);
#else
		hx.timer = 50;
		Hx_MotionSet(&hx.motion, -12.566371f, 10.0f, 15.0f, 25.0f);
#endif
		break;
	case 1:
		if (Hx_TimerCountDown() == 0) {
			hx.animationState++;
#ifdef HX_TV_FORMAT_TIMING
			hx.timer = STATE2_TIME;
#else
			hx.timer = 10;
#endif
		}
#ifdef HX_TV_FORMAT_TIMING
		mag  = mag + 3.7f / STATE1_TIME;
		rot  = Hx_MotionUpdate(&hx.motion);
		fade = fade + 255.0f / STATE1_TIME;
#else
		mag  = 0.074f + mag;
		rot  = Hx_MotionUpdate(&hx.motion);
		fade = 5.1f + fade;
#endif
		break;
	case 2:
		if (Hx_TimerCountDown() == 0) {
			hx.animationState++;
			bounddelta = boundtable[0];
			boundtimer = boundtable[1];
			boundstate = 2;
		}
		mag -= 0.1f;
		break;
	case 3:
		if (boundtimer != 0)
			boundtimer--;
		if (boundtimer == 0) {
			bounddelta = boundtable[boundstate++];
			boundtimer = boundtable[boundstate++];
			if (boundtimer == 0) {
				hx.animationState++;
#ifdef HX_TV_FORMAT_TIMING
				hx.timer = STATE3_TIME;
#else
				hx.timer = 32;
#endif
				alpha = 255;
				break;
			}
		}
		mag += bounddelta;
		break;
	case 4:
		if (Hx_TimerCountDown() == 0) {
			hx.timer = 100;
			hx.animationState++;
		}
		alpha += 8;
		Hx_CameraInit();
		Hx_GxInit(0, 1);
		{
			f32 right;
			f32 bottom;
			u32 color;

			color  = 0xFF000000 | alpha;
			bottom = hx.height - 100;
			right  = hx.width - 100;

			GXBegin(GX_QUADS, GX_VTXFMT0, 4);
			GXPosition3f32(100.0f, 100.0f, 0.0f);
			GXColor1u32(color);
			GXPosition3f32(right, 100.0f, 0.0f);
			GXColor1u32(color);
			GXPosition3f32(right, bottom, 0.0f);
			GXColor1u32(color);
			GXPosition3f32(100.0f, bottom, 0.0f);
			GXColor1u32(color);
			GXEnd();
		}
		break;
	case 5:
		Hx_CameraInit();
		Hx_GxInit(0, 1);
		{
			f32 right;
			f32 bottom;

			bottom = hx.height - 100;
			right  = hx.width - 100;
			GXBegin(GX_QUADS, GX_VTXFMT0, 4);
			GXPosition3f32(100.0f, 100.0f, 0.0f);
			GXColor1u32(0xFF0000FF);
			GXPosition3f32(right, 100.0f, 0.0f);
			GXColor1u32(0xFF0000FF);
			GXPosition3f32(right, bottom, 0.0f);
			GXColor1u32(0xFF0000FF);
			GXPosition3f32(100.0f, bottom, 0.0f);
			GXColor1u32(0xFF0000FF);
			GXEnd();
		}
		if (Hx_TimerCountDown() == 0) {
			hx.animationState++;
			hx.state = 3;
		}
		break;
	default:
		hx.state = 3;
		break;
	}

	if ((u32)hx.animationState >= 2)
		Hxs_GameOver(255, 2.0f * mag, rot);
	else
		Hxs_GameOver(fade, 2.0f * mag, rot);
}

#ifdef HX_TV_FORMAT_TIMING
static u32 HX_LOGO_TIMER1;
static u32 HX_LOGO_TIMER2;
#endif
static s32 hxs_logo_resetflag;
static s32 hxs_logodraw_resetflag;

#ifdef HX_TV_FORMAT_TIMING
static HxDrawPath drawpath_table_NTSC[] = {
	{ 49.0f, 277.0f, 0 },  { 20.0f, 294.0f, 1 },  { 22.0f, 234.0f, 1 },
	{ 50.0f, 175.0f, 1 },  { 110.0f, 112.0f, 1 }, { 128.0f, 106.0f, 1 },
	{ 138.0f, 110.0f, 1 }, { 134.0f, 113.0f, 1 }, { 135.0f, 128.0f, 1 },
	{ 117.0f, 166.0f, 1 }, { 119.0f, 201.0f, 1 }, { 129.0f, 229.0f, 1 },
	{ 140.0f, 219.0f, 1 }, { 149.0f, 172.0f, 1 }, { 178.0f, 137.0f, 1 },
	{ 192.0f, 114.0f, 1 }, { 216.0f, 104.0f, 1 }, { 227.0f, 109.0f, 1 },
	{ 224.0f, 130.0f, 1 }, { 183.0f, 265.0f, 3 }, { 161.0f, 18.0f, 0 },
	{ 161.0f, 18.01f, 8 }, { 141.0f, 79.0f, 3 },  { 255.0f, 3.0f, 0 },
	{ 255.0f, 3.01f, 8 },  { 220.0f, 71.0f, 4 },  { -1.0f, -1.0f, -1 },
};

static HxDrawPath drawpath_table_PAL[] = {
	{ 49.0f, 277.0f, 0 },   { 20.0f, 294.0f, 1 },  { 22.0f, 234.0f, 2 },
	{ 50.0f, 175.0f, 2 },   { 110.0f, 112.0f, 1 }, { 128.0f, 106.0f, 2 },
	{ 138.0f, 110.0f, 2 },  { 134.0f, 113.0f, 1 }, { 135.0f, 128.0f, 2 },
	{ 117.0f, 166.0f, 2 },  { 119.0f, 201.0f, 1 }, { 129.0f, 229.0f, 2 },
	{ 140.0f, 219.0f, 2 },  { 149.0f, 172.0f, 1 }, { 178.0f, 137.0f, 2 },
	{ 192.0f, 114.0f, 2 },  { 216.0f, 104.0f, 1 }, { 227.0f, 109.0f, 2 },
	{ 224.0f, 130.0f, 2 },  { 183.0f, 265.0f, 5 }, { 161.0f, 18.0f, 0 },
	{ 161.0f, 18.01f, 13 }, { 141.0f, 79.0f, 5 },  { 255.0f, 3.0f, 0 },
	{ 255.0f, 3.01f, 13 },  { 220.0f, 71.0f, 6 },  { -1.0f, -1.0f, -1 },
};

static HxDrawPath* drawpath_table;
#else
static HxDrawPath drawpath_table[] = {
	{ 49.0f, 277.0f, 0 },  { 20.0f, 294.0f, 1 },  { 22.0f, 234.0f, 1 },
	{ 50.0f, 175.0f, 1 },  { 110.0f, 112.0f, 1 }, { 128.0f, 106.0f, 1 },
	{ 138.0f, 110.0f, 1 }, { 134.0f, 113.0f, 1 }, { 135.0f, 128.0f, 1 },
	{ 117.0f, 166.0f, 1 }, { 119.0f, 201.0f, 1 }, { 129.0f, 229.0f, 1 },
	{ 140.0f, 219.0f, 1 }, { 149.0f, 172.0f, 1 }, { 178.0f, 137.0f, 1 },
	{ 192.0f, 114.0f, 1 }, { 216.0f, 104.0f, 1 }, { 227.0f, 109.0f, 1 },
	{ 224.0f, 130.0f, 1 }, { 183.0f, 265.0f, 3 }, { 161.0f, 18.0f, 0 },
	{ 161.0f, 18.01f, 8 }, { 141.0f, 79.0f, 3 },  { 255.0f, 3.0f, 0 },
	{ 255.0f, 3.01f, 8 },  { 220.0f, 71.0f, 4 },  { -1.0f, -1.0f, -1 },
};
#endif

static void Hxs_Logo_ExtraDraw(u8 alpha, struct ResTIMG* resource)
{
	GXTexObj texObj;

	Hx_CameraInit();
	Hx_GxInit(1, 1);
	Hgx_init_tobj_resource(&texObj, resource);
	{
		GXColor color = { 0xFF, 0xFF, 0xFF, 0xFF };

		color.a = alpha;
		GXSetTevColor(GX_TEVREG0, color);
		GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_ZERO, GX_CC_ZERO,
		                GX_CC_ZERO);
		GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA,
		                GX_CA_ZERO);
		GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
		                GX_TRUE, GX_TEVPREV);
		GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
		                GX_TRUE, GX_TEVPREV);
		GXLoadTexObj(&texObj, GX_TEXMAP0);
		GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA,
		               GX_LO_CLEAR);
		GXBegin(GX_QUADS, GX_VTXFMT0, 4);
		GXPosition3f32(160.0f, 205.0f, 0.0f);
		GXColor1u32(0);
		GXTexCoord2f32(0.0f, 0.0f);
		GXPosition3f32(480.0f, 205.0f, 0.0f);
		GXColor1u32(0);
		GXTexCoord2f32(1.0f, 0.0f);
		GXPosition3f32(480.0f, 237.0f, 0.0f);
		GXColor1u32(0);
		GXTexCoord2f32(1.0f, 1.0f);
		GXPosition3f32(160.0f, 237.0f, 0.0f);
		GXColor1u32(0);
		GXTexCoord2f32(0.0f, 1.0f);
		GXEnd();
	}
}

static void Hxs_Logo_TexSetup(u8 red, u8 alpha, struct ResTIMG* resource)
{
	GXTexObj texObj;

	Hx_CameraInit();
	Hx_GxInit(1, 1);
	Hgx_init_tobj_resource(&texObj, resource);
	{
		GXColor color = { 0xFF, 0, 0, 0 };

		color.r = red;
		color.a = alpha;
		if (alpha > 192)
			color.a = 255;
		else
			color.a = (u8)(1.328 * alpha);
		GXSetTevColor(GX_TEVREG0, color);
		if (alpha > 192)
			color.a = (255 - alpha) * 4;
		else
			color.a = 255;
		GXSetTevColor(GX_TEVREG1, color);
		GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_C0, GX_CC_TEXA,
		                GX_CC_ZERO);
		GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A1, GX_CA_A0, GX_CA_TEXA,
		                GX_CA_ZERO);
		GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
		                GX_TRUE, GX_TEVPREV);
		GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
		                GX_TRUE, GX_TEVPREV);
		GXLoadTexObj(&texObj, GX_TEXMAP0);
		GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA,
		               GX_LO_CLEAR);
	}
}

static void Hxs_Logo_TexDraw(f32 x1, f32 y1, f32 x2, f32 y2, f32 width,
                             f32 height)
{
	Vec direction;
	f32 left;
	f32 top;
	f32 posX0;
	f32 posX1;
	f32 posX2;
	f32 posX3;
	f32 posY0;
	f32 posY1;
	f32 posY2;
	f32 posY3;
	f32 texX0;
	f32 texY0;
	f32 texX1;
	f32 texY1;
	f32 texX2;
	f32 texY2;
	f32 texX3;
	f32 texY3;

	width  = width / 1.9230769f;
	height = height / 1.924138f;
	y1     = y1 / height;
	y2     = y2 / height;
	x1     = x1 / width;
	x2     = x2 / width;
	left   = (f32)(hx.width >> 1) - width / 2.0f;
	top    = (f32)(hx.height >> 1) - height / 2.0f - 32.0f;

	direction.y = x2 - x1;
	direction.x = -(y2 - y1);
	direction.z = 0.0f;
	if (direction.y != 0.0f || direction.x != 0.0f) {
		VECNormalize(&direction, &direction);
		VECScale(&direction, &direction, 0.08f);
		posX0 = width * (x1 + direction.x) + left;
		posY0 = height * (y1 + direction.y) + top;
		posX1 = width * (x2 + direction.x) + left;
		posY1 = height * (y2 + direction.y) + top;
		posX2 = width * (x2 - direction.x) + left;
		posY2 = height * (y2 - direction.y) + top;
		posX3 = width * (x1 - direction.x) + left;
		posY3 = height * (y1 - direction.y) + top;
		GXBegin(GX_QUADS, GX_VTXFMT0, 4);
		texY0 = y1 + direction.y;
		texX0 = x1 + direction.x;
		GXPosition3f32(posX0, posY0, 0.0f);
		GXColor1u32(0);
		GXTexCoord2f32(texX0, texY0);
		texY1 = y2 + direction.y;
		texX1 = x2 + direction.x;
		GXPosition3f32(posX1, posY1, 0.0f);
		GXColor1u32(0);
		GXTexCoord2f32(texX1, texY1);
		texY2 = y2 - direction.y;
		texX2 = x2 - direction.x;
		GXPosition3f32(posX2, posY2, 0.0f);
		GXColor1u32(0);
		GXTexCoord2f32(texX2, texY2);
		texY3 = y1 - direction.y;
		texX3 = x1 - direction.x;
		GXPosition3f32(posX3, posY3, 0.0f);
		GXColor1u32(0);
		GXTexCoord2f32(texX3, texY3);
		GXEnd();
	}
}

static void Hxs_Logo_MagDraw(f32 scale, f32 width, f32 height)
{
	f32 scaledWidth;
	f32 scaledHeight;
	f32 left;
	f32 top;
	f32 right;
	f32 bottom;
	f32 halfWidth;
	f32 halfHeight;
	f32 texLeft;
	f32 texTop;
	f32 texRight;
	f32 texBottom;

	halfWidth  = (f32)(hx.width >> 1);
	halfHeight = (f32)(hx.height >> 1);
#ifdef VERSION_GMSP01_NX
	width *= 0.74418604f;
	height *= 0.875f;
#endif
	width /= 1.9230769f;
	height /= 1.924138f;
	scaledWidth  = width * scale;
	scaledHeight = height * scale;
	left         = halfWidth - scaledWidth / 2.0f;
	top          = halfHeight - scaledHeight / 2.0f;
	right        = halfWidth + scaledWidth / 2.0f;
	bottom       = halfHeight + scaledHeight / 2.0f;
	texLeft      = left / (left - right);
	texTop       = top / (top - bottom);
	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	texRight  = 1.0f - texLeft;
	texBottom = 1.0f - texTop;
#ifdef VERSION_GMSP01_NX
	GXPosition3f32(-110.0f, -64.0f, 0.0f);
	GXColor1u32(0);
	GXTexCoord2f32(texLeft, texTop);
	GXPosition3f32((f32)(hx.width + 110), -64.0f, 0.0f);
	GXColor1u32(0);
	GXTexCoord2f32(texRight, texTop);
	GXPosition3f32((f32)(hx.width + 110), (f32)hx.height, 0.0f);
	GXColor1u32(0);
	GXTexCoord2f32(texRight, texBottom);
	GXPosition3f32(-110.0f, (f32)hx.height, 0.0f);
#else
	GXPosition3f32(0.0f, -32.0f, 0.0f);
	GXColor1u32(0);
	GXTexCoord2f32(texLeft, texTop);
	GXPosition3f32((f32)hx.width, -32.0f, 0.0f);
	GXColor1u32(0);
	GXTexCoord2f32(texRight, texTop);
	GXPosition3f32((f32)hx.width, (f32)(hx.height - 32), 0.0f);
	GXColor1u32(0);
	GXTexCoord2f32(texRight, texBottom);
	GXPosition3f32(0.0f, (f32)(hx.height - 32), 0.0f);
#endif
	GXColor1u32(0);
	GXTexCoord2f32(texLeft, texBottom);
	GXEnd();
}

static void Hxs_PenDraw(f32 startX, f32 startY, u32 count, HxDrawPath* target)
{
	Vec direction;
	f32 prevX;
	f32 nextX;
	f32 prevY;
	f32 nextY;
	f32 progress;
	f32 x[2];
	f32 y[2];
	u32 i;

	if (count != 0) {
		prevX = drawpath_table[0].x;
		prevY = drawpath_table[0].y;
		for (i = 0; i < count - 1; ++i) {
			if (drawpath_table[i + 1].type == -1)
				break;
			nextX = drawpath_table[i + 1].x;
			nextY = drawpath_table[i + 1].y;
			if (drawpath_table[i + 1].type != 0) {
				direction.x = nextX - prevX;
				direction.y = nextY - prevY;
				VECNormalize(&direction, &direction);
				VECScale(&direction, &direction, 6.0f);
				Hxs_Logo_TexDraw(prevX - direction.x, prevY - direction.y,
				                 nextX + direction.x, nextY + direction.y,
				                 img_wx, img_wy);
			}
			prevX = nextX;
			prevY = nextY;
		}
	}
	x[0]     = startX;
	y[0]     = startY;
	y[1]     = target->y;
	x[1]     = target->x;
	progress = (f32)(target->type - hx.timer) / (f32)target->type;
	x[1]     = progress * (x[1] - x[0]) + x[0];
	y[1]     = progress * (y[1] - y[0]) + y[0];
	Hxs_Logo_TexDraw(startX, startY, x[1], y[1], img_wx, img_wy);
}

static void Hx_Logo(void)
{
	static HxDrawPath* dp;
	static f32 bx;
	static f32 by;
	static u32 count;
	struct ResTIMG* resource;
	struct ResTIMG* extraResource;
	u32 i;
#ifdef HX_TV_FORMAT_TIMING
	u32 countDownNum;
#endif

#ifdef HX_TV_FORMAT_TIMING
	switch (hx.isPal) {
	case 1:
		drawpath_table = drawpath_table_PAL;
		HX_LOGO_TIMER1 = 8;
		HX_LOGO_TIMER2 = 42;
		break;
	default:
		drawpath_table = drawpath_table_NTSC;
		HX_LOGO_TIMER1 = 5;
		HX_LOGO_TIMER2 = 25;
		break;
	}
#endif

	resource = (struct ResTIMG*)hx.resource;
	if (hx.hasResourceEx != 0)
		extraResource = (struct ResTIMG*)hx.resourceEx;
	else
		extraResource = (struct ResTIMG*)hx_buffer;

	switch (hx.animationState) {
	case 0:
		if (hx.hasResourceEx == 0)
			Hgx_ReadTexture("/data/title_mini.bti", extraResource);
		hx.animationState++;
		dp                     = drawpath_table;
		count                  = 0;
		hx.timer               = 256;
		hxs_logo_resetflag     = 1;
		hxs_logodraw_resetflag = 1;
		break;
	case 1:
		if (hx.timer <= 192)
			Hxs_Logo_ExtraDraw(255, extraResource);
		else
			Hxs_Logo_ExtraDraw((u8)((256 - hx.timer) * 4), extraResource);
#ifdef HX_TV_FORMAT_TIMING
		if (hx.isPal == 0)
			Hx_TimerCountDown();
#else
		Hx_TimerCountDown();
#endif
		Hx_TimerCountDown();
		Hx_TimerCountDown();
		if (Hx_TimerCountDown() == 0)
			hx.animationState++;
		break;
	case 2:
		if (dp->type == -1) {
			hx.animationState = 3;
			goto begin_fade;
		} else {
			if (dp->type == 0) {
				bx = dp->x;
				by = dp->y;
				dp++;
				count++;
			}
			hx.timer = dp->type;
			hx.animationState++;
		}
		/* fall through */
	case 3:
		Hxs_Logo_ExtraDraw(255, extraResource);
		Hxs_Logo_TexSetup(255, 255, resource);
		Hxs_PenDraw(bx, by, count, dp);
		if (Hx_TimerCountDown() == 0) {
			bx = dp->x;
			by = dp->y;
			dp++;
			hx.animationState = 2;
			count++;
		}
		break;
	case 4:
	begin_fade:
#ifdef HX_TV_FORMAT_TIMING
		hx.timer = HX_LOGO_TIMER1;
#else
		hx.timer = 5;
#endif
		hx.animationState++;
		/* fall through */
	case 5:
		Hxs_Logo_ExtraDraw(255, extraResource);
		Hxs_Logo_TexSetup(255, 255, resource);
		Hxs_PenDraw(bx, by, count, dp);
		if (Hx_TimerCountDown() == 0) {
			hx.timer = 255;
			hx.animationState++;
		}
		break;
	case 6:
		if (hx.timer >= 192) {
			Hxs_Logo_ExtraDraw(255, extraResource);
			Hxs_Logo_TexSetup((u8)hx.timer, (u8)hx.timer, resource);
			if (hx.timer > 248)
				Hxs_PenDraw(bx, by, count, dp);
			else
				Hxs_Logo_MagDraw(1.0f, (f32)img_wx, (f32)img_wy);
		} else {
			Hxs_Logo_TexSetup((u8)hx.timer, (u8)hx.timer, resource);
			Hxs_Logo_MagDraw(1.0f, (f32)img_wx, (f32)img_wy);
		}
#ifdef HX_TV_FORMAT_TIMING
		if (hx.isPal == 0)
			countDownNum = 3;
		else
			countDownNum = 2;
		for (i = 0; i < countDownNum; ++i)
			Hx_TimerCountDown();
		if (Hx_TimerCountDown() == 0) {
			hx.timer = HX_LOGO_TIMER2;
			Hx_MotionSet(&hx.motion, 30.0f, 12.0f, HX_LOGO_TIMER2 - 17, 5.0f);
#else
		for (i = 0; i < 3; ++i)
			Hx_TimerCountDown();
		if (Hx_TimerCountDown() == 0) {
			hx.timer = 25;
			Hx_MotionSet(&hx.motion, 30.0f, 12.0f, 8.0f, 5.0f);
#endif
			hx.animationState++;
		}
		break;
	case 7:
		Hxs_Logo_TexSetup(0, 0, resource);
		Hxs_Logo_MagDraw(1.0f + Hx_MotionUpdate(&hx.motion), (f32)img_wx,
		                 (f32)img_wy);
		if (Hx_TimerCountDown() == 0)
			hx.animationState++;
		break;
	case 8:
		hx.state = 3;
		break;
	}
}

void Hx_MovieStartSync(void) { }

int Hx_MovieStartSyncEx(void)
{
	if (hx.type != 12)
		return 0;
	if ((u32)hx.animationState >= 2 && (u32)hx.animationState <= 5) {
		if (hxs_logodraw_resetflag == 0)
			return 0;
		hxs_logodraw_resetflag = 0;
		return 1;
	}
	if ((u32)hx.animationState >= 6) {
		if (hxs_logo_resetflag == 0)
			return 0;
		if ((u32)hx.animationState == 6 && hx.timer > 192)
			return 0;
		hxs_logo_resetflag = 0;
		return 2;
	}
	return 0;
}

#ifdef HX_TV_FORMAT_TIMING
static u32 HX_TEST1_TIMER;
static u32 HX_TEST1_TIMER2;
#endif

static void Hx_Test1(void)
{
	static f32 r;

#ifdef HX_TV_FORMAT_TIMING
	switch (hx.isPal) {
	case 1:
		HX_TEST1_TIMER  = 25;
		HX_TEST1_TIMER2 = 30;
		break;
	default:
		HX_TEST1_TIMER  = 25;
		HX_TEST1_TIMER2 = 30;
		break;
	}
#endif

	switch (hx.animationState) {
	case 0:
#ifdef HX_TV_FORMAT_TIMING
		if (hx.direction == 1) {
			r = 400.0f;
			Hx_MotionSet(&hx.motion, 400.0f, 10.0f, HX_TEST1_TIMER2 - 18, 8.0f);
		} else {
			r = 1.0f;
			Hx_MotionSet(&hx.motion, 400.0f, 5.0f, HX_TEST1_TIMER - 15, 10.0f);
		}
		hx.animationState++;
		hx.timer = HX_TEST1_TIMER;
#else
		if (hx.direction == 1) {
			r = 400.0f;
			Hx_MotionSet(&hx.motion, 400.0f, 10.0f, 12.0f, 8.0f);
		} else {
			r = 1.0f;
			Hx_MotionSet(&hx.motion, 400.0f, 5.0f, 10.0f, 10.0f);
		}
		hx.animationState++;
		hx.timer = 25;
#endif
		break;
	case 1:
		if (Hx_TimerCountDown() == 0)
			hx.animationState++;
		r = Hx_MotionUpdate(&hx.motion);
		if (hx.direction == 1)
			r = 400.0f - r;
		break;
	default:
		hx.state = 3;
		break;
	}
	Hxs1_Test1(0.0f, 0.0f, r);
	Hxs1_Test1((f32)hx.width, 0.0f, r);
	Hxs1_Test1((f32)hx.width, (f32)hx.height, r);
	Hxs1_Test1(0.0f, (f32)hx.height, r);
}

static void Hxs1_Test1(f32 centerX, f32 centerY, f32 radius)
{
	f32 x[2];
	f32 y[2];
	f32 offset;
	f32 radiusSquared;
	u32 i;

#ifdef VERSION_GMSP01_NX
	Hx_CameraInitWide();
#else
	Hx_CameraInit();
#endif
	Hx_GxInit(0, 1);
	GXSetLineWidth(7, GX_TO_ZERO);
	radiusSquared = radius * radius;
	GXBegin(GX_LINES, GX_VTXFMT0, (u32)radius * 2 + 2);
	for (i = 0; i <= (u32)radius; ++i) {
		offset = sqrtf(radiusSquared - (f32)(i * i));

		if (centerY < (f32)hx.halfHeight) {
			y[0] = centerY + i;
			y[1] = centerY + i;
		} else {
			y[0] = centerY - i;
			y[1] = centerY - i;
		}

		if (centerX < (f32)hx.halfWidth) {
			x[0] = centerX;
			x[1] = centerX + offset;
		} else {
			x[0] = centerX - offset;
			x[1] = centerX;
		}

		GXPosition3f32(x[0], y[0], 1.0f);
		GXColor1u32(0xFF);
		GXPosition3f32(x[1], y[1], 1.0f);
		GXColor1u32(0xFF);
	}
	GXEnd();
}

#ifdef HX_TV_FORMAT_TIMING
static u32 HX_Test2_STATE1_TIME;
static u32 HX_Test2_STATE2_TIME;
static u32 HX_Test2_STATE3_TIME;
static u32 HX_Test2_STATE4_TIME;
#endif

static void Hx_Test2(void)
{
	static f32 r;

#ifdef HX_TV_FORMAT_TIMING
	switch (hx.isPal) {
	case 1:
		HX_Test2_STATE1_TIME = 8;
		HX_Test2_STATE2_TIME = 8;
		HX_Test2_STATE3_TIME = 7;
		HX_Test2_STATE4_TIME = 9;
		break;
	default:
		HX_Test2_STATE1_TIME = 8;
		HX_Test2_STATE2_TIME = 8;
		HX_Test2_STATE3_TIME = 7;
		HX_Test2_STATE4_TIME = 9;
		break;
	}
#endif

	switch (hx.animationState) {
	case 0:
		r = 1.0f;
#ifdef HX_TV_FORMAT_TIMING
		Hx_MotionSet(&hx.motion, 500.0f, 2.0f, HX_Test2_STATE1_TIME, 1.0f);
		hx.animationState++;
		hx.timer = HX_Test2_STATE1_TIME + 3;
#else
		Hx_MotionSet(&hx.motion, 500.0f, 2.0f, 8.0f, 1.0f);
		hx.animationState++;
		hx.timer = 11;
#endif
		break;
	case 1:
		r = Hx_MotionUpdate(&hx.motion);
		Hxs1_Test2((u32)r, 1, (f32)(hx.width + 200), 300.0f, 900.0f, 650.0f);
		if (Hx_TimerCountDown() == 0) {
			hx.animationState++;
#ifdef HX_TV_FORMAT_TIMING
			hx.timer = HX_Test2_STATE2_TIME + 3;
			Hx_MotionSet(&hx.motion, 500.0f, 2.0f, HX_Test2_STATE2_TIME, 1.0f);
#else
			hx.timer = 11;
			Hx_MotionSet(&hx.motion, 500.0f, 2.0f, 8.0f, 1.0f);
#endif
		}
		break;
	case 2:
		Hxs1_Test2(600, 0, (f32)(hx.width + 200), 300.0f, 900.0f, 650.0f);
		r = Hx_MotionUpdate(&hx.motion);
		Hxs1_Test2((u32)r, 0, (f32)(hx.width + 200), 150.0f, 700.0f, 450.0f);
		if (Hx_TimerCountDown() == 0) {
			hx.animationState++;
#ifdef HX_TV_FORMAT_TIMING
			hx.timer = HX_Test2_STATE3_TIME + 3;
			Hx_MotionSet(&hx.motion, 500.0f, 2.0f, HX_Test2_STATE3_TIME, 1.0f);
#else
			hx.timer = 10;
			Hx_MotionSet(&hx.motion, 500.0f, 2.0f, 7.0f, 1.0f);
#endif
		}
		break;
	case 3:
		Hxs1_Test2(600, 0, (f32)(hx.width + 200), 300.0f, 900.0f, 650.0f);
		Hxs1_Test2(600, 0, (f32)(hx.width + 200), 150.0f, 700.0f, 450.0f);
		r = Hx_MotionUpdate(&hx.motion);
		Hxs1_Test2((u32)r, 1, (f32)(hx.width + 250), 370.0f, 650.0f, 400.0f);
		if (Hx_TimerCountDown() == 0) {
			hx.animationState++;
#ifdef HX_TV_FORMAT_TIMING
			hx.timer = HX_Test2_STATE4_TIME + 3;
			Hx_MotionSet(&hx.motion, 500.0f, 2.0f, HX_Test2_STATE4_TIME, 1.0f);
#else
			hx.timer = 12;
			Hx_MotionSet(&hx.motion, 500.0f, 2.0f, 9.0f, 1.0f);
#endif
		}
		break;
	case 4:
		Hxs1_Test2(600, 0, (f32)(hx.width + 200), 300.0f, 900.0f, 650.0f);
		Hxs1_Test2(600, 0, (f32)(hx.width + 200), 150.0f, 700.0f, 450.0f);
		Hxs1_Test2(600, 1, (f32)(hx.width + 250), 370.0f, 650.0f, 400.0f);
		r = Hx_MotionUpdate(&hx.motion);
		Hxs1_Test2((u32)r, 0, (f32)(hx.width + 250), 300.0f, 420.0f, 200.0f);
		if (Hx_TimerCountDown() == 0) {
			hx.animationState++;
			hx.state = 3;
		}
		break;
	default:
		hx.state = 3;
		break;
	}
}

static void Hx_Test2R(void)
{
	static f32 r;

#ifdef HX_TV_FORMAT_TIMING
	switch (hx.isPal) {
	case 1:
		HX_Test2_STATE1_TIME = 8;
		HX_Test2_STATE2_TIME = 8;
		HX_Test2_STATE3_TIME = 7;
		HX_Test2_STATE4_TIME = 9;
		break;
	default:
		HX_Test2_STATE1_TIME = 8;
		HX_Test2_STATE2_TIME = 8;
		HX_Test2_STATE3_TIME = 7;
		HX_Test2_STATE4_TIME = 9;
		break;
	}
#endif

	switch (hx.animationState) {
	case 0:
		r = 1.0f;
#ifdef HX_TV_FORMAT_TIMING
		Hx_MotionSet(&hx.motion, 500.0f, 2.0f, HX_Test2_STATE1_TIME, 1.0f);
		hx.animationState++;
		hx.timer = HX_Test2_STATE1_TIME + 3;
#else
		Hx_MotionSet(&hx.motion, 500.0f, 2.0f, 8.0f, 1.0f);
		hx.animationState++;
		hx.timer = 11;
#endif
	case 1:
		Hxs1_Test2(600, 0, (f32)(hx.width + 200), 150.0f, 700.0f, 450.0f);
		Hxs1_Test2(600, 1, (f32)(hx.width + 250), 370.0f, 650.0f, 400.0f);
		Hxs1_Test2(600, 0, (f32)(hx.width + 250), 300.0f, 420.0f, 200.0f);
		r = Hx_MotionUpdate(&hx.motion);
		r = 500.0f - r;
		Hxs1_Test2((u32)r, 0, (f32)(hx.width + 200), 300.0f, 900.0f, 650.0f);
		if (Hx_TimerCountDown() == 0) {
			hx.animationState++;
#ifdef HX_TV_FORMAT_TIMING
			hx.timer = HX_Test2_STATE2_TIME + 3;
			Hx_MotionSet(&hx.motion, 500.0f, 2.0f, HX_Test2_STATE2_TIME, 1.0f);
#else
			hx.timer = 11;
			Hx_MotionSet(&hx.motion, 500.0f, 2.0f, 8.0f, 1.0f);
#endif
		}
		break;
	case 2:
		Hxs1_Test2(600, 1, (f32)(hx.width + 250), 370.0f, 650.0f, 400.0f);
		Hxs1_Test2(600, 0, (f32)(hx.width + 250), 300.0f, 420.0f, 200.0f);
		r = Hx_MotionUpdate(&hx.motion);
		r = 500.0f - r;
		Hxs1_Test2((u32)r, 1, (f32)(hx.width + 200), 150.0f, 700.0f, 450.0f);
		if (Hx_TimerCountDown() == 0) {
			hx.animationState++;
#ifdef HX_TV_FORMAT_TIMING
			hx.timer = HX_Test2_STATE3_TIME + 3;
			Hx_MotionSet(&hx.motion, 500.0f, 2.0f, HX_Test2_STATE3_TIME, 1.0f);
#else
			hx.timer = 10;
			Hx_MotionSet(&hx.motion, 500.0f, 2.0f, 7.0f, 1.0f);
#endif
		}
		break;
	case 3:
		Hxs1_Test2(600, 0, (f32)(hx.width + 250), 300.0f, 420.0f, 200.0f);
		r = Hx_MotionUpdate(&hx.motion);
		r = 500.0f - r;
		Hxs1_Test2((u32)r, 0, (f32)(hx.width + 250), 370.0f, 650.0f, 400.0f);
		if (Hx_TimerCountDown() == 0) {
			hx.animationState++;
#ifdef HX_TV_FORMAT_TIMING
			hx.timer = HX_Test2_STATE4_TIME + 3;
			Hx_MotionSet(&hx.motion, 500.0f, 2.0f, HX_Test2_STATE4_TIME, 1.0f);
#else
			hx.timer = 12;
			Hx_MotionSet(&hx.motion, 500.0f, 2.0f, 9.0f, 1.0f);
#endif
		}
		break;
	case 4:
		r = Hx_MotionUpdate(&hx.motion);
		r = 500.0f - r;
		Hxs1_Test2((u32)r, 1, (f32)(hx.width + 250), 300.0f, 420.0f, 200.0f);
		if (Hx_TimerCountDown() == 0) {
			hx.animationState++;
			hx.state = 3;
		}
		break;
	default:
		hx.state = 3;
		break;
	}
}

static void Hxs1_Test2(u32 count, u32 direction, f32 centerX, f32 centerY,
                       f32 outerRadius, f32 innerRadius)
{
	f32 x[2];
	f32 y[2];
	f32 outerRadiusSquared;
	f32 innerRadiusSquared;
	f32 outerOffset;
	f32 innerOffset;
	s32 start;
	int i;
	s32 end;
	s32 step;

#ifdef VERSION_GMSP01_NX
	Hx_CameraInitWide();
#else
	Hx_CameraInit();
#endif
	Hx_GxInit(0, 1);
	outerRadiusSquared = outerRadius * outerRadius;
	innerRadiusSquared = innerRadius * innerRadius;
	if (direction == 0) {
		step  = 1;
		start = -centerY;
		end   = outerRadius;
	} else {
		step  = -1;
		start = outerRadius;
		end   = -centerY;
	}

	for (i = start; i != end; i += step) {
		y[0] = centerY + i;
		y[1] = centerY + i;
		if (y[0] < 0.0f || y[0] > hx.height)
			continue;
		if (count == 0)
			break;
		count--;
		outerOffset = sqrtf(outerRadiusSquared - i * i);
		innerOffset = sqrtf(innerRadiusSquared - i * i);
		if (centerX < hx.halfWidth) {
			x[1] = centerX + outerOffset;
			x[0] = centerX + innerOffset;
			if (x[1] < 0.0f)
				continue;
		} else {
			x[0] = centerX - outerOffset;
			x[1] = centerX - innerOffset;
			if (x[0] > hx.width)
				continue;
		}
		GXBegin(GX_LINES, GX_VTXFMT0, 2);
		GXPosition3f32(x[0], y[0], 1.0f);
		GXColor1u32(0xFF);
		GXPosition3f32(x[1], y[1], 1.0f);
		GXColor1u32(0xFF);
		GXEnd();
	}
}

#ifdef HX_TV_FORMAT_TIMING
static u32 HX_TEST4_TIME;
#endif

static void Hx_Test4(void)
{
	static f32 thin;
	static u32 rstep;
	static f32 thin_d;
	static f32 rstep_d;
	f32 angle;
	f32 outerX;
	f32 outerY;
	f32 innerX;
	f32 innerY;
	f32 nextOuterX;
	f32 nextOuterY;
	f32 nextInnerX;
	f32 nextInnerY;
	f32 radius;
	f32 outerRadius;
	f32 innerRadius;
	u32 i;

#ifdef HX_TV_FORMAT_TIMING
	switch (hx.isPal) {
	case 1:
		HX_TEST4_TIME = 31;
		break;
	default:
		HX_TEST4_TIME = 38;
		break;
	}
#endif

	switch (hx.animationState) {
	case 2:
	default:
		break;
	case 0:
#ifdef HX_TV_FORMAT_TIMING
		switch (hx.direction) {
		case 0:
			rstep   = 0;
			thin    = 130.0f - 0.15f * HX_TEST4_TIME;
			rstep_d = 200.0f / HX_TEST4_TIME;
			thin_d  = 0.15f;
			break;
		case 1:
			rstep   = 230;
			thin    = 100.0f;
			rstep_d = -200.0f / HX_TEST4_TIME;
			thin_d  = -0.15f;
			break;
		}
		hx.timer = HX_TEST4_TIME;
#else
		switch (hx.direction) {
		case 0:
			rstep   = 0;
			thin    = 124.3f;
			rstep_d = 5.0f;
			thin_d  = 0.15f;
			break;
		case 1:
			rstep   = 230;
			thin    = 100.0f;
			rstep_d = -5.0f;
			thin_d  = -0.15f;
			break;
		}
		hx.timer = 38;
#endif
		hx.animationState++;
	case 1:
		rstep = rstep + rstep_d;
		thin += thin_d;
		radius      = (hx.width >> 1) + 200;
		angle       = 0.0f;
		outerRadius = radius + thin;
		outerX      = outerRadius * sinf(angle) + hx.halfWidth;
		outerY      = outerRadius * cosf(angle) + hx.halfHeight;
		innerRadius = radius - thin;
		innerX      = innerRadius * sinf(angle) + hx.halfWidth;
		innerY      = innerRadius * cosf(angle) + hx.halfHeight;
		angle       = 0.0f;
		Hx_CameraInit();
		Hx_GxInit(0, 1);
		for (i = 0; i < rstep; ++i) {
			radius -= 2.4f;
			outerRadius = radius + thin;
			if (radius < thin)
				innerRadius = 0.0f;
			else
				innerRadius = radius - thin;
			angle += 0.12f;
			nextOuterX = outerRadius * sinf(angle) + hx.halfWidth;
			nextOuterY = outerRadius * cosf(angle) + hx.halfHeight;
			nextInnerX = innerRadius * sinf(angle) + hx.halfWidth;
			nextInnerY = innerRadius * cosf(angle) + hx.halfHeight;
			GXBegin(GX_QUADS, GX_VTXFMT0, 4);
			GXPosition3f32(outerX, outerY, 0.0f);
			GXColor1u32(0xFF);
			GXPosition3f32(nextOuterX, nextOuterY, 0.0f);
			GXColor1u32(0xFF);
			GXPosition3f32(nextInnerX, nextInnerY, 0.0f);
			GXColor1u32(0xFF);
			GXPosition3f32(innerX, innerY, 0.0f);
			GXColor1u32(0xFF);
			GXEnd();
			outerX = nextOuterX;
			outerY = nextOuterY;
			innerX = nextInnerX;
			innerY = nextInnerY;
		}
		if (Hx_TimerCountDown() == 0) {
			hx.state = 3;
			hx.animationState++;
		}
		break;
	}
}

#ifdef HX_TV_FORMAT_TIMING
static u32 HX_TEST5_FADETIME;
#endif

static void Hx_Test5(void)
{
	u8* buffer = hx_buffer;
	GXTexObj texObj;
	f32 outScale;
	f32 inScale;
	f32 scale;
	f32 twist;
	f32 centerX;
	f32 centerY;
	f32 firstX;
	f32 firstY;
	f32 firstU;
	f32 firstV;
	u32 x;
	u32 y;
	u32 i;

#ifdef HX_TV_FORMAT_TIMING
	switch (hx.isPal) {
	case 1:
		HX_TEST5_FADETIME = 16;
		break;
	default:
		HX_TEST5_FADETIME = 20;
		break;
	}
#endif

#ifdef VERSION_GMSP01_NX
	Hx_CameraInitWide2();
#else
	Hx_CameraInit();
#endif
	Hx_GxInit(1, 0);
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);

	switch (hx.animationState) {
	case 2:
	default:
		break;
	case 0:
#ifdef HX_TV_FORMAT_TIMING
		hx.timer = HX_TEST5_FADETIME;
#else
		hx.timer = 20;
#endif
		hx.animationState++;
	case 1:
		GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY,
		                  GX_FALSE, GX_PTIDENTITY);
		GXSetNumTexGens(1);
		GXSetNumTevStages(1);
		GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
		GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
		GXSetBlendMode(GX_BM_NONE, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_CLEAR);
		GXInitTexObj(&texObj, buffer, 64, 64, GX_TF_RGB565, GX_CLAMP, GX_CLAMP,
		             GX_FALSE);
		GXInitTexObjLOD(&texObj, GX_LINEAR, GX_LINEAR, 0.0f, 10.0f, 0.0f,
		                GX_DISABLE, GX_ENABLE, GX_ANISO_1);
#ifdef HX_TV_FORMAT_TIMING
		outScale = 1.41f - 1.41f * (hx.timer / (f32)HX_TEST5_FADETIME);
		inScale  = 0.1f + 1.41f * (hx.timer / (f32)HX_TEST5_FADETIME);
#else
		outScale = 1.41f - 1.41f * (hx.timer / 20.0f);
		inScale  = 0.1f + 1.41f * (hx.timer / 20.0f);
#endif
		for (y = 0; y < hx.height; y += 64) {
			for (x = 0; x < hx.width; x += 64) {
				centerX = x;
				centerY = y;
				Hx_GetFrBuffer(buffer, x, y, 64, 64);
				GXInvalidateTexAll();
				GXLoadTexObj(&texObj, GX_TEXMAP0);
				if (hx.direction != 0)
					scale = outScale;
				else
					scale = inScale;
				if (scale < 1.0f)
					twist = 3.1415927f * (1.0f - scale);
				else
					twist = 0.0f;
				centerX = 32.0f + centerX;
				centerY = 32.0f + centerY;
				GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, 18);
				GXPosition3f32(centerX, centerY, 0.0f);
				GXColor1u32(0);
				GXTexCoord2f32(0.5f, 0.5f);
				for (i = 0; i < 16; i++) {
					f32 angle = 3.1415927f * (2.0f * i) / 16.0f;
					f32 u     = 0.5f * sinf(angle) + 0.5f;
					f32 v     = 0.5f * cosf(angle) + 0.5f;
					f32 px    = scale * sinf(angle + twist);
					f32 py    = scale * cosf(angle + twist);

					if (scale >= 1.0f) {
						u = scale * sinf(angle) / 2.0f + 0.5f;
						v = scale * cosf(angle) / 2.0f + 0.5f;
					}
					if (px < -1.0f) {
						u  = 0.0f;
						py = -py / px;
						px = -1.0f;
						v  = 0.5f * py + 0.5f;
					}
					if (px > 1.0f) {
						py = py / px;
						px = 1.0f;
						u  = 1.0f;
						v  = 0.5f * py + 0.5f;
					}
					if (py < -1.0f) {
						v  = 0.0f;
						px = -px / py;
						py = -1.0f;
						u  = 0.5f * px + 0.5f;
					}
					if (py > 1.0f) {
						px = px / py;
						py = 1.0f;
						v  = 1.0f;
						u  = 0.5f * px + 0.5f;
					}
					px *= 32.0f;
					py *= 32.0f;
					if (i == 0) {
						firstX = px;
						firstY = py;
						firstU = u;
						firstV = v;
					}
					GXPosition3f32(px + centerX, py + centerY, 0.0f);
					GXColor1u32(0);
					GXTexCoord2f32(u, v);
				}
				GXPosition3f32(firstX + centerX, firstY + centerY, 0.0f);
				GXColor1u32(0);
				GXTexCoord2f32(firstU, firstV);
				GXEnd();
			}
		}
		if (Hx_TimerCountDown() == 0) {
			hx.animationState++;
			hx.state = 3;
		}
		return;
	}
	hx.state = 3;
}
