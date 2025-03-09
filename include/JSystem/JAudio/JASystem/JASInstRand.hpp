#ifndef JASINSTRAND_HPP
#define JASINSTRAND_HPP

#include <JSystem/JAudio/JASystem/JASInstEffect.hpp>
namespace JASystem {

class TInstRand : public TInstEffect {
public:
	virtual f32 getY(int, int) const;

public:
	/* 0x8 */ f32 unk8;
	/* 0xC */ f32 unkC;
};

} // namespace JASystem

#endif // JASINSTRAND_HPP
