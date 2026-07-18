#include <NPC/NpcInitData.hpp>
#include <types.h>

// pch issues
static f32 dummy0[3]       = { 1.0f, 1.0f, 1.0f };
static f32 dummy1[3]       = { 1.0f, 1.0f, 1.0f };
static u32 dummy2[4]       = { 0, 2, 1, 3 };
static const char dummy3[] = "\0\0\0\0\0\0\0\0\0\0\0";
static const char dummy4[] = "メモリが足りません\n";
static const char dummy5[] = "__ROOT_JOINT__";
// pch issues end

// It seems that there were some ODR violations on this symbol...
extern const char* cNpcPartsNameRootJoint;

static const TNpcTakeData sPeach_TakenMiddle
    = { "/scene/peach/peach_carry_offset.bmd", "null_peach_center" };

static const GXColorS10 sMonteM_HatAColorBuf0[] = {
	{ 200, 200, 120, 255 }, { 70, 70, 70, 255 }, { 200, 200, 150, 255 },
	{ 100, 200, 200, 255 }, { 0, 30, 150, 255 }, { 200, 220, 120, 255 },
	{ 140, 50, 0, 255 },
};

static const GXColorS10 sMonteM_HatAColorBuf1[] = {
	{ 160, 130, 50, 255 }, { 70, 70, 70, 255 }, { 200, 200, 150, 255 },
	{ 50, 120, 160, 255 }, { 0, 30, 150, 255 }, { 0, 130, 50, 255 },
	{ 140, 50, 0, 255 },
};

static const TColorChangeInfo sMonteM_HatAColorChange
    = { 0x00000002, "_boushi_mat", sMonteM_HatAColorBuf0,
	    sMonteM_HatAColorBuf1 };

static const GXColorS10 sMonteM_HatABeltColorBuf[] = {
	{ 100, 0, 0, 255 },   { 0, 0, 0, 255 },     { 0, 0, 0, 255 },
	{ 0, 100, 300, 255 }, { 0, 200, 150, 255 }, { 0, 130, 130, 255 },
	{ 100, 0, 0, 255 },
};

static const TColorChangeInfo sMonteM_HatABeltColorChange
    = { 0x00000001, "_obi_mat", sMonteM_HatABeltColorBuf, nullptr };

static const TNpcModelData sMonteM_HatAData = {
	"kubi",
	0,
	{ "hatA_model.bmd" },
	{
	    { &sMonteM_HatAColorChange, 0 },
	    { &sMonteM_HatABeltColorChange, 0 },
	},
	0,
	0,
	0,
};

static const GXColorS10 sMonteM_HatBColorBuf[] = {
	{ 255, 255, 60, 255 },
	{ 255, 130, 0, 255 },
	{ 255, 255, 0, 255 },
};

static const TColorChangeInfo sMonteM_HatBColorChange
    = { 0x00000000, "_boushi_mat", sMonteM_HatBColorBuf, nullptr };

static const GXColorS10 sMonteM_HatBBeltColorBuf[] = {
	{ 100, 70, 50, 255 },
	{ 100, 70, 50, 255 },
	{ 0, 100, 255, 255 },
};

static const TColorChangeInfo sMonteM_HatBBeltColorChange
    = { 0x00000001, "_obi_mat", sMonteM_HatBBeltColorBuf, nullptr };

static const TNpcModelData sMonteM_HatBData = {
	"kubi",
	0,
	{ "hatB_model.bmd" },
	{
	    { &sMonteM_HatBColorChange, 0 },
	    { &sMonteM_HatBBeltColorChange, 0 },
	},
	0,
	0,
	0,
};

static const GXColorS10 sMonteM_HatDColorBuf[] = {
	{ 0, 100, 230, 255 },
	{ 400, 400, 350, 255 },
};

static const TColorChangeInfo sMonteM_HatDColorChange
    = { 0x00000001, "_obi_mat", sMonteM_HatDColorBuf, nullptr };

static const TNpcModelData sMonteM_HatDData = {
	"kubi",
	0,
	{ "hatD_model.bmd" },
	{
	    { &sMonteM_HatDColorChange, 0 },
	},
	0,
	0,
	0,
};

static const GXColorS10 sMonteM_HatEColorBuf0[] = {
	{ 0, 70, 150, 255 },
	{ 230, 230, 210, 255 },
};

static const GXColorS10 sMonteM_HatEColorBuf1[] = {
	{ 230, 230, 210, 255 },
	{ 0, 70, 150, 255 },
};

static const TColorChangeInfo sMonteM_HatEColorChange
    = { 0x00000002, "_boushi_mat", sMonteM_HatEColorBuf0,
	    sMonteM_HatEColorBuf1 };

static const TNpcModelData sMonteM_HatEData = {
	"kubi",
	0,
	{ "hatE_model.bmd" },
	{
	    { &sMonteM_HatEColorChange, 0 },
	},
	0,
	0,
	0,
};

static const GXColorS10 sMonteM_HatFColorBuf0[] = {
	{ 50, 150, 130, 255 },
	{ 60, 40, 0, 255 },
};

static const GXColorS10 sMonteM_HatFColorBuf1[] = {
	{ 230, 230, 210, 255 },
	{ 160, 150, 60, 255 },
};

static const TColorChangeInfo sMonteM_HatFColorChange
    = { 0x00000002, "_boushi_mat", sMonteM_HatFColorBuf0,
	    sMonteM_HatFColorBuf1 };

static const TNpcModelData sMonteM_HatFData = {
	"kubi",
	0,
	{ "hatF_model.bmd" },
	{
	    { &sMonteM_HatFColorChange, 0 },
	},
	0,
	0,
	0,
};

static const GXColorS10 sMonteM_HatGColorBuf0[] = {
	{ 0, 50, 120, 255 },  { 100, 120, 0, 255 }, { 170, 120, 0, 255 },
	{ 0, 100, 255, 255 }, { 140, 50, 0, 255 },
};

static const GXColorS10 sMonteM_HatGColorBuf1[] = {
	{ 0, 50, 120, 255 },  { 100, 120, 0, 255 }, { 170, 120, 0, 255 },
	{ 0, 180, 255, 255 }, { 200, 120, 0, 255 },
};

static const TColorChangeInfo sMonteM_HatGColorChange
    = { 0x00000002, "_boushi_mat", sMonteM_HatGColorBuf0,
	    sMonteM_HatGColorBuf1 };

static const TNpcModelData sMonteM_HatGData = {
	"kubi",
	0,
	{ "hatG_model.bmd" },
	{
	    { &sMonteM_HatGColorChange, 0 },
	},
	0,
	0,
	0,
};

static const GXColorS10 sMonteM_MustacheAColorBuf0[] = {
	{ 0, 0, 0, 255 },
	{ 255, 255, 150, 255 },
	{ 100, 0, 0, 255 },
	{ 255, 200, 0, 255 },
};

static const TColorChangeInfo sMonteM_MustacheAColorChange
    = { 0x00000001, "_hige_mat", sMonteM_MustacheAColorBuf0, nullptr };

static const TNpcModelData sMonteM_MustacheAData = {
	"kubi",
	0,
	{ "higeA_model.bmd" },
	{
	    { &sMonteM_MustacheAColorChange, 0 },
	},
	1,
	0,
	0,
};

static const TNpcModelData sMonteM_GlassesAData = {
	"kubi",
	0,
	{ "glassesA_model.bmd" },
	{
	    { nullptr, 0 },
	},
	2,
	0,
	0,
};

static const GXColorS10 sMonteM_GlassesBColorBuf0[] = {
	{ 255, 230, 50, 255 },
	{ 255, 255, 255, 255 },
	{ 0, 0, 0, 255 },
};

static const TColorChangeInfo sMonteM_GlassesBColorChange
    = { 0x00000000, "_megane_mat", sMonteM_GlassesBColorBuf0, nullptr };

