#include <JSystem/JParticle/JPAExTexShape.hpp>
#include <JSystem/JParticle/JPAMath.hpp>
#include <dolphin/os/OSCache.h>

JPAExTexShape::JPAExTexShape(const u8* data)
{
	unk25 = data[0x10];
	switch (data[0x11]) {
	case 0:
	case 1:
		unk20      = GX_ITM_0;
		unk8[0][0] = JPAConvertFixToFloat(*(s16*)(data + 0x12));
		unk8[0][1] = JPAConvertFixToFloat(*(s16*)(data + 0x14));
		unk8[0][2] = JPAConvertFixToFloat(*(s16*)(data + 0x16));
		unk8[1][0] = JPAConvertFixToFloat(*(s16*)(data + 0x18));
		unk8[1][1] = JPAConvertFixToFloat(*(s16*)(data + 0x1A));
		unk8[1][2] = JPAConvertFixToFloat(*(s16*)(data + 0x1C));
		DCStoreRange(&unk8, 0x18);
		break;
	case 2:
		unk20 = GX_ITM_S0;
		break;
	case 3:
		unk20 = GX_ITM_T0;
		break;
	}
	unk24 = data[0x1E];
	unk26 = data[0x1F];
	unk27 = data[0x20];
	unk28 = data[0x30];
	unk29 = data[0x33];
}
