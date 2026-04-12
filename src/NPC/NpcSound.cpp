#include <NPC/NpcBase.hpp>

// pch issues with J3DJoint and co
static Vec dummy0                    = { 1.0f, 1.0f, 1.0f };
static Vec dummy1                    = { 1.0f, 1.0f, 1.0f };
static u32 dummy2[]                  = { 0, 2, 1, 3 };
static const char dummy3[]           = "\0\0\0\0\0\0\0\0\0\0\0";
static const char dummy4[]           = "メモリが足りません\n";
static const char* MtxCalcTypeName[] = {
	"MActorMtxCalcType_Basic クラシックスケールＯＮ",
	"MActorMtxCalcType_Softimage クラシックスケールＯＦＦ",
	"MActorMtxCalcType_MotionBlend モーションブレンド",
	"MActorMtxCalcType_User ユーザー定義",
};
// end of issues

static const char* monteMCommon_bastable[] = {
	"/scene/monteMCommon/bas/mom_appear.bas",
	"/scene/monteMCommon/bas/mom_dance.bas",
	"/scene/monteMCommon/bas/mom_fall.bas",
	"/scene/monteMCommon/bas/mom_happy.bas",
	"/scene/monteMCommon/bas/mom_mad.bas",
	"/scene/monteMCommon/bas/mom_madloop.bas",
	"/scene/monteMCommon/bas/mom_recover.bas",
	"/scene/monteMCommon/bas/mom_run.bas",
	nullptr,
	nullptr,
	"/scene/monteMCommon/bas/mom_sitwet.bas",
	nullptr,
	nullptr,
	"/scene/monteMCommon/bas/mom_throw.bas",
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	"/scene/monteMCommon/bas/mom_wait_yogore.bas",
	"/scene/monteMCommon/bas/mom_walk.bas",
	"/scene/monteMCommon/bas/mom_wet.bas",
	"/scene/monteMCommon/bas/mom_wet_A.bas",
	"/scene/monteMCommon/bas/mom_wet_yogore.bas",
};

static const char* monteMCommon_bas_c_table[] = {
	"/scene/monteMCommon/bas/mom_appear_c.bas",
	"/scene/monteMCommon/bas/mom_dance_c.bas",
	"/scene/monteMCommon/bas/mom_fall_c.bas",
	"/scene/monteMCommon/bas/mom_happy_c.bas",
	"/scene/monteMCommon/bas/mom_mad_c.bas",
	"/scene/monteMCommon/bas/mom_madloop_c.bas",
	"/scene/monteMCommon/bas/mom_recover_c.bas",
	"/scene/monteMCommon/bas/mom_run_c.bas",
	nullptr,
	nullptr,
	"/scene/monteMCommon/bas/mom_sitwet_c.bas",
	nullptr,
	nullptr,
	"/scene/monteMCommon/bas/mom_throw_c.bas",
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	"/scene/monteMCommon/bas/mom_wait_yogore_c.bas",
	"/scene/monteMCommon/bas/mom_walk_c.bas",
	"/scene/monteMCommon/bas/mom_wet_c.bas",
	"/scene/monteMCommon/bas/mom_wet_A_c.bas",
	"/scene/monteMCommon/bas/mom_wet_yogore_c.bas",
};

static const char* monteME_bastable[] = {
	"/scene/monteME/bas/momE_down.bas", nullptr, nullptr,
	"/scene/monteME/bas/momE_up.bas",   nullptr, nullptr,
	"/scene/monteME/bas/momE_wet.bas",
};

static const char* monteME_bas_c_table[] = {
	"/scene/monteME/bas/momE_down.bas", nullptr, nullptr,
	"/scene/monteME/bas/momE_up.bas",   nullptr, nullptr,
	"/scene/monteME/bas/momE_wet.bas",
};

static const char* monteMF_bastable[] = {
	"/scene/monteMF/bas/momF_swim.bas",
	"/scene/monteMF/bas/momF_swimmad.bas",
	"/scene/monteMF/bas/momF_swimtalk.bas",
};

