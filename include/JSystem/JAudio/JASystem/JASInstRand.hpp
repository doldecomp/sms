#ifndef JASINSTRAND_HPP
#define JASINSTRAND_HPP

#include <JSystem/JAudio/JASystem/JASInstEffect.hpp>
namespace JASystem {

class TInstRand : public TInstEffect {
public:
	virtual int getY(int, int) const;
};

} // namespace JASystem

#endif // JASINSTRAND_HPP
