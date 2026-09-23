/**
 * @file hx_wiper.c
 *
 * @brief The screen-wipe library ("Hx"): the shutter, iris, door, sweep and
 * logo transitions TScrnFader drives.
 *
 * TODO: this TU was built with auto-inlining OFF. Nothing here is inlined in
 * retail -- Hx_Warning (an empty function), Hx_SetVFilter, Hx_CameraInit,
 * Hx_TimerCountDown and Frb2_InitBlackBox are all called out of line -- while
 * the MSL header inlines (sqrtf's Newton iterations) still expand. Building
 * this object with `extra_cflags=["-inline noauto"]` in configure.py takes it
 * from nine exact functions to sixteen (Hx_UpdateWipe, Hx_ProvideResource,
 * Hx_ProvideResourceEx, Hx_RemoveResource, Hx_Test1, Frb2_InitGx and
 * Hx_FrBufferMorf all become byte-exact) and lifts the unit from 50.9% to
 * roughly 84% fuzzy. The flag change is out of scope for this batch.
 *
 * The sqrtf() calls below now inline as retail does (frsqrte plus three Newton
 * steps, a volatile float and an frsp). math.h used to carry that body only
 * inside `namespace std`, behind `#ifdef __cplusplus`, so a C TU got a call;
 * it now defines the same body as a plain global in C mode, which is also how
 * the map names this TU's copies of its two local statics
 * (_half$localstatic0$sqrtf__Ff, with no std qualifier). That lifted
 * Hxs1_Test2 70.7 -> 88.5, Hxs1_Test1 68.6 -> 80.3, Hxs2_Circle 43.4 -> 56.3
 * and Hxs1_Circle 48.1 -> 50.6.
 *
 * @details Everything lives in one file-scope work struct.  TScrnFader calls
 * Hx_ResetWipe() once with the display size, Hx_ProvideResource() to hand over
 * a .bti when a wipe needs one, Hx_StartWipe() to pick a pattern, then
 * Hx_UpdateWipe() every frame until it returns 3.  Each pattern is a handler
 * in handle_table, driven by hx.step / hx.timer; the handlers render straight
 * into the frame buffer with immediate-mode GX and, for the morph wipes, copy
 * the frame buffer back out as a texture.
 */

#include <GC2D/hx_wiper.h>
#include <JSystem/ResTIMG.hpp>
#include <dolphin/dvd.h>
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <dolphin/os.h>
#include <math.h>

// TODO: MarioUtil/ReinitGX.hpp is C++-only (it uses extern "C"), and this TU
// is built with -lang=c, so the prototype is repeated here.
void ReInitializeGX(void);

typedef struct ResTIMG ResTIMG;

/// One point of the logo's pen path.
typedef struct HxDrawPath {
	/* 0x0 */ f32 x;
	/* 0x4 */ f32 y;
	/* 0x8 */ s32 wait;
} HxDrawPath;

/**
 * @brief A trapezoidal speed profile: accelerate, hold, decelerate.
 *
 * @details Hx_MotionSet() solves for the acceleration that covers `distance`
 * over the three phases; Hx_MotionUpdate() integrates one frame of it and
 * returns the distance travelled so far.
 */
typedef struct HxMotion {
	/* 0x00 */ f32 accelEnd;
	/* 0x04 */ f32 holdEnd;
	/* 0x08 */ f32 decelEnd;
	/* 0x0C */ f32 accel;
	/* 0x10 */ f32 unk10;
	/* 0x14 */ f32 decel;
	/* 0x18 */ f32 speed;
	/* 0x1C */ f32 frame;
	/* 0x20 */ f32 position;
} HxMotion;

/// The wipe library's whole state.
typedef struct HxWork {
	/* 0x00 */ u32 width;
	/* 0x04 */ u32 height;
	/* 0x08 */ u32 centerX;
	/* 0x0C */ u32 centerY;
	/* 0x10 */ u8 state;
	/* 0x11 */ u8 wipeNo;
	/* 0x12 */ u8 dir;
	/* 0x13 */ u8 pad13;
	/* 0x14 */ f32 time;
	/* 0x18 */ f32 rate;
	/* 0x1C */ s32 param;
	/* 0x20 */ void (*handler)(void);
	/* 0x24 */ int hasResource;
	/* 0x28 */ int hasResourceEx;
	/* 0x2C */ void* resource;
	/* 0x30 */ void* resourceEx;
	/* 0x34 */ u32 resourceSize;
	/* 0x38 */ u32 step;
	/* 0x3C */ u32 timer;
	/* 0x40 */ HxMotion motion;
} HxWork;

static void Hx_CameraInit(void);
static void Hx_GxInit(int tex, int blend);
static void Hgx_DrawCircle(f32 cx, f32 cy, f32 r, u32 color);
static void Hgx_init_tobj_resource(GXTexObj* obj, const ResTIMG* timg);
static void Hgx_ReadTexture(char* path, void* buffer);
static void Hx_GetFrBuffer(void* dest, u32 left, u32 top, u32 wd, u32 ht);
static void Hx_SetVFilter(f32 rate);
static void Hx_SetVFilterFade(f32 rate);
static void __Hx_FrBufferMorf(u32 x, u32 y);
static void Hx_FrBufferMorf(f32 rate);
static void Frb2_InitGx(GXTexObj* obj);
static void Frb2_InitBlackBox(void);
static void Frb2_RendBox(u32 color, f32 x1, f32 y1, f32 x2, f32 y2);
static void Hx_Warning(int no);
static void SetDisplaySize(u32 width, u32 height);
static void dummy_handler(void);
static void Hx_TimerCountDownDummy(void);
static u32 Hx_TimerCountDown(void);
static void Hx_MotionSet(HxMotion* m, f32 distance, f32 accel_time,
                         f32 hold_time, f32 decel_time);
static f32 Hx_MotionUpdate(HxMotion* m);
static void Hx_Circle(void);
static void Hxs1_Circle(f32 r);
static void Hxs2_Circle(u8 alpha, f32 r_in, f32 r_out);
static void Hxs_FrBufferMorf2(f32 x);
static void Hxs_FrBufferMorf2B(f32 x);
static void Hx_Door(void);
static void Hxs_GameOver(u8 fade, f32 mag, f32 rot);
static void InitWipe(void);
static void Hx_GameOver(void);
static void Hxs_Logo_ExtraDraw();
static void Hxs_Logo_TexSetup(u8 alpha, u8 fade, const ResTIMG* timg);
static void Hxs_Logo_TexDraw(f32 x1, f32 y1, f32 x2, f32 y2, f32 wd, f32 ht);
static void Hxs_Logo_MagDraw(f32 mag, f32 wd, f32 ht);
static void Hxs_PenDraw(f32 x, f32 y, u32 num, const HxDrawPath* dp);
static void Hx_Logo(void);
static void Hx_Test1(void);
static void Hxs1_Test1(f32 cx, f32 cy, f32 r);
static void Hx_Test2(void);
static void Hx_Test2R(void);
static void Hxs1_Test2(u32 num, u32 dir, f32 cx, f32 cy, f32 r_out, f32 r_in);
static void Hx_Test4(void);
static void Hx_Test5(void);

static u16 img_wx;
static u16 img_wy;
static u8 vtable[7];

static HxWork hx;
static u8 hx_buffer[0x3300] __attribute__((aligned(32)));

static void* fbuf        = hx_buffer;
static u8 vtable_org[7]  = { 0x10, 0x10, 0x00, 0x00, 0x00, 0x10, 0x10 };
static u8 dec_step[4]    = { 0, 1, 5, 6 };
static u8 inc_step[3]    = { 2, 3, 4 };
static void* fbuf2       = hx_buffer;
static void* gmover_tex_buffer = hx_buffer;

/* ------------------------------------------------------------------------- */