static const char* monteMF_bas_c_table[] = {
	"/scene/monteMF/bas/momF_swim.bas",
	"/scene/monteMF/bas/momF_swimmad.bas",
	"/scene/monteMF/bas/momF_swimtalk.bas",
};

static const char* monteMG_bastable[] = {
	"/scene/monteMG/bas/momG_wait_cleanup.bas",
	"/scene/monteMG/bas/momG_walk_cleanup.bas",
};

// Unused, but expected to be:
// static const char* monteMG_bas_c_table[] = {
// 	"/scene/monteMG/bas/momG_wait_cleanup.bas",
// 	"/scene/monteMG/bas/momG_walk_cleanup.bas",
// };

static const char* monteMH_bastable[] = {
	"/scene/monteMH/bas/momH_play.bas",
	nullptr,
	"/scene/monteMH/bas/momH_walk_play.bas",
	"/scene/monteMH/bas/momH_wet_play.bas",
};

static const char* monteMH_bas_c_table[] = {
	"/scene/monteMH/bas/momH_play.bas",
	nullptr,
	"/scene/monteMH/bas/momH_walk_play.bas",
	"/scene/monteMH/bas/momH_wet_play.bas",
};

static const char* monteWCommon_bastable[] = {
	"/scene/monteWCommon/bas/mow_appear.bas",
	"/scene/monteWCommon/bas/mow_dance.bas",
	"/scene/monteWCommon/bas/mow_fall.bas",
	"/scene/monteWCommon/bas/mow_happy.bas",
	"/scene/monteWCommon/bas/mow_mad.bas",
	"/scene/monteWCommon/bas/mow_madloop.bas",
	nullptr,
	"/scene/monteWCommon/bas/mow_run.bas",
	nullptr,
	nullptr,
	"/scene/monteWCommon/bas/mow_sitwet.bas",
	nullptr,
	nullptr,
	"/scene/monteWCommon/bas/mow_throw.bas",
	nullptr,
	nullptr,
	"/scene/monteWCommon/bas/mow_wait_arrow.bas",
	nullptr,
	nullptr,
	"/scene/monteWCommon/bas/mow_wait_yogore.bas",
	"/scene/monteWCommon/bas/mow_walk.bas",
	"/scene/monteWCommon/bas/mow_wet.bas",
	"/scene/monteWCommon/bas/mow_wet_A.bas",
	"/scene/monteWCommon/bas/mow_wet_yogore.bas",
};

static const char* monteWCommon_bas_c_table[] = {
	"/scene/monteWCommon/bas/mow_appear_c.bas",
	"/scene/monteWCommon/bas/mow_dance_c.bas",
	"/scene/monteWCommon/bas/mow_fall_c.bas",
	"/scene/monteWCommon/bas/mow_happy_c.bas",
	"/scene/monteWCommon/bas/mow_mad_c.bas",
	"/scene/monteWCommon/bas/mow_madloop_c.bas",
	nullptr,
	"/scene/monteWCommon/bas/mow_run_c.bas",
	nullptr,
	nullptr,
	"/scene/monteWCommon/bas/mow_sitwet_c.bas",
	nullptr,
	nullptr,
	"/scene/monteWCommon/bas/mow_throw_c.bas",
	nullptr,
	nullptr,
	"/scene/monteWCommon/bas/mow_wait_arrow_c.bas",
	nullptr,
	nullptr,
	"/scene/monteWCommon/bas/mow_wait_yogore_c.bas",
	"/scene/monteWCommon/bas/mow_walk_c.bas",
	"/scene/monteWCommon/bas/mow_wet_c.bas",
	"/scene/monteWCommon/bas/mow_wet_A_c.bas",
	"/scene/monteWCommon/bas/mow_wet_yogore_c.bas",
};

