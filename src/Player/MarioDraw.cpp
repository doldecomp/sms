#include <Player/MarioMain.hpp>
#include <Player/MarioAnimeData.hpp>
#include <Player/MarioCap.hpp>
#include <Player/Watergun.hpp>

#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/PacketUtil.hpp>
#include <MarioUtil/TexUtil.hpp>
#include <MarioUtil/ModelUtil.hpp>
#include <M3DUtil/SampleCtrlModel.hpp>
#include <M3DUtil/M3UJoint.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <MoveBG/MapObjWave.hpp>
#include <JSystem/JGeometry.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTexture.hpp>
#include <JSystem/J3D/J3DGraphBase/Blocks/J3DPEBlocks.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DMaterialAnm.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <System/MarDirector.hpp>
#include <NPC/NpcBase.hpp>

// NOTE: hack for matching setLength
static void dummy(JGeometry::TVec3<f32>& v) { v.setLength(v, 1.0f); }
static void dummy2(JGeometry::TVec3<f32>& v) { dummy(v); }
static void dummy3(JGeometry::TVec3<f32>& v) { dummy2(v); }
static void dummy4(JGeometry::TVec3<f32>& v) { dummy3(v); }

static const char* MtxCalcTypeName[4]
    = { "MActorMtxCalcType_Basic クラシックスケールＯＮ",
	    "MActorMtxCalcType_Softimage クラシックスケールＯＦＦ",
	    "MActorMtxCalcType_MotionBlend モーションブレンド",
	    "MActorMtxCalcType_User ユーザー定義" };

static unkTMarioAnimeFilesStruct marioAnimeFiles[199] = {
	{ 0x00000001, "hgup" },
	{ 0x00000001, "bdwn" },
	{ 0x00000001, "bkdwn" },
	{ 0x00000001, "tree_climb" },
	{ 0x00000001, "tree_catch" },
	{ 0x00000000, "tree_stand" },
	{ 0x00000000, "tree_wait" },
	{ 0x00000000, "brake" },
	{ 0x00000001, "brend" },
	{ 0x00000001, "hgdwn" },
	{ 0x00000001, "fjpend" },
	{ 0x00000000, "firejmp" },
	{ 0x00000001, "sdwnf" },
	{ 0x00000001, "jfdwn" },
	{ 0x00000000, "hang" },
	{ 0x00000001, "hgjmp" },
	{ 0x00000001, "ladder_hang_catch" },
	{ 0x00000000, "hiped" },
	{ 0x00000001, "hipsr" },
	{ 0x00000000, "hipat" },
	{ 0x00000001, "run1" },
	{ 0x00000001, "2jmed" },
	{ 0x00000000, "2jmp2" },
	{ 0x00000001, "jump" },
	{ 0x00000001, "jmped" },
	{ 0x00000001, "2jmp1" },
	{ 0x00000000, "land" },
	{ 0x00000001, "laend" },
	{ 0x00000001, "lost" },
	{ 0x00000001, "door_openr" },
	{ 0x00000001, "door_openl" },
	{ 0x00000001, "throw" },
	{ 0x00000001, "ladder_hang_move_l" },
	{ 0x00000001, "ladder_hang_move_r" },
	{ 0x00000001, "raise" },
	{ 0x00000001, "push" },
	{ 0x00000001, "ride_shell" },
	{ 0x00000001, "put" },
	{ 0x00000001, "roll" },
	{ 0x00000001, "run2" },
	{ 0x00000000, "shock" },
	{ 0x00000001, "sfbdn" },
	{ 0x00000001, "sffdn" },
	{ 0x00000001, "sdown" },
	{ 0x00000001, "shfdn" },
	{ 0x00000000, "swait" },
	{ 0x00000001, "swlkl" },
	{ 0x00000001, "swlkr" },
	{ 0x00000000, "sldct" },
	{ 0x00000000, "slpbk" },
	{ 0x00000001, "sldwn" },
	{ 0x00000001, "slped" },
	{ 0x00000000, "slpla" },
	{ 0x00000000, "slip" },
	{ 0x00000001, "sstep" },
	{ 0x00000000, "sqend" },
	{ 0x00000000, "sqsta" },
	{ 0x00000000, "sqwat" },
	{ 0x00000000, "turn" },
	{ 0x00000000, "trned" },
	{ 0x00000001, "tjmp2" },
	{ 0x00000001, "tjmp1" },
	{ 0x00000000, "wait" },
	{ 0x00000000, "ladder_hang_wait_l" },
	{ 0x00000000, "ladder_hang_wait_r" },
	{ 0x00000000, "walk" },
	{ 0x00000001, "wjmp" },
	{ 0x00000000, "wsld" },
	{ 0x00000000, "pump" },
	{ 0x00000000, "hgpmp" },
	{ 0x00000000, "step1" },
	{ 0x00000001, "step2" },
	{ 0x00000001, "step3" },
	{ 0x00000000, "jkick" },
	{ 0x00000000, "dgrun" },
	{ 0x00000000, "carry_p" },
	{ 0x00000001, "hmov_l" },
	{ 0x00000001, "hmov_r" },
	{ 0x00000000, "t_wait" },
	{ 0x00000001, "hot_wait" },
	{ 0x00000000, "rope_walk" },
	{ 0x00000001, "rope_run" },
	{ 0x00000000, "rope_wait" },
	{ 0x00000000, "rope_wtosw" },
	{ 0x00000000, "rope_wtosw_r" },
	{ 0x00000000, "rope_swait" },
	{ 0x00000001, "rope_whg" },
	{ 0x00000001, "rope_swhg" },
	{ 0x00000000, "rope_hgwat" },
	{ 0x00000001, "rope_return" },
	{ 0x00000001, "rope_hmovr" },
	{ 0x00000001, "rope_hmovl" },
	{ 0x00000000, "sinking" },
	{ 0x00000000, "sink_down" },
	{ 0x00000001, "door_kick" },
	{ 0x00000001, "hold" },
	{ 0x00000000, "hold_wait" },
	{ 0x00000001, "hold_back" },
	{ 0x00000001, "hold_move_r" },
	{ 0x00000001, "hold_move_l" },
	{ 0x00000001, "hold_drag" },
	{ 0x00000000, "hold_to_hang" },
	{ 0x00000000, "hold_hang" },
	{ 0x00000001, "hang_to_hold" },
	{ 0x00000001, "hold_return" },
	{ 0x00000000, "spin_p" },
	{ 0x00000001, "turbo_dash" },
	{ 0x00000001, "broad_jump" },
	{ 0x00000001, "jump_rolling" },
	{ 0x00000001, "giant_rolling" },
	{ 0x00000001, "fence_catch" },
	{ 0x00000001, "fence_jcatch" },
	{ 0x00000000, "fence_wait" },
	{ 0x00000001, "fence_move_l" },
	{ 0x00000001, "fence_move_r" },
	{ 0x00000001, "fence_move_up" },
	{ 0x00000001, "fence_move_down" },
	{ 0x00000001, "fence_punch" },
	{ 0x00000001, "ladder_hang_kick" },
	{ 0x00000001, "ladder_roll_up" },
	{ 0x00000001, "ladder_roll_down" },
	{ 0x00000001, "ride_shell" },
	{ 0x00000000, "ride_shell_wait" },
	{ 0x00000001, "swim_start" },
	{ 0x00000000, "swim_wait" },
	{ 0x00000001, "wait_to_swim" },
	{ 0x00000001, "swim" },
	{ 0x00000001, "swim_to_wait" },
	{ 0x00000001, "swim_damage" },
	{ 0x00000001, "swim_down" },
	{ 0x00000001, "demo_shine_get" },
	{ 0x00000000, "demo_gate_in" },
	{ 0x00000001, "demo_gate_out" },
	{ 0x00000001, "roll_jump" },
	{ 0x00000001, "get_fail" },
	{ 0x00000001, "tree_move_l" },
	{ 0x00000001, "tree_move_r" },
	{ 0x00000001, "die" },
	{ 0x00000000, "monteman_wait" },
	{ 0x00000001, "swim_start" },
	{ 0x00000000, "swim_wait" },
	{ 0x00000001, "wait_to_paddle" },
	{ 0x00000001, "paddle_start" },
	{ 0x00000001, "swim" },
	{ 0x00000001, "paddle_end" },
	{ 0x00000001, "paddle_to_wait" },
	{ 0x00000001, "float" },
	{ 0x00000000, "damage_wait" },
	{ 0x00000000, "fepmp" },
	{ 0x00000000, "swpmp" },
	{ 0x00000000, "thrown" },
	{ 0x00000001, "thrown_end" },
	{ 0x00000001, "bottle_in" },
	{ 0x00000001, "sand_fill_head" },
	{ 0x00000001, "sand_fill_head_end" },
	{ 0x00000001, "sandfill_leg" },
	{ 0x00000001, "sandfill_leg_end" },
	{ 0x00000000, "damage_wait_start" },
	{ 0x00000001, "swim_dive" },
	{ 0x00000001, "draw" },
	{ 0x00000000, "swim_p_damage" },
	{ 0x00000000, "swim_p_down" },
	{ 0x00000001, "pivot" },
	{ 0x00000000, "demo_gate_out_get2" },
	{ 0x00000001, "demo_gate_out_appear" },
	{ 0x00000000, "belt_up" },
	{ 0x00000000, "yawn" },
	{ 0x00000001, "sit" },
	{ 0x00000001, "sit_wait" },
	{ 0x00000001, "sit_end" },
	{ 0x00000001, "sleep" },
	{ 0x00000001, "sleep_wait" },
	{ 0x00000001, "sleep_end" },
	{ 0x00000000, "dive_wait" },
	{ 0x00000001, "dive_land" },
	{ 0x00000001, "door_gacha_l" },
	{ 0x00000001, "door_gacha_r" },
	{ 0x00000001, "shock_down" },
	{ 0x00000001, "demo_gate_out_appear_get" },
	{ 0x00000000, "demo_gate_out_rolling_get" },
	{ 0x00000000, "demo_gate_out_rolling" },
	{ 0x00000000, "fall_down_wait" },
	{ 0x00000000, "yo_wait" },
	{ 0x00000000, "yo_walk" },
	{ 0x00000000, "yo_run" },
	{ 0x00000000, "yo_eat" },
	{ 0x00000000, "yo_eat_end" },
	{ 0x00000000, "yo_jump" },
	{ 0x00000000, "yo_jump_fall" },
	{ 0x00000000, "yo_jump_end" },
	{ 0x00000000, "yo_hold_jump" },
	{ 0x00000000, "yo_ride" },
	{ 0x00000000, "yo_hip_start" },
	{ 0x00000000, "yo_hip_pose" },
	{ 0x00000000, "yo_hip_end" },
	{ 0x00000000, "yo_damage" },
	{ 0x00000001, "demo_shine_get_yo" },
	{ 0x00000001, "yo_slide_pose" },
	{ 0x00000001, "yo_slide_end" },
};

