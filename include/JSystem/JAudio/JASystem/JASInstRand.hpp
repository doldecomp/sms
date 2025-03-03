#ifndef JASINSTRAND_HPP
#define JASINSTRAND_HPP

namespace JASystem {

class TInstRand {
public:
	virtual ~TInstRand() { }
	virtual int getY(int, int) const;
};

} // namespace JASystem

#endif // JASINSTRAND_HPP