static void Hx_CameraInit(void)
{
	static f32 camLoc[3] = { 320.0f, 240.0f, -30.0f };
	static f32 objPt[3]  = { 320.0f, 240.0f, 0.0f };
	static f32 up[3]     = { 0.0f, -10.0f, 0.0f };

	Mtx44 proj;
	Mtx view;
	f32 cx = hx.width >> 1;
	f32 cy = hx.height >> 1;
	f32 near = 0.0f;
	f32 far = 100.0f;

	camLoc[0] = cx;
	camLoc[1] = cy;
	objPt[0]  = cx;
	objPt[1]  = cy;

	C_MTXOrtho(proj, cy, -cy, -cx, cx, near, far);
	GXSetProjection(proj, GX_ORTHOGRAPHIC);
	GXSetViewport(0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
	C_MTXLookAt(view, (Vec*)camLoc, (Vec*)up, (Vec*)objPt);
	GXSetCullMode(GX_CULL_NONE);
	GXSetCoPlanar(GX_FALSE);
	GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
	GXSetNumTexGens(0);
	GXSetNumTevStages(1);
	GXSetNumIndStages(0);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
	              GX_COLOR0A0);
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

static void Hx_GxInit(int tex, int blend)
{
	switch (tex) {
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
		break;

	case 0:
		GXSetBlendMode(GX_BM_NONE, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_CLEAR);
		break;
	}
}

// TODO: dead code, and nothing in the TU resembles it, so the shape below is
// a guess from the name alone; the map says 0x1c8 bytes.
static void Hgx_DrawCircle(f32 cx, f32 cy, f32 r, u32 color)
{
	int i;

	GXBegin(GX_LINESTRIP, GX_VTXFMT0, 17);
	for (i = 0; i <= 16; i++) {
		f32 a = 3.1415927f * (2.0f * (f32)i) / 16.0f;
		GXPosition3f32(cx + (r * sinf(a)), cy + (r * cosf(a)), 0.0f);
		GXColor1u32(color);
	}
}

static void Hgx_init_tobj_resource(GXTexObj* obj, const ResTIMG* timg)
{
	GXTexFmt format = (GXTexFmt)timg->format;
	GXTexWrapMode wrapS = (GXTexWrapMode)timg->wrapS;
	GXTexWrapMode wrapT = (GXTexWrapMode)timg->wrapT;
	u8* data = (u8*)timg + timg->imageDataOffset;
	GXTexFilter minFilter = (GXTexFilter)timg->minFilter;
	GXTexFilter magFilter = (GXTexFilter)timg->magFilter;

	img_wx = timg->width;
	img_wy = timg->height;

	GXInitTexObj(obj, data, img_wx, img_wy, format, wrapS, wrapT, GX_FALSE);
	GXInitTexObjLOD(obj, minFilter, magFilter, 0.0f,
	                0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
}

static void Hgx_ReadTexture(char* path, void* buffer)
{
	DVDFileInfo fileInfo;

	switch (hx.hasResource) {
	case 0:
		if (DVDOpen(path, &fileInfo)) {
			s32 read = DVDReadPrio(&fileInfo, buffer, fileInfo.length, 0, 2);
			DVDClose(&fileInfo);
			DCStoreRange(buffer, read);
		}
		break;
	}
}

static void Hx_GetFrBuffer(void* dest, u32 left, u32 top, u32 wd, u32 ht)
{
	GXColor black = { 0, 0, 0, 0 };

	GXSetTexCopySrc(left, top, wd, ht);
	GXSetTexCopyDst(wd, ht, GX_TF_RGB565, GX_FALSE);
	GXGetTexBufferSize(wd, ht, GX_TF_RGB565, GX_FALSE, 0);
	GXSetCopyClear(black, GX_MAX_Z24);
	GXCopyTex(dest, GX_TRUE);
	GXPixModeSync();
}

static void Hx_SetVFilter(f32 rate)
{
	u32 i;
	u8 num;

	num = 64.0f * rate;
	for (i = 0; i < 7; i++)
		vtable[i] = vtable_org[i];

	for (i = 0; i < num; i++) {
		vtable[dec_step[i & 3]]--;
		vtable[inc_step[i % 3]]++;
	}

	GXSetCopyFilter(GX_FALSE, NULL, GX_TRUE, vtable);
}

// TODO: dead code; the map says 0x358 bytes, so the original is close to
// Hx_SetVFilter with a second pass. Reconstruction pending.
static void Hx_SetVFilterFade(f32 rate)
{
	u32 i;
	u8 num;

	for (i = 0; i < 7; i++)
		vtable[i] = vtable_org[i];

	num = 64.0f * rate;
	for (i = 0; i < num; i++) {
		vtable[dec_step[i & 3]]--;
		vtable[inc_step[i % 3]]++;
	}

	GXSetCopyFilter(GX_FALSE, NULL, GX_TRUE, vtable);
}

static void __Hx_FrBufferMorf(u32 x, u32 y)
{
	GXTexObj obj;

	Hx_CameraInit();
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
	Hx_GetFrBuffer(fbuf, x, y, 0x30, 0x30);
	GXInvalidateTexAll();
	GXSetNumTexGens(1);
	GXSetNumTevStages(1);
	GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
	GXSetBlendMode(GX_BM_NONE, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_CLEAR);
	GXInitTexObj(&obj, fbuf, 0x30, 0x30, GX_TF_RGB565, GX_CLAMP, GX_CLAMP,
	             GX_FALSE);
	GXInitTexObjLOD(&obj, GX_LINEAR, GX_LINEAR, 0.0f, 10.0f, 0.0f, GX_FALSE,
	                GX_TRUE, GX_ANISO_1);
	GXLoadTexObj(&obj, GX_TEXMAP0);

	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition3f32(x, y, 0.0f);
	GXColor1u32(0);
	GXTexCoord2f32(0.0f, 0.0f);
	GXPosition3f32(x + 0x30, y, 0.0f);
	GXColor1u32(0);
	GXTexCoord2f32(1.0f, 0.0f);
	GXPosition3f32(x + 0x30, y + 0x30, 0.0f);
	GXColor1u32(0);
	GXTexCoord2f32(1.0f, 1.0f);
	GXPosition3f32(x, y + 0x30, 0.0f);
	GXColor1u32(0);
	GXTexCoord2f32(0.0f, 1.0f);
}

static void Hx_FrBufferMorf(f32 rate)
{
	Hx_SetVFilter(rate);
	__Hx_FrBufferMorf(hx.centerX - 0x18, hx.centerY - 0x18);
}

static void Frb2_InitGx(GXTexObj* obj)
{
	Hx_CameraInit();
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
	GXInitTexObj(obj, fbuf2, 0xA0, 0x10, GX_TF_RGB565, GX_CLAMP, GX_CLAMP,
	             GX_FALSE);
	GXInitTexObjLOD(obj, GX_LINEAR, GX_LINEAR, 0.0f, 10.0f, 0.0f, GX_FALSE,
	                GX_TRUE, GX_ANISO_1);
	GXLoadTexObj(obj, GX_TEXMAP0);
}

static void Frb2_InitBlackBox(void)
{
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
	GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEX_DISABLE,
	              GX_COLOR0A0);
}

static void Frb2_RendBox(u32 color, f32 x1, f32 y1, f32 x2, f32 y2)
{
	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition3f32(x1, y1, 0.0f);
	GXColor1u32(color);
	GXPosition3f32(x2, y1, 0.0f);
	GXColor1u32(color);
	GXPosition3f32(x2, y2, 0.0f);
	GXColor1u32(color);
	GXPosition3f32(x1, y2, 0.0f);
	GXColor1u32(color);
}

static void Hx_Warning(int no) { }

// TODO: dead code; the map says 0x18 bytes, which is two stores plus the
// prologue, so only the size is fixed.
static void SetDisplaySize(u32 width, u32 height)
{
	hx.width  = width;
	hx.height = height;
}

void Hx_ResetWipe(u32 width, u32 height)
{
	hx.state          = 0;
	hx.width          = width;
	hx.height         = height;
	hx.centerX        = hx.width >> 1;
	hx.centerY        = hx.height >> 1;
	hx.hasResource    = 0;
	hx.hasResourceEx  = 0;
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

void Hx_StartWipe(int wipe_no, int param)
{
	if (hx.hasResource == 0) {
		hx.resource     = hx_buffer;
		hx.resourceSize = sizeof(hx_buffer);
	}

	// A one-case switch: its signed compare and unfused `beq body; b skip`
	// are what retail has (a plain `if` gives cmplwi and fuses to `bne`).
	switch (hx.state) {
	case 2:
		Hx_Warning(1);
		break;
	}

	hx.state  = 1;
	hx.wipeNo = wipe_no;
	hx.time   = 0.0f;
	hx.param  = param;
}

static void dummy_handler(void) { }

static void (*handle_table[15])(void) = {
	dummy_handler, Hx_Circle, Hx_Circle,  Hx_Test1, Hx_Test1,
	Hx_Test5,      Hx_Test5,  Hx_Test4,   Hx_Test4, Hx_Test2R,
	Hx_Test2,      Hx_Door,   Hx_Logo,    Hx_GameOver, dummy_handler,
};

/// Which half of the transition each wipe number plays: 0 closes, 1 opens.
static u8 handle_type[15] = {
	0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0,
};

int Hx_GetWipeType(int wipe_no) { return handle_type[wipe_no]; }

u32 Hx_UpdateWipe(f32 rate)
{
	ReInitializeGX();

	switch (hx.state) {
	case 0:
		break;

	case 3:
		if (hx.dir != 1) {
			Hx_CameraInit();
			Hx_GxInit(0, 0);
			Frb2_InitBlackBox();
			Frb2_RendBox(0xFF, 0.0f, 0.0f, hx.width, hx.height);
		}
		break;

	case 1:
		hx.handler = handle_table[hx.wipeNo];
		hx.dir     = handle_type[hx.wipeNo];
		hx.state   = 2;
		hx.step    = 0;
		/* fall through */

	case 2:
		hx.rate = rate;
		GXDrawDone();
		hx.handler();
		GXDrawDone();
		hx.time += rate;
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

static void Hx_MotionSet(HxMotion* m, f32 distance, f32 accel_time,
                         f32 hold_time, f32 decel_time)
{
	f32 speed;

	m->accelEnd = accel_time;
	m->holdEnd  = m->accelEnd + hold_time;
	m->decelEnd = m->holdEnd + decel_time;

	speed = (2.0f * distance)
	        / (decel_time + (hold_time + (accel_time + hold_time)));

	if (accel_time != 0.0f)
		m->accel = speed / accel_time;
	if (decel_time != 0.0f)
		m->decel = -speed / decel_time;

	m->unk10    = 0.0f;
	m->speed    = 0.0f;
	m->position = 0.0f;
	m->frame    = 0.0f;
}

static f32 Hx_MotionUpdate(HxMotion* m)
{
	if (m->accelEnd > m->frame) {
		m->speed += m->accel;
	} else if (m->holdEnd <= m->frame) {
		m->speed += m->decel;
	}

	m->frame += 1.0f;
	m->position += m->speed;

	return m->position;
}

/* ------------------------------------------------------------------------- */
/* Iris wipe: a ring of expanding circles. */

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

	switch (hx.step) {
	case 0:
		p3   = 0.0f;
		p2   = 0.0f;
		p1   = 0.0f;
		a3   = 0;
		a2   = 0;
		a1   = 0;
		r    = 1.0f;
		boke = 0.0f;

		switch (hx.dir) {
		case 0:
			Hx_MotionSet(&hx.motion, 400.0f, 2.0f, 13.0f, 10.0f);
			hx.timer = 25;
			break;
		case 1:
			Hx_MotionSet(&hx.motion, 400.0f, 5.0f, 10.0f, 15.0f);
			hx.timer = 30;
			break;
		}
		hx.step++;
		/* fall through */

	case 1:
		r = Hx_MotionUpdate(&hx.motion);
		switch (hx.dir) {
		case 1:
			boke += 0.06666667f;
			if (boke > 1.0f)
				boke = 1.0f;
			Hx_FrBufferMorf(boke);
			Hx_SetVFilter(1.0f);
			break;
		case 0:
			r = 400.0f - r;
			if (r < 0.0f)
				r = 0.0f;
			break;
		}
		if (Hx_TimerCountDown() == 0) {
			hx.step++;
			hx.state = 3;
		}
		break;

	default:
		hx.state = 3;
		break;
	}

	Hxs1_Circle(r);

	if (r > 22.0f) {
		Hxs2_Circle(a1 >> 8, (r - 20.0f) + p1, r);
		p1 += 0.05f;
		if (a1 < 0xFF00)
			a1 += 0x180;
	}
	if (r > 42.0f) {
		Hxs2_Circle(a2 >> 8, (r - 40.0f) + p2, (r - 20.0f) + p1);
		p2 += 0.12f;
		if (a2 < 0xFF00)
			a2 += 0xC0;
	}
	if (r > 62.0f) {
		Hxs2_Circle(a3 >> 8, (r - 60.0f) + p3, (r - 40.0f) + p2);
		p3 += 0.25f;
		if (a3 < 0xFF00)
			a3 += 0x80;
	}
}

/// The black field outside the iris, drawn as a stack of horizontal lines.
// TODO: frame only (0x150 vs 0x168). Retail's named block is 0x18 larger,
// with the sqrtf slot at 0xa4 unchanged; p[4] or a spare Vec[2] fits it, but
// neither is used, and a real bottom-row array costs +0x28.
static void Hxs1_Circle(f32 r)
{
	u32 y;
	f32 rr;
	Vec p[2];
	Vec d;

	Hx_CameraInit();
	Hx_GxInit(0, 1);
	rr = r * r;

	for (y = 0; y <= hx.centerY; y++) {
		f32 dy = hx.centerY - y;

		p[0].z = 1.0f;
		p[1].z = 1.0f;
		p[0].y = y;
		p[1].y = y;

		if ((f32)(hx.centerY - y) >= r) {
			GXBegin(GX_LINES, GX_VTXFMT0, 4);
			p[0].x = 0.0f;
			p[1].x = hx.width;
			GXPosition3f32(p[0].x, p[0].y, p[0].z);
			GXColor1u32(0xFF);
			GXPosition3f32(p[1].x, p[1].y, p[1].z);
			GXColor1u32(0xFF);
			p[0].y = hx.height - y;
			p[1].y = hx.height - y;
			GXPosition3f32(p[0].x, p[0].y, p[0].z);
			GXColor1u32(0xFF);
			GXPosition3f32(p[1].x, p[1].y, p[1].z);
			GXColor1u32(0xFF);
		} else {
			d.x = sqrtf(rr - (dy * dy));
			GXBegin(GX_LINES, GX_VTXFMT0, 8);
			p[0].x = 0.0f;
			p[1].x = hx.centerX - d.x;
			GXPosition3f32(p[0].x, p[0].y, p[0].z);
			GXColor1u32(0xFF);
			GXPosition3f32(p[1].x, p[1].y, p[1].z);
			GXColor1u32(0xFF);
			p[0].x = hx.centerX + d.x;
			p[1].x = hx.width;
			GXPosition3f32(p[0].x, p[0].y, p[0].z);
			GXColor1u32(0xFF);
			GXPosition3f32(p[1].x, p[1].y, p[1].z);
			GXColor1u32(0xFF);
			p[0].y = hx.height - y;
			p[1].y = hx.height - y;
			GXPosition3f32(p[0].x, p[0].y, p[0].z);
			GXColor1u32(0xFF);
			GXPosition3f32(p[1].x, p[1].y, p[1].z);
			GXColor1u32(0xFF);
			p[0].x = 0.0f;
			p[1].x = hx.centerX - d.x;
			GXPosition3f32(p[0].x, p[0].y, p[0].z);
			GXColor1u32(0xFF);
			GXPosition3f32(p[1].x, p[1].y, p[1].z);
			GXColor1u32(0xFF);
		}
	}
}

/// One translucent ring of the iris.
// TODO: GPR colouring (retail: colour r28, y r25) and the sqrtf slots
// (retail 0xb0/0xac, adjacent) differ; instructions are otherwise exact.
static void Hxs2_Circle(u8 alpha, f32 r_in, f32 r_out)
{
	u32 color;
	u32 y;
	f32 ri2;
	f32 ro2;
	Vec p[2];
	Vec d[2];

	Hx_CameraInit();
	Hx_GxInit(0, 1);

	ri2 = r_in * r_in;
	color = alpha;
	ro2 = r_out * r_out;

	for (y = hx.centerY - r_out; y <= hx.centerY; y++) {
		f32 dy = hx.centerY - y;

		d[0].x = sqrtf(ro2 - dy * dy);
		p[0].z = 1.0f;
		p[1].z = 1.0f;
		p[0].y = y;
		p[1].y = y;

		if (dy >= r_in) {
			GXBegin(GX_LINES, GX_VTXFMT0, 4);
			p[0].x = hx.centerX - d[0].x;
			p[1].x = hx.centerX + d[0].x;
			GXPosition3f32(p[0].x, p[0].y, p[0].z);
			GXColor1u32(color);
			GXPosition3f32(p[1].x, p[1].y, p[1].z);
			GXColor1u32(color);
			p[0].y = hx.height - y;
			p[1].y = hx.height - y;
			GXPosition3f32(p[0].x, p[0].y, p[0].z);
			GXColor1u32(color);
			GXPosition3f32(p[1].x, p[1].y, p[1].z);
			GXColor1u32(color);
		} else {
			d[1].x = sqrtf(ri2 - dy * dy);
			GXBegin(GX_LINES, GX_VTXFMT0, 8);
			p[0].x = hx.centerX - d[0].x;
			p[1].x = hx.centerX - d[1].x;
			GXPosition3f32(p[0].x, p[0].y, p[0].z);
			GXColor1u32(color);
			GXPosition3f32(p[1].x, p[1].y, p[1].z);
			GXColor1u32(color);
			p[0].x = hx.centerX + d[1].x;
			p[1].x = hx.centerX + d[0].x;
			GXPosition3f32(p[0].x, p[0].y, p[0].z);
			GXColor1u32(color);
			GXPosition3f32(p[1].x, p[1].y, p[1].z);
			GXColor1u32(color);
			p[0].y = hx.height - y;
			p[1].y = hx.height - y;
			GXPosition3f32(p[0].x, p[0].y, p[0].z);
			GXColor1u32(color);
			GXPosition3f32(p[1].x, p[1].y, p[1].z);
			GXColor1u32(color);
			p[0].x = hx.centerX - d[0].x;
			p[1].x = hx.centerX - d[1].x;
			GXPosition3f32(p[0].x, p[0].y, p[0].z);
			GXColor1u32(color);
			GXPosition3f32(p[1].x, p[1].y, p[1].z);
			GXColor1u32(color);
		}
	}
}

/* ------------------------------------------------------------------------- */
/* Door wipe: the two halves of the screen slide together, smeared by a
   frame-buffer copy. */

static void Hxs_FrBufferMorf2(f32 x)
{
	GXTexObj obj;
	f32 y;

	Frb2_InitGx(&obj);

	if (x < (f32)(hx.width >> 2)) {
		for (y = 0.0f; y < hx.height; y += 16.0f) {
			Hx_GetFrBuffer(fbuf2, 0, y, 0xA0, 0x10);
			GXInvalidateTexAll();
			GXLoadTexObj(&obj, GX_TEXMAP0);

			GXBegin(GX_QUADS, GX_VTXFMT0, 4);
			GXPosition3f32(x, y, 0.0f);
			GXColor1u32(0);
			GXTexCoord2f32(0.0f, 0.0f);
			GXPosition3f32(hx.width >> 2, y, 0.0f);
			GXColor1u32(0);
			GXTexCoord2f32(1.0f, 0.0f);
			GXPosition3f32(hx.width >> 2, 16.0f + y, 0.0f);
			GXColor1u32(0);
			GXTexCoord2f32(1.0f, 1.0f);
			GXPosition3f32(x, 16.0f + y, 0.0f);
			GXColor1u32(0);
			GXTexCoord2f32(0.0f, 1.0f);
			GXDrawDone();
		}
	}

	Frb2_InitBlackBox();
	Frb2_RendBox(0xFF, 0.0f, 0.0f, x, hx.height);

	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition3f32(0.0f, 0.0f, 0.0f);
	GXColor1u32(0xFF);
	GXPosition3f32(x, 0.0f, 0.0f);
	GXColor1u32(0xFF);
	GXPosition3f32(x, hx.height, 0.0f);
	GXColor1u32(0xFF);
	GXPosition3f32(0.0f, hx.height, 0.0f);
	GXColor1u32(0xFF);
}

static void Hxs_FrBufferMorf2B(f32 x)
{
	GXTexObj obj;
	s32 left = hx.width / 2 + hx.width / 4;
	f32 right;
	f32 y;

	Frb2_InitGx(&obj);
	right = hx.width - x;

	if (x < (f32)(hx.width >> 2)) {
		for (y = 0.0f; y < hx.height; y += 16.0f) {
			Hx_GetFrBuffer(fbuf2, left, y, 0xA0, 0x10);
			GXInvalidateTexAll();
			GXLoadTexObj(&obj, GX_TEXMAP0);

			GXBegin(GX_QUADS, GX_VTXFMT0, 4);
			GXPosition3f32(left, y, 0.0f);
			GXColor1u32(0);
			GXTexCoord2f32(0.0f, 0.0f);
			GXPosition3f32(right, y, 0.0f);
			GXColor1u32(0);
			GXTexCoord2f32(1.0f, 0.0f);
			GXPosition3f32(right, 16.0f + y, 0.0f);
			GXColor1u32(0);
			GXTexCoord2f32(1.0f, 1.0f);
			GXPosition3f32(left, 16.0f + y, 0.0f);
			GXColor1u32(0);
			GXTexCoord2f32(0.0f, 1.0f);
			GXDrawDone();
		}
	}

	Frb2_InitBlackBox();
	Frb2_RendBox(0xFF, right, 0.0f, hx.width, hx.height);
}

// TODO: every instruction matches, but retail's frame is 8 bytes larger in
// the low region (conversion slots 8 higher), the same gap Hx_Logo shows.
static void Hx_Door(void)
{
	s32 x;

	switch (hx.step) {
	case 0:
		hx.step++;
		Hx_MotionSet(&hx.motion, hx.width / 2, 5.0f, 6.0f, 5.0f);
		break;

	case 1:
		x = Hx_MotionUpdate(&hx.motion);
		Hxs_FrBufferMorf2(x);
		if ((u32)x >= hx.width / 2) {
			hx.step++;
			Hx_MotionSet(&hx.motion, hx.width / 2, 5.0f, 6.0f, 5.0f);
		}
		break;

	case 2:
		Hxs_FrBufferMorf2(hx.width / 2);
		x = Hx_MotionUpdate(&hx.motion);
		Hxs_FrBufferMorf2B(x);
		if ((u32)x >= hx.width / 2)
			hx.step++;
		break;

	case 3:
		Hxs_FrBufferMorf2(hx.width / 2);
		Hxs_FrBufferMorf2B(hx.width / 2);
		hx.state = 3;
		break;
	}
}

/* ------------------------------------------------------------------------- */
/* Game-over wipe: the "GAME OVER" texture zooms in, bounces, then fades out
   behind a white panel. */

// TODO: every instruction matches, but retail's frame is 0x68 bytes larger
// (obj at 0xe0, axis at 0xa0, the colour at 0xac, and holes at 0x88, 0xb0 and
// 0x100), which also costs it one more saved FPR.
static void Hxs_GameOver(u8 fade_alpha, f32 scale, f32 rotation)
{
	GXTexObj obj;
	Mtx rotMtx;
	Vec axis;
	f32 aspect;
	f32 texAspect;
	f32 u;
	f32 s0, t0, s1, t1, s2, t2, s3, t3;
	f32 r;
	f32 cx;
	f32 cy;

	Hx_CameraInit();
	gmover_tex_buffer = hx.resource;
	Hgx_init_tobj_resource(&obj, gmover_tex_buffer);
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

	{
		GXColor fadeColor = { 0, 0, 0, 0 };
		fadeColor.a = fade_alpha;
		GXSetTevColor(GX_TEVREG0, fadeColor);
	}
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
	GXLoadTexObj(&obj, GX_TEXMAP0);
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
	GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA,
	               GX_LO_CLEAR);

	aspect    = hx.width / hx.height;
	texAspect = (f32)img_wx / (f32)img_wy;
	u         = aspect / texAspect;
	axis.x    = 0.5f;
	axis.y    = 0.5f;
	axis.z    = 0.0f;

	VECNormalize(&axis, &axis);
	MTXRotRad(rotMtx, 'Z', rotation);
	MTXMultVec(rotMtx, &axis, &axis);

	cx = 0.5f;
	cy = 0.5f;
	r = scale * sqrtf((cx * cx) + (cy * cy));
	s0 = (u * (-axis.x * r)) + 0.5f;
	t0 = (-axis.y * r) + 0.5f;
	r = scale * sqrtf((cx * cx) + (cy * cy));
	s1 = (u * (axis.y * r)) + 0.5f;
	t1 = (-axis.x * r) + 0.5f;
	r = scale * sqrtf((cx * cx) + (cy * cy));
	s2 = (u * (axis.x * r)) + 0.5f;
	t2 = (axis.y * r) + 0.5f;
	r = scale * sqrtf((cx * cx) + (cy * cy));
	s3 = (u * (-axis.y * r)) + 0.5f;
	t3 = (axis.x * r) + 0.5f;

	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition3f32(0.0f, 0.0f, 0.0f);
	GXColor1u32(0);
	GXTexCoord2f32(s0, t0);
	GXPosition3f32(hx.width, 0.0f, 0.0f);
	GXColor1u32(0);
	GXTexCoord2f32(s1, t1);
	GXPosition3f32(hx.width, hx.height, 0.0f);
	GXColor1u32(0);
	GXTexCoord2f32(s2, t2);
	GXPosition3f32(0.0f, hx.height, 0.0f);
	GXColor1u32(0);
	GXTexCoord2f32(s3, t3);

	GXSetBlendMode(GX_BM_NONE, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_CLEAR);
}

// TODO: dead code; the map says 0xc bytes, i.e. a single store.
static void InitWipe(void) { hx.state = 0; }

// TODO: volatile FPRs are numbered the other way round in case 0 (0.0f/0.3f)
// and in the final Hxs_GameOver call (2.0f/fade); store order, a shared
// `fade = rot = 0.0f` and a cast on fade are inert.
static void Hx_GameOver(void)
{
	static f32 mag = 1.0f;
	static f32 rot;
	static f32 fade;
	/// Alternating scale deltas and hold times for the post-zoom bounce; a
	/// zero hold time ends the sequence.
	static f32 boundtable[14] = {
		-0.13f, 6.0f, 0.13f, 6.0f, -0.12f, 8.0f, 0.12f,
		-8.0f, -0.11f, 12.0f, 0.11f, 12.0f, 0.0f, 0.0f,
	};
	static s32 boundstate;
	static u32 boundtimer;
	static f32 bounddelta;
	static u8 alpha;

	switch (hx.step) {
	case 0:
		Hgx_ReadTexture("/data/wipe_gameover.bti", gmover_tex_buffer);
		rot  = 0.0f;
		mag  = 0.3f;
		fade = 0.0f;
		hx.step++;
		hx.timer = 50;
		Hx_MotionSet(&hx.motion, -12.566371f, 10.0f, 15.0f, 25.0f);
		break;

	case 1:
		if (Hx_TimerCountDown() == 0) {
			hx.step++;
			hx.timer = 10;
		}
		mag = mag + 0.074f;
		rot = Hx_MotionUpdate(&hx.motion);
		fade = fade + 5.1f;
		break;

	case 2:
		if (Hx_TimerCountDown() == 0) {
			hx.step++;
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
				hx.step++;
				hx.timer = 0x20;
				alpha    = 0xFF;
				break;
			}
		}
		mag += bounddelta;
		break;

	case 4: {
		u32 color;
		f32 x2;
		f32 y2;

		if (Hx_TimerCountDown() == 0) {
			hx.timer = 100;
			hx.step++;
		}

		alpha += 8;
		Hx_CameraInit();
		Hx_GxInit(0, 1);
		color = alpha | 0xFF000000;
		y2 = hx.height - 100;
		x2 = hx.width - 100;

		GXBegin(GX_QUADS, GX_VTXFMT0, 4);
		GXPosition3f32(100.0f, 100.0f, 0.0f);
		GXColor1u32(color);
		GXPosition3f32(x2, 100.0f, 0.0f);
		GXColor1u32(color);
		GXPosition3f32(x2, y2, 0.0f);
		GXColor1u32(color);
		GXPosition3f32(100.0f, y2, 0.0f);
		GXColor1u32(color);
		break;
	}

	case 5: {
		f32 x2;
		f32 y2;

		Hx_CameraInit();
		Hx_GxInit(0, 1);
		y2 = hx.height - 100;
		x2 = hx.width - 100;

		GXBegin(GX_QUADS, GX_VTXFMT0, 4);
		GXPosition3f32(100.0f, 100.0f, 0.0f);
		GXColor1u32(0xFF0000FF);
		GXPosition3f32(x2, 100.0f, 0.0f);
		GXColor1u32(0xFF0000FF);
		GXPosition3f32(x2, y2, 0.0f);
		GXColor1u32(0xFF0000FF);
		GXPosition3f32(100.0f, y2, 0.0f);
		GXColor1u32(0xFF0000FF);

		if (Hx_TimerCountDown() == 0) {
			hx.step++;
			hx.state = 3;
		}
		break;
	}

	default:
		hx.state = 3;
		break;
	}

	if (hx.step >= 2)
		Hxs_GameOver(-1, 2.0f * mag, rot);
	else
		Hxs_GameOver(fade, 2.0f * mag, rot);
}

/* ------------------------------------------------------------------------- */
/* Logo wipe: the Mario emblem drawn with a pen stroke, then zoomed out. */

static HxDrawPath drawpath_table[] = {
	{ 49.0f, 277.0f, 0 },    { 20.0f, 294.0f, 1 },  { 22.0f, 234.0f, 1 },
	{ 50.0f, 175.0f, 1 },    { 110.0f, 112.0f, 1 }, { 128.0f, 106.0f, 1 },
	{ 138.0f, 110.0f, 1 },   { 134.0f, 113.0f, 1 }, { 135.0f, 128.0f, 1 },
	{ 117.0f, 166.0f, 1 },   { 119.0f, 201.0f, 1 }, { 129.0f, 229.0f, 1 },
	{ 140.0f, 219.0f, 1 },   { 149.0f, 172.0f, 1 }, { 178.0f, 137.0f, 1 },
	{ 192.0f, 114.0f, 1 },   { 216.0f, 104.0f, 1 }, { 227.0f, 109.0f, 1 },
	{ 224.0f, 130.0f, 1 },   { 183.0f, 265.0f, 3 }, { 161.0f, 18.0f, 0 },
	{ 161.0f, 18.01f, 8 },   { 141.0f, 79.0f, 3 },  { 255.0f, 3.0f, 0 },
	{ 255.0f, 3.01f, 8 },    { 220.0f, 71.0f, 4 },  { -1.0f, -1.0f, -1 },
};

static int hxs_logo_resetflag;
static int hxs_logodraw_resetflag;

/// Defined without a prototype: every call in Hx_Logo passes only the alpha
/// (retail never loads r4 before the `bl`), yet the body reads `timg` from
/// r4, so the original call sites dropped the texture argument.
static void Hxs_Logo_ExtraDraw(alpha_in, timg)
u8 alpha_in;
const ResTIMG* timg;
{
	GXTexObj obj;

	Hx_CameraInit();
	Hx_GxInit(1, 1);
	Hgx_init_tobj_resource(&obj, timg);

	{
		GXColor color = { 0xFF, 0xFF, 0xFF, 0xFF };
		color.a       = alpha_in;
		GXSetTevColor(GX_TEVREG0, color);
	}
	GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_ZERO, GX_CC_ZERO,
	                GX_CC_ZERO);
	GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA,
	                GX_CA_ZERO);
	GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
	                GX_TRUE, GX_TEVPREV);
	GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
	                GX_TRUE, GX_TEVPREV);
	GXLoadTexObj(&obj, GX_TEXMAP0);
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
}