const char cDirtyFileName[] = "/scene/map/pollution/H_ma_rak.bti";
const char cDirtyTexName[]  = "H_ma_rak_dummy";
TMarioAnimeData gMarioAnimeData[336]
    = { { 0x0000, 0x00C8, 0x00, 0x01, 0x04, 0x16 },
	    { 0x0001, 0x00C8, 0x01, 0x00, 0x06, 0x16 },
	    { 0x0002, 0x00C8, 0x02, 0x00, 0x06, 0x0A },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x06, 0x16 },
	    { 0x0003, 0x00C8, 0x00, 0x01, 0x04, 0x16 },
	    { 0x0004, 0x00C8, 0x00, 0x01, 0x04, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x0006, 0x00C8, 0x00, 0x01, 0x04, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x0007, 0x0044, 0x00, 0x00, 0x06, 0x16 },
	    { 0x0008, 0x0044, 0x00, 0x00, 0x06, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x0009, 0x00C8, 0x00, 0x01, 0x04, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x000A, 0x00C8, 0x0A, 0x00, 0x06, 0x0B },
	    { 0x000B, 0x00C8, 0x09, 0x00, 0x06, 0x0A },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x000C, 0x00C8, 0x03, 0x00, 0x06, 0x16 },
	    { 0x000D, 0x00C8, 0x04, 0x00, 0x06, 0x0A },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x000E, 0x0045, 0x00, 0x01, 0x04, 0x16 },
	    { 0x000F, 0x00C8, 0x00, 0x01, 0x06, 0x0A },
	    { 0x0010, 0x00C8, 0x00, 0x01, 0x04, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x0011, 0x00C8, 0x00, 0x02, 0x06, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x0012, 0x0044, 0x00, 0x02, 0x06, 0x16 },
	    { 0x0013, 0x0044, 0x00, 0x02, 0x06, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x0014, 0x0044, 0x00, 0x01, 0x0E, 0x0F },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x0015, 0x0044, 0x00, 0x00, 0x06, 0x0B },
	    { 0x0016, 0x0044, 0x00, 0x00, 0x06, 0x0A },
	    { 0x0017, 0x0044, 0x00, 0x02, 0x06, 0x0A },
	    { 0x0018, 0x0044, 0x00, 0x01, 0x06, 0x0B },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x0019, 0x0044, 0x00, 0x00, 0x06, 0x0B },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x001A, 0x0044, 0x00, 0x00, 0x06, 0x0C },
	    { 0x001B, 0x0044, 0x00, 0x00, 0x06, 0x0B },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x001C, 0x00C8, 0x00, 0x00, 0x06, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x0020, 0x00C8, 0x00, 0x01, 0x04, 0x16 },
	    { 0x0021, 0x00C8, 0x00, 0x01, 0x04, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x001D, 0x00C8, 0x00, 0x00, 0x04, 0x16 },
	    { 0x001E, 0x00C8, 0x00, 0x00, 0x04, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x001F, 0x00C8, 0x00, 0x01, 0x06, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x0022, 0x00C8, 0x00, 0x01, 0x06, 0x16 },
	    { 0x0023, 0x00C8, 0x00, 0x00, 0x06, 0x16 },
	    { 0x0024, 0x00C8, 0x00, 0x00, 0x06, 0x16 },
	    { 0x0025, 0x00C8, 0x00, 0x01, 0x06, 0x16 },
	    { 0x0026, 0x0044, 0x00, 0x02, 0x06, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x0027, 0x0044, 0x00, 0x02, 0x06, 0x0F },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x0029, 0x00C8, 0x00, 0x00, 0x06, 0x16 },
	    { 0x002A, 0x00C8, 0x00, 0x00, 0x06, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00B1, 0x00C8, 0x05, 0x00, 0x06, 0x16 },
	    { 0x0028, 0x00C8, 0x05, 0x00, 0x06, 0x16 },
	    { 0x002B, 0x00C8, 0x05, 0x00, 0x06, 0x16 },
	    { 0x002C, 0x00C8, 0x03, 0x00, 0x06, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x002D, 0x0044, 0x00, 0x00, 0x06, 0x16 },
	    { 0x002E, 0x0044, 0x00, 0x00, 0x06, 0x10 },
	    { 0x002F, 0x0044, 0x00, 0x00, 0x06, 0x11 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x0030, 0x00C8, 0x00, 0x00, 0x06, 0x13 },
	    { 0x0031, 0x00C8, 0x00, 0x00, 0x06, 0x13 },
	    { 0x0032, 0x00C8, 0x06, 0x00, 0x06, 0x13 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x0033, 0x00C8, 0x00, 0x01, 0x06, 0x12 },
	    { 0x0034, 0x00C8, 0x00, 0x01, 0x06, 0x16 },
	    { 0x0035, 0x00C8, 0x00, 0x01, 0x06, 0x13 },
	    { 0x0036, 0x0044, 0x00, 0x00, 0x06, 0x0F },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x0037, 0x0044, 0x00, 0x01, 0x00, 0x16 },
	    { 0x0038, 0x0044, 0x00, 0x01, 0x00, 0x16 },
	    { 0x0039, 0x0044, 0x00, 0x01, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x003A, 0x0044, 0x00, 0x01, 0x06, 0x16 },
	    { 0x003B, 0x0044, 0x00, 0x01, 0x06, 0x16 },
	    { 0x003C, 0x0044, 0x00, 0x02, 0x06, 0x0B },
	    { 0x003D, 0x0044, 0x00, 0x02, 0x06, 0x0A },
	    { 0x003C, 0x0044, 0x00, 0x02, 0x06, 0x0B },
	    { 0x006C, 0x0044, 0x00, 0x02, 0x06, 0x0A },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x003E, 0x0044, 0x00, 0x00, 0x06, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x003F, 0x00C8, 0x00, 0x01, 0x04, 0x16 },
	    { 0x0040, 0x00C8, 0x00, 0x01, 0x04, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x0041, 0x0044, 0x00, 0x00, 0x06, 0x18 },
	    { 0x0042, 0x0044, 0x00, 0x00, 0x06, 0x0A },
	    { 0x0043, 0x00C8, 0x00, 0x00, 0x06, 0x16 },
	    { 0x0082, 0x00C8, 0x00, 0x00, 0x00, 0x02 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x00C8, 0x00C8, 0x19, 0x05, 0x00, 0x16 },
	    { 0x0046, 0x0044, 0x00, 0x02, 0x06, 0x16 },
	    { 0x0047, 0x0044, 0x00, 0x02, 0x06, 0x16 },
	    { 0x0048, 0x0044, 0x00, 0x02, 0x06, 0x16 },
	    { 0x0049, 0x00C8, 0x00, 0x02, 0x06, 0x16 },
	    { 0x004A, 0x00C8, 0x07, 0x02, 0x06, 0x16 },
	    { 0x001A, 0x0044, 0x00, 0x01, 0x06, 0x16 },
	    { 0x004C, 0x00C8, 0x00, 0x01, 0x04, 0x16 },
	    { 0x004D, 0x00C8, 0x00, 0x01, 0x04, 0x16 },
	    { 0x004E, 0x0044, 0x00, 0x00, 0x06, 0x16 },
	    { 0x004F, 0x0044, 0x08, 0x00, 0x06, 0x16 },
	    { 0x0050, 0x0044, 0x00, 0x00, 0x06, 0x18 },
	    { 0x0051, 0x0044, 0x00, 0x00, 0x06, 0x16 },
	    { 0x0052, 0x0044, 0x00, 0x00, 0x06, 0x16 },
	    { 0x0053, 0x0044, 0x00, 0x00, 0x00, 0x16 },
	    { 0x0054, 0x0044, 0x00, 0x00, 0x00, 0x16 },
	    { 0x0055, 0x0044, 0x00, 0x00, 0x06, 0x16 },
	    { 0x0056, 0x00C8, 0x00, 0x00, 0x00, 0x16 },
	    { 0x0057, 0x00C8, 0x00, 0x00, 0x00, 0x16 },
	    { 0x0058, 0x0045, 0x00, 0x00, 0x06, 0x16 },
	    { 0x0059, 0x00C8, 0x00, 0x00, 0x00, 0x16 },
	    { 0x005A, 0x00C8, 0x00, 0x01, 0x06, 0x16 },
	    { 0x005B, 0x00C8, 0x00, 0x01, 0x06, 0x16 },
	    { 0x005C, 0x0044, 0x00, 0x00, 0x06, 0x16 },
	    { 0x005D, 0x0044, 0x00, 0x00, 0x06, 0x16 },
	    { 0x005E, 0x00C8, 0x00, 0x01, 0x06, 0x16 },
	    { 0x005F, 0x00C8, 0x00, 0x01, 0x06, 0x16 },
	    { 0x0060, 0x00C8, 0x00, 0x01, 0x06, 0x16 },
	    { 0x0061, 0x00C8, 0x00, 0x01, 0x06, 0x16 },
	    { 0x0062, 0x00C8, 0x00, 0x01, 0x06, 0x16 },
	    { 0x0063, 0x00C8, 0x00, 0x01, 0x06, 0x16 },
	    { 0x0064, 0x00C8, 0x00, 0x01, 0x06, 0x16 },
	    { 0x0065, 0x00C8, 0x00, 0x01, 0x06, 0x16 },
	    { 0x0066, 0x00C8, 0x00, 0x01, 0x06, 0x16 },
	    { 0x0067, 0x00C8, 0x00, 0x01, 0x06, 0x16 },
	    { 0x0068, 0x00C8, 0x00, 0x01, 0x06, 0x16 },
	    { 0x0069, 0x0044, 0x00, 0x00, 0x00, 0x0A },
	    { 0x006A, 0x0044, 0x00, 0x02, 0x06, 0x0F },
	    { 0x006B, 0x0044, 0x00, 0x00, 0x06, 0x0A },
	    { 0x006C, 0x0044, 0x00, 0x00, 0x06, 0x0A },
	    { 0x006D, 0x0044, 0x00, 0x02, 0x06, 0x16 },
	    { 0x006E, 0x00C8, 0x00, 0x01, 0x06, 0x16 },
	    { 0x006F, 0x00C8, 0x00, 0x01, 0x06, 0x16 },
	    { 0x0070, 0x0094, 0x00, 0x01, 0x06, 0x16 },
	    { 0x0071, 0x0094, 0x00, 0x01, 0x04, 0x16 },
	    { 0x0072, 0x0094, 0x00, 0x01, 0x04, 0x16 },
	    { 0x0073, 0x0094, 0x00, 0x01, 0x04, 0x16 },
	    { 0x0074, 0x0045, 0x00, 0x01, 0x04, 0x16 },
	    { 0x0075, 0x00C8, 0x00, 0x01, 0x06, 0x16 },
	    { 0x0076, 0x00C8, 0x00, 0x01, 0x04, 0x16 },
	    { 0x0077, 0x00C8, 0x00, 0x01, 0x06, 0x16 },
	    { 0x0078, 0x00C8, 0x00, 0x01, 0x06, 0x16 },
	    { 0x0079, 0x00C8, 0x00, 0x01, 0x06, 0x16 },
	    { 0x007A, 0x0044, 0x00, 0x00, 0x06, 0x16 },
	    { 0x007B, 0x0095, 0x00, 0x00, 0x06, 0x16 },
	    { 0x007C, 0x0095, 0x00, 0x00, 0x06, 0x16 },
	    { 0x007D, 0x0095, 0x00, 0x00, 0x06, 0x16 },
	    { 0x007E, 0x0095, 0x00, 0x00, 0x06, 0x16 },
	    { 0x007F, 0x0095, 0x00, 0x00, 0x06, 0x16 },
	    { 0x0080, 0x0095, 0x00, 0x00, 0x06, 0x16 },
	    { 0x0081, 0x0095, 0x00, 0x00, 0x06, 0x16 },
	    { 0x0083, 0x00C8, 0x00, 0x00, 0x00, 0x16 },
	    { 0x0084, 0x00C8, 0x0C, 0x00, 0x00, 0x16 },
	    { 0x0085, 0x0044, 0x00, 0x00, 0x06, 0x16 },
	    { 0x0086, 0x00C8, 0x00, 0x00, 0x06, 0x16 },
	    { 0x0087, 0x00C8, 0x00, 0x00, 0x06, 0x16 },
	    { 0x0088, 0x00C8, 0x00, 0x00, 0x06, 0x16 },
	    { 0x0089, 0x00C8, 0x0D, 0x00, 0x06, 0x16 },
	    { 0x008A, 0x00C8, 0x00, 0x02, 0x06, 0x16 },
	    { 0x008B, 0x0095, 0x00, 0x00, 0x06, 0x16 },
	    { 0x008C, 0x0095, 0x00, 0x00, 0x06, 0x16 },
	    { 0x008D, 0x0095, 0x00, 0x00, 0x06, 0x16 },
	    { 0x008E, 0x0095, 0x00, 0x00, 0x06, 0x16 },
	    { 0x008F, 0x0095, 0x00, 0x00, 0x06, 0x16 },
	    { 0x0090, 0x0095, 0x00, 0x00, 0x06, 0x16 },
	    { 0x0091, 0x0095, 0x00, 0x00, 0x06, 0x16 },
	    { 0x0092, 0x0095, 0x00, 0x00, 0x06, 0x16 },
	    { 0x0093, 0x0044, 0x0E, 0x00, 0x06, 0x16 },
	    { 0x0094, 0x0044, 0x00, 0x00, 0x06, 0x16 },
	    { 0x0095, 0x0044, 0x00, 0x00, 0x06, 0x16 },
	    { 0x0096, 0x00C8, 0x00, 0x00, 0x06, 0x16 },
	    { 0x0097, 0x00C8, 0x00, 0x00, 0x06, 0x16 },
	    { 0x0098, 0x00C8, 0x00, 0x00, 0x06, 0x16 },
	    { 0x0099, 0x00C8, 0x00, 0x00, 0x06, 0x16 },
	    { 0x009A, 0x00C8, 0x00, 0x00, 0x06, 0x16 },
	    { 0x009B, 0x00C8, 0x00, 0x00, 0x06, 0x16 },
	    { 0x009C, 0x00C8, 0x00, 0x00, 0x06, 0x16 },
	    { 0x009D, 0x0044, 0x00, 0x00, 0x06, 0x16 },
	    { 0x009E, 0x00C8, 0x00, 0x00, 0x06, 0x16 },
	    { 0x009F, 0x00C8, 0x00, 0x00, 0x06, 0x16 },
	    { 0x00A0, 0x00C8, 0x0F, 0x00, 0x06, 0x16 },
	    { 0x00A1, 0x00C8, 0x10, 0x00, 0x06, 0x16 },
	    { 0x00A2, 0x0044, 0x00, 0x00, 0x06, 0x16 },
	    { 0x00A3, 0x00C8, 0x00, 0x00, 0x00, 0x16 },
	    { 0x00A4, 0x00C8, 0x00, 0x00, 0x00, 0x16 },
	    { 0x00A5, 0x00C8, 0x00, 0x00, 0x06, 0x16 },
	    { 0x00A6, 0x00C8, 0x11, 0x00, 0x06, 0x16 },
	    { 0x00A7, 0x00C8, 0x12, 0x00, 0x06, 0x16 },
	    { 0x00A8, 0x00C8, 0x13, 0x00, 0x06, 0x16 },
	    { 0x00A9, 0x00C8, 0x14, 0x00, 0x06, 0x16 },
	    { 0x00AA, 0x00C8, 0x15, 0x00, 0x06, 0x16 },
	    { 0x00AB, 0x00C8, 0x16, 0x00, 0x06, 0x16 },
	    { 0x00AC, 0x00C8, 0x17, 0x00, 0x06, 0x16 },
	    { 0x00AD, 0x0044, 0x00, 0x00, 0x06, 0x16 },
	    { 0x00AE, 0x0044, 0x00, 0x00, 0x06, 0x16 },
	    { 0x00AF, 0x00C8, 0x00, 0x00, 0x00, 0x16 },
	    { 0x00B0, 0x00C8, 0x00, 0x00, 0x00, 0x16 },
	    { 0x00B2, 0x00C8, 0x00, 0x02, 0x06, 0x16 },
	    { 0x00B3, 0x00C8, 0x00, 0x00, 0x06, 0x16 },
	    { 0x00B4, 0x00C8, 0x00, 0x00, 0x06, 0x16 },
	    { 0x00B5, 0x00C8, 0x00, 0x00, 0x06, 0x16 },
	    { 0x00B6, 0x00C8, 0x00, 0x00, 0x06, 0x16 },
	    { 0x00B7, 0x00C8, 0x00, 0x00, 0x06, 0x18 },
	    { 0x00B8, 0x00C8, 0x00, 0x00, 0x06, 0x0F },
	    { 0x00B9, 0x00C8, 0x00, 0x00, 0x06, 0x03 },
	    { 0x00BA, 0x00C8, 0x00, 0x00, 0x06, 0x04 },
	    { 0x00BB, 0x00C8, 0x00, 0x00, 0x06, 0x0A },
	    { 0x00BC, 0x00C8, 0x00, 0x00, 0x06, 0x0C },
	    { 0x00BD, 0x00C8, 0x00, 0x00, 0x06, 0x0B },
	    { 0x00BE, 0x00C8, 0x00, 0x00, 0x06, 0x09 },
	    { 0x00BF, 0x00C8, 0x00, 0x00, 0x06, 0x0E },
	    { 0x00C0, 0x00C8, 0x00, 0x00, 0x06, 0x08 },
	    { 0x00C1, 0x00C8, 0x00, 0x00, 0x06, 0x07 },
	    { 0x00C2, 0x00C8, 0x00, 0x00, 0x06, 0x06 },
	    { 0x00C3, 0x00C8, 0x00, 0x00, 0x06, 0x01 },
	    { 0x00C4, 0x00C8, 0x00, 0x00, 0x06, 0x02 },
	    { 0x00C5, 0x00C8, 0x00, 0x00, 0x06, 0x13 },
	    { 0x00C6, 0x00C8, 0x00, 0x00, 0x06, 0x12 } };

