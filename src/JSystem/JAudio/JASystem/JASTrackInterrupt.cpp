#include <JSystem/JAudio/JASystem/JASTrackInterrupt.hpp>
#include <types.h>

namespace JASystem {

void TIntrMgr::init()
{
	unk0 = 1;
	unk1 = 0;
	unk2 = 0;
	unk3 = 0;
	unk4 = 0;
	unk8 = 0;
	for (int i = 0; i < 8; i++)
		unkC[i] = nullptr;
}

void TIntrMgr::request(u32 param)
{
	if ((unk2 & 1 << param) == 0) {
		return;
	}
	unk1 |= 1 << param;
}

void TIntrMgr::setIntr(u32 param, void* data)
{
	unk2 |= 1 << param;
	unkC[param] = data;
}

void TIntrMgr::resetInter(u32 param) { unk2 &= ~(1 << param); }

void* TIntrMgr::checkIntr()
{
	if (unk0 == 0)
		return nullptr;

	u32 r4 = unk2 & unk1;
	for (u32 i = 0; r4; i++) {
		if (r4 & 1) {
			unk1 &= ~(1 << i);
			return unkC[i];
		}
		r4 >>= 1;
	}
	return nullptr;
}

void TIntrMgr::timerProcess()
{
	if (unk4 == 0)
		return;

	unk4--;
	if (unk4 != 0)
		return;

	request(6);
	if (unk3) {
		unk3--;
		if (unk3)
			unk4 = unk8;
	} else {
		unk4 = unk8;
	}
}

} // namespace JASystem
