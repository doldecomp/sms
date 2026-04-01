#include <MSound/MSModBgm.hpp>
#include <JSystem/JAudio/JAInterface/JAISound.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// TODO: this is from the PCH
static Vec dummy1   = { 1.0f, 1.0f, 1.0f };
static Vec dummy2   = { 1.0f, 1.0f, 1.0f };
static u32 dummy3[] = { 0, 2, 1, 3 };

JAISound* MSModBgm::modBgm(u8 param_1, u8 param_2)
{
	switch (param_1) {
	case 0:
	case 1:
		unk0 = true;
		break;

	default:
		break;
	}

	JAISound* sound = MSBgm::getHandle(param_2);
	if (!sound) {
		sound = nullptr;
		unk0  = false;
	} else {
		switch (unk4) {
		case 0:
			sound->setTempoProportion(1.3f, 10);
			sound->setPitch(1.3f, 10, 0);
			break;

		case 5:
			sound->setTempoProportion(0.3f, 180);
			sound->setPitch(0.2f, 180, 0);
			break;

		case 180:
			sound->stop(1);
			sound = nullptr;
			unk0  = false;
			break;
		}
	}
	return sound;
}

void MSModBgm::changeTempo(u8 param_1, u8 param_2)
{
	JAISound* sound = MSBgm::getHandle(param_2);
	if (sound) {
		f32 fVar1 = 1.0f;
		u32 uVar2 = 5;
		switch (param_1) {
		case 0:
			fVar1 = 1.07894f;
			break;

		case 1:
			fVar1 = 1.15789f;
			break;

		case 2:
			fVar1 = 1.2f;
			uVar2 = 20;
			break;

		case 3:
			fVar1 = 1.0f;
			break;
		}

		sound->setTempoProportion(fVar1, uVar2);
	}
}

void MSModBgm::loop()
{
	switch (unk0) {
	case true:
		unk4 += 1;
		break;

	case false:
	default:
		unk4 = 0;
		return;
	}
	unk0 = 0;
}

// The calculation used here is idx / 19, with the start value as 1 and ending at 18.
// The values are pre-calculated and pasted here for optimization, presumably.
f32 MSBgmXFade::scTiming[18] = {
	0.052632f,   0.105263f,   0.157895f, 0.210526f,   0.26315799f, 0.315789f,
	0.36842099f, 0.42105299f, 0.473684f, 0.52631599f, 0.578947f,   0.63157898f,
	0.684211f,   0.73684198f, 0.789474f, 0.84210497f, 0.894737f,   0.947368f,
};

f32 MSBgmXFade::scExp[18] = {
	0.0f,      0.0f,      0.030207f,   0.063591f, 0.100485f, 0.14126f,
	0.186324f, 0.236126f, 0.29116699f, 0.351996f, 0.419223f, 0.49351999f,
	0.57563f,  0.666377f, 0.766667f,   0.877505f, 1.0f,      1.0f,
};

void MSBgmXFade::xFadeBgm(f32 param_1)
{
	// TODO: some stupid trick with casting the second param?
	u8 tmp = getTiming(param_1, nullptr);

	bool b = tmp >= 1 && tmp <= 16;
	if (b) {
		MSBgm::setTrackVolume(0, scExp[17 - tmp], 2, 0);
		MSBgm::setTrackVolume(1, scExp[tmp], 2, 0);
	}
	unk0 = param_1;
}

void MSBgmXFade::xFadeBgmForce(f32 param_1)
{
	u8 tmp = getTimingForce(param_1);
	if (tmp != 0xff) {
		MSBgm::setTrackVolume(0, scExp[17 - tmp], 0, 0);
		MSBgm::setTrackVolume(1, scExp[tmp], 0, 0);
		unk0 = param_1;
	}
}

u8 MSBgmXFade::getTimingForce(f32 param_1)
{
	for (u8 i = 0; i < 17; ++i)
		if (param_1 >= scTiming[i] && param_1 < scTiming[i + 1])
			return i;

	return 0xff;
}

u8 MSBgmXFade::getTiming(f32 param_1, u32* param_2)
{
	f32 f1 = unk0;
	for (u8 i = 0; i < 18; ++i) {
		if (param_1 > scTiming[i] && f1 <= scTiming[i])
			return i;
		if (param_1 < scTiming[i] && scTiming[i] >= f1)
			return i;
	}

	return 0xff;
}