static void Hxs_Logo_TexSetup(u8 alpha_in, u8 fade_in, const ResTIMG* timg)
{
	GXTexObj obj;

	Hx_CameraInit();
	Hx_GxInit(1, 1);
	Hgx_init_tobj_resource(&obj, timg);

	{
		GXColor color = { 0xFF, 0, 0, 0 };
		color.r       = alpha_in;
		color.a       = fade_in;
		if (fade_in > 0xC0)
			color.a = 0xFF;
		else
			color.a = 1.328 * (f64)fade_in;
		GXSetTevColor(GX_TEVREG0, color);

		if (fade_in > 0xC0)
			color.a = ((0xFF - fade_in) * 4) & 0xFC;
		else
			color.a = 0xFF;
		GXSetTevColor(GX_TEVREG1, color);
	}

	GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_C0, GX_CC_TEXA,
	                GX_CC_ZERO);
	GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A1, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
	GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
	                GX_TRUE, GX_TEVPREV);
	GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
	                GX_TRUE, GX_TEVPREV);
	GXLoadTexObj(&obj, GX_TEXMAP0);
	GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA,
	               GX_LO_CLEAR);
}

// TODO: retail's frame is 8 bytes larger (its Vec sits at 0x38, ours at
// 0x30) and its callee-saved FPRs are coloured differently (dx/dy in f31/f30,
// u1..v2 in f22..f25); declaration order is inert.
static void Hxs_Logo_TexDraw(f32 x1, f32 y1, f32 x2, f32 y2, f32 wd, f32 ht)
{
	f32 sy = ht / 1.924138f;
	f32 sx = wd / 1.9230769f;
	f32 v1 = y1 / sy;
	f32 v2 = y2 / sy;
	f32 u1 = x1 / sx;
	f32 u2 = x2 / sx;
	f32 ox = (f32)(hx.width >> 1) - (sx * 0.5f);
	f32 oy = ((f32)(hx.height >> 1) - (sy * 0.5f)) - 32.0f;
	Vec d;
	f32 dx;
	f32 dy;
	f32 px;
	f32 py;

	// The pen stroke is a quad two units wide around the segment, so the
	// offset is the segment's normal: (-dv, du).
	d.y = u2 - u1;
	d.x = -(v2 - v1);
	d.z = 0.0f;

	if ((0.0f != d.y) || (0.0f != d.x)) {
		VECNormalize(&d, &d);
		VECScale(&d, &d, 0.08f);
		dx = d.x;
		dy = d.y;
		px = u1 + dx;
		py = v1 + dy;
		px = (sx * px) + ox;
		py = (sy * py) + oy;

		GXBegin(GX_QUADS, GX_VTXFMT0, 4);
		GXPosition3f32(px, py, 0.0f);
		GXColor1u32(0);
		GXTexCoord2f32(u1 + d.x, v1 + d.y);
		GXPosition3f32((sx * (u2 + dx)) + ox, (sy * (v2 + dy)) + oy, 0.0f);
		GXColor1u32(0);
		GXTexCoord2f32(u2 + d.x, v2 + d.y);
		GXPosition3f32((sx * (u2 - dx)) + ox, (sy * (v2 - dy)) + oy, 0.0f);
		GXColor1u32(0);
		GXTexCoord2f32(u2 - d.x, v2 - d.y);
		GXPosition3f32((sx * (u1 - dx)) + ox, (sy * (v1 - dy)) + oy, 0.0f);
		GXColor1u32(0);
		GXTexCoord2f32(u1 - d.x, v1 - d.y);
	}
}