static const char* monteWC_bastable[] = {
	nullptr,
	"/scene/monteWC/bas/mowC_mad.bas",
	nullptr,
	"/scene/monteWC/bas/mowC_wet.bas",
	"/scene/monteWC/bas/mowC_wet_A.bas",
};

static const char* monteWC_bas_c_table[] = {
	nullptr,
	"/scene/monteWC/bas/mowC_mad.bas",
	nullptr,
	"/scene/monteWC/bas/mowC_wet.bas",
	"/scene/monteWC/bas/mowC_wet_A.bas",
};

static const char* mareM_bastable[] = {
	"/scene/mareM/bas/mareM_appear.bas",
	"/scene/mareM/bas/mareM_dance.bas",
	"/scene/mareM/bas/mareM_fall.bas",
	"/scene/mareM/bas/mareM_happy.bas",
	"/scene/mareM/bas/mareM_jump.bas",
	"/scene/mareM/bas/mareM_recover.bas",
	"/scene/mareM/bas/mareM_run.bas",
	nullptr,
	nullptr,
	"/scene/mareM/bas/mareM_stand.bas",
	"/scene/mareM/bas/mareM_stand_A.bas",
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	"/scene/mareM/bas/mareM_wait_yogore.bas",
	"/scene/mareM/bas/mareM_walk.bas",
	"/scene/mareM/bas/mareM_wash.bas",
	"/scene/mareM/bas/mareM_wet.bas",
	"/scene/mareM/bas/mareM_wet_A.bas",
};

static const char* mareM_bas_c_table[] = {
	"/scene/mareM/bas/mareM_appear_c.bas",
	"/scene/mareM/bas/mareM_dance_c.bas",
	"/scene/mareM/bas/mareM_fall_c.bas",
	"/scene/mareM/bas/mareM_happy_c.bas",
	"/scene/mareM/bas/mareM_jump_c.bas",
	"/scene/mareM/bas/mareM_recover_c.bas",
	"/scene/mareM/bas/mareM_run_c.bas",
	nullptr,
	nullptr,
	"/scene/mareM/bas/mareM_stand_c.bas",
	"/scene/mareM/bas/mareM_stand_A_c.bas",
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	"/scene/mareM/bas/mareM_wait_yogore.bas",
	"/scene/mareM/bas/mareM_walk_c.bas",
	"/scene/mareM/bas/mareM_wash_c.bas",
	"/scene/mareM/bas/mareM_wet_c.bas",
	"/scene/mareM/bas/mareM_wet_A_c.bas",
};

static const char* mareMA_bastable[] = {
	nullptr, nullptr, nullptr, nullptr, nullptr,
};

static const char* mareMA_bas_c_table[] = {
	nullptr, nullptr, nullptr, nullptr, nullptr,
};

static const char* mareMB_bastable[] = {
	nullptr, nullptr, nullptr,
	nullptr, nullptr, "/scene/mareMB/bas/mareMB_wet.bas",
};

static const char* mareMB_bas_c_table[] = {
	nullptr, nullptr, nullptr,
	nullptr, nullptr, "/scene/mareMB/bas/mareMB_wet.bas",
};

static const char* mareMC_bastable[] = {
	"/scene/mareMC/bas/mareMC_stand.bas",
	"/scene/mareMC/bas/mareMC_stand_A.bas",
	nullptr,
	"/scene/mareMC/bas/mareMC_wet.bas",
	"/scene/mareMC/bas/mareMC_wet_A.bas",
};

static const char* mareMC_bas_c_table[] = {
	"/scene/mareMC/bas/mareMC_stand.bas",
	"/scene/mareMC/bas/mareMC_stand_A.bas",
	nullptr,
	"/scene/mareMC/bas/mareMC_wet.bas",
	"/scene/mareMC/bas/mareMC_wet_A.bas",
};

static const char* mareMD_bastable[] = {
	"/scene/mareMD/bas/mareMD_hue.bas",
	"/scene/mareMD/bas/mareMD_wet.bas",
};

