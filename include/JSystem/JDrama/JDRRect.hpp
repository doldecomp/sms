#ifndef JDR_RECT_HPP
#define JDR_RECT_HPP

#include <JSystem/JUtility/JUTRect.hpp>

namespace JDrama {

struct TRect : JUTRect {
	TRect()
	    : JUTRect()
	{
	}
	TRect(int x1, int y1, int x2, int y2)
	    : JUTRect(x1, y1, x2, y2)
	{
	}
};

} // namespace JDrama

#endif