// TODO: retail's frame is 8 bytes larger in the low region (every conversion
// slot sits 8 higher), and the hw/hh products schedule differently; unnaming
// cx/cy is worse.
static void Hxs_Logo_MagDraw(f32 mag_scale, f32 wd, f32 ht)
{
	f32 hw = (wd / 1.9230769f) * mag_scale * 0.5f;
	f32 hh = (ht / 1.924138f) * mag_scale * 0.5f;
	f32 cx = hx.width >> 1;
	f32 cy = hx.height >> 1;
	f32 x1 = cx - hw;
	f32 y1 = cy - hh;
	f32 u1 = x1 / (x1 - (cx + hw));
	f32 v1 = y1 / (y1 - (cy + hh));

	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition3f32(0.0f, -32.0f, 0.0f);
	GXColor1u32(0);
	GXTexCoord2f32(u1, v1);
	GXPosition3f32(hx.width, -32.0f, 0.0f);
	GXColor1u32(0);
	GXTexCoord2f32((1.0f - u1), v1);
	GXPosition3f32(hx.width, hx.height - 32, 0.0f);
	GXColor1u32(0);
	GXTexCoord2f32((1.0f - u1), (1.0f - v1));
	GXPosition3f32(0.0f, hx.height - 32, 0.0f);
	GXColor1u32(0);
	GXTexCoord2f32(u1, (1.0f - v1));
}

