#ifndef GD_BASE_H
#define GD_BASE_H

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif // ifdef __cplusplus

void GXBeginDisplayList();
void GXEndDisplayList();
void GXCallDisplayList(void*, u32);

#ifdef __cplusplus
}
#endif // ifdef __cplusplus


#endif