static const TNpcModelData sMonteM_GlassesBData = {
	"kubi",
	0,
	{ "glassesB_model.bmd" },
	{
	    { &sMonteM_GlassesBColorChange, 0 },
	},
	2,
	0,
	0,
};

static const GXColorS10 sMonteM_LapelAColorBuf0[] = {
	{ 0, 70, 150, 255 },
	{ 255, 255, 230, 255 },
};

static const GXColorS10 sMonteM_LapelAColorBuf1[] = {
	{ 255, 255, 230, 255 },
	{ 0, 70, 150, 255 },
};

static const TColorChangeInfo sMonteM_LapelAColorChange
    = { 0x00000002, "_eri_mat", sMonteM_LapelAColorBuf0,
	    sMonteM_LapelAColorBuf1 };

static const TNpcModelData sMonteM_LapelAData = {
	"body_jnt",
	0,
	{ "eriA_model.bmd" },
	{
	    { &sMonteM_LapelAColorChange, 0 },
	},
	1,
	0,
	0,
};

static const GXColorS10 sMonteM_TieBColorBuf0[] = {
	{ 100, 0, 0, 255 },
};

static const GXColorS10 sMonteM_TieBColorBuf1[] = {
	{ 150, 130, 0, 255 },
};

static const TColorChangeInfo sMonteM_TieBColorChange
    = { 0x00000002, "_tie_mat", sMonteM_TieBColorBuf0, sMonteM_TieBColorBuf1 };

static const TNpcModelData sMonteM_TieBData = {
	"body_jnt",
	0,
	{ "tieB_model.bmd" },
	{
	    { &sMonteM_TieBColorChange, 0 },
	},
	0,
	0,
	0,
};

static const TNpcModelData sMonteM_LuggageData = {
	"body_jnt",
	0,
	{ "nimotsu_model.bmd" },
	{
	    { nullptr, 0 },
	},
	0,
	0,
	0,
};

static const TNpcModelData sMonteMF_TubeData = {
	"body_jnt",
	0,
	{ "tube_model.bmd" },
	{
	    { nullptr, 0 },
	},
	0,
	0,
	0,
};

static const TNpcModelData sMonteMG_MopData = {
	"handR_jnt",
	0,
	{ "mop_model.bmd" },
	{
	    { nullptr, 0 },
	},
	0,
	0,
	0,
};

static const TNpcModelData sMonteMH_UkleleData = {
	"body_jnt",
	0,
	{ "uklele_model.bmd" },
	{
	    { nullptr, 0 },
	},
	0,
	0,
	0,
};

static const GXColorS10 sMonteW_FlowerInColorBuf[] = {
	{ 255, 255, 0, 255 },
	{ 255, 255, 0, 255 },
	{ 255, 255, 0, 255 },
};

static const TColorChangeInfo sMonteW_FlowerInColorChange
    = { 0x00000001, "_naka_mat", sMonteW_FlowerInColorBuf, nullptr };

static const GXColorS10 sMonteW_FlowerOutColorBuf[] = {
	{ 220, 40, 120, 255 },
	{ 220, 40, 0, 255 },
	{ 200, 220, 0, 255 },
};

static const TColorChangeInfo sMonteW_FlowerOutColorChange
    = { 0x00000000, "_hana_mat", sMonteW_FlowerOutColorBuf, nullptr };

static const TNpcModelData sMonteW_FlowerData = {
	"yashi_jnt",
	0,
	{ "flower_model.bmd" },
	{
	    { &sMonteW_FlowerInColorChange, 0 },
	    { &sMonteW_FlowerOutColorChange, 0 },
	},
	1,
	0,
	0,
};

static const GXColorS10 sMonteW_HatAMainColorBuf0[] = {
	{ 200, 200, 120, 255 },
	{ 160, 0, 0, 255 },
	{ 190, 150, 120, 255 },
	{ 200, 220, 220, 255 },
};

static const GXColorS10 sMonteW_HatAMainColorBuf1[] = {
	{ 160, 130, 50, 255 },
	{ 160, 0, 0, 255 },
	{ 120, 70, 50, 255 },
	{ 100, 170, 200, 255 },
};

static const TColorChangeInfo sMonteW_HatAMainColorChange
    = { 0x00000002, "_boushi_mat", sMonteW_HatAMainColorBuf0,
	    sMonteW_HatAMainColorBuf1 };

static const GXColorS10 sMonteW_HatABeltColorBuf[] = {
	{ 300, 100, 100, 255 },
	{ 350, 100, 100, 255 },
	{ 100, 0, 0, 255 },
	{ 400, 400, 380, 255 },
};

static const TColorChangeInfo sMonteW_HatABeltColorChange
    = { 0x00000001, "_obi_mat", sMonteW_HatABeltColorBuf, nullptr };

static const TNpcModelData sMonteW_HatAData = {
	"kubi",
	0,
	{ "hwA_model.bmd" },
	{
	    { &sMonteW_HatAMainColorChange, 0 },
	    { &sMonteW_HatABeltColorChange, 0 },
	},
	0,
	0,
	0,
};

static const GXColorS10 sMonteW_GlassBColorBuf[] = {
	{ 150, 0, 0, 255 },
	{ 0, 200, 0, 255 },
	{ 200, 200, 0, 255 },
};

static const TColorChangeInfo sMonteW_GlassBColorChange
    = { 0x00000000, "_megane_mat", sMonteW_GlassBColorBuf, nullptr };

static const TNpcModelData sMonteW_GlassBData = {
	"kubi",
	0,
	{ "gwB_model.bmd" },
	{
	    { &sMonteW_GlassBColorChange, 0 },
	},
	2,
	0,
	0,
};

static const TNpcModelData sMonteW_ArrowRData = {
	"handR_jnt",
	0,
	{ "arrowR_model.bmd" },
	{
	    { nullptr, 0 },
	},
	0,
	0,
	0,
};

static const TNpcModelData sMonteW_ArrowLData = {
	"handR_jnt",
	0,
	{ "arrowL_model.bmd" },
	{
	    { nullptr, 0 },
	},
	0,
	0,
	0,
};

static const TNpcModelData sMonteWC_HatCData = {
	"kubi",
	0,
	{ "hwC_model.bmd" },
	{
	    { nullptr, 0 },
	},
	0,
	0,
	0,
};

static const TNpcModelData sMonteWC_BraceletRData = {
	"handR_jnt",
	0,
	{ "udewaR_model.bmd" },
	{
	    { nullptr, 0 },
	},
	0,
	0,
	0,
};

static const TNpcModelData sMonteWC_BraceletLData = {
	"handL_jnt",
	0,
	{ "udewaL_model.bmd" },
	{
	    { nullptr, 0 },
	},
	0,
	0,
	0,
};

static const GXColorS10 sMareM_HatAColorBuf0[] = {
	{ 20, 0, 160, 255 },    { 20, 60, 100, 255 },     { 170, 120, 20, 255 },
	{ 10, 10, 10, 255 },    { -40, -10, -10, 255 },   { 0, 20, 30, 255 },
	{ 255, 255, 255, 255 }, { -160, -200, -70, 255 }, { 240, 240, 190, 255 },
};

static const GXColorS10 sMareM_HatAColorBuf1[] = {
	{ 140, 170, 240, 255 }, { 160, 230, 70, 255 },     { -20, 220, 80, 255 },
	{ 150, -30, 40, 255 },  { -120, -160, -220, 255 }, { 100, 160, 140, 255 },
	{ 140, 110, 190, 255 }, { -230, 0, -160, 255 },    { 80, 60, 40, 255 },
};

static const TColorChangeInfo sMareM_HatAColorChange
    = { 0x00000002, "_mat1", sMareM_HatAColorBuf0, sMareM_HatAColorBuf1 };

