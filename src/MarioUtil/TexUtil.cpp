#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTexture.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <dolphin/os.h>

void SMS_ChangeTextureAll(J3DModelData* param_1, const char* param_2,
                          const ResTIMG& param_3)

{
	for (u16 i = 0; i < param_1->getTexture()->getNum(); ++i) {
		if (strcmp((char*)param_2, (char*)param_1->unkA8->getName(i)) == 0) {
			J3DTexture* tex = param_1->getTexture();
			tex->setResTIMG(i, param_3);
			DCFlushRange(param_1->getTexture()->getResTIMG(i), 0x20);
		}
	}
}