static char* marioAnimeTexPatternFilenames[24] = {
	"/mario/btp/ma_wink_tx.btp",        "/mario/btp/ma_bdwn_tx.btp",
	"/mario/btp/ma_bkdwn_tx.btp",       "/mario/btp/ma_sdwnf_tx.btp",
	"/mario/btp/ma_jfdwn_tx.btp",       "/mario/btp/ma_sdown_tx.btp",
	"/mario/btp/ma_sldwn_tx.btp",       "/mario/btp/ma_dgrun_tx.btp",
	"/mario/btp/ma_hot_wait_tx.btp",    "/mario/btp/ma_firejmp_tx.btp",
	"/mario/btp/ma_fjpend_tx.btp",      "/mario/btp/ma_shock_tx.btp",
	"/mario/btp/ma_gate_out_tx.btp",    "/mario/btp/ma_die_tx.btp",
	"/mario/btp/ma_d_wait_tx.btp",      "/mario/btp/ma_swim_p_damage_tx.btp",
	"/mario/btp/ma_swim_p_down_tx.btp", "/mario/btp/ma_yawn_tx.btp",
	"/mario/btp/ma_sit_tx.btp",         "/mario/btp/ma_sit_wait_tx.btp",
	"/mario/btp/ma_sit_end_tx.btp",     "/mario/btp/ma_sleep_tx.btp",
	"/mario/btp/ma_sleep_wait_tx.btp",  "/mario/btp/ma_sleep_end_tx.btp"
};

bool TMarioAnimeData::isPumpOK() const
{
	if (unk2 == 200) {
		return false;
	}
	return true;
}

int MarioHeadCtrl(J3DNode* param_1, int param_2)
{
	// volatile u32 padding[34];
	Mtx transform;
	if (param_2 == 0) {
		if (gpMarioForCallBack->mAction == 0x10001308) {
			if (gpMarDirector->unkA0 == nullptr) {
				return 0;
			}
			JGeometry::TVec3<f32> npcResetToPos;
			gpMarDirector->unkA0->resetToPosition(npcResetToPos);
			JGeometry::TVec3<f32> pos;
			pos.x = gpMarioForCallBack->mPosition.x;
			pos.y = gpMarioForCallBack->mPosition.y + 112.0f;
			pos.z = gpMarioForCallBack->mPosition.z;
			JGeometry::TVec3<f32> other = npcResetToPos - pos;
			f32 mult                    = other.squared();
			if (mult > 0.0f) { }

			s16 angle = -matan(mult, other.y);
			MsMtxSetRotRPH(transform, 0.0f, 0.0f, SHORTANGLE2DEG(angle));
		} else if (gpMarioForCallBack->mAction == 0x88b) {
			if (gpMarioForCallBack->mWaterGun->canSpray() == true) {

				s16 headAngle = gpMarioForCallBack->mUpperBodyParams
				                    .mHoverHeadAngle.get();
				MsMtxSetRotRPH(transform, 0.0f, 0.0f,
				               SHORTANGLE2DEG(headAngle));
			}
		} else if (gpMarioForCallBack->mAction == 0xC400201
		           && gpMarioForCallBack->mUpperBodyParams.mFeelDeepHeadAngle
		                      .get()
		                  < gpMarioForCallBack->unk370) {
			s16 headAngle
			    = gpMarioForCallBack->mUpperBodyParams.mFeelDeepHeadAngle.get();
			MsMtxSetRotRPH(transform, 0.0f, 0.0f, SHORTANGLE2DEG(headAngle));
		} else if (gpMarioForCallBack->isWallInFront()) {

			s16 headAngle = gpMarioForCallBack->mUpperBodyParams
			                    .mFrontWallHeadAngle.get();
			MsMtxSetRotRPH(transform, 0.0f, 0.0f, SHORTANGLE2DEG(headAngle));
		} else {

			TMario* mario = gpMarioForCallBack;
			f32 anmSpeed;
			if (mario->fabricatedIsPumping()) {
				anmSpeed = mario->mUpperBodyParams.mPumpAnmSpeed.get();
			} else {
				anmSpeed = mario->mDirtyParams.mSlipAnmSpeed.get();
			}

			s16 headAngle = mario->unk100 * anmSpeed;
			MsMtxSetRotRPH(transform, 0.0f, SHORTANGLE2DEG(headAngle), 0.0f);
			s16 gunAngle = gpMarioForCallBack->mWaterGun->getCurrentNozzle()
			                   ->getGunAngle()
			               / 2;
			if (gunAngle < 0) {
				Mtx gunMtx;
				MsMtxSetRotRPH(gunMtx, 0.0f, 0.0f, SHORTANGLE2DEG(gunAngle));
				MTXConcat(transform, gunMtx, transform);
			}
		}

		MTXConcat(J3DSys::mCurrentMtx, transform, J3DSys::mCurrentMtx);
	}
	return 1;
}

int MarioWaistCtrl(J3DNode* param_1, int param_2)
{
	volatile u32 padding[3];
	if (param_2 == 0) {
		TMario* mario = gpMarioForCallBack;
		s16* unk      = &mario->unkFC; // This feels wrong
		if (mario == gpMarioOriginal
		    && gpCamera->isLButtonCameraSpecifyMode(gpCamera->mMode) == 1
		    && gpMarioForCallBack->canBendBody() != 0 && gpCamera->unkA4 > 0) {
			unk = &gpCamera->unkA4; // This feels wrong
			Mtx transform;
			MsMtxSetRotRPH(transform, SHORTANGLE2DEG(mario->unk100), 0.0f,
			               SHORTANGLE2DEG(gpCamera->unkA4));
			MTXConcat(J3DSys::mCurrentMtx, transform, J3DSys::mCurrentMtx);
			return 1;
		} else if (gpMarioForCallBack->checkActionFlag(MARIO_FLAG_HAS_FLUDD)
		           && !gpMarioForCallBack->onYoshi()) {
			TNozzleBase* currentNozzle
			    = gpMarioForCallBack->mWaterGun->getCurrentNozzle();
			s16 gunAngle = currentNozzle->getGunAngle();
			if (gunAngle > 0) {
				Mtx gunMtx;
				MsMtxSetRotRPH(gunMtx, 0.0f, 0.0f, SHORTANGLE2DEG(gunAngle));
				MTXConcat(J3DSys::mCurrentMtx, gunMtx, J3DSys::mCurrentMtx);
				return 1;
			}
		} else if (gpMarioForCallBack->mAnimationId == 0x48
		           || gpMarioForCallBack->mAnimationId == 0x72
		           || gpMarioForCallBack->mAnimationId == 0x6D
		                  && !gpMarioForCallBack->checkActionFlag(
		                      MARIO_FLAG_FLUDD_EMITTING)) {

			// Ah, i love storing floats, casting them to s16
			// and then transforming them to floats again...
			// This matches for me...
			// Definition from TMario
			// /* 0x3D8 */ f32 unk3D8;
			// /* 0x3DC */ f32 unk3DC;
			s16 unk3D8 = gpMarioForCallBack->unk3D8;
			s16 unk3DC = gpMarioForCallBack->unk3DC;
			Mtx transform;
			MsMtxSetRotRPH(transform, SHORTANGLE2DEG(unk3D8), 0.0f,
			               SHORTANGLE2DEG(unk3DC));
			MTXConcat(J3DSys::mCurrentMtx, transform, J3DSys::mCurrentMtx);
			return 1;
		} else {
			*unk          = 0;
			mario->unk100 = 0;
		}
	}
	return 1;
}

int MarioFootPosRCtrl(J3DNode* param_1, int param_2)
{
	// volatile u32 padding[9];
	if (param_2 == 0) {

		BOOL check2;
		bool check;

		// Definitely some inline shenanigans
		// And this is wrong
		if ((gpMarioForCallBack->mAction & 0x1C0) == 0
		    && gpMarioForCallBack->mAction != 0xC00023D
		    && gpMarioForCallBack->onYoshi() == 0) {

			check2 = !(gpMarioForCallBack->mAction != 0xC400202
			           && gpMarioForCallBack->mAction != 0xC000203)
			             ? TRUE
			             : FALSE;
		}

		if (check2) {

			MtxPtr footMtx = gpMarioForCallBack->mModel->getModel()->getAnmMtx(
			    gpMarioForCallBack->mBoneIDs[6]);
			const TBGCheckData* checkData;
			f32 dist = gpMap->checkGround(footMtx[0][3], footMtx[1][3],
			                              footMtx[2][3], &checkData);

			if (!checkData->checkFlag(BG_CHECK_FLAG_ILLEGAL)) {

				f32 footMovement = dist - footMtx[1][3] + 10.0f;
				if (footMovement > 15.0f) {
					footMovement = 15.0f;
				}
				if (footMovement < -15.0f) {
					footMovement = -15.0f;
				}
				J3DSys::mCurrentMtx[1][3] += footMovement;
			}
		}
	}
	return 1;
}

