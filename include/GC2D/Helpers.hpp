#ifndef GC2D_HELPERS_H
#define GC2D_HELPERS_H

#include "version.h"

// This TU is ONLY present on the NX build
#ifdef VERSION_GMSP01_NX

namespace Helpers {
bool IsSpecialChar(char the_char);
bool IsSpecialRound(char the_char);
}; // namespace Helpers

#endif

#endif