static void Hxs_PenDraw(f32 x, f32 y, u32 num, const HxDrawPath* dp)
{
	u32 i;
	f32 px;
	f32 nx;
	f32 py;
	f32 ny;

	if (num != 0) {
		px = drawpath_table[0].x;
		py = drawpath_table[0].y;

		for (i = 0; i < num - 1; i++) {
			const HxDrawPath* p = &drawpath_table[i];

			if (p[1].wait == -1)
				break;

			nx = p[1].x;
			ny = p[1].y;
			if (p[1].wait != 0) {
				Vec d;
				d.x = nx - px;
				d.y = ny - py;
				VECNormalize(&d, &d);
				VECScale(&d, &d, 6.0f);
				Hxs_Logo_TexDraw(px - d.x, py - d.y, nx + d.x, ny + d.y,
				                 img_wx, img_wy);
			}

			px = nx;
			py = ny;
		}
	}

	{
		f32 t = (f32)(dp->wait - hx.timer) / (f32)dp->wait;
		f32 x2 = (t * (dp->x - x)) + x;
		f32 y2 = (t * (dp->y - y)) + y;
		Hxs_Logo_TexDraw(x, y, x2, y2, img_wx, img_wy);
	}
}

// TODO: retail's frame is 8 bytes larger in the low region (the MagDraw
// conversion slots sit at 0x10, ours at 0x08) and case 2 keeps dp in r6
// where ours uses r5; a ternary or later `timg` load is worse.
static void Hx_Logo(void)
{
	static HxDrawPath* dp;
	static f32 bx;
	static f32 by;
	static s32 count;

	void* buffer;
	const ResTIMG* timg = hx.resource;

	if (hx.hasResourceEx != 0)
		buffer = hx.resourceEx;
	else
		buffer = hx_buffer;

	switch (hx.step) {
	case 0:
		if (hx.hasResourceEx == 0)
			Hgx_ReadTexture("/data/title_mini.bti", buffer);

		hx.step++;
		dp                    = drawpath_table;
		count                 = 0;
		hx.timer              = 0x100;
		hxs_logo_resetflag    = 1;
		hxs_logodraw_resetflag = 1;
		break;

	case 1:
		if (hx.timer <= 0xC0)
			Hxs_Logo_ExtraDraw(0xFF);
		else
			Hxs_Logo_ExtraDraw(((0x100 - hx.timer) * 4) & 0xFC);

		Hx_TimerCountDown();
		Hx_TimerCountDown();
		Hx_TimerCountDown();
		if (Hx_TimerCountDown() == 0)
			hx.step++;
		break;

	case 2:
		if (dp->wait == -1) {
			hx.step = 3;
			goto board;
		}
		if (dp->wait == 0) {
			bx = dp->x;
			by = dp->y;
			dp++;
			count++;
		}
		hx.timer = dp->wait;
		hx.step++;
		/* fall through */

	case 3:
		Hxs_Logo_ExtraDraw(0xFF);
		Hxs_Logo_TexSetup(0xFF, 0xFF, timg);
		Hxs_PenDraw(bx, by, count, dp);
		if (Hx_TimerCountDown() == 0) {
			bx = dp->x;
			by = dp->y;
			dp++;
			hx.step = 2;
			count++;
		}
		break;

	case 4:
	board:
		hx.timer = 5;
		hx.step++;
		/* fall through */

	case 5:
		Hxs_Logo_ExtraDraw(0xFF);
		Hxs_Logo_TexSetup(0xFF, 0xFF, timg);
		Hxs_PenDraw(bx, by, count, dp);
		if (Hx_TimerCountDown() == 0) {
			hx.timer = 0xFF;
			hx.step++;
		}
		break;

	case 6: {
		u32 i;

		if (hx.timer >= 0xC0) {
			Hxs_Logo_ExtraDraw(0xFF);
			Hxs_Logo_TexSetup(hx.timer, hx.timer, timg);
			if (hx.timer > 0xF8)
				Hxs_PenDraw(bx, by, count, dp);
			else
				Hxs_Logo_MagDraw(1.0f, img_wx, img_wy);
		} else {
			Hxs_Logo_TexSetup(hx.timer, hx.timer, timg);
			Hxs_Logo_MagDraw(1.0f, img_wx, img_wy);
		}

		for (i = 0; i < 3; i++)
			Hx_TimerCountDown();

		if (Hx_TimerCountDown() == 0) {
			hx.timer = 25;
			Hx_MotionSet(&hx.motion, 30.0f, 12.0f, 8.0f, 5.0f);
			hx.step++;
		}
		break;
	}

	case 7:
		Hxs_Logo_TexSetup(0, 0, timg);
		Hxs_Logo_MagDraw(1.0f + Hx_MotionUpdate(&hx.motion), img_wx, img_wy);
		if (Hx_TimerCountDown() == 0)
			hx.step++;
		break;

	case 8:
		hx.state = 3;
		break;
	}
}

