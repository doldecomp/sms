#include <MSound/MSHandle.hpp>
#include <JSystem/JAudio/JAInterface/JAIGlobalParameter.hpp>
#include <JSystem/JAudio/JAInterface/JAIConst.hpp>
#include <JSystem/JAudio/JAInterface/JAIBasic.hpp>
#include <JSystem/JAudio/JALibrary/JALSystem.hpp>
#include <JSystem/JAudio/JALibrary/JALCalc.hpp>
#include <math.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

f32 MSHandle::smACosPrm[101] = {
	3.141592,   2.941258,   2.857799,   2.793427,   2.738877,   2.690566,
	2.6466579,  2.606066,   2.568079,   2.532207,   2.4980919,  2.465462,
	2.434109,   2.403867,   2.374599,   2.346194,   2.3185589,  2.291615,
	2.265295,   2.2395389,  2.214298,   2.1895249,  2.165182,   2.141233,
	2.1176469,  2.0943949,  2.0714509,  2.0487909,  2.026395,   2.004241,
	1.982313,   1.960593,   1.939064,   1.917713,   1.896526,   1.875489,
	1.854591,   1.833819,   1.813162,   1.792611,   1.772154,   1.751783,
	1.731487,   1.711258,   1.691086,   1.670964,   1.650882,   1.630832,
	1.6108069,  1.590798,   1.570796,   1.550795,   1.530786,   1.5107599,
	1.490711,   1.470629,   1.450507,   1.430335,   1.4101059,  1.38981,
	1.369439,   1.348982,   1.328431,   1.3077739,  1.287002,   1.266104,
	1.245067,   1.223879,   1.202528,   1.181,      1.1592799,  1.137351,
	1.115198,   1.092801,   1.070142,   1.047198,   1.023945,   1.000359,
	0.97641098, 0.95206797, 0.927295,   0.902054,   0.876298,   0.849978,
	0.82303399, 0.795399,   0.766994,   0.73772597, 0.70748299, 0.676131,
	0.64350098, 0.609386,   0.57351297, 0.53552699, 0.49493399, 0.451027,
	0.402716,   0.34816599, 0.28379399, 0.200335,   0.0,
};
SeCategory MSHandle::smSeCategory[16] = {
	{ 0x02, 8000.0f, 0.75999999f, 150.0f },
	{ 0x02, 8000.0f, 1.0f, 150.0f },
	{ 0x02, 6000.0f, 1.0f, 500.0f },
	{ 0x03, 6000.0f, 0.81f, 500.0f },
	{ 0x02, 12000.0f, 0.83999997f, 500.0f },
	{ 0x04, 12000.0f, 0.58999997f, 500.0f },
	{ 0x02, 7000.0f, 0.89999998f, 500.0f },
	{ 0x02, 8000.0f, 1.0f, 500.0f },
	{ 0x02, 6000.0f, 0.75999999f, 500.0f },
	{ 0x02, 8000.0f, 1.0f, 500.0f },
	{ 0x02, 8000.0f, 1.0f, 500.0f },
	{ 0x02, 8000.0f, 1.0f, 500.0f },
	{ 0x02, 8000.0f, 1.0f, 500.0f },
	{ 0x02, 8000.0f, 1.0f, 500.0f },
	{ 0x02, 8000.0f, 1.0f, 500.0f },
	{ 0x02, 8000.0f, 1.0f, 500.0f },
};
f32 MSHandle::cPan_MaxAmp           = 0.499f;
f32 MSHandle::cPan_CAdjust          = 0.02f;
f32 MSHandle::cPan_CShift           = 1.6394f;
f32 MSHandle::cPan_HiSence_Dist     = 12.0f;
f32 MSHandle::cMS_DistanceMax_Sence = 0.5f;
f32 MSHandle::cDol_0Rad             = 1.0316f;
f32 MSHandle::cDol_HalfRad          = 1.5707999f;
f32 MSHandle::cDol_FullRad          = 2.1099999f;

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

f32 MSHandle::MSACos(f32 param_1)
{
	int iVar1 = (param_1 + 1.0f) * 50.0f;

	if (iVar1 < 0)
		return smACosPrm[0];

	if (iVar1 >= 101)
		return smACosPrm[100];

	return smACosPrm[iVar1];
}

void MSHandle::setSeDistanceParameters()
{
	u8 type = smSeCategory[get_thing(unk8)].mType;
	if (unk1 == 2)
		type = 0;

	setSeDistanceVolume(type);
	setSeDistancePan(type);
	setSeDistancePitch(type);
	setSePositionDopplar();
	setSeDistanceFir(type);

	if (!(getSwBit() & 0x400)) {
		f32 dVar4 = interPointer->getMapInfoFxParameter(unk18);
		setFxmix(dVar4, 0, 2);
	}

	setSeDistanceDolby(type);
}

void MSHandle::setSeDistancePitch(u8 param_1)
{
	f32 fVar1 = 1.0f;
	if (getSwBit() & 0x10) {
		fVar1 = 1.0f
		        - (int(JAIConst::random.get_ufloat_1() * 16.0f) & 0xF) / 192.0f;
	}

	if (getSwBit() & 0xC0) {
		fVar1 += unk3 / 192.0f;
	}

	setSeInterPitch(4, fVar1, param_1, 0.0f);
}

