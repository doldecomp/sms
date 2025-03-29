#include <JSystem/JParticle/JPABaseShape.hpp>
#include <JSystem/JParticle/JPAMath.hpp>
#include <dolphin/gx/GXStruct.h>

struct JPAColorRegAnmKey {
	s16 unk0;
	GXColor unk2;
};

static GXColor* makeColorTable(JPAColorRegAnmKey* param_1, int param_2,
                               int param_3, JKRHeap* param_4)
{
	GXColor* result
	    = (GXColor*)JKRHeap::alloc((param_3 + 1) * sizeof(GXColor), 4, param_4);

	f32 accumR, accumG, accumB, accumA;
	f32 fR, fG, fB, fA;
	f32 thing;

	fR = (f32)param_1[0].unk2.r;
	fG = (f32)param_1[0].unk2.g;
	fB = (f32)param_1[0].unk2.b;
	fA = (f32)param_1[0].unk2.a;

	accumR = accumG = accumB = accumA = 0.0f;

	int nextKey = 0;
	for (int i = 0; i < param_3 + 1; ++i) {
		if (i == param_1[nextKey].unk0) {
			result[i] = param_1[nextKey].unk2;

			fR = (f32)param_1[nextKey].unk2.r;
			fG = (f32)param_1[nextKey].unk2.g;
			fB = (f32)param_1[nextKey].unk2.b;
			fA = (f32)param_1[nextKey].unk2.a;

			++nextKey;

			if (nextKey < param_2) {
				JPAColorRegAnmKey* key = &param_1[nextKey];

				thing = 1.0f / (key->unk0 - key[-1].unk0);

				accumR = thing * (key->unk2.r - fR);
				accumG = thing * (key->unk2.g - fG);
				accumB = thing * (key->unk2.b - fB);
				accumA = thing * (key->unk2.a - fA);
			} else {
				accumR = accumG = accumB = accumA = 0.0f;
			}
		} else {
			fR += accumR;
			fG += accumG;
			fB += accumB;
			fA += accumA;
			result[i].r = fR;
			result[i].g = fG;
			result[i].b = fB;
			result[i].a = fA;
		}
	}

	return result;
}