static const TNpcModelData sMareM_HatAData = {
	"kubi",
	0,
	{ "mareMhat_A.bmd" },
	{
	    { &sMareM_HatAColorChange, 0 },
	},
	0,
	1,
	1,
};

static const GXColorS10 sMareM_HatBColorBuf0[] = {
	{ 170, 230, 210, 255 }, { 150, 160, 200, 255 }, { 200, 190, 10, 255 },
	{ 255, -10, -50, 255 }, { 60, 100, 150, 255 },  { 50, 10, 110, 255 },
};

static const GXColorS10 sMareM_HatBColorBuf1[] = {
	{ 25, 20, 20, 255 },     { -220, -230, 0, 255 }, { -200, -230, 0, 255 },
	{ -150, -100, 20, 255 }, { 20, 20, 10, 255 },    { 160, 130, 220, 255 },
};

static const TColorChangeInfo sMareM_HatBColorChange
    = { 0x00000002, "_mat1", sMareM_HatBColorBuf0, sMareM_HatBColorBuf1 };

static const TNpcModelData sMareM_HatBData = {
	"kubi",
	0,
	{ "mareMhat_B.bmd" },
	{
	    { &sMareM_HatBColorChange, 0 },
	},
	0,
	0,
	1,
};

static const GXColorS10 sMareM_HatEColorBuf0[] = {
	{ 330, -150, 200, 255 }, { 255, 255, 420, 255 }, { 0, 120, 100, 255 },
	{ 190, 240, 250, 255 },  { 20, 120, 30, 255 },   { -60, -130, 0, 255 },
	{ 220, 220, 240, 255 },  { 200, 230, 160, 255 }, { 0, 10, 20, 255 },
	{ 200, 210, 230, 255 },  { 200, 240, 240, 255 },
};

static const GXColorS10 sMareM_HatEColorBuf1[] = {
	{ 170, 210, 230, 255 }, { 230, -100, 320, 255 }, { -70, -40, 130, 255 },
	{ 60, 90, -70, 255 },   { 220, 230, 100, 255 },  { -10, -50, 50, 255 },
	{ 100, 60, 180, 255 },  { 200, 180, 30, 255 },   { 140, 270, 50, 255 },
	{ 190, 50, 30, 255 },   { 90, 110, 160, 255 },
};

static const TColorChangeInfo sMareM_HatEColorChange
    = { 0x00000002, "_mat1", sMareM_HatEColorBuf0, sMareM_HatEColorBuf1 };

static const TNpcModelData sMareM_HatEData = {
	"kubi",
	0,
	{ "mareMhat_E.bmd" },
	{
	    { &sMareM_HatEColorChange, 0 },
	},
	0,
	0,
	1,
};

static const GXColorS10 sMareM_ShellAColorBuf0[] = {
	{ 100, 60, 50, 255 },      { -60, 450, 250, 255 }, { 0, -200, -250, 255 },
	{ -210, -220, -420, 255 }, { -170, -200, 0, 255 }, { 140, 200, 255, 255 },
	{ 0, -110, 30, 255 },      { 190, 120, 20, 255 },  { 150, 110, -40, 255 },
	{ 200, 50, 40, 255 },
};

static const GXColorS10 sMareM_ShellAColorBuf1[] = {
	{ 500, 500, 350, 255 }, { 320, 70, 100, 255 },  { -10, -20, -350, 255 },
	{ 110, 100, 200, 255 }, { 180, 190, 170, 255 }, { -255, -240, -100, 255 },
	{ 230, 230, 230, 255 }, { 240, 255, 130, 255 }, { 255, 255, 255, 255 },
	{ 255, 255, 255, 255 },
};

static const TColorChangeInfo sMareM_ShellAColorChange
    = { 0x00000002, "_mat1", sMareM_ShellAColorBuf0, sMareM_ShellAColorBuf1 };

static const TNpcModelData sMareM_ShellAData = {
	"koshi",
	0,
	{ "mareMmakigai_A.bmd" },
	{
	    { &sMareM_ShellAColorChange, 0 },
	},
	1,
	1,
	1,
};

static const GXColorS10 sMareM_ShellBColorBuf0[] = {
	{ 110, 125, 200, 255 },  { -130, -120, -100, 255 },
	{ -120, 30, 20, 255 },   { -200, -200, -250, 255 },
	{ -160, -200, 10, 255 }, { -200, -230, -160, 255 },
	{ 40, 40, 40, 255 },     { 40, 20, 0, 255 },
	{ 40, 30, -160, 255 },   { 40, 30, 20, 255 },
};

static const GXColorS10 sMareM_ShellBColorBuf1[] = {
	{ 25, 20, 20, 255 },     { 300, 280, 10, 255 }, { 30, 0, 10, 255 },
	{ -190, -100, 20, 255 }, { -60, -70, 10, 255 }, { -100, -110, 200, 255 },
	{ 30, -150, -60, 255 },  { 140, 70, 10, 255 },  { 90, 80, 150, 255 },
	{ 170, 150, 120, 255 },
};

static const TColorChangeInfo sMareM_ShellBColorChange
    = { 0x00000002, "_mat1", sMareM_ShellBColorBuf0, sMareM_ShellBColorBuf1 };

static const TNpcModelData sMareM_ShellBData = {
	"koshi",
	0,
	{ "mareMmakigai_B.bmd" },
	{
	    { &sMareM_ShellBColorChange, 0 },
	},
	1,
	0,
	1,
};

static const GXColorS10 sMareM_BivalveBColorBuf0[] = {
	{ 330, -150, 200, 255 }, { 255, 255, 420, 255 }, { 0, 120, 100, 255 },
	{ 100, 60, 180, 255 },   { 200, 230, 160, 255 }, { 0, 10, 20, 255 },
};

static const GXColorS10 sMareM_BivalveBColorBuf1[] = {
	{ 170, 210, 230, 255 }, { 230, -100, 320, 255 }, { -70, -40, 130, 255 },
	{ 220, 220, 240, 255 }, { 200, 180, 30, 255 },   { 140, 270, 50, 255 },
};

static const TColorChangeInfo sMareM_BivalveBColorChange
    = { 0x00000002, "_mat1", sMareM_BivalveBColorBuf0,
	    sMareM_BivalveBColorBuf1 };

static const TNpcModelData sMareM_BivalveBData = {
	"koshi",
	0,
	{ "mareMbivalve_B.bmd" },
	{
	    { &sMareM_BivalveBColorChange, 0 },
	},
	1,
	0,
	1,
};

static const GXColorS10 sMareM_GlassColorBuf0[] = {
	{ -170, -200, 20, 255 },
	{ 40, 0, 10, 255 },
	{ 40, 70, 20, 255 },
};

static const GXColorS10 sMareM_GlassColorBuf1[] = {
	{ 190, 180, 220, 255 },
	{ 80, 120, 20, 255 },
	{ 255, 255, 255, 255 },
};

static const TColorChangeInfo sMareM_GlassColorChange
    = { 0x00000002, "_matgrass", sMareM_GlassColorBuf0, sMareM_GlassColorBuf1 };

static const TNpcModelData sMareM_GlassData = {
	"kubi",
	0,
	{ "mareM_grass.bmd" },
	{
	    { &sMareM_GlassColorChange, 0 },
	},
	2,
	0,
	0,
};

static const GXColorS10 sMareMB_ElderMat1ColorBuf0[] = {
	{ -210, 40, 100, 255 },
};

static const GXColorS10 sMareMB_ElderMat1ColorBuf1[] = {
	{ 100, 70, 200, 255 },
};

static const TColorChangeInfo sMareMB_ElderMat1ColorChange
    = { 0x00000002, "_mat1", sMareMB_ElderMat1ColorBuf0,
	    sMareMB_ElderMat1ColorBuf1 };

static const GXColorS10 sMareMB_ElderMat2ColorBuf0[] = {
	{ 170, 170, 230, 255 },
};

