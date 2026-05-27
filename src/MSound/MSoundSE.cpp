#include <MSound/MSoundSE.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSRandVol.hpp>
#include <MSound/MSSetSound.hpp>
#include <JSystem/JAudio/JALibrary/JALSystem.hpp>
#include <JSystem/JAudio/JAInterface/JAIConst.hpp>
#include <MarioUtil/MapUtil.hpp>
#include <System/MSoundMainSide.hpp>
#include <Camera/CubeManagerBase.hpp>

// rogue
#include <MSound/MSoundBGM.hpp>

using namespace MSoundSESystem;

JSUList<MSRandVol> MSRandVol::smList;

JSUList<MSRandPlay> MSRandPlay::smList;

SeInfo SeInfo::smSeSetting(1.0f, 0.9f);

MSoundSE* MSoundSE::mObj = 0;

void MSRandVol::construct(u32 param)
{
	smList.append(&(new MSRandVol(param))->unk4);
}

MSRandVol::MSRandVol(u32 param)
    : unk4(this)
    , unk14(param)
    , unk18(0.5f)
{
	unk1C[0] = 0.0f;
	unk1C[1] = 0.25f;
	unk1C[2] = 0.5f;
	unk1C[3] = 0.75f;
	unk2C[0] = 1.0f;
	unk2C[1] = 1.5f;
	unk2C[2] = 2.0f;
	unk2C[3] = 4.0f;
	unk3C[0] = 0.25f;
	unk3C[1] = 0.5f;
	unk3C[2] = 0.75f;
	unk3C[3] = 1.0f;
}

u32 MSRandVol::getRandomVolume(u32 param_1, u32 param_2) { }

f32 MSRandVol::getRandVol(u32 param_1)
{
	f32 d = JALCalc::getRandom(unk3C[param_1 >> 24 & 0xC] * unk18,
	                           unk2C[param_1 >> 22 & 0xC],
	                           unk1C[param_1 >> 20 & 0xC])
	        + 1.0f;

	if (d < 0.0f)
		return 0.0f;
	if (d > 2.0f)
		return 2.0f;
	return d;
}

void MSRandPlay::construct(u32 param_1, s32 param_2, s32 param_3, f32 param_4,
                           f32 param_5)
{
	smList.append(
	    &(new MSRandPlay(param_1, param_2, param_3, param_4, param_5))->unk0);
}

int MSRandPlay::registerTrans(u32 param, const Vec* vec)
{
	for (JSUListIterator<MSRandPlay> it = smList.getFirst();
	     it != smList.getEnd(); ++it) {
		if (param == it.getObject()->unk1C) {
			return it.getObject()->registerTransDynamic(vec);
		}
	}

	return -1;
}

int MSRandPlay::registerTransDynamic(const Vec* vec)
{
	unk10[unk16].unk0 = vec;
	return unk16++;
}

void MSRandPlay::createRandPlayVec(u32 param_1, u16 param_2)
{
	JSUListIterator<MSRandPlay> it;
	for (it = MSRandPlay::smList.getFirst(); it != MSRandPlay::smList.getEnd();
	     ++it) {
		if (param_1 == it.getObject()->unk1C) {
			it.getObject()->createRandPlayVecDynamic(param_2);
			break;
		}
	}
}

void MSRandPlay::createRandPlayVecDynamic(u16 param_1)
{
	unk10 = new MSRandPlayVec[param_1];
	unk14 = param_1;
	unk16 = 0;
}

void MSRandPlay::startSeRandPlay(u32 param1, u32 param2)
{
	JSUListIterator<MSRandPlay> it;
	for (it = MSRandPlay::smList.getFirst(); it != MSRandPlay::smList.getEnd();
	     ++it) {
		if (param1 == it.getObject()->unk1C) {
			it.getObject()->randPlay(param2);
			break;
		}
	}
}

MSRandPlay::MSRandPlay(u32 param_1, s32 param_2, s32 param_3, f32 param_4,
                       f32 param_5)
    : unk0(this)
    , unk10(0)
    , unk14(0)
    , unk16(0)
    , unk1C(param_1)
    , unk20(param_2)
    , unk24(param_3)
    , unk28(param_4)
    , unk2C(param_5)
{
}

void MSRandPlay::randPlay(u32 param_1)
{
	MSRandPlayVec* self = &unk10[param_1];

	switch (self->unk4) {
	case 0: {
		s32 uVar3  = JALCalc::getRandom(unk24 / 2.0f, unk28, unk2C);
		self->unk8 = unk20 < uVar3 ? uVar3 : unk20;
		self->unk8 = self->unk8 < unk24 ? self->unk8 : unk24;

		if (self->unk8 == 0) {
			self->unk4 = 2;
			break;
		}
		self->unkC = 0;
		self->unk4 = 1;
		return;
	}
	case 1:
		if (self->unk8 > self->unkC) {
			self->unkC += 1;
			return;
		}
		self->unk4 = 2;
		break;
	}

	switch (self->unk4) {
	case 2:
		switch (unk1C) {
		case MSD_SE_OBJ_KAMOME_SOLO:
			MSGMSound->startSoundSetGrp(unk1C, self->unk0, 0, 0.0f, 0, 0, 4);
			break;
		default:
			JAIActor actor(self->unk0, self->unk0, self->unk0, 0);
			MSoundSE::startSoundActorInner(unk1C, &self->unk20, &actor, 0, 4);
			break;
		}
		self->unk4 = 3;
		self->unkC = 0;
		break;

	case 3:
		if (self->unk20 == 0)
			self->unk4 = 0;
		break;
	}
}