// Unused, but expected to be:
// static const char* mareMD_bas_c_table[] = {
//     "/scene/mareMD/bas/mareMD_hue.bas",
//     "/scene/mareMD/bas/mareMD_wet.bas",
// };

static const char* mareW_bastable[] = {
	"/scene/mareW/bas/mareW_appear.bas",
	"/scene/mareW/bas/mareW_dance.bas",
	"/scene/mareW/bas/mareW_fall.bas",
	"/scene/mareW/bas/mareW_happy.bas",
	"/scene/mareW/bas/mareW_recover.bas",
	"/scene/mareW/bas/mareW_run.bas",
	nullptr,
	nullptr,
	"/scene/mareW/bas/mareW_stand.bas",
	"/scene/mareW/bas/mareW_stand_A.bas",
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	"/scene/mareW/bas/mareW_wait_yogore.bas",
	"/scene/mareW/bas/mareW_walk.bas",
	"/scene/mareW/bas/mareW_wash.bas",
	"/scene/mareW/bas/mareW_wet.bas",
	"/scene/mareW/bas/mareW_wet_A.bas",
};

static const char* mareW_bas_c_table[] = {
	"/scene/mareW/bas/mareW_appear_c.bas",
	"/scene/mareW/bas/mareW_dance_c.bas",
	"/scene/mareW/bas/mareW_fall_c.bas",
	"/scene/mareW/bas/mareW_happy_c.bas",
	"/scene/mareW/bas/mareW_recover_c.bas",
	"/scene/mareW/bas/mareW_run_c.bas",
	nullptr,
	nullptr,
	"/scene/mareW/bas/mareW_stand_c.bas",
	"/scene/mareW/bas/mareW_stand_A_c.bas",
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	"/scene/mareW/bas/mareW_wait_yogore.bas",
	"/scene/mareW/bas/mareW_walk_c.bas",
	"/scene/mareW/bas/mareW_wash_c.bas",
	"/scene/mareW/bas/mareW_wet_c.bas",
	"/scene/mareW/bas/mareW_wet_A_c.bas",
};

static const char* mareWA_bastable[] = {
	nullptr,
	nullptr,
	nullptr,
	nullptr,
};

static const char* mareWA_bas_c_table[] = {
	nullptr,
	nullptr,
	nullptr,
	nullptr,
};

static const char* mareWB_bastable[] = {
	"/scene/mareWB/bas/mareWBbaby_hold.bas",
	"/scene/mareWB/bas/mareWBbaby_holdTalk.bas",
	"/scene/mareWB/bas/mareWBbaby_holdWet.bas",
	nullptr,
	"/scene/mareWB/bas/mareWB_holdTalk.bas",
	"/scene/mareWB/bas/mareWB_holdWet.bas",
};

static const char* mareWB_bas_c_table[] = {
	"/scene/mareWB/bas/mareWBbaby_hold.bas",
	"/scene/mareWB/bas/mareWBbaby_holdTalk.bas",
	"/scene/mareWB/bas/mareWBbaby_holdWet.bas",
	nullptr,
	"/scene/mareWB/bas/mareWB_holdTalk.bas",
	"/scene/mareWB/bas/mareWB_holdWet.bas",
};

static const char* kinopio_bastable[] = {
	"/scene/kinopio/bas/kinopio_appear.bas",
	"/scene/kinopio/bas/kinopio_fall.bas",
	"/scene/kinopio/bas/kinopio_happy.bas",
	"/scene/kinopio/bas/kinopio_recover.bas",
	"/scene/kinopio/bas/kinopio_run.bas",
	nullptr,
	nullptr,
	nullptr,
	"/scene/kinopio/bas/kinopio_sitshake.bas",
	nullptr,
	"/scene/kinopio/bas/kinopio_sitwet.bas",
	nullptr,
	nullptr,
	"/scene/kinopio/bas/kinopio_stumble.bas",
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	"/scene/kinopio/bas/kinopio_wait_yogore.bas",
	"/scene/kinopio/bas/kinopio_walk.bas",
	nullptr,
	"/scene/kinopio/bas/kinopio_wet.bas",
	"/scene/kinopio/bas/kinopio_wet_A.bas",
	"/scene/kinopio/bas/kinopio_wet_yogore.bas",
	nullptr,
};