static const GXColorS10 sMareMB_ElderMat2ColorBuf1[] = {
	{ 255, 255, 255, 255 },
};

static const TColorChangeInfo sMareMB_ElderMat2ColorChange
    = { 0x00000002, "_mat2", sMareMB_ElderMat2ColorBuf0,
	    sMareMB_ElderMat2ColorBuf1 };

static const GXColorS10 sMareMB_ElderMat3ColorBuf0[] = {
	{ 10, 20, 30, 255 },
};

static const GXColorS10 sMareMB_ElderMat3ColorBuf1[] = {
	{ 240, 100, 20, 255 },
};

static const TColorChangeInfo sMareMB_ElderMat3ColorChange
    = { 0x00000002, "_mat3", sMareMB_ElderMat3ColorBuf0,
	    sMareMB_ElderMat3ColorBuf1 };

static const TNpcModelData sMareMB_ElderData = {
	"kubi",
	0,
	{ "mareMB_set.bmd" },
	{
	    { &sMareMB_ElderMat1ColorChange, 0 },
	    { &sMareMB_ElderMat2ColorChange, 0 },
	    { &sMareMB_ElderMat3ColorChange, 0 },
	},
	2,
	0,
	1,
};

static const GXColorS10 sMareMB_FishingRodMat1ColorBuf0[] = {
	{ 60, -200, -200, 255 },
};

static const GXColorS10 sMareMB_FishingRodMat1ColorBuf1[] = {
	{ 255, 255, 255, 255 },
};

static const TColorChangeInfo sMareMB_FishingRodMat1ColorChange
    = { 0x00000002, "_mat1", sMareMB_FishingRodMat1ColorBuf0,
	    sMareMB_FishingRodMat1ColorBuf1 };

static const GXColorS10 sMareMB_FishingRodMat2ColorBuf0[] = {
	{ 250, 180, 30, 255 },
};

static const GXColorS10 sMareMB_FishingRodMat2ColorBuf1[] = {
	{ 255, 255, 255, 255 },
};

static const TColorChangeInfo sMareMB_FishingRodMat2ColorChange
    = { 0x00000002, "_mat2", sMareMB_FishingRodMat2ColorBuf0,
	    sMareMB_FishingRodMat2ColorBuf1 };

static const GXColorS10 sMareMB_FishingRodMat3ColorBuf0[] = {
	{ 70, 40, 0, 255 },
};

static const GXColorS10 sMareMB_FishingRodMat3ColorBuf1[] = {
	{ 255, 255, 130, 255 },
};

static const TColorChangeInfo sMareMB_FishingRodMat3ColorChange
    = { 0x00000002, "_mat3", sMareMB_FishingRodMat3ColorBuf0,
	    sMareMB_FishingRodMat3ColorBuf1 };

static TNpcModelData sMareMB_FishingRodData = {
	cNpcPartsNameRootJoint,
	0,
	{ "mareMBTurizao.bmd" },
	{
	    { &sMareMB_FishingRodMat1ColorChange, 0 },
	    { &sMareMB_FishingRodMat2ColorChange, 0 },
	    { &sMareMB_FishingRodMat3ColorChange, 0 },
	},
	0,
	0,
	1,
};

static const GXColorS10 sMareMC_HatFColorBuf0[] = {
	{ -170, -200, 0, 255 },
};

static const GXColorS10 sMareMC_HatFColorBuf1[] = {
	{ 120, 120, 80, 255 },
};

static const TColorChangeInfo sMareMC_HatFColorChange
    = { 0x00000002, "_mat1", sMareMC_HatFColorBuf0, sMareMC_HatFColorBuf1 };

static const TNpcModelData sMareMC_HatFData = {
	"kubi",
	0,
	{ "mareMChat_F.bmd" },
	{
	    { &sMareMC_HatFColorChange, 0 },
	},
	0,
	0,
	1,
};

static const GXColorS10 sMareMC_BeardColorBuf0[] = {
	{ -120, -110, -160, 255 },
};

static const GXColorS10 sMareMC_BeardColorBuf1[] = {
	{ 180, -70, 380, 255 },
};

static const TColorChangeInfo sMareMC_BeardColorChange
    = { 0x00000002, "_mat1", sMareMC_BeardColorBuf0, sMareMC_BeardColorBuf1 };

static const TNpcModelData sMareMC_BeardData = {
	"kubi",
	0,
	{ "mareMCagohige.bmd" },
	{
	    { &sMareMC_BeardColorChange, 0 },
	},
	0,
	0,
	1,
};

static const GXColorS10 sMareMC_MustacheColorBuf0[] = {
	{ -120, -110, -160, 255 },
};

static const GXColorS10 sMareMC_MustacheColorBuf1[] = {
	{ 180, -70, 380, 255 },
};

static const TColorChangeInfo sMareMC_MustacheColorChange
    = { 0x00000002, "_mat1", sMareMC_MustacheColorBuf0,
	    sMareMC_MustacheColorBuf1 };

static const TNpcModelData sMareMC_MustacheData = {
	"kubi",
	0,
	{ "mareMCkuchihige.bmd" },
	{
	    { &sMareMC_MustacheColorChange, 0 },
	},
	0,
	0,
	1,
};

static const GXColorS10 sMareMD_PipeColorBuf0[] = {
	{ 0, -100, 50, 255 },
};

static const GXColorS10 sMareMD_PipeColorBuf1[] = {
	{ 400, 250, 150, 255 },
};

static const TColorChangeInfo sMareMD_PipeColorChange
    = { 0x00000002, "_mat1", sMareMD_PipeColorBuf0, sMareMD_PipeColorBuf1 };

static const TNpcModelData sMareMD_PipeData = {
	"migite",
	0,
	{ "mareMDhoragai_A.bmd" },
	{
	    { &sMareMD_PipeColorChange, 0 },
	},
	2,
	0,
	1,
};

static const GXColorS10 sMareW_PearlAColorBuf0[] = {
	{ -20, -10, -100, 255 }, { -30, -20, 0, 255 },   { 40, 20, 70, 255 },
	{ 20, 110, 0, 255 },     { -10, -10, 240, 255 }, { 160, 150, 170, 255 },
};

static const GXColorS10 sMareW_PearlAColorBuf1[] = {
	{ 220, 255, 255, 255 }, { 220, 240, 180, 255 }, { 100, 70, 165, 255 },
	{ 170, -10, 150, 255 }, { 150, 150, 240, 255 }, { 250, 255, 230, 255 },
};

static const TColorChangeInfo sMareW_PearlAColorChange
    = { 0x00000002, "_mat1", sMareW_PearlAColorBuf0, sMareW_PearlAColorBuf1 };

static const TNpcModelData sMareW_PearlAData = {
	"kubi",
	0,
	{ "mareWpearl_A.bmd" },
	{
	    { &sMareW_PearlAColorChange, 0 },
	},
	0,
	1,
	1,
};

static const GXColorS10 sMareW_HatAColorBuf0[] = {
	{ 10, 0, 120, 255 },   { 260, 60, 20, 255 },  { -170, -200, -250, 255 },
	{ 70, 280, 100, 255 }, { 210, 200, 70, 255 }, { 230, 100, 170, 255 },
};

static const GXColorS10 sMareW_HatAColorBuf1[] = {
	{ 120, 150, -50, 255 }, { -35, -50, -150, 255 }, { 240, 240, 190, 255 },
	{ 140, 80, 220, 255 },  { 140, 100, 50, 255 },   { 255, 255, 255, 255 },
};

static const TColorChangeInfo sMareW_HatAColorChange
    = { 0x00000002, "_mat1", sMareW_HatAColorBuf0, sMareW_HatAColorBuf1 };

static const TNpcModelData sMareW_HatAData = {
	"kubi",
	0,
	{ "mareWhatW_A.bmd" },
	{
	    { &sMareW_HatAColorChange, 0 },
	},
	0,
	0,
	1,
};

