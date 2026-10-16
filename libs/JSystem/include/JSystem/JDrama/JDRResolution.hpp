#ifndef JDR_RESOLUTION_HPP
#define JDR_RESOLUTION_HPP

#include <dolphin/types.h>

namespace JDrama {

#if defined(VERSION_GMSP01) || defined(VERSION_GMSP01_NX)
u16 GetVIWidthMax(u32 tvFormat);
u16 GetVIHeightMax(u32 tvFormat);
#else
u16 GetVIWidthMax();
u16 GetVIHeightMax();
#endif

} // namespace JDrama

#endif