// TODO: dead code; the map says 0x70 bytes. Hx_MovieStartSyncEx below is the
// surviving variant, so this is presumably it without the logo-draw phase.
void Hx_MovieStartSync(void)
{
	if (hx.wipeNo != 0xC)
		return;
	if (hx.step >= 6)
		hxs_logo_resetflag = 0;
}

int Hx_MovieStartSyncEx(void)
{
	if (hx.wipeNo != 0xC)
		return 0;

	if (hx.step >= 2 && hx.step <= 5) {
		if (hxs_logodraw_resetflag == 0)
			return 0;
		hxs_logodraw_resetflag = 0;
		return 1;
	}

	if (hx.step >= 6) {
		if (hxs_logo_resetflag == 0)
			return 0;
		if (hx.step == 6 && hx.timer > 0xC0)
			return 0;
		hxs_logo_resetflag = 0;
		return 2;
	}

	return 0;
}

/* ------------------------------------------------------------------------- */
/* Corner-circle wipes. */

static void Hx_Test1(void)
{
	static f32 r;

	switch (hx.step) {
	case 0:
		if (hx.dir == 1) {
			r = 400.0f;
			Hx_MotionSet(&hx.motion, 400.0f, 10.0f, 12.0f, 8.0f);
		} else {
			r = 1.0f;
			Hx_MotionSet(&hx.motion, 400.0f, 5.0f, 10.0f, 10.0f);
		}
		hx.step++;
		hx.timer = 25;
		break;

	case 1:
		if (Hx_TimerCountDown() == 0)
			hx.step++;
		r = Hx_MotionUpdate(&hx.motion);
		if (hx.dir == 1)
			r = 400.0f - r;
		break;

	default:
		hx.state = 3;
		break;
	}

	Hxs1_Test1(0.0f, 0.0f, r);
	Hxs1_Test1(hx.width, 0.0f, r);
	Hxs1_Test1(hx.width, hx.height, r);
	Hxs1_Test1(0.0f, hx.height, r);
}