static const GXColorS10 sMareW_HatDColorBuf0[] = {
	{ 100, 0, 30, 255 },    { 30, 30, 120, 255 },    { -60, -100, -40, 255 },
	{ -10, 140, 60, 255 },  { -200, 40, -100, 255 }, { 255, 255, 255, 255 },
	{ 220, 240, -20, 255 }, { 250, 150, 210, 255 },  { 220, 240, -20, 255 },
	{ 255, 255, 150, 255 }, { 230, 250, 230, 255 },
};

static const GXColorS10 sMareW_HatDColorBuf1[] = {
	{ -50, 20, 60, 255 },  { 40, 120, 200, 255 },   { 80, 30, 140, 255 },
	{ 220, 10, 10, 255 },  { 0, 10, 10, 255 },      { 40, 160, 80, 255 },
	{ 200, 210, 50, 255 }, { 255, 255, 255, 255 },  { 170, 150, 210, 255 },
	{ -10, -100, 0, 255 }, { -200, -60, -20, 255 },
};

static const TColorChangeInfo sMareW_HatDColorChange
    = { 0x00000002, "_mat1", sMareW_HatDColorBuf0, sMareW_HatDColorBuf1 };

static const TNpcModelData sMareW_HatDData = {
	"kubi",
	0,
	{ "mareWhatW_D.bmd" },
	{
	    { &sMareW_HatDColorChange, 0 },
	},
	0,
	0,
	1,
};

static const GXColorS10 sMareW_ShellAColorBuf0[] = {
	{ 160, 20, 40, 255 },  { 0, 100, 170, 255 },   { 80, 20, -70, 255 },
	{ 20, 120, 260, 255 }, { 200, 210, -30, 255 }, { 220, 210, 160, 255 },
};

static const GXColorS10 sMareW_ShellAColorBuf1[] = {
	{ 255, 200, 255, 255 }, { 255, 255, 255, 255 }, { 165, 145, 185, 255 },
	{ 240, 250, 240, 255 }, { 80, 80, 160, 255 },   { 360, 340, 100, 255 },
};

static const TColorChangeInfo sMareW_ShellAColorChange
    = { 0x00000002, "_mat1", sMareW_ShellAColorBuf0, sMareW_ShellAColorBuf1 };

static const TNpcModelData sMareW_ShellAData = {
	"koshi",
	0,
	{ "mareWkai_A.bmd" },
	{
	    { &sMareW_ShellAColorChange, 0 },
	},
	1,
	1,
	1,
};

static const GXColorS10 sMareW_ShellBColorBuf0[] = {
	{ -100, -50, -10, 255 }, { -50, -40, -120, 255 }, { -180, -210, 10, 255 },
	{ 130, 90, -40, 255 },   { 230, 250, 130, 255 },  { 200, 60, 120, 255 },
	{ 50, 30, 20, 255 },     { 210, 90, 30, 255 },    { 0, 125, 50, 255 },
};

static const GXColorS10 sMareW_ShellBColorBuf1[] = {
	{ 10, 20, 130, 255 },   { 0, 70, 0, 255 },    { 240, 240, 190, 255 },
	{ -230, 0, 40, 255 },   { 160, 100, 0, 255 }, { 255, 255, 255, 255 },
	{ 190, -70, 200, 255 }, { -10, -65, 0, 255 }, { 160, 240, 130, 255 },
};

static const TColorChangeInfo sMareW_ShellBColorChange
    = { 0x00000002, "_mat1", sMareW_ShellBColorBuf0, sMareW_ShellBColorBuf1 };

static const TNpcModelData sMareW_ShellBData = {
	"koshi",
	0,
	{ "mareWkai_B.bmd" },
	{
	    { &sMareW_ShellBColorChange, 0 },
	},
	1,
	0,
	1,
};

static const GXColorS10 sMareW_BivalveAColorBuf0[] = {
	{ 100, 0, 30, 255 },   { 30, 30, 120, 255 },    { -60, -100, -40, 255 },
	{ -10, 140, 60, 255 }, { -200, 40, -100, 255 }, { 255, 255, 255, 255 },
};

static const GXColorS10 sMareW_BivalveAColorBuf1[] = {
	{ -50, 20, 60, 255 }, { 40, 120, 200, 255 }, { 80, 30, 140, 255 },
	{ 220, 10, 10, 255 }, { 0, 10, 10, 255 },    { 40, 160, 80, 255 },
};

static const TColorChangeInfo sMareW_BivalveAColorChange
    = { 0x00000002, "_mat1", sMareW_BivalveAColorBuf0,
	    sMareW_BivalveAColorBuf1 };

static const TNpcModelData sMareW_BivalveAData = {
	"koshi",
	0,
	{ "mareWbivalveW_A.bmd" },
	{
	    { &sMareW_BivalveAColorChange, 0 },
	},
	1,
	0,
	1,
};

static const GXColorS10 sMareWB_BabyBodyColorBuf0[] = {
	{ 440, 330, 255, 255 },
};

static const TColorChangeInfo sMareWB_BabyBodyColorChange
    = { 0x00000001, "_body", sMareWB_BabyBodyColorBuf0, nullptr };

static const GXColorS10 sMareWB_BabyHatColorBuf0[] = {
	{ 70, 0, 0, 255 },
};

static const GXColorS10 sMareWB_BabyHatColorBuf1[] = {
	{ 190, 60, 30, 255 },
};

static const TColorChangeInfo sMareWB_BabyHatColorChange
    = { 0x00000002, "_baby_hat", sMareWB_BabyHatColorBuf0,
	    sMareWB_BabyHatColorBuf1 };

static const GXColorS10 sMareWB_BabyBedColorBuf0[] = {
	{ 70, 10, 10, 255 },
};

static const GXColorS10 sMareWB_BabyBedColorBuf1[] = {
	{ 200, 180, 10, 255 },
};

static const TColorChangeInfo sMareWB_BabyBedColorChange
    = { 0x00000002, "_baby_bed", sMareWB_BabyBedColorBuf0,
	    sMareWB_BabyBedColorBuf1 };

static TNpcModelData sMareWB_BabyData = {
	cNpcPartsNameRootJoint,
	0,
	{ "mareWBbaby.bmd" },
	{
	    { &sMareWB_BabyBodyColorChange, 0 },
	    { &sMareWB_BabyHatColorChange, 0 },
	    { &sMareWB_BabyBedColorChange, 0 },
	},
	2,
	0,
	0,
};

static const TNpcModelData sKinopio_SunGlassesData = {
	"kubi",
	0,
	{ "kinopio_sunmegane.bmd" },
	{
	    { nullptr, 0 },
	},
	0,
	0,
	0,
};

static const TNpcModelData sKinojii_StickData = {
	"jnt_rfinger_1",
	0,
	{ "kinoji_stick.bmd" },
	{
	    { nullptr, 0 },
	},
	0,
	0,
	0,
};

static const TNpcModelData sPeach_HairNormalData = {
	"kubi",
	0,
	{ "peach_hair_normal.bmd" },
	{
	    { nullptr, 0 },
	},
	0,
	0,
	0,
};

static const TNpcModelData sPeach_Hand1LData = {
	"jnt_hand_L",
	0,
	{ "peach_hand1_l.bmd" },
	{
	    { nullptr, 0 },
	},
	0,
	0,
	0,
};

static const TNpcModelData sPeach_Hand1RData = {
	"jnt_hand_R",
	0,
	{ "peach_hand1_r.bmd" },
	{
	    { nullptr, 0 },
	},
	0,
	0,
	0,
};

static const TNpcModelData sPeach_HairPonytailData = {
	"kubi",
	0,
	{ "peach_hair_ponytail.bmd" },
	{
	    { nullptr, 0 },
	},
	0,
	0,
	0,
};

