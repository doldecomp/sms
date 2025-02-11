#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTexture.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <dolphin/os.h>

void SMS_ChangeTextureAll(J3DModelData* param_1, const char* param_2,
                          const ResTIMG& param_3)

{
	char trash[0x18];
	for (u16 i = 0; i < param_1->unkAC->mResourceCount; i = i + 1) {
		if (strcmp((char*)param_2, (char*)param_1->unkA8->getName(i)) == 0) {
			J3DTexture* tex    = param_1->unkAC;
			tex->mResources[i] = param_3;

			// TODO: these are most likely an inlined call, figure it out
			{
				ResTIMG* ptr = tex->mResources + i;
				ptr->imageDataOffset
				    = ptr->imageDataOffset + (u32)&param_3 - (u32)ptr;
			}
			{
				ResTIMG* ptr = tex->mResources + i;
				ptr->paletteOffset
				    = ptr->paletteOffset + (u32)&param_3 - (u32)ptr;
			}
			DCFlushRange(param_1->unkAC->mResources + i, 0x20);
		}
	}
}
