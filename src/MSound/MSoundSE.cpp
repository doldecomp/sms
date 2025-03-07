#include <MSound/MSoundSE.hpp>
#include <MSound/MSRandVol.hpp>
#include <JSystem/JAudio/JALibrary/JALSystem.hpp>

namespace MSoundSESystem {

MSRandVol::MSRandVol(u32 param)
    : unk4(this)
    , unk14(param)
    , unk18(0.5f)
    , unk1C(0.0f)
    , unk20(0.25f)
    , unk24(0.5f)
    , unk28(0.75f)
    , unk2C(1.0f)
    , unk30(1.5f)
    , unk34(2.0f)
    , unk38(4.0f)
    , unk3C(0.25f)
    , unk40(0.5f)
    , unk44(0.75f)
    , unk48(1.0f)
{
}

void MSRandVol::construct(u32 param)
{
	smList.append(&(new MSRandVol(param))->unk4);
}
u32 MSRandVol::getRandVol(u32 param) { return 0; }
u32 MSRandVol::getRandomVolume(u32 param1, u32 param2) { return 0; }
JSUList<MSRandVol> MSRandVol::smList;

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
void MSRandPlay::construct(u32 param_1, s32 param_2, s32 param_3, f32 param_4,
                           f32 param_5)
{
	smList.append(
	    &(new MSRandPlay(param_1, param_2, param_3, param_4, param_5))->unk0);
}
void MSRandPlay::createRandPlayVec(u32 param1, u16 param2) { }
void MSRandPlay::createRandPlayVecDynamic(u16 param) { }
void MSRandPlay::randPlay(u32 param) { }
void MSRandPlay::registerTrans(u32 param, const Vec* vec) { }
void MSRandPlay::registerTransDynamic(const Vec* vec) { }
void MSRandPlay::startSeRandPlay(u32 param1, u32 param2) { }
JSUList<MSRandPlay> MSRandPlay::smList;

void* SeInfo::smSeSetting = 0;

MSoundSE::MSoundSE() { }

void MSoundSE::construct()
{
	mObj = new MSoundSE;

	MSRandVol::construct(0);
	// clang-format off
	MSRandPlay::construct(0x3813, 0x1e, 0xf0,  JALCalc::cEqualCSlope, JALCalc::cPlusPSlope);
	MSRandPlay::construct(0x7865, 3,    0x14,  JALCalc::cEqualCSlope, JALCalc::cPlusPSlope);
	MSRandPlay::construct(0x7094, 3,    0x14,  JALCalc::cEqualCSlope, JALCalc::cPlusPSlope);
	MSRandPlay::construct(0x1950, 5,    0x3c,  JALCalc::cEqualCSlope, JALCalc::cPlusPSlope);
	MSRandPlay::construct(0x3869, 0x66, 0x181, JALCalc::cEqualCSlope, JALCalc::cPlusPSlope);
	MSRandPlay::construct(0x386d, 0x66, 0x181, JALCalc::cEqualCSlope, JALCalc::cPlusPSlope);
	MSRandPlay::construct(0x5814, 0x66, 0x181, JALCalc::cEqualCSlope, JALCalc::cPlusPSlope);
	MSRandPlay::construct(0x581b, 0x66, 0x181, JALCalc::cEqualCSlope, JALCalc::cPlusPSlope);
	MSRandPlay::construct(0x5820, 0x66, 0x181, JALCalc::cEqualCSlope, JALCalc::cPlusPSlope);
	MSRandPlay::construct(0x3870, 0x66, 0x181, JALCalc::cEqualCSlope, JALCalc::cPlusPSlope);
	MSRandPlay::construct(0x5814, 0x56, 0xf3,  JALCalc::cEqualCSlope, JALCalc::cPlusPSlope);
	MSRandPlay::construct(0x581b, 0x56, 0xf3,  JALCalc::cEqualCSlope, JALCalc::cPlusPSlope);
	MSRandPlay::construct(0x5820, 8,    0xd,   JALCalc::cEqualCSlope, JALCalc::cPlusPSlope);
	// clang-format on

	// clang-format off
  JALSystem::append(JALSystem::ModType_JALSeModPitFunk, "ボスパックン・汚染飛行擬音",
    0x2052, 0.0f,     55000.0f, 6.0f,  0.5f,  2.0f, JALCalc::CS_NEGATIVE_CURVE,  0.0f, 100000.0f, 0);
  JALSystem::append(JALSystem::ModType_JALSeModPitFunk, "水音・距離によるピッチ下げ",
    0x6800, 2000.0f,  0.0f,     6.0f,  0.82f, 1.0f, JALCalc::CS_NEGATIVE_CURVE,  0.0f, 20000.0f,  0);
  JALSystem::append(JALSystem::ModType_JALSeModPitFunk, "潜水艦の風車ピッチ",
    0x3031, 0.0f,     3.5f,     3.4f,  0.2f,  1.35f, JALCalc::CS_POSITIVE_CURVE, 0.0f, 5.0f,      0);
  JALSystem::append(JALSystem::ModType_JALSeModPitFunk, "潜水艦の鎖のこすれる音",
    0x3030, 0.0f,     0.7f,     6.0f,  0.5f,  1.0f,  JALCalc::CS_UNKNOWN_2,      0.0f, 1.0f,      0);
  JALSystem::append(JALSystem::ModType_JALSeModVolFunk, "潜水艦の風車音量",
    0x3031, 0.0f,     3.5f,     3.4f,  0.0f,  1.0f,  JALCalc::CS_NEGATIVE_CURVE, 0.0f, 1.0f,      0);
  JALSystem::append(JALSystem::ModType_JALSeModVolFunk, "潜水艦の浮上する水音",
    0x3023, 0.0f,     0.7f,     6.0f,  0.5f,  1.0f,  JALCalc::CS_UNKNOWN_2,      0.0f, 1.0f,      0);
  JALSystem::append(JALSystem::ModType_JALSeModPitFunk, "コイン風車",
    0x3045, 0.14f,    10.5f,    0.78f, 0.2f,  1.93f, JALCalc::CS_POSITIVE_CURVE, 0.0f, 20.0f,     1);
  JALSystem::append(JALSystem::ModType_JALSeModVolFunk, "コイン風車",
    0x3045, 0.14f,    10.5f,    0.78f, 0.5f,  1.0f,  JALCalc::CS_UNKNOWN_2,      0.0f, 20.0f,     0);
  JALSystem::append(JALSystem::ModType_JALSeModPitFunk, "上下向回転門",
    0x3044, 0.0f,     10.0f,    3.4f,  0.2f,  1.35f, JALCalc::CS_UNKNOWN_2,      0.0f, 20.0f,     0);
  JALSystem::append(JALSystem::ModType_JALSeModVolFunk, "上下向回転門",
    0x3044, 0.0f,     10.0f,    3.4f,  0.44f, 1.35f, JALCalc::CS_UNKNOWN_2,      0.0f, 20.0f,     0);
  JALSystem::append(JALSystem::ModType_JALSeModVolFunk, "足台風車(足台移動音)",
    0x3042, 0.0f,     0.05f,    6.0f,  0.0f,  1.0f,  JALCalc::CS_UNKNOWN_2,      0.0f, 20.0f,     1);
  JALSystem::append(JALSystem::ModType_JALSeModPitFunk, "足台風車(風車音)",
    0x3040, 0.0f,     2.36f,    6.0f,  0.3f,  1.5f,  JALCalc::CS_UNKNOWN_2,      0.0f, 20.0f,     0);
  JALSystem::append(JALSystem::ModType_JALSeModVolFunk, "足台風車(風車音)",
    0x3040, 0.0f,     2.36f,    6.0f,  0.2f,  1.0f,  JALCalc::CS_UNKNOWN_2,      0.0f, 20.0f,     0);
  JALSystem::append(JALSystem::ModType_JALSeModPitFunk, "放水音",
    0,      0.01f,    1.0f,     6.0f,  0.3f,  1.0f,  JALCalc::CS_UNKNOWN_2,      0.0f, 5.0f,      1);
  JALSystem::append(JALSystem::ModType_JALSeModVolFunk, "マリオしりもち音",
    0x195a, 0.01f,    50.0f,    10.0f, 0.2f,  1.0f,  JALCalc::CS_POSITIVE_CURVE, 0.0f, 100.0f,    1);
  JALSystem::append(JALSystem::ModType_JALSeModPitFunk, "ジャンプキノコ",
    0x3866, 15000.0f, 3000.0f,  1.0f,  1.0f,  1.3f,  JALCalc::CS_UNKNOWN_2,      0.0f, 30000.0f,  0);
  JALSystem::append(JALSystem::ModType_JALSeModVolFunk, "ジャンプキノコ",
    0x3866, 3000.0f,  15000.0f, 1.0f,  0.68f, 1.0f,  JALCalc::CS_UNKNOWN_2,      0.0f, 30000.0f,  0);
  JALSystem::append(JALSystem::ModType_JALSeModVolFunk, "マーレー壷を支えるロープ",
    0x3060, 0.0f,     20.0f,    1.0f,  0.0f,  1.0f,  JALCalc::CS_UNKNOWN_2,      0.0f, 50.0f,     1);
  JALSystem::append(JALSystem::ModType_JALSeModPitFunk, "マーレー壷を支えるロープ",
    0x3060, 0.0f,     20.0f,    1.0f,  0.5f,  1.0f,  JALCalc::CS_UNKNOWN_2,      0.0f, 50.0f,     1);
  JALSystem::append(JALSystem::ModType_JALSeModPitFunk, "マーレー壷",
    0x3061, 0.0f,     20.0f,    1.0f,  0.5f,  1.5f,  JALCalc::CS_UNKNOWN_2,      0.0f, 300.0f,    0);
  JALSystem::append(JALSystem::ModType_JALSeModVolFunk, "コインバウンド音",
    0x4842, 0.0f,     20.0f,    1.0f,  0.0f,  1.0f,  JALCalc::CS_NEGATIVE_CURVE, 0.0f, 100.0f,    1);
  JALSystem::append(JALSystem::ModType_JALSeModPitFunk, "ノーマルノズルのエアノイズ",
    0x24,   0.01f,    0.56f,    5.0f,  0.25f, 1.5f,  JALCalc::CS_POSITIVE_CURVE, 0.0f, 100.0f,    1);
  JALSystem::append(JALSystem::ModType_JALSeModVolFunk, "お化けスイカバウンド",
    0x3889, 0.7f,     15.0f,    10.0f, 0.2f,  1.0f,  JALCalc::CS_NEGATIVE_CURVE, 0.0f, 30.0f,     1);
  JALSystem::append(JALSystem::ModType_JALSeModVolFunk, "通常スイカバウンド",
    0x388c, 0.7f,     15.0f,    10.0f, 0.2f,  1.0f,  JALCalc::CS_NEGATIVE_CURVE, 0.0f, 30.0f,     1);
  JALSystem::append(JALSystem::ModType_JALSeModVolFunk, "お化けスイカ回転",
    0x308a, 0.01f,    10.0f,    10.0f, 0.2f,  1.0f,  JALCalc::CS_NEGATIVE_CURVE, 0.0f, 30.0f,     1);
  JALSystem::append(JALSystem::ModType_JALSeModVolFunk, "通常スイカバ回転",
    0x308b, 0.01f,    10.0f,    10.0f, 0.2f,  1.0f,  JALCalc::CS_NEGATIVE_CURVE, 0.0f, 30.0f,     1);
  JALSystem::append(JALSystem::ModType_JALSeModPitFunk, "お化けスイカ回転",
    0x308a, 0.01f,    10.0f,    1.0f,  0.8f,  1.2f,  JALCalc::CS_NEGATIVE_CURVE, 0.0f, 30.0f,     1);
  JALSystem::append(JALSystem::ModType_JALSeModPitFunk, "通常スイカバ回転",
    0x308b, 0.01f,    10.0f,    1.0f,  0.8f,  1.2f,  JALCalc::CS_NEGATIVE_CURVE, 0.0f, 30.0f,     1);

  JALSystem::append(JALSystem::ModType_JALSeModVolFGrp, "ロープ揺れ音",
    0,      1.0f,     52.0f,    3.98f, 0.0f,  1.0f,  JALCalc::CS_UNKNOWN_2,      0.0f, 300.0f,    1);
  JALSystem::appendGrpMember(JALSystem::ModType_JALSeModVolFGrp, 0, 0x381c);
  JALSystem::appendGrpMember(JALSystem::ModType_JALSeModVolFGrp, 0, 0x381d);
  JALSystem::appendGrpMember(JALSystem::ModType_JALSeModVolFGrp, 0, 0x381e);
  JALSystem::appendGrpMember(JALSystem::ModType_JALSeModVolFGrp, 0, 0x381f);
  JALSystem::appendGrpMember(JALSystem::ModType_JALSeModVolFGrp, 0, 0x3820);

  JALSystem::append(JALSystem::ModType_JALSeModVolFGrp, "ロープ揺れ時のマリオの風切音",
    1,      1.0f,     83.0f,    1.2f,  0.0f,  1.0f, JALCalc::CS_UNKNOWN_2,       0.0f, 300.0f,    1);
  JALSystem::appendGrpMember(JALSystem::ModType_JALSeModVolFGrp, 1, 0x1815);
  JALSystem::appendGrpMember(JALSystem::ModType_JALSeModVolFGrp, 1, 0x1816);
  JALSystem::appendGrpMember(JALSystem::ModType_JALSeModVolFGrp, 1, 0x1817);
  JALSystem::appendGrpMember(JALSystem::ModType_JALSeModVolFGrp, 1, 0x180f);

  JALSystem::append(JALSystem::ModType_JALSeModPitFGrp, "ボール系のバウンド音(ピッチ上昇)",
    2,      30.0f,    0.0f,     1.0f,  1.0f,  2.0f, JALCalc::CS_UNKNOWN_2,       0.0f, 100.0f,    1);
  JALSystem::appendGrpMember(JALSystem::ModType_JALSeModPitFGrp, 2, 0x3804);
  JALSystem::appendGrpMember(JALSystem::ModType_JALSeModPitFGrp, 2, 0x3862);

  JALSystem::append(JALSystem::ModType_JALSeModVolFGrp, "ボール系のバウンド音(ボリューム減少)",
    2,      0.0f,     20.0f,    1.0f,  0.0f,  1.0f, JALCalc::CS_NEGATIVE_CURVE,  0.0f, 100.0f,    1);
  JALSystem::appendGrpMember(JALSystem::ModType_JALSeModVolFGrp, 2, 0x3804);
  JALSystem::appendGrpMember(JALSystem::ModType_JALSeModVolFGrp, 2, 0x3862);

  JALSystem::append(JALSystem::ModType_JALSeModPitFunk, "ジェットコースター実音",
    0x305a, 0.0f,     50.0f,    2.87f, 1.0f,  2.3f, JALCalc::CS_NEGATIVE_CURVE,  0.0f, 300.0f,    0);

  JALSystem::append(JALSystem::ModType_JALSeModVolFGrp, "ブランコ(vol)",
    3,      0.0f,     30.0f,    1.0f,  0.0f,  1.0f, JALCalc::CS_UNKNOWN_2,       0.0f, 100.0f,    1);
  JALSystem::appendGrpMember(JALSystem::ModType_JALSeModVolFGrp, 3, 0x3867);
  JALSystem::appendGrpMember(JALSystem::ModType_JALSeModVolFGrp, 3, 0x3868);

  JALSystem::append(JALSystem::ModType_JALSeModPitFGrp, "ブランコ(pitch)",
    3,      0.0f,     30.0f,    1.0f, 0.3f,   1.0f, JALCalc::CS_UNKNOWN_2,       0.0f, 100.0f,    1);
  JALSystem::appendGrpMember(JALSystem::ModType_JALSeModPitFGrp, 3, 0x3867);
  JALSystem::appendGrpMember(JALSystem::ModType_JALSeModPitFGrp, 3, 0x3868);

  JALSystem::append(JALSystem::ModType_JALSeModVolDist, "コロパク着地音",
    0x2844, 38468.0f, 0.0f,    12.2f, 0.0f,  1.0f, JALCalc::CS_POSITIVE_CURVE, 0.0f, 100000.0f, 0);
  JALSystem::append(JALSystem::ModType_JALSeModVolDist, "コロパク回転音",
    0x2054, 40966.0f, 0.0f,    16.8f, 0.0f,  1.0f, JALCalc::CS_POSITIVE_CURVE, 0.0f, 100000.0f, 0);
  JALSystem::append(JALSystem::ModType_JALSeModVolDist, "ボスパックン汚染飛行音(real)",
    0x2045, 50000.0f, 0.0f,    20.0f, 0.0f,  1.0f, JALCalc::CS_POSITIVE_CURVE, 0.0f, 100000.0f, 0);
  JALSystem::append(JALSystem::ModType_JALSeModVolDist, "ボスパックン汚染飛行音(imit)",
    0x2052, 50000.0f, 0.0f,    28.2f, 0.0f,  1.0f, JALCalc::CS_POSITIVE_CURVE, 0.0f, 100000.0f, 0);
  JALSystem::append(JALSystem::ModType_JALSeModPitFunk, "メカクッパ戦・キラー飛行音",
    0x20ff, 5000.0f,  50.0f,   1.0f,  0.7f,  1.2f, JALCalc::CS_POSITIVE_CURVE, 0.0f, 50000.0f,  0);
  JALSystem::append(JALSystem::ModType_JALSeModPitFunk, "ポポのふくらみ音",
    0x20c2, 2.5f,     1.0f,    3.0f,  0.54f, 1.1f, JALCalc::CS_POSITIVE_CURVE, 0.0f, 3.0f,      0);
  JALSystem::append(JALSystem::ModType_JALSeModVolFunk, "ポポのふくらみ音",
    0x20c2, 2.5f,     1.0f,    3.0f,  0.55f, 1.0f, JALCalc::CS_POSITIVE_CURVE, 0.0f, 3.0f,      0);
  JALSystem::append(JALSystem::ModType_JALSeModPitFunk, "キャンキャン尻尾ひっぱり音",
    0x20df, 100.0f,   500.0f,  5.0f,  0.0f,  1.3f, JALCalc::CS_UNKNOWN_2,      0.0f, 500.0f,    0);
  JALSystem::append(JALSystem::ModType_JALSeModVolFunk, "ハチの巣揺れ音",
    0x28f7, 0.04f,    1.2f,    1.0f,  0.0f,  1.0f, JALCalc::CS_UNKNOWN_2,      0.0f, 150.0f,    0);
  JALSystem::append(JALSystem::ModType_JALSeModVolFunk, "イガイガバウンド音",
    0x28ad, 0.8f,     1.01f,   10.0f, 0.4f,  1.0f, JALCalc::CS_POSITIVE_CURVE, 0.0f, 1.5f,      0);
  JALSystem::append(JALSystem::ModType_JALSeModPitFunk, "イガイガバウンド音",
    0x28ad, 0.8f,     1.01f,   10.0f, 0.4f,  1.0f, JALCalc::CS_POSITIVE_CURVE, 0.0f, 1.5f,      0);
  JALSystem::append(JALSystem::ModType_JALSeModVolFunk, "イガイガ転がり音",
    0x212f, 0.95f,    2.3f,    4.0f,  0.2f,  1.0f, JALCalc::CS_POSITIVE_CURVE, 0.0f, 2.5f,      0);
  JALSystem::append(JALSystem::ModType_JALSeModPitFunk, "イガイガ転がり音",
    0x212f, 100.0f,   500.0f,  5.0f,  0.0f,  1.3f, JALCalc::CS_UNKNOWN_2,      0.0f, 500.0f,    0);
  JALSystem::append(JALSystem::ModType_JALSeModPitFunk, "ボスパックン竜巻音",
    0x210c, 1538.0f,  500.0f,  3.0f,  0.65f, 1.7f, JALCalc::CS_UNKNOWN_2,      0.0f, 10000.0f,  0);
  JALSystem::append(JALSystem::ModType_JALSeModPitFunk, "ボスゲッソー回転攻撃音",
    0x215c, 13500.0f, 9700.0f, 4.0f,  0.6f,  1.8f, JALCalc::CS_POSITIVE_CURVE, 0.0f, 14000.0f,  0);
  JALSystem::append(JALSystem::ModType_JALSeModVolFunk, "ボスゲッソー回転攻撃音",
    0x215c, 13500.0f, 9700.0f, 5.0f,  0.3f,  1.0f, JALCalc::CS_POSITIVE_CURVE, 0.0f, 14000.0f,  0);
  JALSystem::append(JALSystem::ModType_JALSeModPitFunk, "ボスワンワン引きずり音",
    0x20d2, 4.0f,     8.0f,    1.0f,  0.9f,  1.1f, JALCalc::CS_NEGATIVE_CURVE, 0.0f, 500.0f,    0);
  JALSystem::append(JALSystem::ModType_JALSeModPitFunk, "メカクッパ火炎",
    0x8135, 0.3f,     1.6f,    2.0f,  0.3f,  1.0f, JALCalc::CS_NEGATIVE_CURVE, 0.0f, 2.0f,      1);
  JALSystem::append(JALSystem::ModType_JALSeModPitFunk, "ウナギ回転",
    0x8921, 0.0f,     9.0f,    2.0f,  0.2f,  1.0f, JALCalc::CS_NEGATIVE_CURVE, 0.0f, 2.0f,      0);
  JALSystem::append(JALSystem::ModType_JALSeModVolFunk, "ウナギ回転",
    0x8921, 0.0f,     9.0f,    2.0f,  0.2f,  1.0f, JALCalc::CS_NEGATIVE_CURVE, 0.0f, 2.0f,      0);
  JALSystem::append(JALSystem::ModType_JALSeModPitFunk, "クッパ風呂あふれ水",
    0x819d, 0.0f,     1.0f,    2.0f,  0.3f,  1.0f, JALCalc::CS_UNKNOWN_2,      0.0f, 2.0f,      0);
  MSSetSound::init();
	// clang-format on

	{
		MSSetSoundGrp* grp = new MSSetSoundGrp(
		    0, "カモメ", 3, 2, 13, 2, 3.0f, 1, 44.0f, 3.0f, 1.0f, 1.0f, 0.0f,
		    0xf, 200.0f, 0xb4, 1.0f, 1.0f, 0.0f, false);
		// clang-format off
		MSSetSoundMember* memb1 = new MSSetSoundMember(0x3813, nullptr, 60.0f); grp->append(&memb1->unk4); memb1->unk14 = grp;
		MSSetSoundMember* memb2 = new MSSetSoundMember(0x3813, nullptr, 60.0f); grp->append(&memb2->unk4); memb2->unk14 = grp;
		MSSetSoundMember* memb3 = new MSSetSoundMember(0x3813, nullptr, 60.0f); grp->append(&memb3->unk4); memb3->unk14 = grp;
		MSSetSoundMember* memb4 = new MSSetSoundMember(0x3813, nullptr, 60.0f); grp->append(&memb4->unk4); memb4->unk14 = grp;
		MSSetSoundMember* memb5 = new MSSetSoundMember(0x3813, nullptr, 60.0f); grp->append(&memb5->unk4); memb5->unk14 = grp;
		MSSetSoundMember* memb6 = new MSSetSoundMember(0x3813, nullptr, 60.0f); grp->append(&memb6->unk4); memb6->unk14 = grp;
		MSSetSoundMember* memb7 = new MSSetSoundMember(0x3813, nullptr, 60.0f); grp->append(&memb7->unk4); memb7->unk14 = grp;
		MSSetSoundMember* memb8 = new MSSetSoundMember(0x3813, nullptr, 60.0f); grp->append(&memb8->unk4); memb8->unk14 = grp;
		MSSetSoundMember* memb9 = new MSSetSoundMember(0x3813, nullptr, 60.0f); grp->append(&memb9->unk4); memb9->unk14 = grp;
		// clang-format on
	}
}

bool MSoundSE::checkMonoSound(u32 param, JAIActor* actor) { return false; }
void MSoundSE::checkSoundArea(u32 param, const Vec& vec) { }
u32 MSoundSE::getNewIDByGroundCode(u32 param, JAIActor* actor) { return 0; }
u32 MSoundSE::getNewIDBySurfaceCode(u32 param, JAIActor* actor) { return 0; }
u32 MSoundSE::getRandomID(u32 param) { return 0; }
void MSoundSE::startSoundActor(u32 p1, const Vec* p2, u32 p3, JAISound** p4,
                               u32 p5, u8 p6)
{
}
void MSoundSE::startSoundActorInner(u32 p1, JAISound** p2, JAIActor* p3, u32 p4,
                                    u8 p5)
{
}
void MSoundSE::startSoundActorWithInfo(u32 p1, const Vec* p2, Vec* p3, f32 p4,
                                       u32 p5, u32 p6, JAISound** p7, u32 p8,
                                       u8 p9)
{
}
void MSoundSE::startSoundNpcActor(u32 p1, const Vec* p2, u32 p3, JAISound** p4,
                                  u32 p5, u8 p6)
{
}
void MSoundSE::startSoundSystemSE(u32 p1, u32 p2, JAISound** p3, u32 p4) { }
MSoundSE* MSoundSE::mObj = 0;

} // namespace MSoundSESystem