static const TNpcModelData sPeach_ParasolData = {
	"jnt_hand_R",
	0,
	{ "peach_parasol.bmd" },
	{
	    { nullptr, 0 },
	},
	0,
	0,
	0,
};

static const TNpcModelData sPeach_Hand2LData = {
	"jnt_hand_L",
	0,
	{ "peach_hand2_l.bmd" },
	{
	    { nullptr, 0 },
	},
	0,
	0,
	0,
};

static const TNpcModelData sPeach_Hand2RData = {
	"jnt_hand_R",
	0,
	{ "peach_hand2_r.bmd" },
	{
	    { nullptr, 0 },
	},
	0,
	0,
	0,
};

static const GXColorS10 sRaccoonDog_TubeColorBuf0[] = {
	{ 155, 0, 0, 255 },
	{ 10, 30, 150, 255 },
};

static const GXColorS10 sRaccoonDog_TubeColorBuf1[] = {
	{ 255, 255, 255, 255 },
	{ 240, 220, 200, 255 },
};

static const TColorChangeInfo sRaccoonDog_TubeColorChange
    = { 0x00000002, "_ukiwa", sRaccoonDog_TubeColorBuf0,
	    sRaccoonDog_TubeColorBuf1 };

static const TNpcModelData sRaccoonDog_TubeData = {
	"ukiwa_null",
	0,
	{ "ukiwa.bmd" },
	{
	    { &sRaccoonDog_TubeColorChange, 0 },
	},
	0,
	0,
	0,
};

static const GXColorS10 sMonteM_BodyColorBuf[] = {
	{ 100, 255, 300, 255 }, { 120, 120, 300, 255 }, { 350, 300, 0, 255 },
	{ 200, 70, 0, 255 },    { 300, 130, 255, 255 }, { 255, 350, 0, 255 },
	{ 400, 255, 255, 255 }, { 320, 140, 0, 255 },   { 200, 255, 400, 255 },
	{ 400, 250, 100, 255 },
};

static const TColorChangeInfo sMonteM_BodyColor
    = { 0x00000001, "_hand_mat", sMonteM_BodyColorBuf, nullptr };

static const GXColorS10 sMonteMA_ClothColorBuf0[] = {
	{ 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 255, 255, 255, 255 },
	{ 200, 200, 170, 255 }, { 50, 50, 50, 255 },    { 150, 200, 255, 255 },
	{ 0, 70, 150, 255 },    { 400, 300, 200, 255 }, { 255, 255, 255, 255 },
	{ 255, 255, 255, 255 }, { 255, 255, 150, 255 },
};

static const GXColorS10 sMonteMA_ClothColorBuf1[] = {
	{ 250, 130, 50, 255 },  { 50, 130, 100, 255 },  { 150, 180, 20, 255 },
	{ 200, 200, 170, 255 }, { 50, 50, 50, 255 },    { 150, 200, 255, 255 },
	{ 0, 70, 150, 255 },    { 230, 150, 100, 255 }, { 60, 150, 230, 255 },
	{ 180, 150, 200, 255 }, { 100, 220, 300, 255 },
};

static const TColorChangeInfo sMonteMA_ClothColor
    = { 0x00000002, "_fuku_mat", sMonteMA_ClothColorBuf0,
	    sMonteMA_ClothColorBuf1 };

static const GXColorS10 sMonteMB_BodyColorBuf[] = {
	{ 160, 200, 300, 255 },
	{ 255, 160, 150, 255 },
	{ 300, 200, 80, 255 },
	{ 200, 300, 100, 255 },
};

static const TColorChangeInfo sMonteMB_BodyColor
    = { 0x00000001, "_hand_mat", sMonteMB_BodyColorBuf, nullptr };

static const GXColorS10 sMonteMB_ClothColorBuf[] = {
	{ 70, 130, 200, 255 }, { 200, 20, 20, 255 },  { 130, 30, 80, 255 },
	{ 130, 200, 80, 255 }, { 230, 200, 80, 255 }, { 50, 100, 150, 255 },
};

static const TColorChangeInfo sMonteMB_ClothColor
    = { 0x00000001, "_fuku_mat", sMonteMB_ClothColorBuf, nullptr };

static const GXColorS10 sMonteMC_ClothColorBuf0[] = {
	{ 230, 230, 210, 255 }, { 150, 70, 0, 255 },   { 230, 230, 210, 255 },
	{ 0, 70, 150, 255 },    { 50, 150, 130, 255 }, { 60, 40, 0, 255 },
	{ 0, 100, 100, 255 },   { 0, 150, 200, 255 },  { 0, 50, 100, 255 },
	{ 100, 100, 0, 255 },   { 100, 0, 0, 255 },
};

static const GXColorS10 sMonteMC_ClothColorBuf1[] = {
	{ 230, 230, 210, 255 }, { 150, 70, 0, 255 },    { 0, 70, 150, 255 },
	{ 230, 230, 210, 255 }, { 230, 230, 210, 255 }, { 160, 150, 60, 255 },
	{ 0, 100, 100, 255 },   { 0, 150, 200, 255 },   { 0, 50, 100, 255 },
	{ 0, 0, 0, 255 },       { 0, 0, 0, 255 },
};

static const TColorChangeInfo sMonteMC_ClothColor
    = { 0x00000002, "_fuku_mat", sMonteMC_ClothColorBuf0,
	    sMonteMC_ClothColorBuf1 };

static const GXColorS10 sMonteMD_ClothColorBuf[] = {
	{ 350, 360, 340, 255 }, { 50, 100, 0, 255 },  { 150, 0, 0, 255 },
	{ 0, 300, 350, 255 },   { 0, 100, 250, 255 },
};

static const TColorChangeInfo sMonteMD_ClothColor
    = { 0x00000001, "_fuku_mat", sMonteMD_ClothColorBuf, nullptr };

static const GXColorS10 sMonteW_ColorBuf[] = {
	{ 300, 100, 200, 255 }, { 400, 150, 0, 255 }, { 300, 330, 0, 255 },
	{ 400, 330, 0, 255 },   { 330, 40, 0, 255 },  { 400, 200, 255, 255 },
};

static const TColorChangeInfo sMonteW_BodyColor
    = { 0x00000001, "_hand_mat", sMonteW_ColorBuf, nullptr };

static const GXColorS10 sMonteWA_ClothColorBuf[] = {
	{ 380, 330, 150, 255 }, { 300, 100, 200, 255 }, { 360, 350, 300, 255 },
	{ 300, 50, 0, 255 },    { 400, 150, 100, 255 }, { 120, 150, 300, 255 },
};

static const TColorChangeInfo sMonteWA_ClothColor
    = { 0x00000001, "_fuku_mat", sMonteWA_ClothColorBuf, nullptr };

static const GXColorS10 sMonteWB_ClothColorBuf0[] = {
	{ 220, 200, 220, 255 }, { 200, 220, 220, 255 }, { 255, 255, 255, 255 },
	{ 255, 255, 255, 255 }, { 220, 230, 220, 255 }, { 180, 100, 110, 255 },
	{ 200, 100, 0, 255 },   { 0, 100, 150, 255 },   { 255, 200, 100, 255 },
};

static const GXColorS10 sMonteWB_ClothColorBuf1[] = {
	{ 100, 80, 200, 255 },  { 100, 170, 200, 255 }, { 150, 0, 60, 255 },
	{ 180, 120, 200, 255 }, { 140, 180, 300, 255 }, { 180, 100, 110, 255 },
	{ 200, 100, 0, 255 },   { 0, 100, 150, 255 },   { 255, 200, 100, 255 },
};

static const TColorChangeInfo sMonteWB_ClothColor
    = { 0x00000002, "_fuku_mat", sMonteWB_ClothColorBuf0,
	    sMonteWB_ClothColorBuf1 };

static const GXColorS10 sMareM_ColorBuf[] = {
	{ 370, 290, 170, 255 }, { 350, 340, 120, 255 }, { 260, 180, 100, 255 },
	{ 300, 150, 130, 255 }, { 300, 200, 60, 255 },  { 240, 160, 230, 255 },
};