static void Hxs1_Test1(f32 cx, f32 cy, f32 r)
{
	u32 i;
	f32 rr;
	Vec p[2];
	Vec d;

	Hx_CameraInit();
	Hx_GxInit(0, 1);
	GXSetLineWidth(7, GX_TO_ZERO);
	rr = r * r;
	GXBegin(GX_LINES, GX_VTXFMT0, ((u32)r * 2) + 2);

	for (i = 0; i <= (u32)r; i++) {
		p[0].z = 1.0f;
		p[1].z = 1.0f;
		d.x = sqrtf(rr - (f32)(i * i));

		if (cy < hx.centerY) {
			p[0].y = cy + i;
			p[1].y = p[0].y;
		} else {
			p[0].y = cy - i;
			p[1].y = p[0].y;
		}

		if (cx < hx.centerX) {
			p[0].x = cx;
			p[1].x = cx + d.x;
		} else {
			p[0].x = cx - d.x;
			p[1].x = cx;
		}

		GXPosition3f32(p[0].x, p[0].y, p[0].z);
		GXColor1u32(0xFF);
		GXPosition3f32(p[1].x, p[1].y, p[1].z);
		GXColor1u32(0xFF);
	}
}

/* ------------------------------------------------------------------------- */
/* Sweeping-arc wipes. */

