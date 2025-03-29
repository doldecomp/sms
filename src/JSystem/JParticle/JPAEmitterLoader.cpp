#include <JSystem/JParticle/JPAEmitterLoader.hpp>
#include <JSystem/JParticle/JPAResourceManager.hpp>
#include <JSystem/JParticle/JPABaseShape.hpp>
#include <JSystem/JParticle/JPAExtraShape.hpp>
#include <JSystem/JParticle/JPASweepShape.hpp>
#include <JSystem/JParticle/JPAExTexShape.hpp>
#include <types.h>

JPAEmitterData* JPAEmitterLoaderDataBase::load(const u8* param_1,
                                               JKRHeap* param_2,
                                               JPATextureResource* param_3)
{
	JPABinaryHeader* header = (JPABinaryHeader*)param_1;
	if (header->unk0 == 'JEFF' && header->unk4 == 'jpa1') {
		JPAEmitterLoader_v10 loader(param_2, param_1, header);
		return loader.load(param_3);
	} else {
		return nullptr;
	}
}

JPAEmitterLoader_v10::JPAEmitterLoader_v10(JKRHeap* param_1, const u8* param_2,
                                           const JPABinaryHeader* param_3)
    : JPAEmitterLoader(param_1, param_2, param_3)
{
}

// fabricated, name from TWW
struct JPAEmitterBlockHeader {
	/* 0x0 */ u32 unk0;
	/* 0x4 */ u32 unk4;
};

JPAEmitterData* JPAEmitterLoader_v10::load(JPATextureResource* param_1)
{
	// TODO: fakematch, all vars were probably defined at the top originally
	u32 nextOffset;

	unk10 = new (unk4, 0) JPAEmitterData;

	u32 offset = 0x20;

	u8 fld1Count = 0;
	u8 tex1Count = 0;
	u8 kfa1Count = 0;

	for (int i = 0; i < unkC->unkC; ++i) {
		u32 type   = *(u32*)(unk8 + offset);
		nextOffset = *(u32*)(unk8 + offset + 4);

		if (type == 'FLD1') {
			++fld1Count;
		} else if (type == 'TEX1') {
			++tex1Count;
		} else if (type == 'KFA1') {
			++kfa1Count;
		}

		offset += nextOffset;
	}
	unk10->unk4 = 1;
	unk10->unk0 = new (unk4, 0) JPADataBlockLinkInfo*[unk10->unk4];
	JPADataBlockLinkInfo* linkInfo = new (unk4, 0) JPADataBlockLinkInfo;
	linkInfo->unk22                = kfa1Count;
	linkInfo->unk14
	    = kfa1Count ? new (unk4, 0) JPAKeyFrameAnime*[kfa1Count] : nullptr;
	linkInfo->unk20 = fld1Count;
	linkInfo->unk18
	    = fld1Count ? new (unk4, 0) JPADataBlock*[fld1Count] : nullptr;
	linkInfo->unk21 = tex1Count;
	linkInfo->unk1C
	    = tex1Count ? (u16*)JKRHeap::alloc(tex1Count * sizeof(u16), 4, unk4)
	                : nullptr;

	u32 offset2 = 0x20;

	u32 nextFld1 = 0;
	u32 nextKfa1 = 0;
	for (int i = 0; i < unkC->unkC - tex1Count; ++i) {
		nextOffset = *(u32*)(unk8 + offset2 + 4);

		if (*(u32*)(unk8 + offset2) == 'FLD1') {
			linkInfo->unk18[nextFld1]
			    = new (unk4, 0) JPADataBlock(unk8 + offset2, unk4);
			++nextFld1;
		} else if (*(u32*)(unk8 + offset2) == 'KFA1') {
			linkInfo->unk14[nextKfa1]
			    = new (unk4, 0) JPAKeyFrameAnime(unk8 + offset2, unk4);
			++nextKfa1;
		} else if (*(u32*)(unk8 + offset2) == 'BEM1') {
			linkInfo->unk0 = new (unk4, 0) JPADataBlock(unk8 + offset2, unk4);
		} else if (*(u32*)(unk8 + offset2) == 'BSP1') {
			linkInfo->unk4 = new (unk4, 0) JPABaseShape(unk8 + offset2, unk4);
		} else if (*(u32*)(unk8 + offset2) == 'ESP1') {
			linkInfo->unk8 = new (unk4, 0) JPAExtraShape(unk8 + offset2);
		} else if (*(u32*)(unk8 + offset2) == 'SSP1') {
			linkInfo->unkC = new (unk4, 0) JPASweepShape(unk8 + offset2);
		} else if (*(u32*)(unk8 + offset2) == 'ETX1') {
			linkInfo->unk10 = new (unk4, 0) JPAExTexShape(unk8 + offset2);
		}

		offset2 += nextOffset;
	}

	for (int i = 0, nextTex1 = 0; i < tex1Count; ++i) {
		nextOffset = *(u32*)(unk8 + offset2 + 4);

		linkInfo->unk1C[nextTex1] = param_1->registration(unk8 + offset2, unk4);
		++nextTex1;
		offset2 += nextOffset;
	}

	unk10->unk0[0] = linkInfo;

	return unk10;
}
