#include <JSystem/JParticle/JPASweepShape.hpp>
#include <JSystem/JParticle/JPAMath.hpp>

JPASweepShape::JPASweepShape(const u8* data)
{
	unk44 = data[0x10];
	unk45 = data[0x11];
	unk46 = data[0x12];

	unk40 = *(s16*)(data + 0x14);
	unk42 = *(s16*)(data + 0x16);

	unk47 = data[0x1A];

	unk8 = *(f32*)(data + 0x28);
	unkC = *(f32*)(data + 0x2C);

	unk48 = data[0x36];
	unk49 = data[0x44];
	unk4A = data[0x45];
	unk4B = data[0x46];
	unk4C = data[0x47];

	unk10 = *(f32*)(data + 0x4C);
	unk14 = *(f32*)(data + 0x50);

	unk4D = data[0x56];
	unk4E = data[0x57];

	unk38 = *(GXColor*)(data + 0x58);
	unk3C = *(GXColor*)(data + 0x5C);

	unk18 = JPAConvertFixToFloat(*(s16*)(data + 0x18));
	unk1C = JPAConvertFixToFloat(*(s16*)(data + 0x30));
	unk20 = JPAConvertFixToFloat(*(s16*)(data + 0x32));
	unk24 = JPAConvertFixToFloat(*(s16*)(data + 0x34));
	unk28 = JPAConvertFixToFloat(*(s16*)(data + 0x48));
	unk2C = JPAConvertFixToFloat(*(s16*)(data + 0x4A));
	unk34 = JPAConvertFixToFloat(*(s16*)(data + 0x60));
	unk30 = JPAConvertFixToFloat(*(s16*)(data + 0x54));
}