void MSoundSE::construct()
{
	mObj = new MSoundSE;

	MSRandVol::construct(0);
	// clang-format off
	MSRandPlay::construct(MSD_SE_OBJ_KAMOME_SOLO, 0x1e, 0xf0,  JALCalc::cEqualCSlope, JALCalc::cPlusPSlope);
	MSRandPlay::construct(MSD_SE_MV10A_CRY_SHORT_01, 3,    0x14,  JALCalc::cEqualCSlope, JALCalc::cPlusPSlope);
	MSRandPlay::construct(MSD_SE_MV16_EXERT_CONT_01, 3,    0x14,  JALCalc::cEqualCSlope, JALCalc::cPlusPSlope);
	MSRandPlay::construct(MSD_SE_MA_WATER_WAIT, 5,    0x3c,  JALCalc::cEqualCSlope, JALCalc::cPlusPSlope);
	MSRandPlay::construct(MSD_SE_OBJ_BIRD_DOL_FLYING1, 0x66, 0x181, JALCalc::cEqualCSlope, JALCalc::cPlusPSlope);
	MSRandPlay::construct(MSD_SE_OBJ_BIRD_MAH_A1, 0x66, 0x181, JALCalc::cEqualCSlope, JALCalc::cPlusPSlope);
	MSRandPlay::construct(MSD_SE_OBJ_BIRD_BIA_1, 0x66, 0x181, JALCalc::cEqualCSlope, JALCalc::cPlusPSlope);
	MSRandPlay::construct(MSD_SE_OBJ_MONTE_DAY_A1, 0x66, 0x181, JALCalc::cEqualCSlope, JALCalc::cPlusPSlope);
	MSRandPlay::construct(MSD_SE_OBJ_MONTE_NIGHT_A1, 0x66, 0x181, JALCalc::cEqualCSlope, JALCalc::cPlusPSlope);
	MSRandPlay::construct(MSD_SE_OBJ_BIRD_DOL_CHUN, 0x66, 0x181, JALCalc::cEqualCSlope, JALCalc::cPlusPSlope);
	MSRandPlay::construct(MSD_SE_OBJ_BIRD_BIA_1, 0x56, 0xf3,  JALCalc::cEqualCSlope, JALCalc::cPlusPSlope);
	MSRandPlay::construct(MSD_SE_OBJ_MONTE_DAY_A1, 0x56, 0xf3,  JALCalc::cEqualCSlope, JALCalc::cPlusPSlope);
	MSRandPlay::construct(MSD_SE_OBJ_MONTE_NIGHT_A1, 8,    0xd,   JALCalc::cEqualCSlope, JALCalc::cPlusPSlope);
	// clang-format on

	// clang-format off
  JALSystem::append(JALSystem::ModType_JALSeModPitFunk, "ボスパックン・汚染飛行擬音",
    MSD_SE_BS_BSPAKU_POLLUT_IMI, 0.0f,     55000.0f, 6.0f,  0.5f,  2.0f, JALCalc::CS_NEGATIVE_CURVE,  0.0f, 100000.0f, 0);
  JALSystem::append(JALSystem::ModType_JALSeModPitFunk, "水音・距離によるピッチ下げ",
    MSD_SE_WT_GND, 2000.0f,  0.0f,     6.0f,  0.82f, 1.0f, JALCalc::CS_NEGATIVE_CURVE,  0.0f, 20000.0f,  0);
  JALSystem::append(JALSystem::ModType_JALSeModPitFunk, "潜水艦の風車ピッチ",
    MSD_SE_OBJ_SUBMARINE_MILL, 0.0f,     3.5f,     3.4f,  0.2f,  1.35f, JALCalc::CS_POSITIVE_CURVE, 0.0f, 5.0f,      0);
  JALSystem::append(JALSystem::ModType_JALSeModPitFunk, "潜水艦の鎖のこすれる音",
    MSD_SE_OBJ_SUBMARINE_CRANE, 0.0f,     0.7f,     6.0f,  0.5f,  1.0f,  JALCalc::CS_UNKNOWN_2,      0.0f, 1.0f,      0);
  JALSystem::append(JALSystem::ModType_JALSeModVolFunk, "潜水艦の風車音量",
    MSD_SE_OBJ_SUBMARINE_MILL, 0.0f,     3.5f,     3.4f,  0.0f,  1.0f,  JALCalc::CS_NEGATIVE_CURVE, 0.0f, 1.0f,      0);
  JALSystem::append(JALSystem::ModType_JALSeModVolFunk, "潜水艦の浮上する水音",
    MSD_SE_OBJ_SUBMARINE_SINK, 0.0f,     0.7f,     6.0f,  0.5f,  1.0f,  JALCalc::CS_UNKNOWN_2,      0.0f, 1.0f,      0);
  JALSystem::append(JALSystem::ModType_JALSeModPitFunk, "コイン風車",
    MSD_SE_OBJ_BI_COINMILL, 0.14f,    10.5f,    0.78f, 0.2f,  1.93f, JALCalc::CS_POSITIVE_CURVE, 0.0f, 20.0f,     1);
  JALSystem::append(JALSystem::ModType_JALSeModVolFunk, "コイン風車",
    MSD_SE_OBJ_BI_COINMILL, 0.14f,    10.5f,    0.78f, 0.5f,  1.0f,  JALCalc::CS_UNKNOWN_2,      0.0f, 20.0f,     0);
  JALSystem::append(JALSystem::ModType_JALSeModPitFunk, "上下向回転門",
    MSD_SE_OBJ_BI_UPDOWNMILL, 0.0f,     10.0f,    3.4f,  0.2f,  1.35f, JALCalc::CS_UNKNOWN_2,      0.0f, 20.0f,     0);
  JALSystem::append(JALSystem::ModType_JALSeModVolFunk, "上下向回転門",
    MSD_SE_OBJ_BI_UPDOWNMILL, 0.0f,     10.0f,    3.4f,  0.44f, 1.35f, JALCalc::CS_UNKNOWN_2,      0.0f, 20.0f,     0);
  JALSystem::append(JALSystem::ModType_JALSeModVolFunk, "足台風車(足台移動音)",
    MSD_SE_OBJ_BI_STEPMILL_MOVE, 0.0f,     0.05f,    6.0f,  0.0f,  1.0f,  JALCalc::CS_UNKNOWN_2,      0.0f, 20.0f,     1);
  JALSystem::append(JALSystem::ModType_JALSeModPitFunk, "足台風車(風車音)",
    MSD_SE_OBJ_BI_STEPMILL_WIND, 0.0f,     2.36f,    6.0f,  0.3f,  1.5f,  JALCalc::CS_UNKNOWN_2,      0.0f, 20.0f,     0);
  JALSystem::append(JALSystem::ModType_JALSeModVolFunk, "足台風車(風車音)",
    MSD_SE_OBJ_BI_STEPMILL_WIND, 0.0f,     2.36f,    6.0f,  0.2f,  1.0f,  JALCalc::CS_UNKNOWN_2,      0.0f, 20.0f,     0);
  JALSystem::append(JALSystem::ModType_JALSeModPitFunk, "放水音",
    0,      0.01f,    1.0f,     6.0f,  0.3f,  1.0f,  JALCalc::CS_UNKNOWN_2,      0.0f, 5.0f,      1);
  JALSystem::append(JALSystem::ModType_JALSeModVolFunk, "マリオしりもち音",
    MSD_SE_MA_BOUND, 0.01f,    50.0f,    10.0f, 0.2f,  1.0f,  JALCalc::CS_POSITIVE_CURVE, 0.0f, 100.0f,    1);
  JALSystem::append(JALSystem::ModType_JALSeModPitFunk, "ジャンプキノコ",
    MSD_SE_OBJ_JUMP_KINOKO, 15000.0f, 3000.0f,  1.0f,  1.0f,  1.3f,  JALCalc::CS_UNKNOWN_2,      0.0f, 30000.0f,  0);
  JALSystem::append(JALSystem::ModType_JALSeModVolFunk, "ジャンプキノコ",
    MSD_SE_OBJ_JUMP_KINOKO, 3000.0f,  15000.0f, 1.0f,  0.68f, 1.0f,  JALCalc::CS_UNKNOWN_2,      0.0f, 30000.0f,  0);
  JALSystem::append(JALSystem::ModType_JALSeModVolFunk, "マーレー壷を支えるロープ",
    MSD_SE_OBJ_MR_TSUBO_PULL, 0.0f,     20.0f,    1.0f,  0.0f,  1.0f,  JALCalc::CS_UNKNOWN_2,      0.0f, 50.0f,     1);
  JALSystem::append(JALSystem::ModType_JALSeModPitFunk, "マーレー壷を支えるロープ",
    MSD_SE_OBJ_MR_TSUBO_PULL, 0.0f,     20.0f,    1.0f,  0.5f,  1.0f,  JALCalc::CS_UNKNOWN_2,      0.0f, 50.0f,     1);
  JALSystem::append(JALSystem::ModType_JALSeModPitFunk, "マーレー壷",
    MSD_SE_OBJ_MR_TSUBO_WATER, 0.0f,     20.0f,    1.0f,  0.5f,  1.5f,  JALCalc::CS_UNKNOWN_2,      0.0f, 300.0f,    0);
  JALSystem::append(JALSystem::ModType_JALSeModVolFunk, "コインバウンド音",
    MSD_SE_SY_COIN_BOUND, 0.0f,     20.0f,    1.0f,  0.0f,  1.0f,  JALCalc::CS_NEGATIVE_CURVE, 0.0f, 100.0f,    1);
  JALSystem::append(JALSystem::ModType_JALSeModPitFunk, "ノーマルノズルのエアノイズ",
    0x24,   0.01f,    0.56f,    5.0f,  0.25f, 1.5f,  JALCalc::CS_POSITIVE_CURVE, 0.0f, 100.0f,    1);
  JALSystem::append(JALSystem::ModType_JALSeModVolFunk, "お化けスイカバウンド",
    MSD_SE_OBJ_WATERMELON_BBUND, 0.7f,     15.0f,    10.0f, 0.2f,  1.0f,  JALCalc::CS_NEGATIVE_CURVE, 0.0f, 30.0f,     1);
  JALSystem::append(JALSystem::ModType_JALSeModVolFunk, "通常スイカバウンド",
    MSD_SE_OBJ_WATERMELON_SBUND, 0.7f,     15.0f,    10.0f, 0.2f,  1.0f,  JALCalc::CS_NEGATIVE_CURVE, 0.0f, 30.0f,     1);
  JALSystem::append(JALSystem::ModType_JALSeModVolFunk, "お化けスイカ回転",
    MSD_SE_OBJ_WATERMELON_BROLL, 0.01f,    10.0f,    10.0f, 0.2f,  1.0f,  JALCalc::CS_NEGATIVE_CURVE, 0.0f, 30.0f,     1);
  JALSystem::append(JALSystem::ModType_JALSeModVolFunk, "通常スイカバ回転",
    MSD_SE_OBJ_WATERMELON_SROLL, 0.01f,    10.0f,    10.0f, 0.2f,  1.0f,  JALCalc::CS_NEGATIVE_CURVE, 0.0f, 30.0f,     1);
  JALSystem::append(JALSystem::ModType_JALSeModPitFunk, "お化けスイカ回転",
    MSD_SE_OBJ_WATERMELON_BROLL, 0.01f,    10.0f,    1.0f,  0.8f,  1.2f,  JALCalc::CS_NEGATIVE_CURVE, 0.0f, 30.0f,     1);
  JALSystem::append(JALSystem::ModType_JALSeModPitFunk, "通常スイカバ回転",
    MSD_SE_OBJ_WATERMELON_SROLL, 0.01f,    10.0f,    1.0f,  0.8f,  1.2f,  JALCalc::CS_NEGATIVE_CURVE, 0.0f, 30.0f,     1);

  JALSystem::append(JALSystem::ModType_JALSeModVolFGrp, "ロープ揺れ音",
    0,      1.0f,     52.0f,    3.98f, 0.0f,  1.0f,  JALCalc::CS_UNKNOWN_2,      0.0f, 300.0f,    1);
  JALSystem::appendGrpMember(JALSystem::ModType_JALSeModVolFGrp, 0, MSD_SE_OBJ_ROPE_CLEAK_A);
  JALSystem::appendGrpMember(JALSystem::ModType_JALSeModVolFGrp, 0, MSD_SE_OBJ_ROPE_CLEAK_B);
  JALSystem::appendGrpMember(JALSystem::ModType_JALSeModVolFGrp, 0, MSD_SE_OBJ_ROPE_CLEAK_ROLL);
  JALSystem::appendGrpMember(JALSystem::ModType_JALSeModVolFGrp, 0, MSD_SE_OBJ_ROPE_CLEAK_HALFA);
  JALSystem::appendGrpMember(JALSystem::ModType_JALSeModVolFGrp, 0, MSD_SE_OBJ_ROPE_CLEAK_HALFB);

  JALSystem::append(JALSystem::ModType_JALSeModVolFGrp, "ロープ揺れ時のマリオの風切音",
    1,      1.0f,     83.0f,    1.2f,  0.0f,  1.0f, JALCalc::CS_UNKNOWN_2,       0.0f, 300.0f,    1);
  JALSystem::appendGrpMember(JALSystem::ModType_JALSeModVolFGrp, 1, MSD_SE_MA_ROPE_ROLL_HALF_A);
  JALSystem::appendGrpMember(JALSystem::ModType_JALSeModVolFGrp, 1, MSD_SE_MA_ROPE_ROLL_HALF_B);
  JALSystem::appendGrpMember(JALSystem::ModType_JALSeModVolFGrp, 1, MSD_SE_MA_ROPE_ROLL_FULL_A);
  JALSystem::appendGrpMember(JALSystem::ModType_JALSeModVolFGrp, 1, MSD_SE_MA_ROPE_ROLL_FULL_B);

  JALSystem::append(JALSystem::ModType_JALSeModPitFGrp, "ボール系のバウンド音(ピッチ上昇)",
    2,      30.0f,    0.0f,     1.0f,  1.0f,  2.0f, JALCalc::CS_UNKNOWN_2,       0.0f, 100.0f,    1);
  JALSystem::appendGrpMember(JALSystem::ModType_JALSeModPitFGrp, 2, MSD_SE_IT_EGG_BOUND);
  JALSystem::appendGrpMember(JALSystem::ModType_JALSeModPitFGrp, 2, MSD_SE_IT_DRIAN_BOUND);

  JALSystem::append(JALSystem::ModType_JALSeModVolFGrp, "ボール系のバウンド音(ボリューム減少)",
    2,      0.0f,     20.0f,    1.0f,  0.0f,  1.0f, JALCalc::CS_NEGATIVE_CURVE,  0.0f, 100.0f,    1);
  JALSystem::appendGrpMember(JALSystem::ModType_JALSeModVolFGrp, 2, MSD_SE_IT_EGG_BOUND);
  JALSystem::appendGrpMember(JALSystem::ModType_JALSeModVolFGrp, 2, MSD_SE_IT_DRIAN_BOUND);

  JALSystem::append(JALSystem::ModType_JALSeModPitFunk, "ジェットコースター実音",
    MSD_SE_OBJ_JET_COASTER, 0.0f,     50.0f,    2.87f, 1.0f,  2.3f, JALCalc::CS_NEGATIVE_CURVE,  0.0f, 300.0f,    0);

  JALSystem::append(JALSystem::ModType_JALSeModVolFGrp, "ブランコ(vol)",
    3,      0.0f,     30.0f,    1.0f,  0.0f,  1.0f, JALCalc::CS_UNKNOWN_2,       0.0f, 100.0f,    1);
  JALSystem::appendGrpMember(JALSystem::ModType_JALSeModVolFGrp, 3, MSD_SE_OBJ_SWING1);
  JALSystem::appendGrpMember(JALSystem::ModType_JALSeModVolFGrp, 3, MSD_SE_OBJ_SWING2);

  JALSystem::append(JALSystem::ModType_JALSeModPitFGrp, "ブランコ(pitch)",
    3,      0.0f,     30.0f,    1.0f, 0.3f,   1.0f, JALCalc::CS_UNKNOWN_2,       0.0f, 100.0f,    1);
  JALSystem::appendGrpMember(JALSystem::ModType_JALSeModPitFGrp, 3, MSD_SE_OBJ_SWING1);
  JALSystem::appendGrpMember(JALSystem::ModType_JALSeModPitFGrp, 3, MSD_SE_OBJ_SWING2);

  JALSystem::append(JALSystem::ModType_JALSeModVolDist, "コロパク着地音",
    MSD_SE_BS_KRPAKU_GND, 38468.0f, 0.0f,    12.2f, 0.0f,  1.0f, JALCalc::CS_POSITIVE_CURVE, 0.0f, 100000.0f, 0);
  JALSystem::append(JALSystem::ModType_JALSeModVolDist, "コロパク回転音",
    MSD_SE_BS_KRPAKU_ROLL, 40966.0f, 0.0f,    16.8f, 0.0f,  1.0f, JALCalc::CS_POSITIVE_CURVE, 0.0f, 100000.0f, 0);
  JALSystem::append(JALSystem::ModType_JALSeModVolDist, "ボスパックン汚染飛行音(real)",
    MSD_SE_BS_BSPAKU_POLLUT_FLY, 50000.0f, 0.0f,    20.0f, 0.0f,  1.0f, JALCalc::CS_POSITIVE_CURVE, 0.0f, 100000.0f, 0);
  JALSystem::append(JALSystem::ModType_JALSeModVolDist, "ボスパックン汚染飛行音(imit)",
    MSD_SE_BS_BSPAKU_POLLUT_IMI, 50000.0f, 0.0f,    28.2f, 0.0f,  1.0f, JALCalc::CS_POSITIVE_CURVE, 0.0f, 100000.0f, 0);
  JALSystem::append(JALSystem::ModType_JALSeModPitFunk, "メカクッパ戦・キラー飛行音",
    MSD_SE_EN_KILLER_FLY_KUPPA, 5000.0f,  50.0f,   1.0f,  0.7f,  1.2f, JALCalc::CS_POSITIVE_CURVE, 0.0f, 50000.0f,  0);
  JALSystem::append(JALSystem::ModType_JALSeModPitFunk, "ポポのふくらみ音",
    MSD_SE_EN_POPO_GROW, 2.5f,     1.0f,    3.0f,  0.54f, 1.1f, JALCalc::CS_POSITIVE_CURVE, 0.0f, 3.0f,      0);
  JALSystem::append(JALSystem::ModType_JALSeModVolFunk, "ポポのふくらみ音",
    MSD_SE_EN_POPO_GROW, 2.5f,     1.0f,    3.0f,  0.55f, 1.0f, JALCalc::CS_POSITIVE_CURVE, 0.0f, 3.0f,      0);
  JALSystem::append(JALSystem::ModType_JALSeModPitFunk, "キャンキャン尻尾ひっぱり音",
    MSD_SE_EN_WANWAN_PULL, 100.0f,   500.0f,  5.0f,  0.0f,  1.3f, JALCalc::CS_UNKNOWN_2,      0.0f, 500.0f,    0);
  JALSystem::append(JALSystem::ModType_JALSeModVolFunk, "ハチの巣揺れ音",
    MSD_SE_EN_BEENEST_SWING, 0.04f,    1.2f,    1.0f,  0.0f,  1.0f, JALCalc::CS_UNKNOWN_2,      0.0f, 150.0f,    0);
  JALSystem::append(JALSystem::ModType_JALSeModVolFunk, "イガイガバウンド音",
    MSD_SE_EN_IGAIGA_BOUND, 0.8f,     1.01f,   10.0f, 0.4f,  1.0f, JALCalc::CS_POSITIVE_CURVE, 0.0f, 1.5f,      0);
  JALSystem::append(JALSystem::ModType_JALSeModPitFunk, "イガイガバウンド音",
    MSD_SE_EN_IGAIGA_BOUND, 0.8f,     1.01f,   10.0f, 0.4f,  1.0f, JALCalc::CS_POSITIVE_CURVE, 0.0f, 1.5f,      0);
  JALSystem::append(JALSystem::ModType_JALSeModVolFunk, "イガイガ転がり音",
    MSD_SE_EN_IGAIGA_ROLL, 0.95f,    2.3f,    4.0f,  0.2f,  1.0f, JALCalc::CS_POSITIVE_CURVE, 0.0f, 2.5f,      0);
  JALSystem::append(JALSystem::ModType_JALSeModPitFunk, "イガイガ転がり音",
    MSD_SE_EN_IGAIGA_ROLL, 100.0f,   500.0f,  5.0f,  0.0f,  1.3f, JALCalc::CS_UNKNOWN_2,      0.0f, 500.0f,    0);
  JALSystem::append(JALSystem::ModType_JALSeModPitFunk, "ボスパックン竜巻音",
    MSD_SE_BS_BSPAKU_TORNADO, 1538.0f,  500.0f,  3.0f,  0.65f, 1.7f, JALCalc::CS_UNKNOWN_2,      0.0f, 10000.0f,  0);
  JALSystem::append(JALSystem::ModType_JALSeModPitFunk, "ボスゲッソー回転攻撃音",
    MSD_SE_BS_GESO_ROLL, 13500.0f, 9700.0f, 4.0f,  0.6f,  1.8f, JALCalc::CS_POSITIVE_CURVE, 0.0f, 14000.0f,  0);
  JALSystem::append(JALSystem::ModType_JALSeModVolFunk, "ボスゲッソー回転攻撃音",
    MSD_SE_BS_GESO_ROLL, 13500.0f, 9700.0f, 5.0f,  0.3f,  1.0f, JALCalc::CS_POSITIVE_CURVE, 0.0f, 14000.0f,  0);
  JALSystem::append(JALSystem::ModType_JALSeModPitFunk, "ボスワンワン引きずり音",
    MSD_SE_BS_WANWAN_M_DRAG, 4.0f,     8.0f,    1.0f,  0.9f,  1.1f, JALCalc::CS_NEGATIVE_CURVE, 0.0f, 500.0f,    0);
  JALSystem::append(JALSystem::ModType_JALSeModPitFunk, "メカクッパ火炎",
    MSD_SE_BS_MKP_FIRE, 0.3f,     1.6f,    2.0f,  0.3f,  1.0f, JALCalc::CS_NEGATIVE_CURVE, 0.0f, 2.0f,      1);
  JALSystem::append(JALSystem::ModType_JALSeModPitFunk, "ウナギ回転",
    MSD_SE_BS_UNG_ROLL, 0.0f,     9.0f,    2.0f,  0.2f,  1.0f, JALCalc::CS_NEGATIVE_CURVE, 0.0f, 2.0f,      0);
  JALSystem::append(JALSystem::ModType_JALSeModVolFunk, "ウナギ回転",
    MSD_SE_BS_UNG_ROLL, 0.0f,     9.0f,    2.0f,  0.2f,  1.0f, JALCalc::CS_NEGATIVE_CURVE, 0.0f, 2.0f,      0);
  JALSystem::append(JALSystem::ModType_JALSeModPitFunk, "クッパ風呂あふれ水",
    MSD_SE_BS_KOOPA_FLOOD, 0.0f,     1.0f,    2.0f,  0.3f,  1.0f, JALCalc::CS_UNKNOWN_2,      0.0f, 2.0f,      0);
  MSSetSound::init();
	// clang-format on

	{
		MSSetSoundGrp* grp = new MSSetSoundGrp(
		    0, "カモメ", 3, 2, 13, 2, 3.0f, 1, 44.0f, 3.0f, 1.0f, 1.0f, 0.0f,
		    0xf, 200.0f, 0xb4, 1.0f, 1.0f, 0.0f, false);
		// clang-format off
		grp->append(new MSSetSoundMember(MSD_SE_OBJ_KAMOME_SOLO, nullptr, 60.0f));
		grp->append(new MSSetSoundMember(MSD_SE_OBJ_KAMOME_SOLO, nullptr, 60.0f));
		grp->append(new MSSetSoundMember(MSD_SE_OBJ_KAMOME_SOLO, nullptr, 60.0f));
		grp->append(new MSSetSoundMember(MSD_SE_OBJ_KAMOME_SOLO, nullptr, 60.0f));
		grp->append(new MSSetSoundMember(MSD_SE_OBJ_KAMOME_SOLO, nullptr, 60.0f));
		grp->append(new MSSetSoundMember(MSD_SE_OBJ_KAMOME_SOLO, nullptr, 60.0f));
		grp->append(new MSSetSoundMember(MSD_SE_OBJ_KAMOME_SOLO, nullptr, 60.0f));
		grp->append(new MSSetSoundMember(MSD_SE_OBJ_KAMOME_SOLO, nullptr, 60.0f));
		grp->append(new MSSetSoundMember(MSD_SE_OBJ_KAMOME_SOLO, nullptr, 60.0f));
		// clang-format on
	}
}