int MarioFootDirRCtrl(J3DNode* param_1, int param_2)
{
	if (param_2 == 0) {

		BOOL check2;
		bool check;

		// Definitely some inline shenanigans
		// And this is wrong
		if ((gpMarioForCallBack->mAction & 0x1C0) == 0
		    && gpMarioForCallBack->mAction != 0xC00023D
		    && gpMarioForCallBack->onYoshi() == 0) {

			check2 = !(gpMarioForCallBack->mAction != 0xC400202
			           && gpMarioForCallBack->mAction != 0xC000203)
			             ? TRUE
			             : FALSE;
		}

		if (check2) {

			MtxPtr footMtx = gpMarioForCallBack->mModel->getModel()->getAnmMtx(
			    gpMarioForCallBack->mBoneIDs[7]);
			const TBGCheckData* checkData;
			f32 dist = gpMap->checkGround(footMtx[0][3], footMtx[1][3],
			                              footMtx[2][3], &checkData);
			if (!checkData->checkFlag(BG_CHECK_FLAG_ILLEGAL)) {

				// A lot of stuff is not matching with these copies
				Vec currentMtxDir;
				currentMtxDir.x = J3DSys::mCurrentMtx[0][0];
				currentMtxDir.y = J3DSys::mCurrentMtx[1][0];
				currentMtxDir.z = J3DSys::mCurrentMtx[2][0];

				Vec normalDir;
				normalDir.x = -checkData->getNormal().x;
				normalDir.y = -checkData->getNormal().y;
				normalDir.z = -checkData->getNormal().z;

				Vec currentNormalCross1;
				Vec currentNormalCross2;
				PSVECCrossProduct(&currentMtxDir, &normalDir,
				                  &currentNormalCross1);
				PSVECCrossProduct(&normalDir, &currentMtxDir,
				                  &currentNormalCross2);
				PSVECNormalize(&currentNormalCross1, &currentNormalCross1);
				PSVECNormalize(&currentNormalCross2, &currentNormalCross2);

				J3DSys::mCurrentMtx[0][0] = currentNormalCross2.x;
				J3DSys::mCurrentMtx[1][0] = currentNormalCross2.y;
				J3DSys::mCurrentMtx[2][0] = currentNormalCross2.z;

				J3DSys::mCurrentMtx[0][1] = normalDir.x;
				J3DSys::mCurrentMtx[1][1] = normalDir.y;
				J3DSys::mCurrentMtx[2][1] = normalDir.z;

				J3DSys::mCurrentMtx[0][2] = currentNormalCross1.x;
				J3DSys::mCurrentMtx[1][2] = currentNormalCross1.y;
				J3DSys::mCurrentMtx[2][2] = currentNormalCross1.z;

				footMtx[0][0] = currentNormalCross2.x;
				footMtx[1][0] = currentNormalCross2.y;
				footMtx[2][0] = currentNormalCross2.z;

				footMtx[0][1] = normalDir.x;
				footMtx[1][1] = normalDir.y;
				footMtx[2][1] = normalDir.z;

				footMtx[0][2] = currentNormalCross1.x;
				footMtx[1][2] = currentNormalCross1.y;
				footMtx[2][2] = currentNormalCross1.z;
			}
		}
	}

	return 1;
}

int MarioFootPosLCtrl(J3DNode* param_1, int param_2)
{
	// volatile u32 padding[9];
	if (param_2 == 0) {

		BOOL check2;
		bool check;

		// Definitely some inline shenanigans
		// And this is wrong
		if ((gpMarioForCallBack->mAction & 0x1C0) == 0
		    && gpMarioForCallBack->mAction != 0xC00023D
		    && gpMarioForCallBack->onYoshi() == 0) {

			check2 = !(gpMarioForCallBack->mAction != 0xC400202
			           && gpMarioForCallBack->mAction != 0xC000203)
			             ? TRUE
			             : FALSE;
		}

		if (check2) {

			MtxPtr footMtx = gpMarioForCallBack->mModel->getModel()->getAnmMtx(
			    gpMarioForCallBack->mBoneIDs[8]);
			const TBGCheckData* checkData;
			f32 dist = gpMap->checkGround(footMtx[0][3], footMtx[1][3],
			                              footMtx[2][3], &checkData);

			if (!checkData->checkFlag(BG_CHECK_FLAG_ILLEGAL)) {

				f32 footMovement = dist - footMtx[1][3] + 10.0f;
				if (footMovement > 15.0f) {
					footMovement = 15.0f;
				}
				if (footMovement < -15.0f) {
					footMovement = -15.0f;
				}
				J3DSys::mCurrentMtx[1][3] += footMovement;
			}
		}
	}
	return 1;
}

int MarioFootDirLCtrl(J3DNode* param_1, int param_2)
{
	if (param_2 == 0) {

		BOOL check2;
		bool check;

		// Definitely some inline shenanigans
		// And this is wrong
		if ((gpMarioForCallBack->mAction & 0x1C0) == 0
		    && gpMarioForCallBack->mAction != 0xC00023D
		    && gpMarioForCallBack->onYoshi() == 0) {

			check2 = !(gpMarioForCallBack->mAction != 0xC400202
			           && gpMarioForCallBack->mAction != 0xC000203)
			             ? TRUE
			             : FALSE;
		}

		if (check2) {

			MtxPtr footMtx = gpMarioForCallBack->mModel->getModel()->getAnmMtx(
			    gpMarioForCallBack->mBoneIDs[9]);
			const TBGCheckData* checkData;
			f32 dist = gpMap->checkGround(footMtx[0][3], footMtx[1][3],
			                              footMtx[2][3], &checkData);
			if (!checkData->checkFlag(BG_CHECK_FLAG_ILLEGAL)) {

				// A lot of stuff is not matching with these copies
				Vec currentMtxDir;
				currentMtxDir.x = J3DSys::mCurrentMtx[0][0];
				currentMtxDir.y = J3DSys::mCurrentMtx[1][0];
				currentMtxDir.z = J3DSys::mCurrentMtx[2][0];

				Vec normalDir;
				normalDir.x = -checkData->getNormal().x;
				normalDir.y = -checkData->getNormal().y;
				normalDir.z = -checkData->getNormal().z;

				Vec currentNormalCross1;
				Vec currentNormalCross2;
				PSVECCrossProduct(&currentMtxDir, &normalDir,
				                  &currentNormalCross1);
				PSVECCrossProduct(&normalDir, &currentMtxDir,
				                  &currentNormalCross2);
				PSVECNormalize(&currentNormalCross1, &currentNormalCross1);
				PSVECNormalize(&currentNormalCross2, &currentNormalCross2);

				J3DSys::mCurrentMtx[0][0] = currentNormalCross2.x;
				J3DSys::mCurrentMtx[1][0] = currentNormalCross2.y;
				J3DSys::mCurrentMtx[2][0] = currentNormalCross2.z;

				J3DSys::mCurrentMtx[0][1] = normalDir.x;
				J3DSys::mCurrentMtx[1][1] = normalDir.y;
				J3DSys::mCurrentMtx[2][1] = normalDir.z;

				J3DSys::mCurrentMtx[0][2] = currentNormalCross1.x;
				J3DSys::mCurrentMtx[1][2] = currentNormalCross1.y;
				J3DSys::mCurrentMtx[2][2] = currentNormalCross1.z;

				footMtx[0][0] = currentNormalCross2.x;
				footMtx[1][0] = currentNormalCross2.y;
				footMtx[2][0] = currentNormalCross2.z;

				footMtx[0][1] = normalDir.x;
				footMtx[1][1] = normalDir.y;
				footMtx[2][1] = normalDir.z;

				footMtx[0][2] = currentNormalCross1.x;
				footMtx[1][2] = currentNormalCross1.y;
				footMtx[2][2] = currentNormalCross1.z;
			}
		}
	}

	return 1;
}

Mtx* TMario::getRootAnmMtx() { return mModel->getModel()->mNodeMatrices; }

MtxPtr TMario::getCenterAnmMtx() { return getRootAnmMtx()[unk3C4]; }

f32 TMario::getPumpFrame() const { return mModel->getFrameCtrl(1).getFrame(); }

s16 TMario::getWallAngle() const
{
	return matan(mWallPlane->getNormal().z, mWallPlane->getNormal().x);
}

void TMario::setDivHelm()
{
	if (mCap == nullptr)
		return;
	emitGetEffect();
	startSoundActor(0x1968);
	mCap->setModelActive(TMarioCap::E_CAP_MODEL_HELMET);
}

bool TMario::isWearingCap()
{
	if (mCap == nullptr)
		return true;
	return mCap->isModelActive(TMarioCap::E_CAP_MODEL_HAT);
}

bool TMario::isWearingHelm()
{
	if (mCap == nullptr)
		return false;
	return mCap->isModelActive(TMarioCap::E_CAP_MODEL_HELMET);
}

void TMario::wearGlass()
{
	if (mCap == nullptr)
		return;

	emitGetEffect();
	mCap->setModelActive(TMarioCap::E_CAP_MODEL_SUNGLASSES);
}

void TMario::takeOffGlass()
{
	if (mCap == nullptr)
		return;

	emitGetEffect();
	mCap->setModelInactive(TMarioCap::E_CAP_MODEL_SUNGLASSES);
}

void TMario::setPositions()
{
	MtxPtr root = getRootAnmMtx()[1];
	unk160[0].x = root[0][3];
	unk160[0].y = root[1][3];
	unk160[0].z = root[2][3];

	unk160[1].x = unk1CC;
	unk160[1].y = unk1DC;
	unk160[1].z = unk1EC;

	MtxPtr centerMtx = getCenterAnmMtx();
	unk160[2].x      = centerMtx[0][3];
	unk160[2].y      = centerMtx[1][3];
	unk160[2].z      = centerMtx[2][3];

	MtxPtr takingMtx = getRootAnmMtx()[mBoneIDs[4]];
	unk160[3].x      = takingMtx[0][3];
	unk160[3].y      = takingMtx[1][3];
	unk160[3].z      = takingMtx[2][3];
}

u32 TMario::getTrampleCt()
{
	switch (mAnimationId) {
	case 0xd1:
		return 1;
	case 0xd2:
		return 2;
	case 0xd3:
		return 3;
	}
	return 0;
}

MtxPtr TMario::getTakingMtx() { return getRootAnmMtx()[mBoneIDs[4]]; }

MtxPtr TMario::getTakenMtx() { return mModel->unk8->getBaseTRMtx(); }

f32 TMario::getCurrentFrame(int idx)
{
	return mModel->getFrameCtrl(idx).getFrame();
}

J3DFrameCtrl& TMario::getMotionFrameCtrl() { return mModel->getFrameCtrl(0); }

BOOL TMario::isLast1AnimeFrame()
{
	J3DFrameCtrl& frameCtrl = getMotionFrameCtrl();
	return frameCtrl.getFrame() >= frameCtrl.getEnd() - 1.0f;
}

BOOL TMario::isAnimeLoopOrStop()
{
	if (getMotionFrameCtrl().getState()
	    & (J3DFrameCtrl::STATE_COMPLETED_ONCE
	       | J3DFrameCtrl::STATE_LOOPED_ONCE)) {
		return TRUE;
	}
	return FALSE;
}

// Fabricated - Probably somewhere else
void flagOnAllShapes(J3DModelData* modelData, u32 flag)
{
	for (u16 i = 0; i < modelData->getShapeNum(); ++i) {
		modelData->getShapeNodePointer(i)->unk8 |= flag;
	}
}

// Fabricated - Probably somewhere else
void flagOffAllShapes(J3DModelData* modelData, u32 flag)
{
	for (u16 i = 0; i < modelData->getShapeNum(); ++i) {
		modelData->getShapeNodePointer(i)->offFlag(flag);
	}
}

