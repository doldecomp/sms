#ifndef GC2D_HX_WIPER_H
#define GC2D_HX_WIPER_H

#include <dolphin/types.h>

#if __cplusplus
extern "C" {
#endif

/**
 * @brief The public face of the screen-wipe library in GC2D/hx_wiper.c.
 *
 * @details Every wipe pattern and helper in that file is static; the map
 * lists exactly these eight as global, so nothing else belongs here.
 */

/// Forget any loaded resource and record the display size.
void Hx_ResetWipe(u32 width, u32 height);

/// Hand the library a .bti for the wipes that draw a texture.
void Hx_ProvideResource(void* resource, int size);

/// Hand the library a second .bti (the logo wipe's small title texture).
void Hx_ProvideResourceEx(void* resource);

void Hx_RemoveResource(void);

/// Begin wipe @p wipe_no (an index into the pattern table).
void Hx_StartWipe(int wipe_no, int param);

/// 0 if wipe @p wipe_no closes the screen, 1 if it opens it.
int Hx_GetWipeType(int wipe_no);

/// Advance the running wipe by @p rate; returns 3 once it is finished.
u32 Hx_UpdateWipe(f32 rate);

/// 1 when the logo wipe has reached its pen-draw phase, 2 once the emblem is
/// complete, 0 otherwise. Each answer is reported only once.
int Hx_MovieStartSyncEx(void);

#if __cplusplus
}
#endif

#endif
