#include <JSystem/JParticle/JPAExtraShape.hpp>
#include <JSystem/JParticle/JPAMath.hpp>

JPAExtraShape::JPAExtraShape(const u8* data)
{
	unk8  = JPAConvertFixToFloat(*(s16*)(data + 0x14));
	unkC  = JPAConvertFixToFloat(*(s16*)(data + 0x16));
	unk10 = JPAConvertFixToFloat(*(s16*)(data + 0x18));
	unk14 = JPAConvertFixToFloat(*(s16*)(data + 0x1A));
	unk18 = JPAConvertFixToFloat(*(s16*)(data + 0x1C));

	unk78 = data[0x1E];
	unk79 = data[0x1F];

	unk1C = JPAConvertFixToFloat(*(s16*)(data + 0x20));
	unk20 = JPAConvertFixToFloat(*(s16*)(data + 0x22));
	unk24 = JPAConvertFixToFloat(*(s16*)(data + 0x24));
	unk28 = JPAConvertFixToFloat(*(s16*)(data + 0x26));

	unk2C = unk8 != 0.0f ? (unk14 - unk10) / unk8 : 1.0f;
	unk30 = unkC != 1.0f ? (unk18 - unk14) / (1.0f - unkC) : 1.0f;

	unk34 = JPAConvertFixToFloat(*(s16*)(data + 0x34));
	unk38 = JPAConvertFixToFloat(*(s16*)(data + 0x36));
	unk3C = JPAConvertFixToFloat(*(s16*)(data + 0x38));
	unk40 = JPAConvertFixToFloat(*(s16*)(data + 0x3A)) * 10.0f;
	unk44 = JPAConvertFixToFloat(*(s16*)(data + 0x3E)) * 10.0f;

	unk7A = data[0x40];
	unk7B = data[0x41];
	unk74 = *(s16*)(data + 0x42);
	unk48 = JPAConvertFixToFloat(*(s16*)(data + 0x44)) * 10.0f;
	unk4C = JPAConvertFixToFloat(*(s16*)(data + 0x48)) * 10.0f;
	unk7C = data[0x4A];
	unk7D = data[0x4B];
	unk76 = *(s16*)(data + 0x4C);
	unk7E = data[0x4E];

	if (unk38 != 0.0f) {
		unk50 = (1.0f - unk48) / unk38;
		unk58 = (1.0f - unk40) / unk38;
	} else {
		unk58 = 1.0f;
		unk50 = 1.0f;
	}

	if (unk3C != 1.0f) {
		unk54 = (unk4C - 1.0f) / (1.0f - unk3C);
		unk5C = (unk44 - 1.0f) / (1.0f - unk3C);
	} else {
		unk5C = 1.0f;
		unk54 = 1.0f;
	}

	unk60 = JPAConvertFixToFloat(*(s16*)(data + 0x5A));
	unk64 = JPAConvertFixToFloat(*(s16*)(data + 0x5C));
	unk68 = JPAConvertFixToFloat(*(s16*)(data + 0x5E));
	unk6C = JPAConvertFixToFloat(*(s16*)(data + 0x60));
	unk70 = JPAConvertFixToFloat(*(s16*)(data + 0x62));

	unk7F = data[0x64];
}