void TMario::changeHand(int idx)
{
	if (mHandModels[0][0] == nullptr)
		return;
	switch (idx) {
	default:
	case 0:
		// 100% an inline
		mModel->unk8->getModelData()->getShapeNodePointer(5)->offFlag(1);
		mModel->unk8->getModelData()->getShapeNodePointer(6)->offFlag(1);
		flagOnAllShapes(mHandModels[0][0]->getModelData(), 1);
		flagOnAllShapes(mHandModels[0][1]->getModelData(), 1);
		flagOnAllShapes(mHandModels[1][0]->getModelData(), 1);
		flagOnAllShapes(mHandModels[1][1]->getModelData(), 1);
		break;
	case 1:
		// 100% an inline
		mModel->unk8->getModelData()->getShapeNodePointer(5)->onFlag(1);
		mModel->unk8->getModelData()->getShapeNodePointer(6)->onFlag(1);
		flagOffAllShapes(mHandModels[0][0]->getModelData(), 1);
		flagOffAllShapes(mHandModels[0][1]->getModelData(), 1);
		flagOnAllShapes(mHandModels[1][0]->getModelData(), 1);
		flagOnAllShapes(mHandModels[1][1]->getModelData(), 1);
		break;
	case 2:
		// 100% an inline
		mModel->unk8->getModelData()->getShapeNodePointer(5)->onFlag(1);
		mModel->unk8->getModelData()->getShapeNodePointer(6)->onFlag(1);
		flagOnAllShapes(mHandModels[0][0]->getModelData(), 1);
		flagOnAllShapes(mHandModels[0][1]->getModelData(), 1);
		flagOffAllShapes(mHandModels[1][0]->getModelData(), 1);
		flagOffAllShapes(mHandModels[1][1]->getModelData(), 1);
		break;
	}

	flagOnAllShapes(mRHand4ndModel->getModelData(), 1);
}

f32 TMario::setAnimation(int param_1, f32 param_2)
{
	volatile u32 padding[37];
	if (onYoshi()) {
		if (param_1 == 0x5a) {
			mYoshi->changeAnimation(0x12);
		}

		switch (mYoshi->mActor->getCurAnmIdx(0)) {
		case 0x1:
			param_1 = 0xB6;
			break;
		case 0x2:
			param_1 = 0xB7;
			break;
		case 0x3:
			param_1 = 0xB8;
			break;
		case 0x4:
			param_1 = 0xB9;
			break;
		case 0x6:
			param_1 = 0xBF;
			break;
		case 0x7:
			param_1 = 0xBA;
			break;
		case 0x8:
			param_1 = 0xBB;
			break;
		case 0x9:
			param_1 = 0xBC;
			break;
		case 0xA:
			param_1 = 0xBD;
			break;
		case 0xB:
			param_1 = 0xBE;
			break;
		case 0xC:
			param_1 = 0xB8;
			break;
		case 0xE:
			param_1 = 0xC0;
			break;
		case 0xF:
			param_1 = 0xC1;
			break;
		case 0x10:
		case 0x11:
			param_1 = 0xC2;
			break;
		case 0x12:
			param_1 = 0xC4;
			break;
		case 0x13:
			param_1 = 0xC5;
			break;
		case 0x16:
			param_1 = 0xC6;
			break;
		case 0x18:
		default:
			param_1 = 0xB6;
			break;
		}

		bool check = false;
		if (checkFlag(MARIO_FLAG_HAS_FLUDD) && mWaterGun->canSpray()) {
			check = true;
		}

		if (mAction == 0x10001308) {
			check = true;
		}

		if (check == true) {
			param_1 = 0xB8;
		}
	}

	if (param_1 != mAnimationId) {
		mAnimationId = param_1;

		if (onYoshi()) {
			mModel->changeMtxCalcSIAnmBQAnmTransform(0, 0, param_1);
			mModel->unk20->unk18->unk50 = 0.0f;
			getMotionFrameCtrl().setAttribute(
			    mModel->unk4->unk4[param_1]->mAttribute);
			changeHand(0);
			mAnmSound->stop();
		} else {
			mModel->changeMtxCalcSIAnmBQAnmTransform(
			    0, 0, gMarioAnimeData[param_1].unk0);
			mModel->unk20->unk18->unk50 = 0.0f;
			getMotionFrameCtrl().setAttribute(
			    mModel->unk4->unk4[param_1]->mAttribute);
			s8 unk1 = gMarioAnimeData[param_1].unk4;
			if (mTrembleModelEffect != nullptr
			    && (mTrembleModelEffect->checkUnk8(1))) {
				unk1 = 8;
			}
			if (unk1 < 0x18) {
				mModel->changeMtxCalcAnmTransform(0, unk1);
			}

			u32 unk; // SoundId?
			if (param_2 >= 0.0f) {
				unk = -1;
			} else {
				unk = 1;
			}
			mAnmSound->initAnmSound(
			    &mAnmSoundTbl[gMarioAnimeData[param_1].unk0], unk, 0.0f);

			changeHand(gMarioAnimeData[param_1].unk5);
		}
	}
	getMotionFrameCtrl().setRate(param_2 * 0.5f);
	mModel->getFrameCtrl(2).setRate(param_2 * 0.5f);
	return getCurrentFrame(0);
}

f32 TMario::setReverseAnimation(int param_1, f32 param_2)
{
	// volatile u32 padding[4];
	if (param_1 != mAnimationId) {
		setAnimation(param_1, param_2);
		// Tried using existing functions, but couldn make it match
		// I suspect there might be another inline?
		// Maybe a reset FrameCtrl or something
		J3DFrameCtrl* frameCtrl       = mModel->unkC;
		J3DFrameCtrl& motionFrameCtrl = frameCtrl[0];
		motionFrameCtrl.setRate(param_2 * -0.5f);
		motionFrameCtrl.setFrame(motionFrameCtrl.getEnd() - 0.1f);

		J3DFrameCtrl& otherFrameCtrl = frameCtrl[2];
		otherFrameCtrl.setRate(param_2 * -0.5f);
		otherFrameCtrl.setFrame(otherFrameCtrl.getEnd() - 0.1f);
	}
	return getCurrentFrame(0);
}

void TMario::loadBas(void** param_1, const char* param_2)
{
	if (param_2 != nullptr) {
		*param_1 = JKRFileLoader::getGlbResource(param_2);
	} else {
		*param_1 = nullptr;
	}
}

void TMario::loadAnm(J3DAnmTransform** param_1, const char* param_2)
{
	void* data = JKRFileLoader::getGlbResource(param_2);
	if (data != nullptr) {
		*param_1 = (J3DAnmTransform*)J3DAnmLoaderDataBase::load(data);
	}
}

void TMario::loadAnmTexPattern(J3DAnmTexPattern** param_1, char* param_2,
                               J3DModelData* param_3)
{
	void* data = JKRFileLoader::getGlbResource(param_2);
	if (data == nullptr) {
		*param_1 = nullptr;
		return;
	}

	*param_1 = (J3DAnmTexPattern*)J3DAnmLoaderDataBase::load(data);
	(*param_1)->searchUpdateMaterialID(param_3);
}