u32 MSoundSE::getRandomID(u32 param_1)
{
	u32 i = 0;

	u32 local_a0[16];
	for (; i < 15; ++i) {
		u32 uVar6 = param_1 + i;
		u32 uVar4 = MSound::getBstSwitch(uVar6);

		u8 a = (param_1 >> 0xb & 1) | (param_1 >> 0x18 & 0xc0);
		u8 b = (uVar6 >> 0xb & 1) | (uVar6 >> 0x18 & 0xc0);
		if (a != b)
			break;

		if (uVar4 == 0xffffffff)
			break;

		if (i != 0 && ((uVar4 & 0x80000000) != 0))
			break;

		if ((uVar4 & 0x70000000) == 0)
			break;

		local_a0[i] = (uVar4 & 0x70000000) >> 0x1c;
	}

	if (i <= 1)
		return param_1;

	f32 dVar10 = 0.0f;
	for (u32 j = 0; j < i; ++j)
		dVar10 += (f32)local_a0[j];

	f32 dVar9 = JALCalc::getRandom_0_1();
	f32 fVar1 = 0.0;
	int r6    = 0;
	for (u32 j = 0; j < i; ++j) {
		fVar1 += (f32)local_a0[j];

		if (dVar10 * dVar9 < fVar1) {
			r6 = j;
			break;
		}
	}

	return param_1 + r6;
}