JPABaseShape::JPABaseShape(const u8* data, JKRHeap* heap)
{
	unk18 = *(f32*)(data + 0x18);
	unk14 = *(f32*)(data + 0x1c);
	unk58 = *(s16*)(data + 0x20);
	unk69 = data[0x24];
	unk6A = data[0x25];
	unk6B = data[0x26];

	u8 r5 = data[0x22];
	u8 r6 = data[0x23];
	unk5A = (r5 & 1) ? 0xffff : 0;
	unk5C = (r6 & 1) ? 0xffff : 0;

	// TODO: something awful is happening here (no inlines involved?!)
	u8 r5_3 = true;
	u8 r5_2 = 1;
	if (!(r5 & 2) && unk69 != 5) {
		r5_3 = false;
	}

	if (!r5_3 && (unk69 != 6)) {
		r5_2 = 0;
	}

	u8 r6_2 = r5_2 ? 1 : 0;

	u8 r5_4;
	u8 r4;
	r5_4 = true;
	r4   = 1;
	if (!(r6 & 2) && unk69 != 5) {
		r5_4 = false;
	}

	if (!r5_4 && (unk69 != 6)) {
		r4 = 0;
	}

	u8 r0 = r4 ? 1 : 0;

	unk68 = r6_2 << 1 | r0;

	switch (data[0x30]) {
	case 0:
		unk48.unk0 = GX_CC_ZERO;
		unk48.unk4 = GX_CC_TEXC;
		unk48.unk8 = GX_CC_ONE;
		unk48.unkC = GX_CC_ZERO;
		break;
	case 1:
		unk48.unk0 = GX_CC_ZERO;
		unk48.unk4 = GX_CC_C0;
		unk48.unk8 = GX_CC_TEXC;
		unk48.unkC = GX_CC_ZERO;
		break;
	case 2:
		unk48.unk0 = GX_CC_C0;
		unk48.unk4 = GX_CC_ONE;
		unk48.unk8 = GX_CC_TEXC;
		unk48.unkC = GX_CC_ZERO;
		break;
	case 3:
		unk48.unk0 = GX_CC_C1;
		unk48.unk4 = GX_CC_C0;
		unk48.unk8 = GX_CC_TEXC;
		unk48.unkC = GX_CC_ZERO;
		break;
	case 4:
		unk48.unk0 = GX_CC_ZERO;
		unk48.unk4 = GX_CC_TEXC;
		unk48.unk8 = GX_CC_C0;
		unk48.unkC = GX_CC_C1;
		break;
	case 5:
		unk48.unk0 = GX_CC_ZERO;
		unk48.unk4 = GX_CC_ZERO;
		unk48.unk8 = GX_CC_ZERO;
		unk48.unkC = GX_CC_C0;
		break;
	}

	unk6C = data[0x31];
	unk6D = data[0x35];
	unk6E = data[0x36];
	unk6F = data[0x37];
	unk70 = data[0x38];
	unk71 = data[0x39];
	unk72 = data[0x3a];
	unk73 = data[0x3b];
	unk74 = data[0x3c];
	unk75 = data[0x3d];
	unk76 = !(data[0x3e] >> 1 & 1);
	unk77 = data[0x3f];
	unk78 = data[0x40];
	unk79 = data[0x41];
	unk7A = data[0x42];
	unk7B = data[0x43];
	unk7C = data[0x44];
	unk7D = data[0x4d];
	unk7E = data[0x4e];
	unk7F = data[0x4f];
	unk80 = (data[0x4c] & 1) && unk7E != 0 ? 1 : 0;
	unk81 = (data[0x4c] & 2) ? 0 : 1;
	unk5E = *(s16*)(data + 0x5c);
	unk82 = data[0x5e];
	unk83 = data[0x60];
	unk84 = data[0x61];
	unk85 = data[0x62];
	unk86 = data[99];
	unk60 = *(u32*)(data + 100);
	unk64 = *(u32*)(data + 0x68);
	unk1C = JPAConvertFixToFloat(*(s16*)(data + 0x80)) * 10.0f;
	unk20 = JPAConvertFixToFloat(*(s16*)(data + 0x82)) * 10.0f;
	unk24 = JPAConvertFixToFloat(*(s16*)(data + 0x84)) * 10.0f;
	unk28 = JPAConvertFixToFloat(*(s16*)(data + 0x86)) * 10.0f;
	unk2C = JPAConvertFixToFloat(*(s16*)(data + 0x88)) * 10.0f;
	unk30 = JPAConvertFixToFloat(*(s16*)(data + 0x8a)) * 10.0f;
	unk34 = JPAConvertFixToFloat(*(s16*)(data + 0x8c));
	unk38 = JPAConvertFixToFloat(*(s16*)(data + 0x8e));

	unk3C = JPAConvertFixToFloat(*(s16*)(data + 0x90)) * 0.1f;
	unk40 = JPAConvertFixToFloat(*(s16*)(data + 0x92)) * 0.1f;
	unk44 = JPAConvertFixToFloat(*(s16*)(data + 0x94));
	unk87 = data[0x96];

	if (unk80 != 0) {
		unk8 = JKRHeap::alloc(unk7E, 4, heap);
		memcpy(unk8, data + *(s16*)(data + 0x12), unk7E);
	} else {
		unk8 = nullptr;
	}

	if (unk83 & 2) {
		unkC = makeColorTable((JPAColorRegAnmKey*)(data + *(s16*)(data + 0x14)),
		                      unk85, unk5E, heap);
	} else {
		unkC = nullptr;
	}

	if (unk84 & 2) {
		unk10
		    = makeColorTable((JPAColorRegAnmKey*)(data + *(s16*)(data + 0x16)),
		                     unk86, unk5E, heap);
	} else {
		unk10 = nullptr;
	}
}