void TMario::initModel()
{
	unk394         = new J3DDrawBuffer(0x20);
	unk398         = new J3DDrawBuffer(0x20);
	unk39C         = 0;
	unk3A0         = 0;
	mBodyModelData = J3DModelLoaderDataBase::load(
	    JKRFileLoader::getGlbResource("/mario/bmd/ma_mdl1.bmd"), 0x10100000);
	unk3C4       = mBodyModelData->getJointName()->getIndex("center");
	mBoneIDs[0]  = mBodyModelData->getJointName()->getIndex("chn_chest");
	mBoneIDs[1]  = mBodyModelData->getJointName()->getIndex("jnt_chest");
	mBoneIDs[2]  = mBodyModelData->getJointName()->getIndex("jnt_arm_R1");
	mBoneIDs[3]  = mBodyModelData->getJointName()->getIndex("jnt_arm_L1");
	mBoneIDs[4]  = mBodyModelData->getJointName()->getIndex("jnt_hand_R");
	mBoneIDs[5]  = mBodyModelData->getJointName()->getIndex("jnt_hand_L");
	mBoneIDs[6]  = mBodyModelData->getJointName()->getIndex("chn_foot_R");
	mBoneIDs[7]  = mBodyModelData->getJointName()->getIndex("jnt_foot_R");
	mBoneIDs[8]  = mBodyModelData->getJointName()->getIndex("chn_foot_L");
	mBoneIDs[9]  = mBodyModelData->getJointName()->getIndex("jnt_foot_L");
	mBoneIDs[10] = mBodyModelData->getJointName()->getIndex("jnt_head");
	mBoneIDs[11] = mBodyModelData->getJointName()->getIndex("M_head");

	mBodyPollutionTex = (ResTIMG*)JKRFileLoader::getGlbResource(cDirtyFileName);
	if (mBodyPollutionTex != nullptr) {
		SMS_ChangeTextureAll(mBodyModelData, cDirtyTexName, *mBodyPollutionTex);
	}

	J3DModel* bodyModel = new J3DModel(mBodyModelData, 0, 1);

	mHandModels[0][0]
	    = SMS_CreatePartsModel("/mario/bmd/ma_hnd2r.bmd", 0x10100000);
	mHandModels[0][1]
	    = SMS_CreatePartsModel("/mario/bmd/ma_hnd2l.bmd", 0x10100000);
	mHandModels[1][0]
	    = SMS_CreatePartsModel("/mario/bmd/ma_hnd3r.bmd", 0x10100000);
	mHandModels[1][1]
	    = SMS_CreatePartsModel("/mario/bmd/ma_hnd3l.bmd", 0x10100000);
	mRHand4ndModel
	    = SMS_CreatePartsModel("/mario/bmd/ma_hnd4r.bmd", 0x10100000);

	// possible inlines around setting ResTIMG through J3DTexture?
	mHandModels[0][0]->getModelData()->getTexture()->setResTIMG(
	    0, *mBodyModelData->getTexture()->getResTIMG(0));
	DCFlushRange(mHandModels[0][0]->getModelData()->getTexture()->getResTIMG(0),
	             0x20);

	mHandModels[0][1]->getModelData()->getTexture()->setResTIMG(
	    0, *mBodyModelData->getTexture()->getResTIMG(0));
	DCFlushRange(mHandModels[0][1]->getModelData()->getTexture()->getResTIMG(0),
	             0x20);

	mHandModels[1][0]->getModelData()->getTexture()->setResTIMG(
	    0, *mBodyModelData->getTexture()->getResTIMG(0));
	DCFlushRange(mHandModels[1][0]->getModelData()->getTexture()->getResTIMG(0),
	             0x20);
	mHandModels[1][1]->getModelData()->getTexture()->setResTIMG(
	    0, *mBodyModelData->getTexture()->getResTIMG(0));
	DCFlushRange(mHandModels[1][1]->getModelData()->getTexture()->getResTIMG(0),
	             0x20);

	mRHand4ndModel->getModelData()->getTexture()->setResTIMG(
	    0, *mBodyModelData->getTexture()->getResTIMG(0));
	DCFlushRange(mRHand4ndModel->getModelData()->getTexture()->getResTIMG(0),
	             0x20);

	mBodyModelData->getShapeNodePointer(4)->onFlag(1);

	if (mBodyPollutionTex != nullptr) {
		for (int handIdx = 0; handIdx < 2; ++handIdx) {
			for (int modelIdx = 0; modelIdx < 2; ++modelIdx) {
				SMS_ChangeTextureAll(
				    mHandModels[handIdx][modelIdx]->getModelData(),
				    cDirtyTexName, *mBodyPollutionTex);
			}
		}
	}

	J3DAnmTransform** anmTransform = new J3DAnmTransform*[199];
	mAnmSoundTbl                   = new JAIAnimeSound*[199];

	char buffer[0x10C];
	for (int i = 0; i < 199; ++i) {
		snprintf(buffer, 0xff, "/mario/bck/ma_%s.bck", marioAnimeFiles[i].unk4);
		J3DAnmTransform** trans = &anmTransform[i];
		loadAnm(trans, buffer);

		snprintf(buffer, 0xff, "/mario/bas/ma_%s.bas", marioAnimeFiles[i].unk4);
		loadBas((void**)&mAnmSoundTbl[i], buffer);
	}

	J3DAnmTexPattern** anmTexPattern = new J3DAnmTexPattern*[0x18];
	J3DTexNoAnm** anmTexNoAnm        = new J3DTexNoAnm*[0x18];

	for (int i = 0; i < 0x18; ++i) {
		loadAnmTexPattern(&anmTexPattern[i], marioAnimeTexPatternFilenames[i],
		                  mBodyModelData);
		u16 matCount   = anmTexPattern[i]->getUpdateMaterialNum();
		anmTexNoAnm[i] = new J3DTexNoAnm[matCount];

		for (int j = 0; j < matCount; ++i) {
			anmTexNoAnm[i][j].setAnmIndex(j);
			anmTexNoAnm[i][j].setAnmTexPattern(anmTexPattern[i]);
		}
	}

	M3UMtxCalcSIAnmBlendQuat* anmBlendQuat = new M3UMtxCalcSIAnmBlendQuat[2];
	anmBlendQuat[0].unk50                  = 0.0f;
	J3DFrameCtrl* frameCtrl                = new J3DFrameCtrl[3];

	M3UModelCommonMario* marioCommon = new M3UModelCommonMario();

	marioCommon->unk4  = anmTransform;
	marioCommon->unk18 = anmBlendQuat;
	marioCommon->unk8  = anmTexPattern;
	marioCommon->unkC  = anmTexNoAnm;

	u16 eyeIdxL = mBodyModelData->getMaterialName()->getIndex("_mat_eye_L");
	u16 eyeIdxR = mBodyModelData->getMaterialName()->getIndex("_mat_eye_R");

	mBodyModelData->getMaterialNodePointer(eyeIdxL)->change();
	mBodyModelData->getMaterialNodePointer(eyeIdxL)->setMaterialAnm(
	    new J3DMaterialAnm());
	mBodyModelData->getMaterialNodePointer(eyeIdxR)->change();
	mBodyModelData->getMaterialNodePointer(eyeIdxR)->setMaterialAnm(
	    new J3DMaterialAnm());

	unk3D4 = eyeIdxL;
	unk3D6 = eyeIdxR;

	M3UModelMario* modelMario = new M3UModelMario();
	modelMario->unk8          = bodyModel;
	modelMario->unk4          = marioCommon;
	modelMario->unk20         = marioCommon;
	modelMario->unkC          = frameCtrl;

	frameCtrl[2].setRate(SMSGetAnmFrameRate());

	// Something weird is going on around here
	// M3UMarioMtxCalcSetInfo might be a wrong assumption?
	M3UMarioMtxCalcSetInfo* setInfo = new M3UMarioMtxCalcSetInfo();
	setInfo->mJointIdx              = 0;
	setInfo->unk2                   = 2;
	setInfo->mMtxCalcIdx            = 0;
	setInfo->mAnmTransformIdx[0]    = 0x14;
	setInfo->mAnmTransformIdx[1]    = 0x41;
	setInfo->mFrameCtrlIdx          = 0;
	setInfo->unkA                   = mBoneIDs[0]; // Probably wrong?
	setInfo->unkE                   = 0;
	setInfo->unk12                  = 0x100;
	modelMario->unk10               = 2;
	modelMario->unk24               = setInfo;

	u8* unk           = new u8[2];
	unk[0]            = 0;
	unk[1]            = 2;
	modelMario->unk1C = unk;

	modelMario->changeMtxCalcSIAnmBQAnmTransform(0, 0, 0x3e);
	modelMario->changeMtxCalcSIAnmBQAnmTransform(1, 0, 0x41);

	frameCtrl[1].setRate(0.0f);
	marioCommon->unk18[1].unk58 = nullptr;
	mModel                      = modelMario;

	setAnimation(0xC3, 1.0f);

	J3DTransformInfo transformInfo;
	transformInfo.mScale.x     = 1.0f;
	transformInfo.mScale.y     = 1.0f;
	transformInfo.mScale.z     = 1.0f;
	transformInfo.mRotation.x  = mFaceAngle.x;
	transformInfo.mRotation.y  = mFaceAngle.y;
	transformInfo.mRotation.z  = mFaceAngle.z;
	transformInfo.mTranslate.x = mPosition.x;
	transformInfo.mTranslate.y = mPosition.y;
	transformInfo.mTranslate.z = mPosition.z;
	Mtx transform;
	J3DGetTranslateRotateMtx(transformInfo, transform);
	mModel->unk8->setBaseTRMtx(transform);

	mModel->updateInMotion();

	mModel->unk8->calc();

	if (mHandModels[0][0] != nullptr) {
		// Possibly inline since this exact same thing exists in
		// TMario::calcAnim
		mHandModels[0][0]->setBaseTRMtx(mModel->unk8->getAnmMtx(mBoneIDs[4]));
		mHandModels[0][1]->setBaseTRMtx(mModel->unk8->getAnmMtx(mBoneIDs[5]));
		mHandModels[1][0]->setBaseTRMtx(mModel->unk8->getAnmMtx(mBoneIDs[4]));
		mHandModels[1][1]->setBaseTRMtx(mModel->unk8->getAnmMtx(mBoneIDs[5]));
		mRHand4ndModel->setBaseTRMtx(mModel->unk8->getAnmMtx(mBoneIDs[4]));

		mHandModels[0][0]->calc();
		mHandModels[0][1]->calc();
		mHandModels[1][0]->calc();
		mHandModels[1][1]->calc();
		mRHand4ndModel->calc();
	}

	mSurfGesso = nullptr;
	if (gpMarDirector->mMap == 58) {
		if (gpMarDirector->unk7D == 0 || gpMarDirector->unk7D == 1) {
			MActorAnmData* anmData = new MActorAnmData();
			anmData->init("/scene/map/map/Torocco", nullptr);
			mTorocco = new MActor(anmData);

			J3DModelData* toroccoData = J3DModelLoaderDataBase::load(
			    JKRFileLoader::getGlbResource(
			        "/scene/map/map/Torocco/Torocco.bmd"),
			    0x10040000);
			J3DModel* toroccoModel = new J3DModel(toroccoData, 0, 1);
			mTorocco->setModel(toroccoModel, 0);
			if (gpMarDirector->unk7D == 0) {
				mRailType              = 0;
				MActorAnmData* anmData = new MActorAnmData();
				anmData->init("/scene/map/map/Pinna_rail", nullptr);
				mPinaRail = new MActor(anmData);

				J3DModelData* pinaRailData = J3DModelLoaderDataBase::load(
				    JKRFileLoader::getGlbResource(
				        "/scene/map/map/Pinna_rail/Pinna_rail.bmd"),
				    0x10040000);
				J3DModel* pinaRailModel = new J3DModel(pinaRailData, 0, 1);
				mPinaRail->setModel(pinaRailModel, 0);

				mPinaRail->getFrameCtrl(0)->setRate(0.5f);

				Mtx pinnaMtx;
				MTXIdentity(pinnaMtx);
				mPinaRail->getModel()->setBaseTRMtx(pinnaMtx);
				mPinaRail->calcAnm();
				mPinaRail->getModel()->setBaseTRMtx(
				    mTorocco->getModel()->getAnmMtx(0));
			}
			if (gpMarDirector->unk7D == 1) {
				mRailType              = 1;
				MActorAnmData* anmData = new MActorAnmData();
				anmData->init("/scene/map/map/Koopa_rail", nullptr);
				mKoopaRail = new MActor(anmData);

				J3DModelData* pinaRailData = J3DModelLoaderDataBase::load(
				    JKRFileLoader::getGlbResource(
				        "/scene/map/map/Koopa_rail/Koopa_rail.bmd"),
				    0x10040000);
				J3DModel* pinaRailModel = new J3DModel(pinaRailData, 0, 1);
				mKoopaRail->setModel(pinaRailModel, 0);

				mKoopaRail->getFrameCtrl(0)->setRate(0.5f);

				Mtx koopaMtx;
				MTXIdentity(koopaMtx);
				mKoopaRail->getModel()->setBaseTRMtx(koopaMtx);
				mKoopaRail->calcAnm();
				mKoopaRail->getModel()->setBaseTRMtx(
				    mTorocco->getModel()->getAnmMtx(0));
			}
			unk118 |= MARIO_FLAG_HAS_FLUDD;
			mTorocco->calcAnm();
			MtxPtr toroccoMtx = mTorocco->getModel()->getAnmMtx(2);
			mPosition.x       = toroccoMtx[0][3];
			mPosition.y       = toroccoMtx[1][3];
			mPosition.z       = toroccoMtx[2][3];
			mToroccoAngle     = mFaceAngle.y;
		}
	} else {
		mTorocco   = nullptr;
		mPinaRail  = nullptr;
		mKoopaRail = nullptr;
	}

	mMultiMtxEffect                 = new TMultiMtxEffect();
	mMultiMtxEffect->mNumBones      = 3;
	u16* boneIds                    = new u16[3];
	boneIds[0]                      = mBoneIDs[0];
	boneIds[1]                      = mBoneIDs[2];
	boneIds[2]                      = mBoneIDs[3];
	mMultiMtxEffect->mBoneIDs       = boneIds;
	u8* mtxTypes                    = new u8[3];
	mtxTypes[0]                     = 0;
	mtxTypes[1]                     = 0;
	mtxTypes[2]                     = 0;
	mMultiMtxEffect->mMtxEffectType = mtxTypes;
	mMultiMtxEffect->setup(mModel->unk8, "Mario");

	mTrembleModelEffect = new TTrembleModelEffect();
	mTrembleModelEffect->init(mModel->unk8);

	unk3A4 = new SampleCtrlModelData(mBodyModelData);
}

void TMario::initMirrorModel()
{
	if (unk388 != 0) {
		return;
	}

	if (mCap != nullptr) {
		mCap->createMirrorCap();
	}
	TMirrorActor* mirrorActor = new TMirrorActor("マリオin鏡");
	mirrorActor->init(mModel->getModel(), 4);

	for (int handIdx = 0; handIdx < 2; ++handIdx) {
		for (int modelIdx = 0; modelIdx < 2; ++modelIdx) {
			TMirrorActor* handMirrorActor = new TMirrorActor("マリオ手in鏡");
			handMirrorActor->init(mHandModels[handIdx][modelIdx], 4);
		}
	}

	if (mTorocco != nullptr) {
		TMirrorActor* toroccoMirrorActor = new TMirrorActor("トロッコin鏡");
		toroccoMirrorActor->init(mTorocco->getModel(), 2);
	}
}

void TMario::finalDrawInitialize()
{
	// volatile u32 padding[10];
	changeHand(0);
	SMS_MakeDLAndLock(mModel->unk8);

	// Something feels weird around here
	int packetId = 0;
	for (int i = 0; i < mBodyModelData->getMaterialNum(); ++i) {
		if (i == unk3D4 || i == unk3D6) {
			mModel->unk8->mMatPackets[packetId].offFlag(0x1);
		}
		packetId++;
	}

	for (int i = 0; i < mBodyModelData->getMaterialNum(); ++i) {
		SMS_InitPacket_OneTevKColorAndFog(mModel->unk8, i, GX_KCOLOR0, nullptr);
	}
}

bool TMario::isUpperPumpingStyle() const
{
	if (unk380 == 0 || unk380 == 1) {
		return true;
	}
	return false;
}

void TMario::considerWaist()
{
	// volatile u32 padding[6];
	f32 maxPitch;
	f32 targetPitch;
	f32 angleChangeRate;

	// Possibly unused get params function?
	if (checkActionFlag(0x10000)) {
		if (mGroundPlane->isWaterSurface()) {
			maxPitch    = mSurfingParamsWaterRed.mWaistPitchMax.get();
			targetPitch = mSurfingParamsWaterRed.mWaistPitch.get();
			angleChangeRate
			    = mSurfingParamsWaterRed.mWaistAngleChangeRate.get();
		} else {
			maxPitch    = mSurfingParamsGroundRed.mWaistPitchMax.get();
			targetPitch = mSurfingParamsGroundRed.mWaistPitch.get();
			angleChangeRate
			    = mSurfingParamsGroundRed.mWaistAngleChangeRate.get();
		}
	} else {
		if (fabricatedIsPumping() || isHolding()) {
			maxPitch    = mBodyAngleParamsWaterGun.mWaistPitchMax.get();
			targetPitch = mBodyAngleParamsWaterGun.mWaistPitch.get();
			angleChangeRate
			    = mBodyAngleParamsWaterGun.mWaistAngleChangeRate.get();
		} else {
			maxPitch        = mBodyAngleParamsFree.mWaistPitchMax.get();
			targetPitch     = mBodyAngleParamsFree.mWaistPitch.get();
			angleChangeRate = mBodyAngleParamsFree.mWaistAngleChangeRate.get();
		}
	}

	// Likely clamp inline, but i couldn't find an existing that worked
	f32 targetPitchCopy = targetPitch * mForwardVel;
	if (targetPitchCopy > maxPitch) {
		targetPitchCopy = maxPitch;
	}
	if (targetPitchCopy < -maxPitch) {
		targetPitchCopy = -maxPitch;
	}
	targetPitch = targetPitchCopy;
	unk3DC += angleChangeRate * (targetPitch - unk3DC);

	f32 rollMax;
	f32 targetRoll;
	s16 diffAngle = mFaceAngle.y - unk9C;
	// Possibly unused get params function?
	if (checkActionFlag(0x10000)) {
		if (mGroundPlane->isWaterSurface()) {
			rollMax    = mSurfingParamsWaterRed.mWaistRollMax.get();
			targetRoll = mSurfingParamsWaterRed.mWaistRoll.get();
			angleChangeRate
			    = mSurfingParamsWaterRed.mWaistAngleChangeRate.get();
		} else {
			rollMax    = mSurfingParamsGroundRed.mWaistRollMax.get();
			targetRoll = mSurfingParamsGroundRed.mWaistRoll.get();
			angleChangeRate
			    = mSurfingParamsGroundRed.mWaistAngleChangeRate.get();
		}
	} else {
		if (fabricatedIsPumping()) {
			rollMax    = mBodyAngleParamsWaterGun.mWaistRollMax.get();
			targetRoll = mBodyAngleParamsWaterGun.mWaistRoll.get();
			angleChangeRate
			    = mBodyAngleParamsWaterGun.mWaistAngleChangeRate.get();
		} else {
			rollMax         = mBodyAngleParamsFree.mWaistRollMax.get();
			targetRoll      = mBodyAngleParamsFree.mWaistRoll.get();
			angleChangeRate = mBodyAngleParamsFree.mWaistAngleChangeRate.get();
		}
	}

	// Likely clamp inline, but i couldn't find an existing that worked
	f32 targetRollCopy = targetRoll * diffAngle * mForwardVel;
	if (targetRollCopy > rollMax) {
		targetRollCopy = rollMax;
	}
	if (targetRollCopy < -rollMax) {
		targetRollCopy = -rollMax;
	}
	targetRoll = targetRollCopy;

	unk3D8 += angleChangeRate * (targetRoll - unk3D8);
}

