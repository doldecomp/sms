#include <JSystem/JParticle/JPAExTexShape.hpp>
#include <JSystem/JParticle/JPAMath.hpp>
#include <dolphin/os/OSCache.h>

JPAExTexShape::JPAExTexShape(const u8* data)
{
	mIndTexMode = data[0x10];
	switch (data[0x11]) {
	case 0:
	case 1:
		mIndTexMtxID     = GX_ITM_0;
		mIndTexMtx[0][0] = JPAConvertFixToFloat(*(s16*)(data + 0x12));
		mIndTexMtx[0][1] = JPAConvertFixToFloat(*(s16*)(data + 0x14));
		mIndTexMtx[0][2] = JPAConvertFixToFloat(*(s16*)(data + 0x16));
		mIndTexMtx[1][0] = JPAConvertFixToFloat(*(s16*)(data + 0x18));
		mIndTexMtx[1][1] = JPAConvertFixToFloat(*(s16*)(data + 0x1A));
		mIndTexMtx[1][2] = JPAConvertFixToFloat(*(s16*)(data + 0x1C));
		DCStoreRange(&mIndTexMtx, 0x18);
		break;
	case 2:
		mIndTexMtxID = GX_ITM_S0;
		break;
	case 3:
		mIndTexMtxID = GX_ITM_T0;
		break;
	}
	mExpScale       = data[0x1E];
	mIndTextureID   = data[0x1F];
	mSubTextureID   = data[0x20];
	unk28           = data[0x30];
	mSecondTexIndex = data[0x33];
}
