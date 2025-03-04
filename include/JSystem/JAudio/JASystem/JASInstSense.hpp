#ifndef JASINSTSENSE_HPP
#define JASINSTSENSE_HPP

#include <dolphin/types.h>
namespace JASystem {

class TInstSense {
public:
	virtual int getY(int, int) const;
	void setParams(int, int, f32, f32);
};

} // namespace JASystem

#endif // JASINSTSENSE_HPP
