#include "dolphin.h"

#include "types.h"
#include "JSystem/J3DSys.hpp"

void SMS_ResetTexCacheRegion()
{
	j3dSys._034 |= 0x80000000;
	j3dSys.setTexCacheRegion(GX_TEXCACHE_128K);
}