JAISound* MSoundSE::startSoundActor(u32 param_1, const Vec* param_2,
                                    u32 param_3, JAISound** param_4,
                                    u32 param_5, u8 param_6)
{
	JAIActor actor(param_2, param_2, param_2, param_3);
	return startSoundActorInner(param_1, param_4, &actor, param_5, param_6);
}

JAISound* MSoundSE::startSoundSystemSE(u32 param_1, u32 param_2,
                                       JAISound** param_3, u32 param_4)
{
	u32 tmp = param_1;
	switch (param_1) {
	case MSD_SE_SY_E3_MENU_CURSOR:
		--param_2;
		switch (param_2) {
			// clang-format off
		case 1: tmp = MSD_SE_SY_E3_MENU_CURSOR1; break;
		case 2: tmp = MSD_SE_SY_E3_MENU_CURSOR2; break;
		case 3: tmp = MSD_SE_SY_E3_MENU_CURSOR3; break;
		case 4: tmp = MSD_SE_SY_E3_MENU_CURSOR4; break;
		case 5: tmp = MSD_SE_SY_E3_MENU_CURSOR5; break;
			// clang-format on
		}
		break;
	}

	JAISound* sound
	    = startSoundActorInner(tmp, param_3, (JAIActor*)0xffffffff, param_4, 4);

	if (!sound)
		return nullptr;

	switch (param_1) {
	case MSD_SE_SY_E3_MENU_CURSOR:
		f32 f1  = 0.5f;
		f32 f31 = 0.5f;
		switch (param_2) {
			// clang-format off
    case 0: f1 = 0.1f; f31 = 0.1f; break;
    case 1: f1 = 0.5f; f31 = 0.0f; break;
    case 2: f1 = 0.9f; f31 = 0.1f; break;
    case 3: f1 = 0.1f; f31 = 0.9f; break;
    case 4: f1 = 0.5f; f31 = 1.0f; break;
    case 5: f1 = 0.9f; f31 = 0.9f; break;
			// clang-format on
		}
		sound->setPan(f1, 0, 0);
		sound->setDolby(f31, 0, 0);
		break;
	}

	return sound;
}

