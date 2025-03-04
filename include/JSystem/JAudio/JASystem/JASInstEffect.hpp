#ifndef JASINSTEFFECT_HPP
#define JASINSTEFFECT_HPP

namespace JASystem {

class TInstEffect {
public:
	virtual int getY(int, int) const = 0;

	void setTarget(int);
};

} // namespace JASystem

#endif // JASINSTEFFECT_HPP