static const TColorChangeInfo sMareM_BodyColor
    = { 0x00000001, "_body", sMareM_ColorBuf, nullptr };

static const GXColorS10 sMareW_ColorBuf[] = {
	{ 410, 255, 280, 255 }, { 430, 330, 150, 255 }, { 440, 330, 255, 255 },
	{ 300, 200, 130, 255 }, { 300, 190, 220, 255 }, { 400, 250, 90, 255 },
};

static const TColorChangeInfo sMareW_BodyColor
    = { 0x00000001, "_body", sMareW_ColorBuf, nullptr };

static const GXColorS10 sKinopio_BodyColorData0[] = {
	{ 30, 30, 200, 255 },  { 230, 0, 0, 255 },   { 30, 120, 30, 255 },
	{ 250, 220, 30, 255 }, { 150, 0, 200, 255 },
};

static const GXColorS10 sKinopio_BodyColorData1[] = {
	{ 230, 230, 230, 255 }, { 230, 230, 230, 255 }, { 230, 230, 230, 255 },
	{ 230, 230, 230, 255 }, { 230, 230, 230, 255 },
};

static const TColorChangeInfo sKinopio_BodyColor
    = { 0x00000002, "_mat_cap", sKinopio_BodyColorData0,
	    sKinopio_BodyColorData1 };

static const GXColorS10 sKinopio_ClothColorData0[] = {
	{ 250, 150, 0, 255 }, { 250, 150, 0, 255 }, { 250, 150, 0, 255 },
	{ 250, 150, 0, 255 }, { 250, 150, 0, 255 },
};

static const GXColorS10 sKinopio_ClothColorData1[] = {
	{ 30, 30, 200, 255 },  { 230, 0, 0, 255 },   { 30, 120, 30, 255 },
	{ 250, 220, 30, 255 }, { 150, 0, 200, 255 },
};

static const TColorChangeInfo sKinopio_ClothColor
    = { 0x00000002, "_mat_cloth", sKinopio_ClothColorData0,
	    sKinopio_ClothColorData1 };

static const GXColorS10 sRaccoonDog_ClothColorBuf0[] = {
	{ 0, 0, 0, 255 },
	{ 270, 280, 275, 255 },
};

static const GXColorS10 sRaccoonDog_ClothColorBuf1[] = {
	{ 255, 255, 255, 255 },
	{ -20, 100, -50, 255 },
};

static const TColorChangeInfo sRaccoonDog_ClothColor
    = { 0x00000002, "_pants", sRaccoonDog_ClothColorBuf0,
	    sRaccoonDog_ClothColorBuf1 };

static const TNpcInitInfo sMonteM_InitData = {
	nullptr,
	{ &sMonteM_HatAData, &sMonteM_MustacheAData, &sMonteM_GlassesAData,
	  &sMonteM_GlassesBData, &sMonteM_HatBData, &sMonteM_HatEData,
	  &sMonteM_HatDData, &sMonteM_HatFData, &sMonteM_HatGData,
	  &sMonteM_LapelAData, &sMonteM_TieBData, &sMonteM_LuggageData },
	{ { &sMonteM_BodyColor } },
	100.000f,
	200.000f,
	100.000f,
	200.000f,
};

static const TNpcInitInfo sMonteMA_InitData = {
	nullptr,
	{ &sMonteM_HatAData, &sMonteM_MustacheAData, &sMonteM_GlassesAData,
	  &sMonteM_GlassesBData, &sMonteM_HatBData, &sMonteM_HatEData,
	  &sMonteM_HatDData, &sMonteM_HatFData, &sMonteM_HatGData,
	  &sMonteM_LapelAData, &sMonteM_TieBData, &sMonteM_LuggageData },
	{ { &sMonteM_BodyColor }, { &sMonteMA_ClothColor } },
	100.000f,
	200.000f,
	100.000f,
	200.000f,
};

static const TNpcInitInfo sMonteMB_InitData = {
	nullptr,
	{ &sMonteM_HatAData, &sMonteM_MustacheAData, &sMonteM_GlassesAData,
	  &sMonteM_GlassesBData, &sMonteM_HatBData, &sMonteM_HatEData,
	  &sMonteM_HatDData, &sMonteM_HatFData, &sMonteM_HatGData,
	  &sMonteM_LapelAData, &sMonteM_TieBData, &sMonteM_LuggageData },
	{ { &sMonteMB_BodyColor }, { &sMonteMB_ClothColor } },
	100.000f,
	200.000f,
	100.000f,
	200.000f,
};

static const TNpcInitInfo sMonteMC_InitData = {
	nullptr,
	{ &sMonteM_HatAData, &sMonteM_MustacheAData, &sMonteM_GlassesAData,
	  &sMonteM_GlassesBData, &sMonteM_HatBData, &sMonteM_HatEData,
	  &sMonteM_HatDData, &sMonteM_HatFData, &sMonteM_HatGData,
	  &sMonteM_LapelAData, &sMonteM_TieBData, &sMonteM_LuggageData },
	{ { &sMonteM_BodyColor }, { &sMonteMC_ClothColor } },
	100.000f,
	200.000f,
	100.000f,
	200.000f,
};

static const TNpcInitInfo sMonteMD_InitData = {
	nullptr,
	{ &sMonteM_HatAData, &sMonteM_MustacheAData, &sMonteM_GlassesAData,
	  &sMonteM_GlassesBData, &sMonteM_HatBData, &sMonteM_HatEData,
	  &sMonteM_HatDData, &sMonteM_HatFData, &sMonteM_HatGData,
	  &sMonteM_LapelAData, &sMonteM_TieBData, &sMonteM_LuggageData },
	{ { &sMonteM_BodyColor }, { &sMonteMD_ClothColor } },
	100.000f,
	200.000f,
	100.000f,
	200.000f,
};

static const TNpcInitInfo sMonteME_InitData = {
	nullptr,
	{ &sMonteM_HatAData, &sMonteM_MustacheAData, &sMonteM_GlassesAData,
	  &sMonteM_GlassesBData, &sMonteM_HatBData, &sMonteM_HatEData,
	  &sMonteM_HatDData, &sMonteM_HatFData, &sMonteM_HatGData,
	  &sMonteM_LapelAData, &sMonteM_TieBData, &sMonteM_LuggageData },
	{},
	100.000f,
	200.000f,
	100.000f,
	200.000f,
};

static const TNpcInitInfo sMonteMF_InitData = {
	nullptr,
	{ &sMonteM_HatAData, &sMonteM_MustacheAData, &sMonteM_GlassesAData,
	  &sMonteM_GlassesBData, &sMonteM_HatBData, &sMonteM_HatEData,
	  &sMonteM_HatDData, &sMonteM_HatFData, &sMonteM_HatGData,
	  &sMonteM_LapelAData, &sMonteM_TieBData, &sMonteMF_TubeData },
	{ { &sMonteM_BodyColor } },
	150.000f,
	200.000f,
	150.000f,
	200.000f,
};

static const TNpcInitInfo sMonteMG_InitData = {
	nullptr,
	{ &sMonteM_HatAData, &sMonteM_MustacheAData, &sMonteM_GlassesAData,
	  &sMonteM_GlassesBData, &sMonteM_HatBData, &sMonteM_HatEData,
	  &sMonteM_HatDData, &sMonteM_HatFData, &sMonteM_HatGData,
	  &sMonteM_LapelAData, &sMonteM_TieBData, &sMonteMG_MopData },
	{ { &sMonteM_BodyColor }, { &sMonteMC_ClothColor } },
	100.000f,
	200.000f,
	100.000f,
	200.000f,
};

