#include "GC2D/Helpers.hpp"

// This TU is ONLY present on the NX build
#ifdef VERSION_GMSP01_NX

#include <dolphin/types.h>

namespace Helpers {
bool IsSpecialChar(char the_char)
{
	static const s8 SpecialChars[] = "#$%*+<>@^`|~\xEF\xBD\xA5";
	const s8* p                    = SpecialChars;
	while (*p != (s8)the_char && *p != 0) {
		p++;
	}
	if ((s8)*p == 0) {
		return false;
	}
	return true;
}
bool IsSpecialRound(char the_char)
{
	static const s8 SpecialCharsRound[] = "#%*+@^`";
	const s8* p                         = SpecialCharsRound;
	while (*p != (s8)the_char && *p != 0) {
		p++;
	}
	if (*p == 0) {
		return false;
	}
	return true;
}
} // namespace Helpers

#endif