// This needs work!
void TMario::calcBaseMtx(MtxPtr mtx)
{
	if (mAction == 0x800447) {
		if (mRailType == 0) {
			mPinaRail->calcAnm();
			MTXCopy(mPinaRail->getModel()->getAnmMtx(0),
			        mTorocco->getModel()->getBaseTRMtx());
		}
		if (mRailType == 1) {
			mKoopaRail->calcAnm();
			MTXCopy(mKoopaRail->getModel()->getAnmMtx(0),
			        mTorocco->getModel()->getBaseTRMtx());
		}

		mTorocco->calcAnm();
		Mtx transform;
		MsMtxSetRotRPH(transform, 0.0f, SHORTANGLE2DEG(mToroccoAngle), 0.0f);
		MTXConcat(mTorocco->getModel()->getAnmMtx(2), transform, mtx);

		mToroccoPos = mPosition;
		mPosition.x = mtx[0][3];
		mPosition.y = mtx[1][3];
		mPosition.z = mtx[2][3];
	} else if (checkActionFlag(0x100000)) {
		J3DTransformInfo ti;
		ti.mScale.x     = 1.0f;
		ti.mScale.y     = 1.0f;
		ti.mScale.z     = 1.0f;
		ti.mRotation.x  = 0;
		ti.mRotation.y  = mModelFaceAngle;
		ti.mRotation.z  = 0;
		f32 radiusAtY   = mHolder->getRadiusAtY(mPosition.y);
		ti.mTranslate.x = mPosition.x - (radiusAtY * JMASSin(mModelFaceAngle));
		ti.mTranslate.y = mPosition.y;
		ti.mTranslate.z = mPosition.z - radiusAtY * JMASCos(mModelFaceAngle);
		J3DGetTranslateRotateMtx(ti, mtx);

	} else {
		if (!checkActionFlag(0x2000)) {
			J3DTransformInfo ti;
			ti.mScale.x     = 1.0f;
			ti.mScale.y     = 1.0f;
			ti.mScale.z     = 1.0f;
			ti.mRotation.x  = 0;
			ti.mRotation.y  = mModelFaceAngle;
			ti.mRotation.z  = 0;
			ti.mTranslate.x = mPosition.x;
			ti.mTranslate.y = mPosition.y;
			ti.mTranslate.z = mPosition.z;
			ti.mTranslate.y += gpMapObjWave->getHeight(
			                       mPosition.x, mFloorPosition.z, mPosition.z)
			                   - mFloorPosition.z;
			J3DGetTranslateRotateMtx(ti, mtx);
		} else {
			if (mHolder != nullptr && mHolder->getTakingMtx() != nullptr) {
				MTXCopy(mHolder->getTakingMtx(), mtx);
			} else {

				if (mAction != 0x80088A && mAction != 0x10000358) {
					mFaceAngle.x = 0;
				}
				// Probably another checkFlag inline
				if ((unk114 & 8) ? true : false) {
					// This is too many inlines for me to try even figure out,
					// so i won't even attempt it rn...
					// Keeping my working draft, but it is 100% wrong

					// JGeometry::TVec3<f32> norm1;
					// JGeometry::TVec3<f32> norm2;
					// JGeometry::TVec3<f32> norm3;
					// norm1.x
					//     = JMASSin(mFaceAngle.y + 0x2AAA) * 40.0f +
					//     mPosition.x;
					// norm1.z
					//     = JMASCos(mFaceAngle.y + 0x2AAA) * 40.0f +
					//     mPosition.z;
					// norm2.x
					//     = JMASSin(mFaceAngle.y + 0x8000) * 40.0f +
					//     mPosition.x;
					// norm2.z
					//     = JMASCos(mFaceAngle.y + 0x8000) * 40.0f +
					//     mPosition.z;
					// norm3.x
					//     = JMASSin(mFaceAngle.y + 0xD555) * 40.0f +
					//     mPosition.x;
					// norm3.z
					//     = JMASCos(mFaceAngle.y + 0xD555) * 40.0f +
					//     mPosition.z;

					// const TBGCheckData* unkResultData1;
					// checkGroundPlane(norm1.x, mPosition.y + 160.0f, norm1.z,
					//                  &norm1.y, &unkResultData1);
					// checkGroundPlane(norm2.x, mPosition.y + 160.0f, norm2.z,
					//                  &norm2.y, &unkResultData1);
					// checkGroundPlane(norm3.x, mPosition.y + 160.0f, norm3.z,
					//                  &norm3.y, &unkResultData1);

					// if (norm1.y - mPosition.y < -120.0f) {
					// 	norm1.y = mPosition.y;
					// }
					// if (norm2.y - mPosition.y < -120.0f) {
					// 	norm2.y = mPosition.y;
					// }
					// if (norm3.y - mPosition.y < -120.0f) {
					// 	norm3.y = mPosition.y;
					// }

					// f32 avg = (norm1.y + norm2.y + norm3.y) / 3.0f;

					// JGeometry::TVec3<f32> prod;
					// prod.cross(norm3, norm2);
					// prod.cross(prod, norm1);
					// prod.normalize();
				} else {
					J3DTransformInfo ti;
					ti.mScale.x     = 1.0f;
					ti.mScale.y     = 1.0f;
					ti.mScale.z     = 1.0f;
					ti.mRotation.x  = mFaceAngle.x;
					ti.mRotation.y  = mModelFaceAngle;
					ti.mRotation.z  = mFaceAngle.z;
					ti.mTranslate.x = mPosition.x;
					ti.mTranslate.y = mPosition.y;
					ti.mTranslate.z = mPosition.z;
					J3DGetTranslateRotateMtx(ti, mtx);
				}
			}
		}
	}
}

void TMario::addCallBack(JDrama::TGraphics* graphics)
{
	// volatile u32 padding[27];
	gpMarioForCallBack      = this;
	J3DModelData* modelData = mModel->unk8->getModelData();
	if (isMario()) {
		modelData->getJointNodePointer(mBoneIDs[10])
		    ->setCallBack(MarioHeadCtrl);
	}

	modelData->getJointNodePointer(mBoneIDs[1])->setCallBack(MarioWaistCtrl);

	if (0x4B0 > gpMarDirector->unk58 || fabricatedUnk380Inline()) {
		if (mMultiMtxEffect != nullptr) {
			mMultiMtxEffect->flagOff();
		}

		if (checkFlag(MARIO_FLAG_HAS_FLUDD)) {
			mWaterGun->unk1CDC->mMtxEffectTbl[1]->mFlags &= ~1;
		}

		if (mCap != nullptr) {
			mCap->mtxEffectHide();
		}
	} else {
		if ((gMarioAnimeData[mAnimationId].unk6 & 2) != 0 && unk380 == 5) {
			if (mMultiMtxEffect != nullptr) {
				mMultiMtxEffect->flagOn();
			}
		} else {
			if (mMultiMtxEffect != nullptr) {
				mMultiMtxEffect->flagOff();
			}
		}

		if (mCap != nullptr) {
			mCap->mtxEffectShow();
		}
	}

	if ((gMarioAnimeData[mAnimationId].unk6 & 4) != 0 && unk380 == 5) {
		if (checkFlag(MARIO_FLAG_HAS_FLUDD)) {
			mWaterGun->unk1CDC->mMtxEffectTbl[1]->mFlags |= 1;
		}
	} else if (checkFlag(MARIO_FLAG_HAS_FLUDD)) {
		mWaterGun->unk1CDC->mMtxEffectTbl[1]->mFlags &= ~1;
	}

	if ((graphics->unk0 & 2) != 0) {
		modelData->getJointNodePointer(mBoneIDs[6])
		    ->setCallBack(MarioFootPosRCtrl);
		modelData->getJointNodePointer(mBoneIDs[7])
		    ->setCallBack(MarioFootDirRCtrl);
		modelData->getJointNodePointer(mBoneIDs[8])
		    ->setCallBack(MarioFootPosLCtrl);
		modelData->getJointNodePointer(mBoneIDs[9])
		    ->setCallBack(MarioFootDirLCtrl);
	} else {
		modelData->getJointNodePointer(mBoneIDs[6])->setCallBack(nullptr);
		modelData->getJointNodePointer(mBoneIDs[7])->setCallBack(nullptr);
		modelData->getJointNodePointer(mBoneIDs[8])->setCallBack(nullptr);
		modelData->getJointNodePointer(mBoneIDs[9])->setCallBack(nullptr);
	}
}

void TMario::setUpperDamageRun()
{

	J3DFrameCtrl& frameCtrl      = mModel->getFrameCtrl(1);
	mModel->unk24[1].mMtxCalcIdx = 1;
	mModel->changeMtxCalcSIAnmBQAnmTransform(1, 0, 0x4a);
	frameCtrl.setFrame(frameCtrl.getStart());
	frameCtrl.setRate(1.0f);
	frameCtrl.setRate(0.5f);
	unk380 = 4;
}

void TMario::addUpper()
{
	// volatile u32 padding[17];
	J3DFrameCtrl& frameCtrl = mModel->getFrameCtrl(1);
	if (unk380 != 4) {
		switch (unk380) {
		case 0:
		case 1:
			if (onYoshi()) {
				mModel->unk24[1].mMtxCalcIdx = 0xff;
				return;
			}
			if (gMarioAnimeData[mAnimationId].unk2 != 0xC8) {
				mModel->unk24[1].mMtxCalcIdx = 1;
				mModel->changeMtxCalcSIAnmBQAnmTransform(
				    1, 0, gMarioAnimeData[mAnimationId].unk2);
			}
			break;
		case 2:
			mModel->unk24[1].mMtxCalcIdx = 1;
			mModel->changeMtxCalcSIAnmBQAnmTransform(1, 0, 0x4b);
			break;
		case 5:
			mModel->unk24[1].mMtxCalcIdx = 0xff;
			break;
		}

		if (unk380 == 0) {
			frameCtrl.setAttribute(J3DFrameCtrl::ATTR_PING_PONG_LOOP);
			unk348
			    = mGamePad->mCompSPos[3] * mUpperBodyParams.mPumpAnmSpeed.get();
			if (mWaterGun->mCurrentWater <= 0) {
				if (frameCtrl.getRate() >= 0.0f) {
					frameCtrl.setRate(unk348);
					if (frameCtrl.checkPass(8.0f)) {
						startSoundActor(0x816);
					}
				} else {
					frameCtrl.setRate(-unk348);
					if (frameCtrl.checkPass(13.0f)) {
						startSoundActor(0x817);
					}
				}
			}
		}
	}
}