static void Hx_Test2(void)
{
	static f32 r;

	switch (hx.step) {
	case 0:
		r = 1.0f;
		Hx_MotionSet(&hx.motion, 500.0f, 2.0f, 8.0f, 1.0f);
		hx.step++;
		hx.timer = 11;
		return;

	case 1:
		r = Hx_MotionUpdate(&hx.motion);
		Hxs1_Test2(r, 1, hx.width + 200, 300.0f, 900.0f, 650.0f);
		if (Hx_TimerCountDown() == 0) {
			hx.step++;
			hx.timer = 11;
			Hx_MotionSet(&hx.motion, 500.0f, 2.0f, 8.0f, 1.0f);
		}
		return;

	case 2:
		Hxs1_Test2(600, 0, hx.width + 200, 300.0f, 900.0f, 650.0f);
		r = Hx_MotionUpdate(&hx.motion);
		Hxs1_Test2(r, 0, hx.width + 200, 150.0f, 700.0f, 450.0f);
		if (Hx_TimerCountDown() == 0) {
			hx.step++;
			hx.timer = 10;
			Hx_MotionSet(&hx.motion, 500.0f, 2.0f, 7.0f, 1.0f);
		}
		break;

	case 3:
		Hxs1_Test2(600, 0, hx.width + 200, 300.0f, 900.0f, 650.0f);
		Hxs1_Test2(600, 0, hx.width + 200, 150.0f, 700.0f, 450.0f);
		r = Hx_MotionUpdate(&hx.motion);
		Hxs1_Test2(r, 1, hx.width + 250, 370.0f, 650.0f, 400.0f);
		if (Hx_TimerCountDown() == 0) {
			hx.step++;
			hx.timer = 12;
			Hx_MotionSet(&hx.motion, 500.0f, 2.0f, 9.0f, 1.0f);
		}
		break;

	case 4:
		Hxs1_Test2(600, 0, hx.width + 200, 300.0f, 900.0f, 650.0f);
		Hxs1_Test2(600, 0, hx.width + 200, 150.0f, 700.0f, 450.0f);
		Hxs1_Test2(600, 1, hx.width + 250, 370.0f, 650.0f, 400.0f);
		r = Hx_MotionUpdate(&hx.motion);
		Hxs1_Test2(r, 0, hx.width + 250, 300.0f, 420.0f, 200.0f);
		if (Hx_TimerCountDown() == 0) {
			hx.step++;
			hx.state = 3;
			return;
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

	switch (hx.step) {
	case 0:
		r = 1.0f;
		Hx_MotionSet(&hx.motion, 500.0f, 2.0f, 8.0f, 1.0f);
		hx.step++;
		hx.timer = 11;
		/* fall through */

	case 1:
		Hxs1_Test2(600, 0, hx.width + 200, 150.0f, 700.0f, 450.0f);
		Hxs1_Test2(600, 1, hx.width + 250, 370.0f, 650.0f, 400.0f);
		Hxs1_Test2(600, 0, hx.width + 250, 300.0f, 420.0f, 200.0f);
		r = Hx_MotionUpdate(&hx.motion);
		r = 500.0f - r;
		Hxs1_Test2(r, 0, hx.width + 200, 300.0f, 900.0f, 650.0f);
		if (Hx_TimerCountDown() == 0) {
			hx.step++;
			hx.timer = 11;
			Hx_MotionSet(&hx.motion, 500.0f, 2.0f, 8.0f, 1.0f);
		}
		return;

	case 2:
		Hxs1_Test2(600, 1, hx.width + 250, 370.0f, 650.0f, 400.0f);
		Hxs1_Test2(600, 0, hx.width + 250, 300.0f, 420.0f, 200.0f);
		r = Hx_MotionUpdate(&hx.motion);
		r = 500.0f - r;
		Hxs1_Test2(r, 1, hx.width + 200, 150.0f, 700.0f, 450.0f);
		if (Hx_TimerCountDown() == 0) {
			hx.step++;
			hx.timer = 10;
			Hx_MotionSet(&hx.motion, 500.0f, 2.0f, 7.0f, 1.0f);
		}
		break;

	case 3:
		Hxs1_Test2(600, 0, hx.width + 250, 300.0f, 420.0f, 200.0f);
		r = Hx_MotionUpdate(&hx.motion);
		r = 500.0f - r;
		Hxs1_Test2(r, 0, hx.width + 250, 370.0f, 650.0f, 400.0f);
		if (Hx_TimerCountDown() == 0) {
			hx.step++;
			hx.timer = 12;
			Hx_MotionSet(&hx.motion, 500.0f, 2.0f, 9.0f, 1.0f);
		}
		break;

	case 4:
		r = Hx_MotionUpdate(&hx.motion);
		r = 500.0f - r;
		Hxs1_Test2(r, 1, hx.width + 250, 300.0f, 420.0f, 200.0f);
		if (Hx_TimerCountDown() == 0) {
			hx.step++;
			hx.state = 3;
			return;
		}
		break;

	default:
		hx.state = 3;
		break;
	}
}

static void Hxs1_Test2(u32 num, u32 dir, f32 cx, f32 cy, f32 r_out, f32 r_in)
{
	s32 i;
	s32 start;
	s32 end;
	s32 step;
	f32 ro2;
	f32 ri2;
	Vec p[2];

	Hx_CameraInit();
	Hx_GxInit(0, 1);

	ro2 = r_out * r_out;
	ri2 = r_in * r_in;

	if (dir == 0) {
		step  = 1;
		start = -cy;
		end   = r_out;
	} else {
		step  = -1;
		start = r_out;
		end   = -cy;
	}

	for (i = start; i != end; i += step) {
		f32 y = cy + (f32)i;
		f32 dx_out;
		f32 dx_in;

		if (y < 0.0f || y > hx.height)
			continue;
		if (num == 0)
			break;
		num--;

		dx_out = sqrtf(ro2 - (f32)(i * i));
		dx_in = sqrtf(ri2 - (f32)(i * i));

		if (cx < hx.centerX) {
			p[1].x = cx + dx_out;
			p[0].x = cx + dx_in;
			if (p[1].x < 0.0f)
				continue;
		} else {
			p[0].x = cx - dx_out;
			p[1].x = cx - dx_in;
			if (p[0].x > hx.width)
				continue;
		}

		p[0].y = y;
		p[0].z = 1.0f;
		p[1].y = y;
		p[1].z = 1.0f;
		GXBegin(GX_LINES, GX_VTXFMT0, 2);
		GXPosition3f32(p[0].x, p[0].y, p[0].z);
		GXColor1u32(0xFF);
		GXPosition3f32(p[1].x, p[1].y, p[1].z);
		GXColor1u32(0xFF);
	}
}

/* ------------------------------------------------------------------------- */
/* Spiral wipe. */

// TODO: retail's frame is 8 bytes larger in the low region (its conversion
// slots start at 0x10, the gap Hx_Door shows), and it carries a dead `b` to
// the epilogue ahead of case 0; `default` first or last does not produce it.
static void Hx_Test4(void)
{
	static f32 thin;
	static u32 rstep;
	static f32 thin_d;
	static f32 rstep_d;

	f32 r;
	f32 ro;
	f32 ri;
	f32 a;
	f32 x1;
	f32 y1;
	f32 x2;
	f32 y2;
	u32 i;

	switch (hx.step) {
	case 0:
		switch (hx.dir) {
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
		hx.step++;
		/* fall through */

	case 1:
		rstep += rstep_d;
		thin += thin_d;

		r  = (hx.width >> 1) + 200;
		a  = 0.0f;
		ro = r + thin;
		x1 = (ro * sinf(a)) + hx.centerX;
		y1 = (ro * cosf(a)) + hx.centerY;
		ri = r - thin;
		x2 = (ri * sinf(a)) + hx.centerX;
		y2 = (ri * cosf(a)) + hx.centerY;

		a = 0.0f;
		Hx_CameraInit();
		Hx_GxInit(0, 1);

		for (i = 0; i < rstep; i++) {
			f32 nx1;
			f32 ny1;
			f32 nx2;
			f32 ny2;

			r -= 2.4f;
			ro = r + thin;
			if (r < thin)
				ri = 0.0f;
			else
				ri = r - thin;
			a += 0.12f;

			nx1 = (ro * sinf(a)) + hx.centerX;
			ny1 = (ro * cosf(a)) + hx.centerY;
			nx2 = (ri * sinf(a)) + hx.centerX;
			ny2 = (ri * cosf(a)) + hx.centerY;

			GXBegin(GX_QUADS, GX_VTXFMT0, 4);
			GXPosition3f32(x1, y1, 0.0f);
			GXColor1u32(0xFF);
			GXPosition3f32(nx1, ny1, 0.0f);
			GXColor1u32(0xFF);
			GXPosition3f32(nx2, ny2, 0.0f);
			GXColor1u32(0xFF);
			GXPosition3f32(x2, y2, 0.0f);
			GXColor1u32(0xFF);

			x1 = nx1;
			y1 = ny1;
			x2 = nx2;
			y2 = ny2;
		}

		if (Hx_TimerCountDown() == 0) {
			hx.state = 3;
			hx.step++;
		}
		return;
	}
}

/* ------------------------------------------------------------------------- */
/* Mosaic wipe: every 64x64 tile of the frame buffer is redrawn as a
   ring-shaped fan that closes up. */

// TODO: retail's frame is 0x10 larger (GXTexObj at 0xc, the first-vertex
// block and conversion slots higher), it carries the same dead `b` ahead of
// case 0 as Hx_Test4, and the u/v fmadds and one fcmpo take their operands
// the other way round; reordering the products and the compare is inert.
static void Hx_Test5(void)
{
	GXTexObj obj;
	f32 firstX;
	f32 firstY;
	f32 firstU;
	f32 firstV;
	f32 mag_in;
	f32 mag_out;
	u32 x;
	u32 y;
	u32 i;
	void* buffer = hx_buffer;

	Hx_CameraInit();
	Hx_GxInit(1, 0);
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);

	switch (hx.step) {
	case 0:
		hx.timer = 20;
		hx.step++;
		/* fall through */

	case 1: {
		f32 t;

		GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY,
		                  GX_FALSE, GX_PTIDENTITY);
		GXSetNumTexGens(1);
		GXSetNumTevStages(1);
		GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
		GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
		GXSetBlendMode(GX_BM_NONE, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_CLEAR);
		GXInitTexObj(&obj, buffer, 0x40, 0x40, GX_TF_RGB565, GX_CLAMP,
		             GX_CLAMP, GX_FALSE);
		GXInitTexObjLOD(&obj, GX_LINEAR, GX_LINEAR, 0.0f, 10.0f, 0.0f,
		                GX_FALSE, GX_TRUE, GX_ANISO_1);

		t       = 1.41f * ((f32)hx.timer / 20.0f);
		mag_out = 1.41f - t;
		mag_in  = 0.1f + t;

		for (y = 0; y < hx.height; y += 0x40) {
			for (x = 0; x < hx.width; x += 0x40) {
				f32 cx;
				f32 cy;
				f32 mag_use;
				f32 twist;

				cx = x;
				cy = y;
				Hx_GetFrBuffer(buffer, x, y, 0x40, 0x40);
				GXInvalidateTexAll();
				GXLoadTexObj(&obj, GX_TEXMAP0);

				if (hx.dir != 0)
					mag_use = mag_out;
				else
					mag_use = mag_in;

				if (mag_use < 1.0f)
					twist = 3.1415927f * (1.0f - mag_use);
				else
					twist = 0.0f;

				cx = 32.0f + cx;
				cy = 32.0f + cy;

				GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, 18);
				GXPosition3f32(cx, cy, 0.0f);
				GXColor1u32(0);
				GXTexCoord2f32(0.5f, 0.5f);

				for (i = 0; i < 16; i++) {
					f32 a = 3.1415927f * (2.0f * (f32)i) / 16.0f;
					f32 u = (0.5f * sinf(a)) + 0.5f;
					f32 v = (0.5f * cosf(a)) + 0.5f;
					f32 px = mag_use * sinf(a + twist);
					f32 py = mag_use * cosf(a + twist);

					if (mag_use <= 1.0f) {
						u = (mag_use * sinf(a) * 0.5f) + 0.5f;
						v = (mag_use * cosf(a) * 0.5f) + 0.5f;
					}
					if (px < -1.0f) {
						u  = 0.0f;
						py = -py / px;
						px = -1.0f;
						v  = (0.5f * py) + 0.5f;
					}
					if (px > 1.0f) {
						py /= px;
						px = 1.0f;
						u  = 1.0f;
						v  = (0.5f * py) + 0.5f;
					}
					if (py < -1.0f) {
						v  = 0.0f;
						px = -px / py;
						py = -1.0f;
						u  = (0.5f * px) + 0.5f;
					}
					if (py > 1.0f) {
						px /= py;
						py = 1.0f;
						v  = 1.0f;
						u  = (0.5f * px) + 0.5f;
					}

					px *= 32.0f;
					py *= 32.0f;

					if (i == 0) {
						firstX = px;
						firstY = py;
						firstU = u;
						firstV = v;
					}

					GXPosition3f32(px + cx, py + cy, 0.0f);
					GXColor1u32(0);
					GXTexCoord2f32(u, v);
				}

				GXPosition3f32(firstX + cx, firstY + cy, 0.0f);
				GXColor1u32(0);
				GXTexCoord2f32(firstU, firstV);
			}
		}

		if (Hx_TimerCountDown() == 0) {
			hx.step++;
			hx.state = 3;
		}
		return;
	}

	default:
		hx.state = 3;
		break;
	}
}