static f32 vecLength(const Vec& vec)
{
	return std::sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

void MSoundSE::startSoundActorWithInfo(u32 param_1, const Vec* param_2,
                                       Vec* param_3, f32 param_4, u32 param_5,
                                       u32 param_6, JAISound** param_7,
                                       u32 param_8, u8 param_9)
{
	f32 fVar7 = param_4;
	switch (param_1) {
	case MSD_SE_BS_BSPAKU_POLLUT_IMI:
		fVar7 = param_2->y;
		break;

	case MSD_SE_OBJ_ROPE_CLEAK_A:
		fVar7 = abs(fVar7);
		break;

	case MSD_SE_OBJ_JET_COASTER_IMI:
		fVar7 = param_2->y;
		break;

	case MSD_SE_IT_EGG_BOUND:
	case MSD_SE_IT_DRIAN_BOUND:
		fVar7 = vecLength(*param_3);
		fVar7 = abs(fVar7);
		break;

	case MSD_SE_MA_KICK_ENEMY:
		if (param_5 < 4)
			param_1 += param_5;
		else
			param_1 += 0x4;
		break;
	}

	if (JALSystem::gateCheckFunc(param_1, fVar7) != true) {
		JAIActor actor(param_2, param_2, param_2, param_6);
		JAISound* sound
		    = startSoundActorInner(param_1, param_7, &actor, param_8, param_9);
		if (sound) {
			switch (param_1) {
			case MSD_SE_BS_HINO_SEED_LQ_LEV: {
				f32 fVar1 = SeInfo::smSeSetting.unk4;
				for (u32 i = 0; i < sound->unk14; ++i)
					fVar1 *= SeInfo::smSeSetting.unk0;
				sound->setSeInterPitch(0, fVar1, 0, 0.0f);
				break;
			}

			case MSD_SE_OBJ_JET_COASTER_IMI:
				f32 d = JALCalc::linearTransform(param_4, 0.0f, 20.0f, 0.0f,
				                                 1.0f, true);
				sound->setVolume(d, 0, 0);
				break;
			}

			JALSystem::processModFunc(sound, fVar7, 0, 0);
		}
	}
}

bool MSoundSE::checkSoundArea(u32 param_1, const Vec& param_2)
{
	bool result = true;

	switch (param_1) {
	case 7: {
		Vec vec = *MSGMSound->unkAC[0].unk0;
		vec.y += 75.0f;
		Vec vec1  = vec;
		int iVar2 = gpCubeCamera->getInCubeNo(vec1);

		Vec vec2 = param_2;
		vec2.y += 75.0f;
		Vec vec3  = vec2;
		int iVar3 = gpCubeCamera->getInCubeNo(vec3);

		if (iVar3 != -1) {
			if (iVar3 == iVar2)
				result = true;
			else
				result = false;
		} else {
			result = true;
		}

	} break;

	case 8: {
		int iVar2 = SMS_GetMonteVillageAreaInMario();
		int iVar3 = MSMainProc::getMonteVillageActorArea(param_2);
		if (iVar2 == 1)
			iVar2 = 3;

		if (iVar2 == 3)
			result = true;
		else if (iVar2 == iVar3)
			result = true;
		else
			result = false;
	} break;
	}

	return result;
}

// TODO: find a home for this
static u32 get_thing(u32 param_1)
{
	u32 uVar1 = param_1 >> 30;
	u32 uVar2 = param_1 >> 12 & 0xF;

	if (uVar1 == 0)
		return uVar2;

	if (uVar1 == 2)
		return 0x10;

	if (uVar1 == 3)
		return 0x11;

	return 0xffffffff;
}

JAISound* MSoundSE::startSoundActorInner(u32 param_1, JAISound** param_2,
                                         JAIActor* param_3, u32 param_4,
                                         u8 param_5)
{
	u32 uVar2 = MSound::getBstSwitch(param_1);
	if (param_3 != (JAIActor*)0xffffffff) {
		switch (MSGMSound->unkCD) {
		case 7:
			if (!checkSoundArea(MSGMSound->unkCD, *param_3->unk4)) {
				if (get_thing(param_1) != 1 && get_thing(param_1) != 0)
					return nullptr;
			}
			break;

		case 8:
			if (!checkSoundArea(MSGMSound->unkCD, *param_3->unk4)) {
				if (get_thing(param_1) != 1 && get_thing(param_1) != 0)
					return nullptr;
			}

			switch (param_1) {
			case MSD_SE_OBJ_FENCE_REVERSE1:
				param_1 = MSD_SE_OBJ_BBFENCE_TURN1;
				break;
			case MSD_SE_OBJ_FENCE_REVERSE2:
				param_1 = MSD_SE_OBJ_BBFENCE_TURN2;
				break;
			case MSD_SE_MA_FENCE_PUNCH:
				param_1 = MSD_SE_OBJ_BBFENCE_PUNCH;
				break;
			case MSD_SE_MA_FENCE_CATCH:
				param_1 = MSD_SE_OBJ_BBFENCE_CATCH;
				break;
			}
			break;
		}

		if (uVar2 & 0x800) {
			u32 uVar3 = param_3->unkC & 0x10000000;
			if (uVar3) {
				switch (param_1) {
				case MSD_SE_MA_WALK_STONE_L_HEEL:
					startSoundActorInner(MSD_SE_YO_WALK_L, nullptr, param_3,
					                     param_4, param_5);
					break;

				case MSD_SE_MA_WALK_STONE_R_HEEL:
					return startSoundActorInner(MSD_SE_YO_WALK_R, nullptr,
					                            param_3, param_4, param_5);
				}
			}

			param_1 = getNewIDBySurfaceCode(param_1, param_3);
			if (param_1 == 0xffffffff)
				return nullptr;

			u32 copy;
			if (param_3->unkC & 0xf00) {
				copy = param_1;
			} else {
				copy = param_1;
				switch (param_1) {
				case MSD_SE_MA_WALK_STONE_L_HEEL:
				case MSD_SE_MA_WALK_STONE_L_TIP:
				case MSD_SE_MA_WALK_STONE_R_HEEL:
				case MSD_SE_MA_WALK_STONE_R_TIP:
					copy += param_3->unkC << 3 & 0x7F8;
					break;
				}
			}
			param_1 = copy;

			if (param_1 == 0xffffffff)
				return nullptr;

			if (uVar3) {
				switch (param_1) {
				case MSD_SE_MA_WALK_STONE_L_HEEL:
				case MSD_SE_MA_WALK_STONE_R_HEEL:
				case MSD_SE_MA_WALK_STN_SND_LH:
				case MSD_SE_MA_WALK_STN_SND_RH:
				case MSD_SE_MA_WALK_MARBLE_L_H:
				case MSD_SE_MA_WALK_MARBLE_R_H:
					return nullptr;
				}
			}
		}
	}

	if (uVar2 & 0x80000000)
		param_1 = getRandomID(param_1);

	if (MSGMSound->unkCD == 8 && param_1 >= MSD_SE_MA_WALK_METALNET_LH1
	    && param_1 <= MSD_SE_MA_WALK_METALNET_RT2) {
		param_1 -= 8;
	}

	if (param_3 == (JAIActor*)0xffffffff) {
		if (param_2 != nullptr) {
			MSGBasic->startSoundDirectID(param_1, param_2, nullptr, param_4, 4);
			return *param_2;
		} else {
			return MSGBasic->startSoundActorReturnHandle(param_1, nullptr,
			                                             param_4, 4);
		}
	} else {
		if (param_2 != nullptr) {
			MSGBasic->startSoundActor(param_1, param_2, param_3, param_4,
			                          param_5);
			return *param_2;
		} else {
			return MSGBasic->startSoundActorReturnHandle(param_1, param_3,
			                                             param_4, param_5);
		}
	}
}

u32 MSoundSE::getNewIDByGroundCode(u32 param, JAIActor* actor) { return 0; }

u32 MSoundSE::getNewIDBySurfaceCode(u32 param_1, JAIActor* param_2)
{
	u32 uVar1 = param_2->unkC & 0xf00;
	if (!uVar1)
		return param_1;

	switch (param_1) {
	case MSD_SE_MA_WALK_STONE_L_HEEL:
		switch (uVar1) {
		case 0x100:
		case 0x700:
			return MSD_SE_MA_WALK_WATER_S_L;
		case 0x200:
			return MSD_SE_MA_WALK_WATER_D_L;
		case 0x300:
		case 0x500:
			return MSD_SE_MA_WALK_MUD_S_L;
		case 0x400:
		case 0x600:
			return 0x1930;
		}
		break;

	case MSD_SE_MA_WALK_STONE_R_HEEL:
		switch (uVar1) {
		case 0x100:
		case 0x700:
			return MSD_SE_MA_WALK_WATER_S_R;
		case 0x200:
			return MSD_SE_MA_WALK_WATER_D_R;
		case 0x300:
		case 0x500:
			return MSD_SE_MA_WALK_MUD_S_R;
		case 0x400:
		case 0x600:
			return MSD_SE_MA_WALK_MUD_D_R;
		}
		break;

	case MSD_SE_MA_WALK_STONE_L_TIP:
	case MSD_SE_MA_WALK_STONE_R_TIP:
		return -1;

	case 0x1821:
	case 0x1823:
	case 0x1825:
		break;
	}

	return param_1;
}

void MSoundSE::startSoundNpcActor(u32 param_1, const Vec* param_2, u32 param_3,
                                  JAISound** param_4, u32 param_5, u8 param_6)
{
	JAIActor actor(param_2, param_2, param_2, param_3);
	checkMonoSound(param_1, &actor);
	startSoundActorInner(param_1, param_4, &actor, param_5, param_6);
}

bool MSoundSE::checkMonoSound(u32 param_1, JAIActor* param_2)
{
	JAISoundInfo* local_c;
	JAIBasic::basic->unk0->getInfoPointer(param_1, (void**)&local_c);
	if (local_c->unk0 & 0x4000) {
		u32 uVar1       = JAIBasic::basic->changeIDToCategory(param_1);
		JAISound* sound = JAIBasic::basic->unk0->unk1E8[uVar1 & 0xff].unk4;
		JAISound* nextSound;
		for (; sound != nullptr; sound = nextSound) {
			nextSound         = sound->unk30;
			JAISoundInfo* tmp = (JAISoundInfo*)sound->unk3C;

			if (sound->unk20 != param_2->unk0)
				continue;

			if (!(tmp->unk0 & 0x4000))
				continue;

			if (param_1 == sound->unk8)
				continue;

			JAIBasic::basic->stopSoundHandle(sound, 0);
			break;
		}
	}

	return true;
}

// namespace MSoundSESystem