void TMario::calcAnim(u32 param_1, JDrama::TGraphics* graphics)
{
	addCallBack(graphics);
	addUpper();
	Mtx baseMtx;
	calcBaseMtx(baseMtx);
	considerWaist();
	MTXCopy(baseMtx, mModel->unk8->getBaseTRMtx());
	mModel->perform(param_1, graphics);
	gpMarioForCallBack = nullptr;

	J3DModelData* modelData = mModel->unk8->getModelData();
	modelData->getJointNodePointer(mBoneIDs[10])->setCallBack(nullptr);
	modelData->getJointNodePointer(mBoneIDs[1])->setCallBack(nullptr);
	modelData->getJointNodePointer(mBoneIDs[6])->setCallBack(nullptr);
	modelData->getJointNodePointer(mBoneIDs[7])->setCallBack(nullptr);
	modelData->getJointNodePointer(mBoneIDs[8])->setCallBack(nullptr);
	modelData->getJointNodePointer(mBoneIDs[9])->setCallBack(nullptr);

	if (mHandModels[0][0] != nullptr) {
		mHandModels[0][0]->setBaseTRMtx(mModel->unk8->getAnmMtx(mBoneIDs[4]));
		mHandModels[0][1]->setBaseTRMtx(mModel->unk8->getAnmMtx(mBoneIDs[5]));
		mHandModels[1][0]->setBaseTRMtx(mModel->unk8->getAnmMtx(mBoneIDs[4]));
		mHandModels[1][1]->setBaseTRMtx(mModel->unk8->getAnmMtx(mBoneIDs[5]));
		mRHand4ndModel->setBaseTRMtx(mModel->unk8->getAnmMtx(mBoneIDs[4]));

		mHandModels[0][0]->calc();
		mHandModels[0][1]->calc();
		mHandModels[1][0]->calc();
		mHandModels[1][1]->calc();
		mRHand4ndModel->calc();
	}

	if (mCap != nullptr) {
		mCap->unkC->setBaseTRMtx(mModel->unk8->getAnmMtx(mBoneIDs[11]));
		mCap->unk10[2]->setBaseTRMtx(mModel->unk8->getAnmMtx(mBoneIDs[10]));
		mCap->unk10[3]->setBaseTRMtx(mModel->unk8->getAnmMtx(mBoneIDs[11]));
		mCap->perform(2, graphics);
	}

	if (checkActionFlag(0x10000)) {
		mSurfGesso->getModel()->setBaseTRMtx(mModel->unk8->getBaseTRMtx());
		mSurfGesso->perform(2, graphics);
	}

	if (mAction == 0x800447) {
		mTorocco->perform(2, graphics);
	}

	if (mYoshi != nullptr) {
		MActor* yoshiActor = mYoshi->mActor;
		if (yoshiActor->getCurAnmIdx(0) == 0xf) {
			if (yoshiActor->unkC != nullptr) {
				yoshiActor->unkC->initNormalMotionBlend();
			}
			f32 blendRatio = unk414.z;
			// Some weird stuff here
			if (yoshiActor->unkC != nullptr) {
				yoshiActor->unkC->setMotionBlendRatio(blendRatio);
			}
			yoshiActor->getFrameCtrl(0)->setRate(
			    getMotionFrameCtrl().getRate());
		} else {
			if (yoshiActor->unkC != nullptr) {
				yoshiActor->unkC->initNormalMotionBlend();
			}
			if (yoshiActor->unkC != nullptr) {
				yoshiActor->unkC->setMotionBlendRatio(0.0f);
			}

			yoshiActor->getFrameCtrl(0)->setRate(0.5f);
		}
	}

	if (mAction == 0x4000440 && mAnimationId != 0x92) {
		f32 blendRatio = unk414.z;
		if (blendRatio < 0.3f) {
			changeHand(2);
		}
		if (0.3f <= blendRatio && blendRatio <= 0.7f) {
			changeHand(1);
		}
		if (0.7f < blendRatio) {
			changeHand(0);
		}
	}

	if (mAnimationId == 0x12d) {
		mModel->unk8->getModelData()->getShapeNodePointer(5)->onFlag(1);
		mModel->unk8->getModelData()->getShapeNodePointer(6)->onFlag(1);
		flagOnAllShapes(mHandModels[0][0]->getModelData(), 1);
		flagOnAllShapes(mHandModels[0][1]->getModelData(), 1);
		flagOffAllShapes(mHandModels[1][1]->getModelData(), 1);

		if (getMotionFrameCtrl().getFrame() < 158.0f) {
			flagOnAllShapes(mHandModels[1][0]->getModelData(), 1);
			flagOffAllShapes(mRHand4ndModel->getModelData(), 1);
		} else {
			flagOffAllShapes(mHandModels[1][0]->getModelData(), 1);
			flagOnAllShapes(mRHand4ndModel->getModelData(), 1);
		}
	}
}

void TMario::calcView(JDrama::TGraphics* graphics)
{
	// volatile u32 padding[4];
	MTXCopy(graphics->mViewMtx, j3dSys.mViewMtx);
	mModel->unk8->viewCalc();
	if (mHandModels[0][0] != nullptr) {
		mHandModels[0][0]->viewCalc();
		mHandModels[0][1]->viewCalc();
		mHandModels[1][0]->viewCalc();
		mHandModels[1][1]->viewCalc();
		mRHand4ndModel->viewCalc();
	}

	if (mCap != nullptr) {
		mCap->perform(4, graphics);
	}

	if (checkActionFlag(0x10000)) {
		mSurfGesso->perform(4, graphics);
	}

	if (mAction == 0x800447) {
		mTorocco->perform(4, graphics);
	}
}

void TMario::entryModels(JDrama::TGraphics* graphics)
{
	mModel->perform(0x200, graphics);
	if (mHandModels[0][0] != nullptr) {
		mHandModels[0][0]->entry();
		mHandModels[0][1]->entry();
		mHandModels[1][0]->entry();
		mHandModels[1][1]->entry();
		mRHand4ndModel->entry();
	}

	if (mCap != nullptr) {
		mCap->perform(0x200, graphics);
	}

	if (checkActionFlag(0x10000)) {
		mSurfGesso->setLightData(mGroundPlane, mPosition);
		mSurfGesso->perform(0x200, graphics);
	}

	if (mAction == 0x800447) {
		mTorocco->perform(0x200, graphics);
	}
}

void TMario::drawSpecial(JDrama::TGraphics* graphics)
{
	if (mAction == 0x20338) {
		unk4EC = 1;
		unk114 |= 0x20;
	} else {
		unk4EC = 0;
		unk114 &= ~0x20;
	}
	// Probably some enum? I see no reason why this is a switch...
	switch (unk4EC) {
	case 1:
		drawLogic();
		break;
	}
}

void TMario::drawLogic()
{
	// volatile u32 padding[2];
	j3dSys.mFlags |= 2;
	j3dSys.unk4C = 7;
	SMS_DrawInit();
	GXSetNumChans(1);
	GXSetChanCtrl(GX_COLOR0A0, FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetChanCtrl(GX_COLOR1A1, FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetChanMatColor(GX_COLOR0A0, (GXColor) { 0xff, 0xff, 0xff, 0xff });
	GXSetNumTexGens(0);
	GXSetNumTevStages(1);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
	GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
	GXSetBlendMode(GX_BM_LOGIC, GX_BL_ONE, GX_BL_ZERO,
	               (GXLogicOp)mBlendLogicOp);
	GXSetZCompLoc(TRUE);
	GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
	GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
	GXSetColorUpdate(GX_FALSE);
	GXSetAlphaUpdate(GX_FALSE);
	unk394->draw();
	unk398->draw();
	GXSetColorUpdate(GX_TRUE);
	GXSetAlphaUpdate(GX_FALSE);
	if (unk114 & 0x20 ? true : false) {
		unk394->draw();
		unk398->draw();
	}

	j3dSys.mFlags &= ~2;
}

void TMario::boxDrawPrepare(MtxPtr mtx)
{
	// Some weird stack stuff happening here
	f32 psave[7];
	GXGetProjectionv(psave);

	f32 wpsave[5];
	GXGetViewportv(wpsave);

	GXProject(mPosition.x, mPosition.y + 80.0f, mPosition.z, mtx, psave, wpsave,
	          &mMarioScreenPos.x, &mMarioScreenPos.y, &mMarioScreenPos.z);

	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);
	GXSetCurrentMtx(0);

	Mtx stackMtx;
	MTXScale(stackMtx, 200.0f, 200.0f, 200.0f);

	MTXConcat(mtx, stackMtx, stackMtx);

	GXLoadPosMtxImm(stackMtx, 0);
	GXSetCullMode(GX_CULL_BACK);
	GXSetNumChans(1);
	GXSetChanCtrl(GX_COLOR0A0, FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetChanCtrl(GX_COLOR1A1, FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);

	GXSetChanMatColor(GX_COLOR0A0, (GXColor) { 0xff, 0xff, 0xff, 0xff });
	GXSetNumTexGens(0);
	GXSetNumTevStages(1);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
	GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
	GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ZERO, GX_LO_NOOP);
	GXSetZCompLoc(TRUE);
	GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
	GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
}

void TMario::addDirty()
{

	for (u16 i = 0; i < mBodyModelData->getMaterialNum(); ++i) {
		J3DGXColor* konstColor = mBodyModelData->getMaterialNodePointer(i)
		                             ->getTevBlock()
		                             ->getTevKColor(0);
		konstColor->color.a = unk134;
	}

	if (mHandModels[0][0] != nullptr) {
		for (int handIdx = 0; handIdx < 2; ++handIdx) {
			for (int modelIdx = 0; modelIdx < 2; ++modelIdx) {
				J3DModelData* mHandModelData
				    = mHandModels[handIdx][modelIdx]->getModelData();
				for (u16 i = 0; i < mHandModelData->getMaterialNum(); ++i) {
					J3DGXColor* konstColor
					    = mHandModelData->getMaterialNodePointer(i)
					          ->getTevBlock()
					          ->getTevKColor(0);
					konstColor->color.a = unk134;
				}
			}
		}
	}
}

void TMario::addDamageFog(JDrama::TGraphics* graphics)
{
	// volatile u32 padding[30];
	bool check       = true;
	GXColor fogColor = (GXColor) { 0xff, 0x00, 0x80, 0xff };

	// Inlined "bool getFogColor(GXColor* color)"?
	if (unk14C == 0) {
		check = false;
	}
	if (checkFlag(MARIO_FLAG_UNK_40) && unk350 == 2) {
		check = true;
		if (unk34E > mDirtyParams.mFogTimeYellow.get()) {
			fogColor = (GXColor) { 0x82, 0x96, 0x00, 0xff };
		} else {
			fogColor = (GXColor) { 0xff, 0x00, 0x80, 0xff };
		}
	}

	if (checkFlag(MARIO_FLAG_NPC_TALKING)) {
		check = false;
	}

	if (check == true) {
		// Very likely an inline since it is duplicated
		J3DModelData* modelData = mModel->unk8->mModelData;
		for (u16 i = 0; i < modelData->getMaterialNum(); ++i) {
			J3DFog* fog
			    = modelData->getMaterialNodePointer(i)->getPEBlock()->getFog();
			fog->mColor = fogColor;
		}

		SMS_AddDamageFogEffect(mModel->unk8->getModelData(), mPosition,
		                       graphics);

		if (mCap != nullptr) {
			J3DModelData* mCapModelData = mCap->unkC->getModelData();
			for (u16 i = 0; i < mCapModelData->getMaterialNum(); ++i) {
				J3DFog* fog = mCapModelData->getMaterialNodePointer(i)
				                  ->getPEBlock()
				                  ->getFog();
				fog->mColor = fogColor;
			}
			SMS_AddDamageFogEffect(mCap->unkC->getModelData(), mPosition,
			                       graphics);
		}

		if (mHandModels[0][0] != nullptr) {
			for (int handIdx = 0; handIdx < 2; ++handIdx) {
				for (int modelIdx = 0; modelIdx < 2; ++modelIdx) {
					J3DModelData* mHandModelData
					    = mHandModels[handIdx][modelIdx]->getModelData();
					for (u16 i = 0; i < mHandModelData->getMaterialNum(); ++i) {
						J3DFog* fog = mHandModelData->getMaterialNodePointer(i)
						                  ->getPEBlock()
						                  ->getFog();
						fog->mColor = fogColor;
					}
					SMS_AddDamageFogEffect(
					    mHandModels[handIdx][modelIdx]->getModelData(),
					    mPosition, graphics);
				}
			}
			SMS_AddDamageFogEffect(mRHand4ndModel->getModelData(), mPosition,
			                       graphics);
		}

	} else {
		SMS_ResetDamageFogEffect(mModel->unk8->getModelData());
		if (mCap != nullptr) {
			SMS_ResetDamageFogEffect(mCap->unkC->getModelData());
		}
		if (mHandModels[0][0] != nullptr) {
			for (int handIdx = 0; handIdx < 2; ++handIdx) {
				for (int modelIdx = 0; modelIdx < 2; ++modelIdx) {
					SMS_ResetDamageFogEffect(
					    mHandModels[handIdx][modelIdx]->getModelData());
				}
			}
			SMS_ResetDamageFogEffect(mRHand4ndModel->mModelData);
		}
	}
}