static const char* kinopio_bas_c_table[] = {
	"/scene/kinopio/bas/kinopio_appear.bas",
	"/scene/kinopio/bas/kinopio_fall.bas",
	"/scene/kinopio/bas/kinopio_happy.bas",
	"/scene/kinopio/bas/kinopio_recover.bas",
	"/scene/kinopio/bas/kinopio_run.bas",
	nullptr,
	nullptr,
	nullptr,
	"/scene/kinopio/bas/kinopio_sitshake.bas",
	nullptr,
	"/scene/kinopio/bas/kinopio_sitwet.bas",
	nullptr,
	nullptr,
	"/scene/kinopio/bas/kinopio_stumble.bas",
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	"/scene/kinopio/bas/kinopio_wait_yogore.bas",
	"/scene/kinopio/bas/kinopio_walk.bas",
	nullptr,
	"/scene/kinopio/bas/kinopio_wet.bas",
	"/scene/kinopio/bas/kinopio_wet_A.bas",
	"/scene/kinopio/bas/kinopio_wet_yogore.bas",
	nullptr,
};

static const char* kinojii_bastable[] = {
	"/scene/kinojii/bas/kinoji_fall.bas",
	"/scene/kinojii/bas/kinoji_recover.bas",
	nullptr,
	nullptr,
	"/scene/kinojii/bas/kinoji_sitshake.bas",
	nullptr,
	"/scene/kinojii/bas/kinoji_sitwet.bas",
	nullptr,
	nullptr,
	"/scene/kinojii/bas/kinoji_stumble.bas",
	nullptr,
	nullptr,
	nullptr,
	"/scene/kinojii/bas/kinoji_walk.bas",
	nullptr,
	"/scene/kinojii/bas/kinoji_wet.bas",
	"/scene/kinojii/bas/kinoji_wet_A.bas",
	nullptr,
};

static const char* kinojii_bas_c_table[] = {
	"/scene/kinojii/bas/kinoji_fall.bas",
	"/scene/kinojii/bas/kinoji_recover.bas",
	nullptr,
	nullptr,
	"/scene/kinojii/bas/kinoji_sitshake.bas",
	nullptr,
	"/scene/kinojii/bas/kinoji_sitwet.bas",
	nullptr,
	nullptr,
	"/scene/kinojii/bas/kinoji_stumble.bas",
	nullptr,
	nullptr,
	nullptr,
	"/scene/kinojii/bas/kinoji_walk.bas",
	nullptr,
	"/scene/kinojii/bas/kinoji_wet.bas",
	"/scene/kinojii/bas/kinoji_wet_A.bas",
	nullptr,
};

static const char* peach_bastable[] = {
	"/scene/peach/bas/peach_anger_wait.bas",
	"/scene/peach/bas/peach_carry.bas",
	"/scene/peach/bas/peach_fear_wait.bas",
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	"/scene/peach/bas/peach_parasol_wet.bas",
	"/scene/peach/bas/peach_recover.bas",
	"/scene/peach/bas/peach_tired_wait.bas",
	nullptr,
	"/scene/peach/bas/peach_wet.bas",
};

static const char* peach_bas_c_table[] = {
	"/scene/peach/bas/peach_anger_wait.bas",
	"/scene/peach/bas/peach_carry.bas",
	"/scene/peach/bas/peach_fear_wait.bas",
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	"/scene/peach/bas/peach_parasol_wet.bas",
	"/scene/peach/bas/peach_recover.bas",
	"/scene/peach/bas/peach_tired_wait.bas",
	nullptr,
	"/scene/peach/bas/peach_wet.bas",
};

