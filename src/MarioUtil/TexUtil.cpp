#include <JSystem/J3DModel.hpp>
#include <JSystem/JUTNameTab.hpp>
#include <dolphin/os.h>
#include <functions.h>

void SMS_ChangeTextureAll(J3DModelData* param_1, const char* param_2,
                          const ResTIMG& param_3)

{
	char trash[0x18];
	for (u16 i = 0; i < param_1->unkAC->unk0; i = i + 1) {
		if (strcmp((char*)param_2, (char*)param_1->unkA8->getName(i)) == 0) {
			J3DModelData::UnknownStruct* us = param_1->unkAC;
			us->unk4[i]                     = param_3;

			// TODO: these are most likely an inlined call, figure it out
			{
				ResTIMG* ptr = us->unk4 + i;
				ptr->mImageDataOffset
				    = ptr->mImageDataOffset + (u32)&param_3 - (u32)ptr;
			}
			{
				ResTIMG* ptr = us->unk4 + i;
				ptr->mPaletteOffset
				    = ptr->mPaletteOffset + (u32)&param_3 - (u32)ptr;
			}
			DCFlushRange(param_1->unkAC->unk4 + i, 0x20);
		}
	}
}