static const TNpcInitInfo sMonteMH_InitData = {
	nullptr,
	{ &sMonteM_HatAData, &sMonteM_MustacheAData, &sMonteM_GlassesAData,
	  &sMonteM_GlassesBData, &sMonteM_HatBData, &sMonteM_HatEData,
	  &sMonteM_HatDData, &sMonteM_HatFData, &sMonteM_HatGData,
	  &sMonteM_LapelAData, &sMonteM_TieBData, &sMonteMH_UkleleData },
	{ { &sMonteM_BodyColor }, { &sMonteMA_ClothColor } },
	100.000f,
	200.000f,
	100.000f,
	200.000f,
};

static const TNpcInitInfo sMonteW_InitData = {
	nullptr,
	{ &sMonteW_FlowerData, &sMonteW_HatAData, &sMonteW_GlassBData,
	  &sMonteW_ArrowRData, &sMonteW_ArrowLData },
	{ { &sMonteW_BodyColor } },
	110.000f,
	190.000f,
	110.000f,
	190.000f,
};

static const TNpcInitInfo sMonteWA_InitData = {
	nullptr,
	{ &sMonteW_FlowerData, &sMonteW_HatAData, &sMonteW_GlassBData,
	  &sMonteW_ArrowRData, &sMonteW_ArrowLData },
	{ { &sMonteW_BodyColor }, { &sMonteWA_ClothColor } },
	110.000f,
	190.000f,
	110.000f,
	190.000f,
};

static const TNpcInitInfo sMonteWB_InitData = {
	nullptr,
	{ &sMonteW_FlowerData, &sMonteW_HatAData, &sMonteW_GlassBData,
	  &sMonteW_ArrowRData, &sMonteW_ArrowLData },
	{ { &sMonteW_BodyColor }, { &sMonteWB_ClothColor } },
	110.000f,
	190.000f,
	110.000f,
	190.000f,
};

static const TNpcInitInfo sMonteWC_InitData = {
	nullptr,
	{ &sMonteW_FlowerData, &sMonteW_HatAData, &sMonteW_GlassBData,
	  &sMonteW_ArrowRData, &sMonteW_ArrowLData, &sMonteWC_HatCData,
	  &sMonteWC_BraceletRData, &sMonteWC_BraceletLData },
	{ { &sMonteW_BodyColor } },
	110.000f,
	190.000f,
	110.000f,
	190.000f,
};

static const TNpcInitInfo sMareM_InitData = {
	nullptr,
	{ &sMareM_HatAData, &sMareM_HatBData, nullptr, &sMareM_HatEData,
	  &sMareM_ShellAData, &sMareM_ShellBData, &sMareM_GlassData,
	  &sMareM_BivalveBData },
	{ { &sMareM_BodyColor } },
	80.000f,
	110.000f,
	80.000f,
	110.000f,
};

static const TNpcInitInfo sMareMB_InitData = {
	nullptr,
	{ &sMareM_HatAData, &sMareM_HatBData, nullptr, &sMareM_HatEData,
	  &sMareM_ShellAData, &sMareM_ShellBData, &sMareM_GlassData,
	  &sMareM_BivalveBData, &sMareMB_ElderData, &sMareMB_FishingRodData },
	{ { &sMareM_BodyColor } },
	80.000f,
	110.000f,
	80.000f,
	110.000f,
};

static const TNpcInitInfo sMareMC_InitData = {
	nullptr,
	{ &sMareM_HatAData, &sMareM_HatBData, nullptr, &sMareM_HatEData,
	  &sMareM_ShellAData, &sMareM_ShellBData, &sMareM_GlassData,
	  &sMareM_BivalveBData, &sMareMC_HatFData, &sMareMC_BeardData,
	  &sMareMC_MustacheData },
	{ { &sMareM_BodyColor } },
	80.000f,
	110.000f,
	80.000f,
	110.000f,
};

static const TNpcInitInfo sMareMD_InitData = {
	nullptr,
	{ &sMareM_HatAData, &sMareM_HatBData, nullptr, &sMareM_HatEData,
	  &sMareM_ShellAData, &sMareM_ShellBData, &sMareM_GlassData,
	  &sMareM_BivalveBData, &sMareMD_PipeData },
	{ { &sMareM_BodyColor } },
	80.000f,
	110.000f,
	80.000f,
	110.000f,
};

static const TNpcInitInfo sMareW_InitData = {
	nullptr,
	{ &sMareW_PearlAData, &sMareW_HatAData, nullptr, nullptr,
	  &sMareW_ShellAData, &sMareW_ShellBData, nullptr, nullptr,
	  &sMareW_BivalveAData, &sMareW_HatDData },
	{ { &sMareW_BodyColor } },
	80.000f,
	130.000f,
	80.000f,
	130.000f,
};

static const TNpcInitInfo sMareWB_InitData = {
	nullptr,
	{ &sMareW_PearlAData, &sMareW_HatAData, nullptr, nullptr,
	  &sMareW_ShellAData, &sMareW_ShellBData, nullptr, nullptr,
	  &sMareW_BivalveAData, &sMareW_HatDData, &sMareWB_BabyData },
	{ { &sMareW_BodyColor } },
	80.000f,
	130.000f,
	80.000f,
	130.000f,
};

static const TNpcInitInfo sKinopio_InitData = {
	nullptr,
	{ &sKinopio_SunGlassesData },
	{ { &sKinopio_BodyColor }, { &sKinopio_ClothColor } },
	40.000f,
	110.000f,
	40.000f,
	110.000f,
};

static const TNpcInitInfo sKinojii_InitData = {
	nullptr, { &sKinojii_StickData }, {}, 40.000f, 110.000f, 40.000f, 110.000f,
};

static const TNpcInitInfo sPeach_InitData = {
	&sPeach_TakenMiddle,
	{ &sPeach_HairNormalData, &sPeach_Hand1LData, &sPeach_Hand1RData,
	  &sPeach_HairPonytailData, &sPeach_ParasolData, &sPeach_Hand2LData,
	  &sPeach_Hand2RData },
	{},
	50.000f,
	170.000f,
	50.000f,
	170.000f,
};

static const TNpcInitInfo sRaccoonDog_InitData = {
	nullptr,
	{ &sRaccoonDog_TubeData },
	{ {}, { &sRaccoonDog_ClothColor } },
	100.000f,
	180.000f,
	100.000f,
	180.000f,
};

static const TNpcInitInfo sSunflowerL_InitData = {
	nullptr, {}, {}, 200.000f, 750.000f, 200.000f, 750.000f,
};

static const TNpcInitInfo sSunflowerS_InitData = {
	nullptr, {}, {}, 80.000f, 300.000f, 80.000f, 300.000f,
};

static const TNpcInitInfo sBoardNpc_InitData = {
	nullptr, {}, {}, 80.000f, 160.000f, 80.000f, 160.000f,
};

const TNpcInitInfo* SMSGetNpcInitData(u32 idx)
{
	static const TNpcInitInfo* sAllNpcInitData[] = {
		&sMonteM_InitData,     &sMonteMA_InitData,
		&sMonteMB_InitData,    &sMonteMC_InitData,
		&sMonteMD_InitData,    &sMonteME_InitData,
		&sMonteMF_InitData,    &sMonteMG_InitData,
		&sMonteMH_InitData,    &sMonteW_InitData,
		&sMonteWA_InitData,    &sMonteWB_InitData,
		&sMonteWC_InitData,    &sMareM_InitData,
		&sMareM_InitData,      &sMareMB_InitData,
		&sMareMC_InitData,     &sMareMD_InitData,
		&sMareW_InitData,      &sMareW_InitData,
		&sMareWB_InitData,     &sKinopio_InitData,
		&sKinojii_InitData,    &sPeach_InitData,
		&sRaccoonDog_InitData, &sSunflowerL_InitData,
		&sSunflowerS_InitData, nullptr,
		&sBoardNpc_InitData,
	};
	return sAllNpcInitData[idx];
}
