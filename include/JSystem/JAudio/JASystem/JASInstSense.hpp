#ifndef JASINSTSENSE_HPP
#define JASINSTSENSE_HPP

#include <JSystem/JAudio/JASystem/JASInstEffect.hpp>
#include <dolphin/types.h>

namespace JASystem {

class TInstSense : public TInstEffect {
public:
	virtual int getY(int, int) const;
	void setParams(int, int, f32, f32);
};

} // namespace JASystem

#endif // JASINSTSENSE_HPP