void MSHandle::setSeDistancePan(u8 param_1)
{
	FabricatedPositionInfo* ptr = unk1C;

	f32 thing = ptr->unk18;

	f32 d = calcPan(ptr->unk0, thing, smSeCategory[get_thing(unk8)].unk4);
	setSeInterPan(4, d, param_1, 0);
}

f32 MSHandle::calcPan(const Vec& param_1, f32 param_2, f32 param_3)
{
	f32 fVar2 = cPan_MaxAmp;
	f32 dVar3 = param_2 <= 0.0f ? 0.0f : MSACos(-param_1.x / param_2);

	f32 fVar4
	    = cPan_CAdjust + fVar2 * 2.0f * dVar3 / M_PI - fVar2 - cPan_CAdjust;

	f32 shift = cPan_CShift;

	if (fVar4 < 0.0f) {
		fVar4 = powf(-fVar4 / fVar2, shift);
		fVar4 = -fVar2 * fVar4;
	} else {
		fVar4 = powf(fVar4 / fVar2, shift);
		fVar4 = fVar2 * fVar4;
	}

	f32 fVar1;
	if (param_2 < cPan_HiSence_Dist) {
		fVar4 *= param_2 / cPan_HiSence_Dist;
	} else {
		fVar4 *= (cMS_DistanceMax_Sence - 1.0f) / (param_3 - cPan_HiSence_Dist)
		             * (param_2 - cPan_HiSence_Dist)
		         + 1.0f;
	}

	fVar4 += fVar2;

	if (fVar4 > 1.0f)
		return 1.0f;

	if (fVar4 < 0.0f)
		return 0.0f;

	return fVar4;
}

void MSHandle::setSeDistanceDolby(u8 param_1)
{
	f32 d = calcDolby(unk1C->unk0, unk1C->unk18);
	setSeInterDolby(4, d, param_1, 0);
}

f32 MSHandle::calcDolby(const Vec& param_1, f32 param_2)
{
	f32 dVar2 = param_2 <= 0.0f ? 0.0f : MSACos(-param_1.y / param_2);

	f32 zeroRad = cDol_0Rad;
	f32 halfRad = cDol_HalfRad;
	f32 fullRad = cDol_FullRad;

	if (dVar2 < zeroRad) {
		dVar2 = 0.0f;
	} else if (dVar2 < halfRad) {
		dVar2 = 0.5f / (halfRad - zeroRad) * (dVar2 - zeroRad);
	} else if (dVar2 < fullRad) {
		dVar2 = 0.5f / (fullRad - halfRad) * (dVar2 - halfRad) + 0.5f;
	} else {
		dVar2 = 1.0f;
	}

	if (param_2 < cPan_HiSence_Dist) {
		dVar2 = param_2 * ((dVar2 - 0.5f) / cPan_HiSence_Dist) + 0.5f;
	}

	if (dVar2 > 1.0f)
		return 1.0f;

	if (dVar2 < 0.0f)
		return 0.0f;

	return dVar2;
}

void MSHandle::setSeDistanceVolume(u8 param_1)
{
	u32 uVar2 = getSwBit();
	if (uVar2 & 0x200000) {
		f32 d = JALSystem::processModDistVolume(unk8, unk1C->unk18);
		setSeInterVolume(4, d, param_1, 0);
		return;
	}

	f32 dVar4;
	if (!(uVar2 & 0x2)) {
		// TODO: inline?
		u32 tmp = getSwBit() >> 16 & 0x7;
		dVar4
		    = setDistanceVolumeCommon(smSeCategory[get_thing(unk8)].unk4, tmp);
	} else {
		dVar4 = 1.0f;
	}

	setSeInterVolume(4, dVar4, param_1, 0);
}

f32 MSHandle::setDistanceVolumeCommon(f32 volume, u8 param_2)
{
	f32 fVar1         = unk1C->unk18;
	f32 maxVolumeDist = JAIGlobalParameter::getParamMaxVolumeDistance();
	u32 uVar1         = get_thing(unk8);
	return calcVolume(fVar1, volume, maxVolumeDist, param_2, uVar1);
}

f32 MSHandle::calcVolume(f32 param_1, f32 param_2, f32 param_3, u8 param_4,
                         u8 param_5)
{
	if (param_1 < param_3)
		return 1.0f;

	f32 fVar2 = param_1 - param_3;
	f32 fVar1 = param_2 - param_3;

	switch (param_4) {
	case 1:
		fVar1 = fVar1 * 4.0f / 3.0f;
		break;
	case 2:
		fVar1 = fVar1 * 5.0f / 3.0f;
		break;
	case 3:
		fVar1 = fVar1 * 2.0f;
		break;
	case 4:
		fVar1 = fVar1 * 3.0f / 4.0f;
		break;
	case 5:
		fVar1 = fVar1 / 2.0f;
		break;
	case 6:
		fVar1 = fVar1 / 4.0f;
		break;
	case 7:
		fVar1 = smSeCategory[param_5].unkC;
		break;
	}
	return JALCalc::linearTransform(fVar2, 0.0f, fVar1, 1.0f, 0.0f, false);
}
