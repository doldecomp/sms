#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>

void SMS_ResetTexCacheRegion()
{
	j3dSys.mFlags |= 0x80000000;
	j3dSys.setTexCacheRegion(GX_TEXCACHE_128K);
}