static const char* raccoonDog_bastable[] = {
	"/scene/raccoonDog/bas/tanuki_wait_A.bas",
	"/scene/raccoonDog/bas/tanuki_wait_B.bas",
	nullptr,
	"/scene/raccoonDog/bas/tanuki_wet.bas",
};

static const char* raccoonDog_bas_c_table[] = {
	"/scene/raccoonDog/bas/tanuki_wait_A.bas",
	"/scene/raccoonDog/bas/tanuki_wait_B.bas",
	nullptr,
	"/scene/raccoonDog/bas/tanuki_wet.bas",
};

static const char* sunflowerL_bastable[] = {
	nullptr,
	"/scene/sunflowerL/bas/sunflower_stand.bas",
	nullptr,
	"/scene/sunflowerL/bas/sunflower_wet.bas",
	"/scene/sunflowerL/bas/sunflower_wet_B.bas",
};

static const char* sunflowerL_bas_c_table[] = {
	nullptr,
	"/scene/sunflowerL/bas/sunflower_stand.bas",
	nullptr,
	"/scene/sunflowerL/bas/sunflower_wet.bas",
	"/scene/sunflowerL/bas/sunflower_wet_B.bas",
};

static const char* sunflowerS_bastable[] = {
	nullptr,
	"/scene/sunflowerS/bas/sunflower_s_stand.bas",
	nullptr,
	"/scene/sunflowerS/bas/sunflower_s_wet.bas",
	"/scene/sunflowerS/bas/sunflower_s_wet_B.bas",
};

static const char* sunflowerS_bas_c_table[] = {
	nullptr,
	"/scene/sunflowerS/bas/sunflower_s_stand.bas",
	nullptr,
	"/scene/sunflowerS/bas/sunflower_s_wet.bas",
	"/scene/sunflowerS/bas/sunflower_s_wet_B.bas",
};

const char** TBaseNPC::getBasNameTable() const
{
	const char** result = nullptr;
	bool child          = isChild();
	if (isNormalMonteM()) {
		if (child) {
			result = monteMCommon_bas_c_table;
		} else {
			result = monteMCommon_bastable;
		}
	} else if (isNormalMonteW()) {
		if (child) {
			result = monteWCommon_bas_c_table;
		} else {
			result = monteWCommon_bastable;
		}
	} else {
		switch (mActorType) {
		case 0x4000006:
			result = monteME_bastable;
			break;
		case 0x4000007:
			result = monteMF_bastable;
			break;
		case 0x4000008:
			result = monteMG_bastable;
			break;
		case 0x4000009:
			result = monteMH_bastable;
			break;
		case 0x400000D:
			result = monteWC_bastable;
			break;
		case 0x400000E:
			if (child) {
				result = mareM_bas_c_table;
				break;
			} else {
				result = mareM_bastable;
				break;
			}
		case 0x400000F:
			result = mareMA_bastable;
			break;
		case 0x4000010:
			result = mareMB_bastable;
			break;
		case 0x4000011:
			result = mareMC_bastable;
			break;
		case 0x4000012:
			result = mareMD_bastable;
			break;
		case 0x4000013:
			if (child) {
				result = mareW_bas_c_table;
				break;
			} else {
				result = mareW_bastable;
				break;
			}
		case 0x4000014:
			result = mareWA_bastable;
			break;
		case 0x4000015:
			result = mareWB_bastable;
			break;
		case 0x4000016:
			result = kinopio_bastable;
			break;
		case 0x4000017:
			result = kinojii_bastable;
			break;
		case 0x4000018:
			result = peach_bastable;
			break;
		case 0x4000019:
			result = raccoonDog_bastable;
			break;
		case 0x400001A:
			result = sunflowerL_bastable;
			break;
		case 0x400001B:
			result = sunflowerS_bastable;
			break;
		// Uncertain if this is correct
		case 0x400001C:
			break;
		// Uncertain if this is correct
		case 0x400001D:
			break;
		}
	}
	return result;
}
