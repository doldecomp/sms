#include <string.h>

#include <dolphin/gx.h>
#include <dolphin/os.h>
#include <macros.h>

#include "__gx.h"

void GXCallDisplayList(void* list, u32 nbytes)
{
	CHECK_GXBEGIN(0xEC, "GXCallDisplayList");

	if (gx->dirtyState != 0) {
		__GXSetDirtyState();
	}

	if (*(u32*)&gx->vNum == 0) { // checks both vNum and bpSent
		__GXSendFlushPrim();
	}
	GX_WRITE_U8(0x40);
	GX_WRITE_U32(list);
	GX_WRITE_U32(nbytes);
}
