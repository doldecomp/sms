#ifndef JDR_SIZE_HPP
#define JDR_SIZE_HPP

#include <dolphin/types.h>

namespace JDrama {

class TSize {
public:
	TSize() { }
	TSize(u32 width, u32 height)
	    : mWidth(width)
	    , mHeight(height)
	{
	}

	u32 mWidth;
	u32 mHeight;
};

}; // namespace JDrama

#endif
