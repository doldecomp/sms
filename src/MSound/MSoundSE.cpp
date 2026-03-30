#include <MSound/MSoundSE.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSRandVol.hpp>
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
		case 0x3813:
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
		grp->append(new MSSetSoundMember(0x3813, nullptr, 60.0f));
		grp->append(new MSSetSoundMember(0x3813, nullptr, 60.0f));
		grp->append(new MSSetSoundMember(0x3813, nullptr, 60.0f));
		grp->append(new MSSetSoundMember(0x3813, nullptr, 60.0f));
		grp->append(new MSSetSoundMember(0x3813, nullptr, 60.0f));
		grp->append(new MSSetSoundMember(0x3813, nullptr, 60.0f));
		grp->append(new MSSetSoundMember(0x3813, nullptr, 60.0f));
		grp->append(new MSSetSoundMember(0x3813, nullptr, 60.0f));
		grp->append(new MSSetSoundMember(0x3813, nullptr, 60.0f));
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

JAISound* MSoundSE::startSoundActor(u32 param_1, const Vec* param_2, u32 param_3,
                               JAISound** param_4, u32 param_5, u8 param_6)
{
	JAIActor actor(param_2, param_2, param_2, param_3);
	return startSoundActorInner(param_1, param_4, &actor, param_5, param_6);
}

JAISound* MSoundSE::startSoundSystemSE(u32 param_1, u32 param_2,
                                       JAISound** param_3, u32 param_4)
{
	u32 tmp = param_1;
	switch (param_1) {
	case 0x481E:
		--param_2;
		switch (param_2) {
			// clang-format off
		case 1: tmp = 0x482E; break;
		case 2: tmp = 0x482F; break;
		case 3: tmp = 0x4830; break;
		case 4: tmp = 0x4831; break;
		case 5: tmp = 0x4832; break;
			// clang-format on
		}
		break;
	}

	JAISound* sound
	    = startSoundActorInner(tmp, param_3, (JAIActor*)0xffffffff, param_4, 4);

	if (!sound)
		return nullptr;

	switch (param_1) {
	case 0x481E:
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
	case 0x2052:
		fVar7 = param_2->y;
		break;

	case 0x381C:
		fVar7 = abs(fVar7);
		break;

	case 0x305B:
		fVar7 = param_2->y;
		break;

	case 0x3804:
	case 0x3862:
		fVar7 = vecLength(*param_3);
		fVar7 = abs(fVar7);
		break;

	case 0x1818:
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
			case 0x2007: {
				f32 fVar1 = SeInfo::smSeSetting.unk4;
				for (u32 i = 0; i < sound->unk14; ++i)
					fVar1 *= SeInfo::smSeSetting.unk0;
				sound->setSeInterPitch(0, fVar1, 0, 0.0f);
				break;
			}

			case 0x305B:
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
			case 0x3824:
				param_1 = 0x38AD;
				break;
			case 0x3825:
				param_1 = 0x38AF;
				break;
			case 0x193A:
				param_1 = 0x38AB;
				break;
			case 0x193B:
				param_1 = 0x38AC;
				break;
			}
			break;
		}

		if (uVar2 & 0x800) {
			u32 uVar3 = param_3->unkC & 0x10000000;
			if (uVar3) {
				switch (param_1) {
				case 0x1820:
					startSoundActorInner(0x1942, nullptr, param_3, param_4,
					                     param_5);
					break;

				case 0x1824:
					return startSoundActorInner(0x1943, nullptr, param_3,
					                            param_4, param_5);
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
				case 0x1820:
				case 0x1822:
				case 0x1824:
				case 0x1826:
					copy += param_3->unkC << 3 & 0x7F8;
					break;
				}
			}
			param_1 = copy;

			if (param_1 == 0xffffffff)
				return nullptr;

			if (uVar3) {
				switch (param_1) {
				case 0x1820:
				case 0x1824:
				case 0x1828:
				case 0x182C:
				case 0x1830:
				case 0x1834:
					return nullptr;
				}
			}
		}
	}

	if (uVar2 & 0x80000000)
		param_1 = getRandomID(param_1);

	if (MSGMSound->unkCD == 8 && param_1 >= 0x1878 && param_1 <= 0x187F) {
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
	case 0x1820:
		switch (uVar1) {
		case 0x100:
		case 0x700:
			return 0x1924;
		case 0x200:
			return 0x1928;
		case 0x300:
		case 0x500:
			return 0x192C;
		case 0x400:
		case 0x600:
			return 0x1930;
		}
		break;

	case 0x1824:
		switch (uVar1) {
		case 0x100:
		case 0x700:
			return 0x1926;
		case 0x200:
			return 0x192A;
		case 0x300:
		case 0x500:
			return 0x192e;
		case 0x400:
		case 0x600:
			return 0x1932;
		}
		break;

	case 0x1822:
	case 0x1826:
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
	void* local_c;
	JAIBasic::basic->unk0->getInfoPointer(param_1, &local_c);
	if (*(u32*)local_c & 0x4000) {
		u32 uVar1       = JAIBasic::basic->changeIDToCategory(param_1);
		JAISound* sound = JAIBasic::basic->unk0->unk1E8[uVar1 & 0xff].unk4;
		JAISound* nextSound;
		for (; sound != nullptr; sound = nextSound) {
			nextSound = sound->unk30;
			void* tmp = sound->unk3C;

			if (sound->unk20 != param_2->unk0)
				continue;

			if (!(*(u32*)tmp & 0x4000))
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
